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

/** \struct Coordinates3d
 *  \brief Coordinates3d stores coordinates x, y, and z.
 *
 * This is simple coordinate vector.
 * \ingroup Gap
 */
struct Coordinates3d
{
    double x;
    double y;
    double z;
};

/** \struct NodeIn3D
 *  \brief NodeIn3D stores node description.
 *
 * This structure derives from the Coordinates3d.
 * It stores coordinates of a node, number of other nodes
 * connected to it and diameter of a vessel arround the node.
 * \ingroup Gap
 */
struct NodeIn3D:Coordinates3d
{
/** Number of other nodes connected to this one. */
    unsigned int connections;
/** Diameter of a vessel arround the node. */
    unsigned int diameter;
};

/** \struct BasicBranch
 *  \brief BasicBranch stores indexes of nodes building a branch.
 * BasicBranch stores indexes of nodes building a branch.
 * \ingroup Gap
 */
struct BasicBranch
{
    std::vector<unsigned int> nodeIndex;
};

/** \struct TreeSkeletonStructure
 *  \brief TreeSkeletonStructure stores information on a vessel tree.
 * TreeSkeletonStructure stores indexes of nodes building individual
 * branches in branches vector and corresponding nodes description in
 * nodes vector.
 * \ingroup Gap
 */
struct TreeSkeletonStructure
{
    std::vector <NodeIn3D> nodes;
    std::vector <BasicBranch> branches;
};

//-------------------------------------------------------------------------------------
/** \classt TreeSkeleton
 *  \brief TreeSkeleton derives from TreeSkeletonStructure.
 * TreeSkeleton defines functions to safely add and remove branches
 * in TreeSkeleton structure.
 * \ingroup Gap
 */
class TreeSkeleton:TreeSkeletonStructure
{
public:
/** Constructor. The mindistance parameter defines a minimum distance
 * below which tip nodes of branch being added is joined with the closest
 * node of a tree.*/
    TreeSkeleton(double mindistance = 0.5);

/** Saves a tree to a file of a given fileName in interna format
 * format=0 or as separate branches format=1.*/
    bool saveTree(char *fileName, unsigned int format);
/** Loads a tree from a file of a given fileName.*/
    bool loadTree(const char *fileName);

/** Returns count of branches.*/
    unsigned int count(void);
/** Returns count of nodes in a branch with an index branchIndex.*/
    int count(unsigned int branchIndex);
/** Returns count of all the nodes forming the tree.*/
    unsigned int nodeCount(void);

/** Returns vector of nodes formin a branch with an index branchIndex.*/
    std::vector<NodeIn3D> branch(unsigned int branchIndex);
/** Returns a subsequent Inode node of a branch with an index branchI.*/
    NodeIn3D node(unsigned int branchI, unsigned int nodeI);
/** Returns a subsequent node of a tree.*/
    NodeIn3D node(unsigned int index);
/** Exchanges a node given by index with a new information provided
 *  in a newNode.*/
    bool setNode(NodeIn3D newNode, unsigned int index);
/** Adds a new branch newBranch to the tree.*/
    bool addBranch(std::vector<NodeIn3D> newBranch);
/** Removes a branch of an index index from the tree.*/
    bool removeBranch(unsigned int index);

private:
    double joinDistance; /**< Distance to join nodes, set by the constructor.*/
};

#endif // TREESKEL_H
