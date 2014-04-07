/*
 * VesselTree - MRI image segmentation and characterization
 *
 * Copyright 2014  Piotr M. Szczypiński <piotr.szczypinski@p.lodz.pl>
 *                 Adam Sankowski <sankowski@gmail.com>
 *                 Grzegorz Dwojakowski <grzegorz.dwojakowski@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef BUILDTREE_H
#define BUILDTREE_H


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
#include "treeskel.h"
#include "obraz.h"
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
//----------------------------------------------------------------------------------------
strukturaObrazu itkImageToStructure(ImageType::Pointer par1);

ImageType::Pointer StructureToItkImage(strukturaObrazu par1);

strukturaObrazu openAnalyzeImage(std::string par1);

strukturaObrazu gaussianFilter(strukturaObrazu par1, float par2);

void saveImage(strukturaObrazu par1, std::string par2);

strukturaObrazu reskalowanie (strukturaObrazu par1, float par2, float par3);

strukturaObrazu wth (strukturaObrazu par1, float par2);

strukturaObrazu bth (strukturaObrazu par1, float par2);

strukturaObrazu dylatacja (strukturaObrazu par1, float par2);

strukturaObrazu erozja (strukturaObrazu par1, float par2);

strukturaObrazu rozrost (strukturaObrazu par1, float par2, float par3, float par4, float par5, float par6);

strukturaObrazu rozrost_automatyczny (strukturaObrazu par1, float par2, float par3);

strukturaObrazu wypelnianie (strukturaObrazu par1, float par2, float par3);

strukturaObrazu pocienianie (strukturaObrazu par1);

TreeSkeleton szacowanie_polaczen(strukturaObrazu par1);

TreeSkeleton skeletonToTree(strukturaObrazu inputImage);

TreeSkeleton szacowanie_srednicy(strukturaObrazu par1, TreeSkeleton par2);


#endif // BUILDTREE_H
