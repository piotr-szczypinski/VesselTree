TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH +=  "../" \
                "../ThirdParty"

INCLUDEPATH +=  "/usr/include/vtk-5.8"

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
    ../tree.cpp \
    ../image.cpp \
    renderwindow.cpp \
    ../treerender.cpp \
    ../renderit.cpp \
    ../buildtree.cpp \
    ../imagefilters.cpp

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
