#include "treeskel.h"
#include "buildtree.h"

int main(int argc, char *argv[])
{
    std::cout << "Start\n";

    Obraz mojaKlasa;
    TreeSkeleton out;
    TreeSkeleton out2;

    mojaKlasa.fillStructure(openAnalyzeImage("/home/piotr/Program/VesselTree/Test/thinning.img"));

    out = skeletonToTree(mojaKlasa.returnStruct());


   // out = szacowanie_polaczen (mojaKlasa.returnStruct());

    mojaKlasa.fillStructure(openAnalyzeImage("/home/piotr/Program/VesselTree/Test/ctoutput.img"));
    out2 = szacowanie_srednicy (mojaKlasa.returnStruct(),out);

    out2.saveTree("/home/piotr/Program/VesselTree/Test/treetest.txt", 0);
    out2.saveTree("/home/piotr/Program/VesselTree/Test/treetesta.txt", 1);

    std::cout << "Koniec\n";

/*
    for (unsigned int i = 0;i<out2.nodes.size();i++)
    {
        std::cout<<out2.nodes[i].x<<" "<<out2.nodes[i].y<<" "<<out2.nodes[i].z<<" "<<out2.nodes[i].connections<<" "<<out2.nodes[i].diameter<<std::endl;
    }
    */
    //for (unsigned int i = 0;i<out2.branches.size();i++)
    //{
    //    for (unsigned int j = 0;j<out2.branches[i].nodeIndex.size();j++)
    //    {
    //        std::cout<<out2.branches[i].nodeIndex[j]<<" "<<i<<" "<<j<<std::endl;
    //    }
    //}
    //mojaKlasa.fillStructure(openAnalyzeImage("D:/Dysk Google/programy/1.4/input.img"));
    //mojaKlasa.fillStructure(erozja(mojaKlasa.returnStruct(),3));
    //saveImage(mojaKlasa.returnStruct(), "D:/Dysk Google/programy/grant_solid/szacowanie/plik.nii");

//    system("PAUSE");
    return 0;
}


/*
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
*/
