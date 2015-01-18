#include "FileManager.h"

FileManager::FileManager(QWidget *parent) :
    QTabWidget(parent)
{
    setTabsClosable(true);
    setMovable(true);

    createBackgroundImage();
}

int FileManager::newFile()
{
    // removes the background image (need to move this elsewhere)
    setStyleSheet("");

    Editor *editor = new Editor(QWidget::window());
    editor->setAttribute(Qt::WA_DeleteOnClose);
    editor->installEventFilter(QWidget::window());
    editor->saveContent();

    int index = addTab(editor,tr("Untitled"));

    setCurrentIndex(index);
    setTabToolTip(index,"");

//  newProjectTrees();

    connect(editor,SIGNAL(textChanged()),this,SLOT(fileChanged()));

    emit fileUpdated(index);

    return index;
}

void FileManager::open()
{
    QString dir = QDir(tabToolTip(currentIndex())).path();
    QString fileName = QFileDialog::getOpenFileName(this,
                tr("Open File"), dir, "Spin Files (*.spin);;All Files (*)");

    if (!fileName.isEmpty())
        openFile(fileName);
}


void FileManager::openFile(const QString & fileName)
{
    qDebug() << "FileManager::openFile(" << fileName << ")";

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Recent Files"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    int index = newFile();

    QTextStream in(&file);
    in.setAutoDetectUnicode(true);
    in.setCodec("UTF-8");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    getEditor(index)->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setTabToolTip(index,QFileInfo(fileName).canonicalFilePath());
    setTabText(index,QFileInfo(fileName).fileName());
    getEditor(index)->saveContent();

    //setProject();
}


void FileManager::save()
{
    int index = currentIndex();
    save(index);
}

void FileManager::save(int index)
{
    QString fileName = tabToolTip(index);

    if (fileName.isEmpty())
        saveAs();
    else
        saveFile(fileName, index);
}


void FileManager::saveAs()
{
    int n = currentIndex();

    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File As..."), 
            QDir(tabToolTip(n)).path(), 
            "Spin Files (*.spin)");

    if (fileName.isEmpty())
        return;

    saveFile(fileName, n);
}



void FileManager::saveFile(const QString & fileName, int index)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Recent Files"),
                    tr("Cannot write file %1:\n%2.")
                    .arg(fileName)
                    .arg(file.errorString()));
        return;
    }

    QTextStream os(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    os.setCodec("UTF-8");
    os << getEditor(index)->toPlainText();
    QApplication::restoreOverrideCursor();

    setTabToolTip(index,QFileInfo(fileName).canonicalFilePath());
    setTabText(index,QFileInfo(fileName).fileName());
    getEditor(index)->saveContent();

    //setProject();
}


void FileManager::closeFile()
{
    closeFile(currentIndex());
}

void FileManager::closeFile(int index)
{
    if (count() > 0)
    {
        getEditor(index)->disconnect();
        getEditor(index)->close();
        removeTab(index);
    }

    if (count() == 0)
    {
        createBackgroundImage();
    }
}

void FileManager::createBackgroundImage()
{
    setStyleSheet("background-image: url(:/icons/propellerhat.png);"
                  "background-repeat: no-repeat;"
                  "background-position: center;");
}

void FileManager::nextTab()
{
    int n = currentIndex();
    n++;
    if (n > count()-1)
        n = 0;
    changeTab(n);
}

void FileManager::previousTab()
{
    int n = currentIndex();
    n--;
    if (n < 0)
        n = count()-1;
    changeTab(n);


}

void FileManager::changeTab(int index)
{
    setCurrentIndex(index);
    if(index < 0) return;

    getEditor(currentIndex())->setFocus();
}

Editor * FileManager::getEditor(int num)
{
    return (Editor *)widget(num);
}

void FileManager::setEditor(int num, QString shortName, QString fileName, QString text)
{
    getEditor(num)->setPlainText(text);
    setTabText(num,shortName);
    setTabToolTip(num,fileName);
    setCurrentIndex(num);
    getEditor(num)->saveContent();
}

void FileManager::fileChanged()
{
    int index = currentIndex();
    QString file = tabToolTip(index);
    QString name = QFileInfo(file).fileName();

    if (file.isEmpty())
        name = tr("Untitled");

    if (getEditor(index)->contentChanged())
        name += '*';

    setTabText(index, name);
}

