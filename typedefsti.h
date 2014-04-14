#ifndef TYPEDEFSTI_H
#define TYPEDEFSTI_H
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkBinaryThinningImageFilter3D.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkConstantBoundaryCondition.h"
#include "itkNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkWhiteTopHatImageFilter.h"
#include "itkBlackTopHatImageFilter.h"
#include "itkGrayscaleDilateImageFilter.h"
#include "itkGrayscaleErodeImageFilter.h"
#include "itkConnectedThresholdImageFilter.h"
#include "itkVotingBinaryIterativeHoleFillingImageFilter.h"
#include "itkImageToVTKImageFilter.h"

typedef itk::Image< float,3 > ImageType;
typedef itk::ImageFileReader< ImageType > ReaderType;
typedef itk::ConstantBoundaryCondition< ImageType > BoundaryConditionType;
typedef itk::NeighborhoodIterator< ImageType > NeighborhoodIteratorType1;
typedef itk::NeighborhoodIterator< ImageType, BoundaryConditionType> NeighborhoodIteratorType;
typedef itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;
typedef itk::ImageFileWriter< ImageType >  WriterType;
typedef itk::DiscreteGaussianImageFilter< ImageType, ImageType> GaussianFilterType;
typedef itk::RescaleIntensityImageFilter< ImageType,ImageType> RescaleFilterType;
typedef itk::BinaryBallStructuringElement< float, 3> StructuringElementType;
typedef itk::WhiteTopHatImageFilter< ImageType, ImageType, StructuringElementType >WhiteFilterType;
typedef itk::BlackTopHatImageFilter< ImageType, ImageType, StructuringElementType >BlackFilterType;
typedef itk::GrayscaleDilateImageFilter <ImageType, ImageType, StructuringElementType>GrayscaleDilateImageFilterType;
typedef itk::GrayscaleErodeImageFilter <ImageType, ImageType, StructuringElementType>GrayscaleErodeImageFilterType;
typedef itk::ConnectedThresholdImageFilter< ImageType, ImageType > ConnectedFilterType;
typedef itk::VotingBinaryIterativeHoleFillingImageFilter<ImageType > HoleFilterType;
typedef itk::BinaryThinningImageFilter3D< ImageType, ImageType > ThinningFilterType;
typedef itk::ImageToVTKImageFilter< ImageType > Connector;

#include "itkHessianRecursiveGaussianImageFilter.h"
typedef itk::HessianRecursiveGaussianImageFilter< ImageType >                   HessianFilterType;
#include "itkHessian3DToVesselnessMeasureImageFilter.h"
typedef itk::Hessian3DToVesselnessMeasureImageFilter< float >                   VesselnessMeasureFilterType;
#include "itkImageRegionConstIterator.h"
typedef itk::ImageRegionConstIterator< ImageType >                              ConstIteratorType;
#include "itkImageRegionConstIteratorWithIndex.h"
typedef itk::ImageRegionIteratorWithIndex< ImageType>                           IteratorTypeWI;
#include "itkMinimumMaximumImageCalculator.h"
typedef itk::MinimumMaximumImageCalculator <ImageType>                          ImageCalculatorFilterType;
#endif // TYPEDEFSTI_H
