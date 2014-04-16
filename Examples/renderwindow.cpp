/** \example renderwindow.cpp
 * Przykład pokazuje jak załadować obraz rastrowy i opis drzewa z plików a następnie wyświetlić je w oknie.
 * \image html renderwindow.jpg
 * \image latex renderwindow.eps
 */

#include "tree.h"
#include "buildtree.h"
#include "image.h"
#include "renderit.h"

int main(int argc, char *argv[])
{
    Image image;
    Tree tree;

    if(argc < 3) return 1;
    image.fillStructure(BuildTree::openAnalyzeImage(argv[1]));
    tree.loadTree(argv[2]);

    RenderITWindow window = RenderITWindow(1);
    window.setImage(&image);
    window.addTree(tree, 1);
    window.setTreeColor(1.0, 0.5, 0, 0);
    window.showAndGo();
    return 0;
}
