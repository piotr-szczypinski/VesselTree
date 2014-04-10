QT       += core gui svg printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TEMPLATE = app
#CONFIG += console
#CONFIG -= app_bundle
#CONFIG -= qt

INCLUDEPATH +=  "/usr/include/vtk-5.8"


#INCLUDEPATH +=  "/usr/include/ITK-4.3/Common"\
#                "/usr/include/ITK-4.3"\
#                "/usr/include/ITK-4.3/Utilities/vxl/vcl"\
#                "/usr/include/ITK-4.3/Utilities/vxl/core"\
#                "/usr/include/ITK-4.3/IO"\
#                "/usr/include/ITK-4.3/Utilities"\
#                "/usr/include/ITK-4.3/BasicFilters"\
#                "/usr/include/ITK-4.3/Review"

INCLUDEPATH +=  "/usr/include/InsightToolkit/Common"\
                "/usr/include/InsightToolkit"\
                "/usr/include/InsightToolkit/Utilities/vxl/vcl"\
                "/usr/include/InsightToolkit/Utilities/vxl/core"\
                "/usr/include/InsightToolkit/IO"\
                "/usr/include/InsightToolkit/Utilities"\
                "/usr/include/InsightToolkit/BasicFilters"\
                "/usr/include/InsightToolkit/Review"

HEADERS += \
    treeskel.h \
    buildtree.h \
    obraz.h \
    widget.h \
    mainWindow.h \
    itkFunctions.h
#    gapsurface.h

SOURCES += \
    treeskel.cpp \
    buildtree.cpp \
    obraz.cpp \
    main.cxx \
    widget.cpp \
    mainWindow.cxx \
    itkFunctions.cxx
#    gapsurface.cpp

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

#unix|win32: LIBS += \
#                    -lITKIO-4\
#                    -lITKCommon-4\
#                    -lITKDICOMParser-4\
#                    -lITKEXPAT-4\
#                    -lITKFEM-4\
#                    -lITKMetaIO-4\
#                    -litkNetlibSlatec-4\
#                    -lITKniftiio-4\
#                    -lITKNrrdIO-4\
#                    -lITKStatistics-4\
#                    -lITKQuadEdgeMesh-4\
#                    -lITKStatistics-4\
#                    -litksys-4\
#                    -litkv3p_lsqr-4\
#                    -litkv3p_netlib-4\
#                    -litkvcl-4\
#                    -litkvnl-4\
#                    -litkvnl_algo-4\
#                    -lITKznz-4\
#                    -lITKAlgorithms-4\
#                    -lITKNumerics-4\
#                    -litksys-4\
#                    -lITKAlgorithms-4

FORMS += \
    widget.ui \
    mainWindow.ui






unix|win32: LIBS += -lvtkWidgets
