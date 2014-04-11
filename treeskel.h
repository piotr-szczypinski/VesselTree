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
 *  \brief Coordinates3d stores coordinates x, y and z.
 *
 * This is simply a vector of coordinates.
 */
struct Coordinates3d
{
    double x;
    double y;
    double z;
};

/** \struct NodeIn3D
 *  \brief NodeIn3D stores node coordinates, number
 *  of neighbors and local diameter of a vessel.
 *
 * This structure derives from the Coordinates3d.
 * It stores coordinates of the node, number of nodes connected to
 * it and diameter of a vessel arround the node.
 */
struct NodeIn3D:Coordinates3d
{
/** Number of nodes connected to this one. */
    unsigned int connections;
/** Diameter of a vessel arround the node. */
    unsigned int diameter;
};

/** \struct BasicBranch
 *  \brief BasicBranch stores indexes of nodes building the branch.
 */
struct BasicBranch
{
    std::vector<unsigned int> nodeIndex;
};

/** \struct TreeSkeletonStructure
 *  \brief TreeSkeletonStructure stores information on a vessel tree.
 *
 * TreeSkeletonStructure stores indexes of nodes building individual
 * branches in a vector of BasicBranch structures.
 * The corresponding nodes coordinate, connectivity and diameter are stored
 * in a vector of NodeIn3D structures.
 * User should not operate directly on the TreeSkeletonStructure.
 * Instead it is preferred to use functions of TreeSkeleton class
 * to add or remove branches.
 */
struct TreeSkeletonStructure
{
    std::vector <NodeIn3D> nodes;
    std::vector <BasicBranch> branches;
};

//-------------------------------------------------------------------------------------
/** \classt TreeSkeleton
 *  \brief TreeSkeleton derives from TreeSkeletonStructure.
 *
 * TreeSkeleton defines functions to safely add and remove branches
 * in TreeSkeleton structure, query the number of nodes and branches,
 * to save and load data, etc.
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
/** Returns count of nodes in a branch.
 * \param branchIndex is an index of the branch
 * \returns count of nodes or negative value if fails*/
    int count(unsigned int branchIndex);
/** Returns count of all the nodes forming the tree.*/
    unsigned int nodeCount(void);

/** Returns vector of nodes belonging to the branch.
 * \param branchIndex is an index of the branch*/
    std::vector<NodeIn3D> branch(unsigned int branchIndex);
/** Returns a node of the branch.
 * \param branchI is an index of the branch
 * \param nodeI is an index of subsequent node of the branch*/
    NodeIn3D node(unsigned int branchI, unsigned int nodeI);
/** Returns a node of a tree.
  * \param index is a global index of node in the tree*/
    NodeIn3D node(unsigned int index);
/** Exchanges a node in the tree with a new one.
  * \param index is an index in the tree of the node to overwrite
  * \param newNode is a structure with a new node specification
  * \returns true on success*/
    bool setNode(NodeIn3D newNode, unsigned int index);
/** Adds a new branch newBranch to the tree.
  * \param newBranch vector of the new branch nodes
  * \returns true on success*/
    bool addBranch(std::vector<NodeIn3D> newBranch);
/** Removes a branch of an index index from the tree.
    * \param index is an index in the branch to remove
    * \returns true on success*/
    bool removeBranch(unsigned int index);

private:
/** Distance to join nodes, set by the constructor.*/
    double joinDistance;
};

#endif // TREESKEL_H
