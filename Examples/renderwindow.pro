TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH +=  "../" \
                "../ThirdParty"

#INCLUDEPATH +=  "/usr/include/vtk-5.8"
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
    ../tree.cpp \
    ../image.cpp \
    renderwindow.cpp \
    ../treerender.cpp \
    ../renderit.cpp \
    ../buildtree.cpp \
    ../imagefilters.cpp


#vtk 5.8 libraries to link
#unix|win32: LIBS += \
#                    -lvtkGraphics\
#                    -lvtkWidgets\
#                    -lvtkRendering\
#                    -lvtkImaging\
#                    -lvtkCommon\
#                    -lvtkFiltering\
#                    -lvtkHybrid\
#                    -lvtkVolumeRendering\
#                    -lQVTK

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

#vtk 6.0 libraries to link
unix|win32: LIBS += -lvtkalglib-6.0 \
        -lvtkChartsCore-6.0 \
        -lvtkCommonColor-6.0 \
        -lvtkCommonDataModel-6.0 \
        -lvtkCommonMath-6.0 \
        -lvtkCommonCore-6.0 \
        -lvtksys-6.0 \
        -lvtkCommonMisc-6.0 \
        -lvtkCommonSystem-6.0 \
        -lvtkCommonTransforms-6.0 \
        -lvtkInfovisCore-6.0 \
        -lvtkFiltersExtraction-6.0 \
        -lvtkCommonExecutionModel-6.0 \
        -lvtkFiltersCore-6.0 \
        -lvtkFiltersGeneral-6.0 \
        -lvtkCommonComputationalGeometry-6.0 \
        -lvtkFiltersStatistics-6.0 \
        -lvtkImagingFourier-6.0 \
        -lvtkImagingCore-6.0 \
        -lvtkRenderingContext2D-6.0 \
        -lvtkRenderingCore-6.0 \
        -lvtkFiltersGeometry-6.0 \
        -lvtkFiltersSources-6.0 \
        -lvtkIOImage-6.0 \
        -lvtkDICOMParser-6.0 \
        -lvtkIOCore-6.0 \
        -lvtkmetaio-6.0 \
        -lvtkIOXMLParser-6.0 \
        -lvtkRenderingFreeType-6.0 \
        -lvtkftgl-6.0 \
        -lvtkRenderingOpenGL-6.0 \
        -lvtkImagingHybrid-6.0 \
        -lvtkDomainsChemistry-6.0 \
        -lvtkIOXML-6.0 \
        -lvtkIOGeometry-6.0 \
        -lvtkjsoncpp-6.0 \
        -lvtkexoIIc-6.0 \
        -lvtkNetCDF-6.0 \
        -lvtkNetCDF_cxx-6.0 \
        -lvtkFiltersAMR-6.0 \
        -lvtkParallelCore-6.0 \
        -lvtkIOLegacy-6.0 \
        -lvtkFiltersFlowPaths-6.0 \
        -lvtkFiltersGeneric-6.0 \
        -lvtkFiltersHybrid-6.0 \
        -lvtkImagingSources-6.0 \
        -lvtkFiltersHyperTree-6.0 \
        -lvtkFiltersImaging-6.0 \
        -lvtkImagingGeneral-6.0 \
        -lvtkFiltersModeling-6.0 \
        -lvtkFiltersParallel-6.0 \
        -lvtkFiltersParallelImaging-6.0 \
        -lvtkFiltersProgrammable-6.0 \
        -lvtkFiltersSelection-6.0 \
        -lvtkFiltersTexture-6.0 \
        -lvtkFiltersVerdict-6.0 \
        -lvtkGeovisCore-6.0 \
        -lvtkInfovisLayout-6.0 \
        -lvtkInteractionStyle-6.0 \
        -lvtkInteractionWidgets-6.0 \
        -lvtkRenderingAnnotation-6.0 \
        -lvtkImagingColor-6.0 \
        -lvtkRenderingVolume-6.0 \
        -lvtkViewsCore-6.0 \
        -lvtkproj4-6.0 \
        -lvtkGUISupportQt-6.0 \
        -lvtkGUISupportQtOpenGL-6.0 \
        -lvtkImagingMath-6.0 \
        -lvtkImagingMorphological-6.0 \
        -lvtkImagingStatistics-6.0 \
        -lvtkImagingStencil-6.0 \
        -lvtkInteractionImage-6.0 \
        -lvtkIOAMR-6.0 \
        -lvtkIOEnSight-6.0 \
        -lvtkIOExodus-6.0 \
        -lvtkIOExport-6.0 \
        -lvtkRenderingGL2PS-6.0 \
        -lvtkIOImport-6.0 \
        -lvtkIOInfovis-6.0 \
        -lvtkIOLSDyna-6.0 \
        -lvtkIOMINC-6.0 \
        -lvtkIOMovie-6.0 \
        -lvtkIONetCDF-6.0 \
        -lvtkIOParallel-6.0 \
        -lvtkIOPLY-6.0 \
        -lvtkIOSQL-6.0 \
        -lvtksqlite-6.0 \
        -lvtkIOVideo-6.0 \
        -lvtkRenderingFreeTypeOpenGL-6.0 \
        -lvtkRenderingHybridOpenGL-6.0 \
        -lvtkRenderingImage-6.0 \
        -lvtkRenderingLabel-6.0 \
        -lvtkRenderingLOD-6.0 \
        -lvtkRenderingVolumeAMR-6.0 \
        -lvtkRenderingVolumeOpenGL-6.0 \
        -lvtkViewsContext2D-6.0 \
        -lvtkViewsGeovis-6.0 \
        -lvtkViewsInfovis-6.0

