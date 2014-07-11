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
    ../imagefilters.h \
    ../tree.h \
    ../buildtree.h \
#    ../image.h \
#    ../renderit.h \
#    ../typedefsti.h


SOURCES += \
    vknife.cpp \
    ../tree.cpp \
#    ../imagefilters.cpp
#    ../image.cpp \
#    ../treerender.cpp \
#    ../renderit.cpp \
#    ../buildtree.cpp \
#    ../imagefilters.cpp



# itk3
#unix|win32: LIBS += \
#                    -lITKBasicFilters\
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


#itk4
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




#vtk 5.8 libraries to link
unix|win32: LIBS += \
                    -lvtkGraphics\
                    -lvtkWidgets\
                    -lvtkRendering\
                    -lvtkImaging\
                    -lvtkCommon\
                    -lvtkFiltering\
                    -lvtkHybrid\
                    -lvtkVolumeRendering
#                    -lQVTK

#vtk 6.0 libraries to link
#unix|win32: LIBS += -lvtkalglib-6.0 \
#        -lvtkChartsCore-6.0 \
#        -lvtkCommonColor-6.0 \
#        -lvtkCommonDataModel-6.0 \
#        -lvtkCommonMath-6.0 \
#        -lvtkCommonCore-6.0 \
#        -lvtksys-6.0 \
#        -lvtkCommonMisc-6.0 \
#        -lvtkCommonSystem-6.0 \
#        -lvtkCommonTransforms-6.0 \
#        -lvtkInfovisCore-6.0 \
#        -lvtkFiltersExtraction-6.0 \
#        -lvtkCommonExecutionModel-6.0 \
#        -lvtkFiltersCore-6.0 \
#        -lvtkFiltersGeneral-6.0 \
#        -lvtkCommonComputationalGeometry-6.0 \
#        -lvtkFiltersStatistics-6.0 \
#        -lvtkImagingFourier-6.0 \
#        -lvtkImagingCore-6.0 \
#        -lvtkRenderingContext2D-6.0 \
#        -lvtkRenderingCore-6.0 \
#        -lvtkFiltersGeometry-6.0 \
#        -lvtkFiltersSources-6.0 \
#        -lvtkIOImage-6.0 \
#        -lvtkDICOMParser-6.0 \
#        -lvtkIOCore-6.0 \
#        -lvtkmetaio-6.0 \
#        -lvtkIOXMLParser-6.0 \
#        -lvtkRenderingFreeType-6.0 \
#        -lvtkftgl-6.0 \
#        -lvtkRenderingOpenGL-6.0 \
#        -lvtkImagingHybrid-6.0 \
#        -lvtkDomainsChemistry-6.0 \
#        -lvtkIOXML-6.0 \
#        -lvtkIOGeometry-6.0 \
#        -lvtkjsoncpp-6.0 \
#        -lvtkexoIIc-6.0 \
#        -lvtkNetCDF-6.0 \
#        -lvtkNetCDF_cxx-6.0 \
#        -lvtkFiltersAMR-6.0 \
#        -lvtkParallelCore-6.0 \
#        -lvtkIOLegacy-6.0 \
#        -lvtkFiltersFlowPaths-6.0 \
#        -lvtkFiltersGeneric-6.0 \
#        -lvtkFiltersHybrid-6.0 \
#        -lvtkImagingSources-6.0 \
#        -lvtkFiltersHyperTree-6.0 \
#        -lvtkFiltersImaging-6.0 \
#        -lvtkImagingGeneral-6.0 \
#        -lvtkFiltersModeling-6.0 \
#        -lvtkFiltersParallel-6.0 \
#        -lvtkFiltersParallelImaging-6.0 \
#        -lvtkFiltersProgrammable-6.0 \
#        -lvtkFiltersSelection-6.0 \
#        -lvtkFiltersTexture-6.0 \
#        -lvtkFiltersVerdict-6.0 \
#        -lvtkGeovisCore-6.0 \
#        -lvtkInfovisLayout-6.0 \
#        -lvtkInteractionStyle-6.0 \
#        -lvtkInteractionWidgets-6.0 \
#        -lvtkRenderingAnnotation-6.0 \
#        -lvtkImagingColor-6.0 \
#        -lvtkRenderingVolume-6.0 \
#        -lvtkViewsCore-6.0 \
#        -lvtkproj4-6.0 \
#        -lvtkGUISupportQt-6.0 \
#        -lvtkGUISupportQtOpenGL-6.0 \
#        -lvtkImagingMath-6.0 \
#        -lvtkImagingMorphological-6.0 \
#        -lvtkImagingStatistics-6.0 \
#        -lvtkImagingStencil-6.0 \
#        -lvtkInteractionImage-6.0 \
#        -lvtkIOAMR-6.0 \
#        -lvtkIOEnSight-6.0 \
#        -lvtkIOExodus-6.0 \
#        -lvtkIOExport-6.0 \
#        -lvtkRenderingGL2PS-6.0 \
#        -lvtkIOImport-6.0 \
#        -lvtkIOInfovis-6.0 \
#        -lvtkIOLSDyna-6.0 \
#        -lvtkIOMINC-6.0 \
#        -lvtkIOMovie-6.0 \
#        -lvtkIONetCDF-6.0 \
#        -lvtkIOParallel-6.0 \
#        -lvtkIOPLY-6.0 \
#        -lvtkIOSQL-6.0 \
#        -lvtksqlite-6.0 \
#        -lvtkIOVideo-6.0 \
#        -lvtkRenderingFreeTypeOpenGL-6.0 \
#        -lvtkRenderingHybridOpenGL-6.0 \
#        -lvtkRenderingImage-6.0 \
#        -lvtkRenderingLabel-6.0 \
#        -lvtkRenderingLOD-6.0 \
#        -lvtkRenderingVolumeAMR-6.0 \
#        -lvtkRenderingVolumeOpenGL-6.0 \
#        -lvtkViewsContext2D-6.0 \
#        -lvtkViewsGeovis-6.0 \
#        -lvtkViewsInfovis-6.0

