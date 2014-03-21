/*
 * VesselTree - MRI image segmentation and characterization
 * 
 * Copyright 2014  Piotr M. Szczypiński <piotr.szczypinski@p.lodz.pl>
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

#ifndef TREESKEL_H
#define TREESKEL_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <math.h>

struct Coordinates3d
{
    double x;
    double y;
    double z;
};

struct NodeIn3D:Coordinates3d
{
    unsigned int connections;
    unsigned int direction;
    unsigned int diameter;
};

struct BasicBranch
{
    std::vector<unsigned int> nodeIndex;
};

struct TreeSkeletonStructure
{
    std::vector <NodeIn3D> nodes;
    std::vector <BasicBranch> branches;
};

//-------------------------------------------------------------------------------------
class TreeSkeleton:TreeSkeletonStructure
{
public:
    TreeSkeleton(double mindistance = 0.5);
    bool saveTree(char *fileName, unsigned int format);
    bool loadTree(char *fileName);

    unsigned int count(void);
    int count(unsigned int branchIndex);
    std::vector<NodeIn3D> branch(unsigned int branchIndex);
    NodeIn3D node(unsigned int branchI, unsigned int nodeI);

    bool addBranch(std::vector<NodeIn3D> newBranch);
    bool removeBranch(unsigned int index);

private:
    double joinDistance;
};

#endif // TREESKEL_H
