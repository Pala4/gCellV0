#-------------------------------------------------
#
# Project created by QtCreator 2013-07-13T12:02:00
#
#-------------------------------------------------
RELEASE_POSTFIX =
DEBUG_POSTFIX = _d
DR_POSTFIX = $$RELEASE_POSTFIX
CONFIG(debug,debug|release){
        DR_POSTFIX = $$DEBUG_POSTFIX
}

PRJ_TARGET = gCellV0
PRJ_TARGET = $$join(PRJ_TARGET,,, $$DR_POSTFIX)
TARGET = $$PRJ_TARGET

DEST_DIR = $${PWD}/../
DEST_DIR = $$join(DEST_DIR,,, $$TARGET)
DEST_API_DIR = $$DEST_DIR/api/

#c++11 enable
greaterThan(QT_MAJOR_VERSION, 4)
{
    CONFIG += c++11
    DEFINES += HAVE_CPP11
}
lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=gnu++11