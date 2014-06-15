TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH +=  "../" \
                "../ThirdParty"

INCLUDEPATH +=  "/usr/include/vtk-5.8"
#INCLUDEPATH +=  "/usr/include/vtk-6.0"


#INCLUDEPATH +=  "/usr/include/InsightToolkit/Common"\
#                "/usr/include/InsightToolkit"\
#                "/usr/include/InsightToolkit/Utilities/vxl/vcl"\
#                "/usr/include/InsightToolkit/Utilities/vxl/core"\
#                "/usr/include/InsightToolkit/IO"\
#                "/usr/include/InsightToolkit/Utilities"\
#                "/usr/include/InsightToolkit/BasicFilters"\
#                "/usr/include/InsightToolkit/Review"



INCLUDEPATH +=  "/usr/include/ITK-4.5/Common"\
                "/usr/include/ITK-4.5"\
                "/usr/include/ITK-4.5/Utilities/vxl/vcl"\
                "/usr/include/ITK-4.5/Utilities/vxl/core"\
                "/usr/include/ITK-4.5/IO"\
                "/usr/include/ITK-4.5/Utilities"\
                "/usr/include/ITK-4.5/BasicFilters"\
                "/usr/include/ITK-4.5/Review"


HEADERS += \
    ../tree.h \
    ../buildtree.h \
    ../image.h \
    registerfactories.h

SOURCES += \
    createtree.cpp \
    ../tree.cpp \
    ../buildtree.cpp \
    ../image.cpp

unix|win32: LIBS += \
-litksys-4.5\
-lITKBiasCorrection-4.5\
-lITKBioCell-4.5\
-lITKCommon-4.5\
-lITKDICOMParser-4.5\
-litkdouble-conversion-4.5\
-lITKEXPAT-4.5\
-lITKFEM-4.5\
-litkhdf5_cpp-4.5\
-litkhdf5-4.5\
-lITKIOBioRad-4.5\
-lITKIOBMP-4.5\
-lITKIOCSV-4.5\
-lITKIOGDCM-4.5\
-lITKIOGE-4.5\
-lITKIOGIPL-4.5\
-lITKIOHDF5-4.5\
-lITKIOImageBase-4.5\
-lITKIOIPL-4.5\
-lITKIOJPEG-4.5\
-lITKIOLSM-4.5\
-lITKIOMesh-4.5\
-lITKIOMeta-4.5\
-lITKIOMRC-4.5\
-lITKIONIFTI-4.5\
-lITKIONRRD-4.5\
-lITKIOPNG-4.5\
-lITKIOSiemens-4.5\
-lITKIOSpatialObjects-4.5\
-lITKIOStimulate-4.5\
-lITKIOTIFF-4.5\
-lITKIOTransformBase-4.5\
-lITKIOTransformHDF5-4.5\
-lITKIOTransformInsightLegacy-4.5\
-lITKIOTransformMatlab-4.5\
-lITKIOVTK-4.5\
-lITKIOXML-4.5\
-lITKKLMRegionGrowing-4.5\
-lITKLabelMap-4.5\
-lITKMesh-4.5\
-lITKMetaIO-4.5\
-litkNetlibSlatec-4.5\
-lITKniftiio-4.5\
-lITKNrrdIO-4.5\
-litkopenjpeg-4.5\
-lITKOptimizers-4.5\
-lITKPath-4.5\
-lITKPolynomials-4.5\
-lITKQuadEdgeMesh-4.5\
-lITKSpatialObjects-4.5\
-lITKStatistics-4.5\
-litkv3p_lsqr-4.5\
-litkv3p_netlib-4.5\
-litkvcl-4.5\
-lITKVideoCore-4.5\
-lITKVideoIO-4.5\
-litkvnl_algo-4.5\
-litkvnl-4.5\
-lITKVNLInstantiation-4.5\
-lITKVTK-4.5\
-lITKWatersheds-4.5\
-lITKznz-4.5





unix|win32: LIBS += \
                    -lvtkGraphics\
                    -lvtkWidgets\
                    -lvtkRendering\
                    -lvtkImaging\
                    -lvtkCommon\
                    -lvtkFiltering\
                    -lvtkHybrid
#                    -lQVTK\

#unix|win32: LIBS += \
#                    -lITKIO\
#                    -lITKCommon\
#                    -lITKDICOMParser\
#                    -lITKEXPAT\
#                    -lITKFEM\
#                    -lITKMetaIO\
#                    -litkNetlibSlatec\
#                    -lITKniftiio\
#                    -lITKNrrdIO\
#                    -lITKStatistics\
#                    -lITKQuadEdgeMesh\
#                    -lITKStatistics\
#                    -litksys\
#                    -litkv3p_lsqr\
#                    -litkv3p_netlib\
#                    -litkvcl\
#                    -litkvnl\
#                    -litkvnl_algo\
#                    -lITKznz\
#                    -lITKAlgorithms\
#                    -lITKNumerics\
#                    -litksys\
#                    -lITKAlgorithms
