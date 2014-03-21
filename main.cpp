#include "treeskel.h"

int main(int argc, char* argv[])
{
    TreeSkeleton treeskel;
    std::vector<NodeIn3D> branch;
    NodeIn3D node;

    node.connections = 0;
    node.diameter = 2.1;
    node.direction = 23;
    node.x = 1.0;
    node.y = 1.0;
    node.z = 1.0;
    branch.push_back(node);

    node.connections = 0;
    node.diameter = 1.1;
    node.direction = 23;
    node.x = 1.0;
    node.y = 2.0;
    node.z = 2.0;
    branch.push_back(node);

    node.connections = 0;
    node.diameter = 0.1;
    node.direction = 23;
    node.x = 1.0;
    node.y = 3.0;
    node.z = 4.0;
    branch.push_back(node);

    node.connections = 0;
    node.diameter = 1.1;
    node.direction = 23;
    node.x = 1.0;
    node.y = 4.0;
    node.z = 7.0;
    branch.push_back(node);

    treeskel.addBranch(branch);

    branch.clear();

    node.connections = 0;
    node.diameter = 1.1;
    node.direction = 23;
    node.x = 1.0;
    node.y = 2.0;
    node.z = 2.0;
    branch.push_back(node);

    node.connections = 0;
    node.diameter = 0.1;
    node.direction = 23;
    node.x = 1.0;
    node.y = 4.0;
    node.z = 5.0;
    branch.push_back(node);

    node.connections = 0;
    node.diameter = 1.1;
    node.direction = 23;
    node.x = 1.0;
    node.y = 8.0;
    node.z = 9.0;
    branch.push_back(node);

    treeskel.addBranch(branch);


    treeskel.saveTree("skeleton.txt", 0);

    treeskel.removeBranch(0);

    treeskel.saveTree("skeleton2.txt", 0);

}
