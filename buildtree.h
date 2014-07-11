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

#include <itkImage.h>
#include <itkNeighborhoodIterator.h>
#include <itkConstantBoundaryCondition.h>
#include <itkImageDuplicator.h>
#include <limits>
#include "tree.h"


const int neighborhoodScanSequence[27][3] =
{
    {0,0,0}, //0 center
    {0,0,1},{0,0,-1},{0,1,0},{0,-1,0},{1,0,0},{-1,0,0}, //1-6 facet neighbor
    {0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1},{1,1,0},{1,-1,0},{-1,1,0},{-1,-1,0},{1,0,1},{1,0,-1},{-1,0,1},{-1,0,-1}, //7-18 edge neighbor
    {1,1,1},{1,1,-1},{1,-1,1},{1,-1,-1},{-1,1,1},{-1,1,-1},{-1,-1,1},{-1,-1,-1} //19-26 vertex neighbor
};

//----------------------------------------------------------------------------------------
/** \classt BuildTree
 *  \brief BuildTree jest klasą funkcji budowania drzewa o opisie wektorowym na podstawie rastrowego obrazu szkiletu
 */


template <class T> class BuildTree
{
public:
    typedef itk::Image< T, 3 > ImageType;

    struct Index3DNeighbor
    {
        typename ImageType::IndexType i;
        int n;
    };


    /**
     * @brief skeletonToTree analizuje obraz rastrowy ze szkieletem i buduje na jego podstawie drzewo
     * @param image wejściowy obraz rastrowy, woksele szkieletu muszą być oznaczone liczbami większymi od zera pozostałe muszą być wyzeorwane
     * @return zwrace obiekt klasy drzewa ze współrzędnymi będącymi indeksami wokseli
     */
    static Tree skeletonToTreeIntSpace(typename ImageType::Pointer input_image)
    {
        typedef itk::ConstantBoundaryCondition< ImageType > BoundaryConditionType;
        typedef itk::NeighborhoodIterator< ImageType, BoundaryConditionType> NeighborhoodIteratorType;
        typename NeighborhoodIteratorType::RadiusType radius;
        radius[0]=1; radius[1]=1; radius[2]=1;
        typename ImageType::IndexType regionIndex;
        regionIndex[0]=0;regionIndex[1]=0;regionIndex[2]=0;
        typename ImageType::SizeType regionSize = input_image->GetRequestedRegion().GetSize();
        typename ImageType::RegionType region;
        region.SetSize(regionSize);
        region.SetIndex(regionIndex);
        itk::NeighborhoodIterator<ImageType> iteratorStart(radius, input_image, region);

        int sc = iteratorStart.Size() / 2;
        int sy = iteratorStart.GetStride(1);
        int sz = iteratorStart.GetStride(2);

        for(iteratorStart.GoToBegin(); !iteratorStart.IsAtEnd(); ++iteratorStart)
        {
            int neighbors = 0;
            unsigned int i = 1;
            if(iteratorStart.GetCenterPixel() <= 0) continue;
            for(; i < 7; i++)
            {
                const int* nss = neighborhoodScanSequence[i];
                if(iteratorStart.GetPixel(sc+nss[0]+nss[1]*sy+nss[2]*sz) > 0) neighbors++;
            }
            if(neighbors == 1) break;
            neighbors = 0;
            for(; i < 19; i++)
            {
                const int* nss = neighborhoodScanSequence[i];
                if(iteratorStart.GetPixel(sc+nss[0]+nss[1]*sy+nss[2]*sz) > 0) neighbors++;
            }
            if(neighbors == 1) break;
            neighbors = 0;
            for(; i < 27; i++)
            {
                const int* nss = neighborhoodScanSequence[i];
                if(iteratorStart.GetPixel(sc+nss[0]+nss[1]*sy+nss[2]*sz) > 0) neighbors++;
            }
            if(neighbors == 1) break;
        }
        typename ImageType::IndexType startNodeIndex = iteratorStart.GetIndex();
    // Now branch tip is available from startNodeIndex
    //    std::cout << startNodeIndex << std::endl;

    // Copy image to a buffer
        typedef itk::ImageDuplicator< ImageType > DuplicatorType;
        typename DuplicatorType::Pointer duplicator = DuplicatorType::New();
        duplicator->SetInputImage(input_image);
        duplicator->Update();
        typename ImageType::Pointer bufferImage = duplicator->GetOutput();

    // Branch tracking based on flood-fill algorithm with stack
        Tree treeToCreate;
        std::vector<NodeIn3D> newBranch;
        std::vector<Index3DNeighbor> bifurStack;
        NodeIn3D newNode;
        Index3DNeighbor newNodeIndex;
        newNodeIndex.i = startNodeIndex;
        newNodeIndex.n = 1;
        newNode.connections = 0;
        newNode.diameter = 0;
        bifurStack.push_back(newNodeIndex);
        bufferImage->SetPixel(newNodeIndex.i, 0);

        while(bifurStack.size() > 0)
        {
            if(bifurStack.back().n >= 27)
            {
                bifurStack.pop_back();
                if(newBranch.size() > 0)
                {
                    treeToCreate.addBranch(newBranch);
                    newBranch.clear();
                }
            }
            else
            {
                regionIndex = bifurStack.back().i;
                region.SetIndex(regionIndex);
                itk::NeighborhoodIterator<ImageType> iteratorN(radius, bufferImage, region);

                for(; bifurStack.back().n < 27; bifurStack.back().n++)
                {
                    const int* nss = neighborhoodScanSequence[bifurStack.back().n];
                    int offset = sc+nss[0]+nss[1]*sy+nss[2]*sz;
                    if(iteratorN.GetPixel(offset) > 0)
                    {
                        if(newBranch.size() <= 0)
                        {
                            newNodeIndex.i = iteratorN.GetIndex(sc);
                            newNode.x = newNodeIndex.i[0];
                            newNode.y = newNodeIndex.i[1];
                            newNode.z = newNodeIndex.i[2];
                            newBranch.push_back(newNode);
    //                        std::cout << newNodeIndex.i << std::endl;
                        }

                        newNodeIndex.i = iteratorN.GetIndex(offset);
                        newNodeIndex.n = 1;
                        bifurStack.push_back(newNodeIndex);

                        typename ImageType::IndexType pindex = iteratorN.GetIndex(offset);
                        bufferImage->SetPixel(pindex, 0);
                        newNode.x = pindex[0];
                        newNode.y = pindex[1];
                        newNode.z = pindex[2];
                        newBranch.push_back(newNode);
    //                    std::cout << newNodeIndex.i << std::endl;
                        break;
                    }
                }
            }
        }
        return treeToCreate;
    }


    /**
     * @brief skeletonToTree analizuje obraz rastrowy ze szkieletem i buduje na jego podstawie drzewo
     * @param image wejściowy obraz rastrowy, woksele szkieletu muszą być oznaczone liczbami większymi od zera pozostałe muszą być wyzeorwane
     * @return zwrace obiekt klasy drzewa ze współrzędnymi rzeczywistymi
     */
    static Tree skeletonToTree(typename ImageType::Pointer image)
    {
        int i, imax;
        typename ImageType::SpacingType spacing;

        imax = image->GetLargestPossibleRegion().GetImageDimension();
        for(i = 0; i<imax; i++)
        {
            spacing[i] = image->GetSpacing()[i];
        }
        Tree tree = skeletonToTreeIntSpace(image);
        imax = tree.nodeCount();
        for(i = 0; i<imax; i++)
        {
            NodeIn3D n = tree.node(i);
            n.x *= spacing[0];
            n.y *= spacing[1];
            n.z *= spacing[2];
            tree.setNode(n, i);
        }
        return tree;
    }


    /**
     * @brief estimateDiameters szacuje średnicę gałęzi drzewa w jego węzłach
     * @param tree drzewo
     * @param image binarny obraz rastrowy drzewa
     */
    static void estimateDiameters(Tree* tree, typename ImageType::Pointer image)
    {
        typedef typename itk::ImageRegionConstIterator< ImageType > ConstIteratorType;
        int i, imax;
        imax = tree->nodeCount();
        double* radius = new double[imax];
        for(i = 0; i<imax; i++) radius[i] = std::numeric_limits<double>::max();
        typename ImageType::SpacingType spacing = image->GetSpacing();
        ConstIteratorType inputIterator(image, image->GetRequestedRegion());
        for(inputIterator.GoToBegin(); !inputIterator.IsAtEnd(); ++inputIterator)
        {
            if(inputIterator.Get() <= 0)
            {
                typename ImageType::IndexType coords = inputIterator.GetIndex();
                for(i = 0; i<imax; i++)
                {
                    NodeIn3D n = tree->node(i);
                    double dx, dy, dz, d;
                    dx = (n.x - coords[0]) * spacing[0];
                    dy = (n.y - coords[1]) * spacing[2];
                    dz = (n.z - coords[2]) * spacing[2];
                    d = dx*dx+dy*dy+dz*dz;
                    if(radius[i] > d) radius[i] = d;
                }
            }
        }
        for(i = 0; i<imax; i++)
        {
            NodeIn3D n = tree->node(i);
            if(radius[i] >= std::numeric_limits<double>::max()) n.diameter = -1;
            else n.diameter = sqrt(radius[i])*2.0;
            tree->setNode(n, i);
        }
        delete[] radius;
    }

};
#endif // BUILDTREE_H
