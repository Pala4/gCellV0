#-------------------------------------------------
#
# Project created by QtCreator 2013-07-13T12:02:00
#
#-------------------------------------------------
CONFIG(release,debug|release){
        DEST_DIR = $${PWD}/../gCellV0
}

CONFIG(debug,debug|release){
        DEST_DIR = $${PWD}/../gCellV0_d
}

#c++11 enable
greaterThan(QT_MAJOR_VERSION, 4)
{
    CONFIG += c++11
    DEFINES += HAVE_CPP11
}
lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=gnu++11