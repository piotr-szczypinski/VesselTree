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
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkHessian3DToVesselnessMeasureImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkMinimumMaximumImageCalculator.h"

typedef itk::Image< float, 3 > ImageType;
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
typedef itk::HessianRecursiveGaussianImageFilter< ImageType > HessianFilterType;
typedef itk::Hessian3DToVesselnessMeasureImageFilter< float > VesselnessMeasureFilterType;
typedef itk::ImageRegionConstIterator< ImageType > ConstIteratorType;
typedef itk::ImageRegionIteratorWithIndex< ImageType > IteratorTypeWI;
typedef itk::MinimumMaximumImageCalculator < ImageType > ImageCalculatorFilterType;
/*
typedef itk::Image< float, 3 > ImageTypeF32;
typedef itk::ImageFileReader< ImageTypeF32 > ReaderType;
typedef itk::ConstantBoundaryCondition< ImageTypeF32 > BoundaryConditionType;
typedef itk::NeighborhoodIterator< ImageTypeF32 > NeighborhoodIteratorType1;
typedef itk::NeighborhoodIterator< ImageTypeF32, BoundaryConditionType> NeighborhoodIteratorType;
typedef itk::ImageRegionIteratorWithIndex< ImageTypeF32 > IteratorType;
typedef itk::ImageFileWriter< ImageTypeF32 >  WriterType;
typedef itk::DiscreteGaussianImageFilter< ImageTypeF32, ImageTypeF32> GaussianFilterType;
typedef itk::RescaleIntensityImageFilter< ImageTypeF32,ImageTypeF32> RescaleFilterType;
typedef itk::BinaryBallStructuringElement< float, 3> StructuringElementType;
typedef itk::WhiteTopHatImageFilter< ImageTypeF32, ImageTypeF32, StructuringElementType >WhiteFilterType;
typedef itk::BlackTopHatImageFilter< ImageTypeF32, ImageTypeF32, StructuringElementType >BlackFilterType;
typedef itk::GrayscaleDilateImageFilter <ImageTypeF32, ImageTypeF32, StructuringElementType>GrayscaleDilateImageFilterType;
typedef itk::GrayscaleErodeImageFilter <ImageTypeF32, ImageTypeF32, StructuringElementType>GrayscaleErodeImageFilterType;
typedef itk::ConnectedThresholdImageFilter< ImageTypeF32, ImageTypeF32 > ConnectedFilterType;
typedef itk::VotingBinaryIterativeHoleFillingImageFilter<ImageTypeF32 > HoleFilterType;
typedef itk::BinaryThinningImageFilter3D< ImageTypeF32, ImageTypeF32 > ThinningFilterType;
typedef itk::ImageToVTKImageFilter< ImageTypeF32 > Connector;
typedef itk::HessianRecursiveGaussianImageFilter< ImageTypeF32 > HessianFilterType;
typedef itk::Hessian3DToVesselnessMeasureImageFilter< float > VesselnessMeasureFilterType;
typedef itk::ImageRegionConstIterator< ImageTypeF32 > ConstIteratorType;
typedef itk::ImageRegionIteratorWithIndex< ImageTypeF32 > IteratorTypeWI;
typedef itk::MinimumMaximumImageCalculator < ImageTypeF32 > ImageCalculatorFilterType;

typedef itk::Image< u_int16_t, 3 > ImageTypeU16;
typedef itk::ImageFileReader< ImageTypeU16 > ReaderType;
typedef itk::ConstantBoundaryCondition< ImageTypeU16 > BoundaryConditionType;
typedef itk::NeighborhoodIterator< ImageTypeU16 > NeighborhoodIteratorType1;
typedef itk::NeighborhoodIterator< ImageTypeU16, BoundaryConditionType> NeighborhoodIteratorType;
typedef itk::ImageRegionIteratorWithIndex< ImageTypeU16 > IteratorType;
typedef itk::ImageFileWriter< ImageTypeU16 >  WriterType;
typedef itk::DiscreteGaussianImageFilter< ImageTypeU16, ImageTypeU16> GaussianFilterType;
typedef itk::RescaleIntensityImageFilter< ImageTypeU16,ImageTypeU16> RescaleFilterType;
typedef itk::BinaryBallStructuringElement< u_int16_t, 3> StructuringElementType;
typedef itk::WhiteTopHatImageFilter< ImageTypeU16, ImageTypeU16, StructuringElementType >WhiteFilterType;
typedef itk::BlackTopHatImageFilter< ImageTypeU16, ImageTypeU16, StructuringElementType >BlackFilterType;
typedef itk::GrayscaleDilateImageFilter <ImageTypeU16, ImageTypeU16, StructuringElementType>GrayscaleDilateImageFilterType;
typedef itk::GrayscaleErodeImageFilter <ImageTypeU16, ImageTypeU16, StructuringElementType>GrayscaleErodeImageFilterType;
typedef itk::ConnectedThresholdImageFilter< ImageTypeU16, ImageTypeU16 > ConnectedFilterType;
typedef itk::VotingBinaryIterativeHoleFillingImageFilter<ImageTypeU16 > HoleFilterType;
typedef itk::BinaryThinningImageFilter3D< ImageTypeU16, ImageTypeU16 > ThinningFilterType;
typedef itk::ImageToVTKImageFilter< ImageTypeU16 > Connector;
typedef itk::HessianRecursiveGaussianImageFilter< ImageTypeU16 > HessianFilterType;
typedef itk::Hessian3DToVesselnessMeasureImageFilter< u_int16_t > VesselnessMeasureFilterType;
typedef itk::ImageRegionConstIterator< ImageTypeU16 > ConstIteratorType;
typedef itk::ImageRegionIteratorWithIndex< ImageTypeU16 > IteratorTypeWI;
typedef itk::MinimumMaximumImageCalculator < ImageTypeU16 > ImageCalculatorFilterType;

typedef itk::Image< u_int8_t, 3 > ImageTypeU8;
typedef itk::ImageFileReader< ImageTypeU8 > ReaderType;
typedef itk::ConstantBoundaryCondition< ImageTypeU8 > BoundaryConditionType;
typedef itk::NeighborhoodIterator< ImageTypeU8 > NeighborhoodIteratorType1;
typedef itk::NeighborhoodIterator< ImageTypeU8, BoundaryConditionType> NeighborhoodIteratorType;
typedef itk::ImageRegionIteratorWithIndex< ImageTypeU8 > IteratorType;
typedef itk::ImageFileWriter< ImageTypeU8 >  WriterType;
typedef itk::DiscreteGaussianImageFilter< ImageTypeU8, ImageTypeU8> GaussianFilterType;
typedef itk::RescaleIntensityImageFilter< ImageTypeU8,ImageTypeU8> RescaleFilterType;
typedef itk::BinaryBallStructuringElement< u_int8_t, 3> StructuringElementType;
typedef itk::WhiteTopHatImageFilter< ImageTypeU8, ImageTypeU8, StructuringElementType >WhiteFilterType;
typedef itk::BlackTopHatImageFilter< ImageTypeU8, ImageTypeU8, StructuringElementType >BlackFilterType;
typedef itk::GrayscaleDilateImageFilter <ImageTypeU8, ImageTypeU8, StructuringElementType>GrayscaleDilateImageFilterType;
typedef itk::GrayscaleErodeImageFilter <ImageTypeU8, ImageTypeU8, StructuringElementType>GrayscaleErodeImageFilterType;
typedef itk::ConnectedThresholdImageFilter< ImageTypeU8, ImageTypeU8 > ConnectedFilterType;
typedef itk::VotingBinaryIterativeHoleFillingImageFilter<ImageTypeU8 > HoleFilterType;
typedef itk::BinaryThinningImageFilter3D< ImageTypeU8, ImageTypeU8 > ThinningFilterType;
typedef itk::ImageToVTKImageFilter< ImageTypeU8 > Connector;
typedef itk::HessianRecursiveGaussianImageFilter< ImageTypeU8 > HessianFilterType;
typedef itk::Hessian3DToVesselnessMeasureImageFilter< u_int8_t > VesselnessMeasureFilterType;
typedef itk::ImageRegionConstIterator< ImageTypeU8 > ConstIteratorType;
typedef itk::ImageRegionIteratorWithIndex< ImageTypeU8 > IteratorTypeWI;
typedef itk::MinimumMaximumImageCalculator < ImageTypeU8 > ImageCalculatorFilterType;
*/
#endif // TYPEDEFSTI_H
