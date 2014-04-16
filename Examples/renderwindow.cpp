#include "tree.h"
#include "buildtree.h"
#include "image.h"
#include "renderit.h"

int main(int argc, char *argv[])
{
    Image image;
    //Tree tree;

    if(argc < 3) return 1;

    std::cout << "A" << std::endl;

    image.fillStructure(BuildTree::openAnalyzeImage(argv[1]));
    //tree.loadTree(argv[2]);
    std::cout << "B" << std::endl;

    RenderITWindow window = RenderITWindow(1);
    std::cout << "C" << std::endl;

    window.setImage(&image);

    window.showAndGo();
    std::cout << "Out" << std::endl;
    //window.addTree(tree);
    return 0;
}
