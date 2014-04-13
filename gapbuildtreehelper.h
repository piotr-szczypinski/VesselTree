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

/*
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
#include "treeskel.h"
#include "obraz.h"
*/

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
typedef itk::ImageToVTKImageFilter< ImageType > Connector;
//----------------------------------------------------------------------------------------


struct Index3DNeighbor
{
    ImageType::IndexType i;
    int n;
};

const int neighborhoodScanSequence[27][3] =
{
    {0,0,0}, //0 center
    {0,0,1},{0,0,-1},{0,1,0},{0,-1,0},{1,0,0},{-1,0,0}, //1-6 facet neighbor
    {0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1},{1,1,0},{1,-1,0},{-1,1,0},{-1,-1,0},{1,0,1},{1,0,-1},{-1,0,1},{-1,0,-1}, //7-18 edge neighbor
    {1,1,1},{1,1,-1},{1,-1,1},{1,-1,-1},{-1,1,1},{-1,1,-1},{-1,-1,1},{-1,-1,-1} //19-26 vertex neighbor
};

//----------------------------------------------------------------------------------------
/** \classt GapBuildTreeHelpers
 *  \brief GapBuildTreeHelpers jest klasą funkcji pomocniczych do szkieletyzacji obrazu i budowania drzewa.
 */
class GapBuildTreeHelpers
{
public:

    static strukturaObrazu itkImageToStructure(ImageType::Pointer par1);

    static ImageType::Pointer StructureToItkImage(strukturaObrazu par1);

    static strukturaObrazu openAnalyzeImage(std::string par1);

    static strukturaObrazu gaussianFilter(strukturaObrazu par1, float par2);

    static void saveImage(strukturaObrazu par1, std::string par2);

    static strukturaObrazu reskalowanie (strukturaObrazu par1, float par2, float par3);

    static strukturaObrazu wth (strukturaObrazu par1, float par2);

    static strukturaObrazu bth (strukturaObrazu par1, float par2);

    static strukturaObrazu dylatacja (strukturaObrazu par1, float par2);

    static strukturaObrazu erozja (strukturaObrazu par1, float par2);

    static strukturaObrazu rozrost (strukturaObrazu par1, float par2, float par3, float par4, float par5, float par6);

    static strukturaObrazu rozrost_automatyczny (strukturaObrazu par1, float par2, float par3);

    static strukturaObrazu wypelnianie (strukturaObrazu par1, float par2, float par3);

    static strukturaObrazu pocienianie (strukturaObrazu par1);

    static TreeSkeleton szacowanie_polaczen(strukturaObrazu par1);

/** Analizuje obraz rastrowy ze szkieletem i buduje drzewo.
 * \param image wejściowy obraz rastrowy, voksele szkieletu dane liczbami większymi od zera pozostałe wyzeorwane.
 * \returns zwrace klasę zbudowanego drzewa
 * \author Piotr M. Szczypiński
*/
    static TreeSkeleton skeletonToTree(strukturaObrazu image);

    static TreeSkeleton szacowanie_srednicy(strukturaObrazu par1, TreeSkeleton par2);

    static strukturaObrazu rescaleIntensity( strukturaObrazu par1, float min = 0, float max = 255 );
};
#endif // BUILDTREE_H
