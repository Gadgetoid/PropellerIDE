QT += gui

greaterThan(QT_MAJOR_VERSION, 4): {
    QT += widgets
    DEFINES += QT5
}

INCLUDEPATH += . .. ../lib

UI_DIR = tmp
MOC_DIR = tmp
OBJECTS_DIR = tmp
RCC_DIR = tmp

CONFIG  += exceptions

win32 {
	# prevent debug/release builds on windows
	CONFIG  -= debug_and_release debug_and_release_target	
}

DEFINES += IDEVERSION=0
DEFINES += MINVERSION=20

# ENABLE_AUTO_ENTER turns on AUTO-INDENT
DEFINES += ENABLE_AUTO_ENTER
# SPIN_AUTOCOMPLETE turns on spinny-sense
DEFINES += SPIN_AUTOCOMPLETE
# Enable ZIP feature
DEFINES += ENABLE_ZIP
# Enable Enter NL/CR option
DEFINES += ENABLE_ENTER_ISNL

# Use a reliable external loader
DEFINES += EXTERNAL_SPIN_LOADER

#enable simple graphing terminal tabs
#DEFINES += ENABLE_TERM_TABS

