QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app


INCLUDEPATH +=  "../" \
                "../ThirdParty"

INCLUDEPATH +=  "/usr/include/vtk-6.0"

INCLUDEPATH +=  "/usr/include/InsightToolkit/Common"\
                "/usr/include/InsightToolkit"\
                "/usr/include/InsightToolkit/Utilities/vxl/vcl"\
                "/usr/include/InsightToolkit/Utilities/vxl/core"\
                "/usr/include/InsightToolkit/IO"\
                "/usr/include/InsightToolkit/Utilities"\
                "/usr/include/InsightToolkit/BasicFilters"\
                "/usr/include/InsightToolkit/Review"

HEADERS += \
    ../tree.h \
    ../image.h \
    ../renderit.h

SOURCES += \
    renderqt.cpp \
    ../tree.cpp \
    ../image.cpp \
    ../treerender.cpp \
    ../renderit.cpp \
    ../buildtree.cpp \
    ../imagefilters.cpp \
    ../renderitqt.cpp


unix|win32: LIBS += \
                    -lvtkGraphics\
                    -lvtkWidgets\
                    -lvtkRendering\
                    -lvtkImaging\
                    -lvtkCommon\
                    -lvtkFiltering\
                    -lvtkHybrid\
                    -lvtkVolumeRendering\
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

