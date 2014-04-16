/** @example createtree.cpp
 * Przykład pokazuje jak załadować obraz, zbinaryzować go, wyznaczyć szkielet,
 * zamienić obraz rastrowy na opis drzewa i zapisać wynik.
 */
#include "tree.h"
#include "buildtree.h"

int main(int argc, char *argv[])
{
    Image image;
    ImageStructure binarizedImage;
    ImageStructure skeletonizedImage;
    Tree tree;
    Tree diametersEstimatedTree;

    if(argc < 3) return 1;
    image.fillStructure(BuildTree::openAnalyzeImage(argv[1]));

    binarizedImage = BuildTree::wth(image.returnStruct(), 12);
    skeletonizedImage = BuildTree::pocienianie(binarizedImage);

    tree = BuildTree::skeletonToTree(skeletonizedImage);
    diametersEstimatedTree = BuildTree::szacowanie_srednicy (binarizedImage, tree);

    diametersEstimatedTree.saveTree(argv[2], 1);
    return 0;
}
