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

#include "typedefsti.h"
#include "tree.h"
#include "image.h"

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
/** \classt BuildTree
 *  \brief BuildTree jest klasą funkcji pomocniczych do szkieletyzacji obrazu i budowania drzewa.
 */
class BuildTree
{
public:

    static ImageStructure itkImageToStructure(ImageType::Pointer par1);

    static ImageType::Pointer StructureToItkImage(ImageStructure par1);

    static ImageStructure openAnalyzeImage(std::string par1);

    static ImageStructure gaussianFilter(ImageStructure par1, float par2);

    static void saveImage(ImageStructure par1, std::string par2);

    static ImageStructure reskalowanie (ImageStructure par1, float par2, float par3);

    static ImageStructure wth (ImageStructure par1, float par2);

    static ImageStructure bth (ImageStructure par1, float par2);

    static ImageStructure dylatacja (ImageStructure par1, float par2);

    static ImageStructure erozja (ImageStructure par1, float par2);

    static ImageStructure rozrost (ImageStructure par1, float par2, float par3, float par4, float par5, float par6);

    static ImageStructure rozrost_automatyczny (ImageStructure par1, float par2, float par3);

    static ImageStructure wypelnianie (ImageStructure par1, float par2, float par3);

    //static ImageStructure pocienianie (ImageStructure par1);

    static Tree szacowanie_polaczen(ImageStructure par1);

/** Analizuje obraz rastrowy ze szkieletem i buduje drzewo.
 * \param image wejściowy obraz rastrowy, voksele szkieletu dane liczbami większymi od zera pozostałe wyzeorwane.
 * \returns zwrace klasę zbudowanego drzewa
 * \author Piotr M. Szczypiński
*/
    static Tree skeletonToTree(ImageStructure image);

    static Tree skeletonToTreeIntSpace(ImageStructure image);

    static Tree szacowanie_srednicy(ImageStructure par1, Tree par2);

    static ImageStructure rescaleIntensity( ImageStructure par1, float min = 0, float max = 255 );

    static ImageStructure create2xBiggerImage(ImageStructure input);

    static ImageStructure upscaleForCenteredSkeleton(ImageStructure input);

    static ImageStructure skeletonFromBinary(ImageStructure par1);


};
#endif // BUILDTREE_H
