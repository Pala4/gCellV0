#-------------------------------------------------
#
# Project created by QtCreator 2013-07-13T12:02:00
#
#-------------------------------------------------
TARGET = gCellV0
CONFIG(debug,debug|release){
        TARGET = $$join(TARGET,,, _d)
}

DEST_DIR = $${PWD}/../
DEST_DIR = $$join(DEST_DIR,,, $$TARGET)

#c++11 enable
greaterThan(QT_MAJOR_VERSION, 4)
{
    CONFIG += c++11
    DEFINES += HAVE_CPP11
}
lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=gnu++11