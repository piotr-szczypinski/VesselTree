#include "obraz.h"

#include "itkImage.h"
typedef itk::Image< float, 3 >                                                  ImageType;
#include "itkImageFileReader.h"
typedef itk::ImageFileReader< ImageType >                                       ReaderType;
#include "itkImageFileWriter.h"
typedef itk::ImageFileWriter< ImageType >                                       WriterType;
#include "itkDiscreteGaussianImageFilter.h"
typedef itk::DiscreteGaussianImageFilter< ImageType, ImageType>                 GaussianFilterType;
#include "itkHessianRecursiveGaussianImageFilter.h"
typedef itk::HessianRecursiveGaussianImageFilter< ImageType >                   HessianFilterType;
#include "itkHessian3DToVesselnessMeasureImageFilter.h"
typedef itk::Hessian3DToVesselnessMeasureImageFilter< float >                   VesselnessMeasureFilterType;
#include "itkImageRegionConstIterator.h"
typedef itk::ImageRegionConstIterator< ImageType >                              ConstIteratorType;
#include "itkImageRegionIterator.h"
typedef itk::ImageRegionIterator< ImageType>                                    IteratorType;
#include "itkImageRegionConstIteratorWithIndex.h"
typedef itk::ImageRegionIteratorWithIndex< ImageType>                           IteratorTypeWI;
#include "itkConnectedThresholdImageFilter.h"
typedef itk::ConnectedThresholdImageFilter< ImageType, ImageType >              ConnectedFilterType;
#include "itkMinimumMaximumImageCalculator.h"
typedef itk::MinimumMaximumImageCalculator <ImageType>                          ImageCalculatorFilterType;
#include "itkImageToVTKImageFilter.h"
typedef itk::ImageToVTKImageFilter< ImageType >                                 Connector;
#include "itkRescaleIntensityImageFilter.h"
typedef itk::RescaleIntensityImageFilter< ImageType,ImageType>                  RescaleFilterType;

class itkFunctions
{
private:

public:
    strukturaObrazu itkImageToStructure(ImageType::Pointer par1);
    ImageType::Pointer StructureToItkImage(strukturaObrazu par1);
    strukturaObrazu openAnalyzeImage(std::string par1);
    strukturaObrazu rescaleIntensity( strukturaObrazu par1, float min = 0, float max = 255 );
    strukturaObrazu gaussianFilter(strukturaObrazu par1, float par2);
    strukturaObrazu hessianFilter(strukturaObrazu par1, float par2);
    strukturaObrazu mipTwoImages(strukturaObrazu par1, strukturaObrazu par2, float par3);
    strukturaObrazu RegionGrowing(strukturaObrazu par1, float par2, float par3, std::vector<unsigned int> coord);
    strukturaObrazu CreateEmptyStructure(strukturaObrazu par1);
    strukturaObrazu MultiscaleHessianAlgorithm(strukturaObrazu par1, float sigmaMin, float sigmaMax, int noOfScales);
    std::vector<unsigned int> FindSeed(strukturaObrazu par1);
    double FindMaximumValue(strukturaObrazu par1);
    double FindMinimumValue(strukturaObrazu par1);
    void saveImage(strukturaObrazu par1, std::string par2);
    strukturaObrazu HVSalgorithm(strukturaObrazu par1, int noOfScales, float thresholdPercent);
};
