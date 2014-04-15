#-------------------------------------------------
#
# Project created by QtCreator 2014-04-15T13:38:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtGUIExample
TEMPLATE = app
INCLUDEPATH +=  "../"
INCLUDEPATH +=  "/usr/include/vtk-5.8"
INCLUDEPATH +=  "/usr/include/InsightToolkit/Common"\
                "/usr/include/InsightToolkit"\
                "/usr/include/InsightToolkit/Utilities/vxl/vcl"\
                "/usr/include/InsightToolkit/Utilities/vxl/core"\
                "/usr/include/InsightToolkit/IO"\
                "/usr/include/InsightToolkit/Utilities"\
                "/usr/include/InsightToolkit/BasicFilters"\
                "/usr/include/InsightToolkit/Review"

SOURCES += main.cpp\
        mainwindow.cpp \
    ../renderitqt.cpp \
    ../renderit.cpp \
    ../tree.cpp \
    ../treerender.cpp \
    ../imagefilters.cpp \
    ../image.cpp \
    ../buildtree.cpp

HEADERS  += mainwindow.h \
    ../renderitqt.h \
    ../renderit.h \
    ../typedefsti.h \
    ../treerender.h \
    ../tree.h \
    ../imagefilters.h \
    ../image.h \
    ../buildtree.h

FORMS    += mainwindow.ui

unix|win32: LIBS += \
                    -lvtkGraphics\
                    -lvtkWidgets\
                    -lvtkRendering\
                    -lvtkImaging\
                    -lvtkCommon\
                    -lvtkFiltering\
                    -lvtkHybrid \
                    -lvtkVolumeRendering \
                    -lQVTK

unix|win32: LIBS += \
                    -lITKBasicFilters\
                    -lITKIO\
                    -lITKCommon\
                    -lITKDICOMParser\
                    -lITKEXPAT\
                    -lITKFEM\
                    -lITKMetaIO\
                    -litkNetlibSlatec\
                    -lITKniftiio\
                    -lITKNrrdIO\
                    -lITKStatistics\
                    -lITKQuadEdgeMesh\
                    -lITKStatistics\
                    -litksys\
                    -litkv3p_lsqr\
                    -litkv3p_netlib\
                    -litkvcl\
                    -litkvnl\
                    -litkvnl_algo\
                    -lITKznz\
                    -lITKAlgorithms\
                    -lITKNumerics\
                    -litksys\
                    -lITKAlgorithms
