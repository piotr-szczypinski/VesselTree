/** \example renderqt.cpp
 * Przykład pokazuje jak załadować obraz rastrowy i opis drzewa z plików a następnie wyświetlić je w formularzu Qt.
 * W przypadku bardziej zaawansowanych aplikacji funkcję setCentralWidget należy wstawić do konstruktora klasy
 * formularza dziedziczącej z klasy QMainWindow.
 */

#include "tree.h"
#include "buildtree.h"
#include "image.h"
#include "renderitqt.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    Image image;
    Tree tree;

    if(argc < 3) return 1;
    image.fillStructure(BuildTree::openAnalyzeImage(argv[1]));
    tree.loadTree(argv[2]);

    QApplication application(argc, argv);
    QMainWindow window;
    window.show();

    RenderITQt widget;
    widget.setImage(&image);
    widget.addTree(tree, 1);
    window.setCentralWidget((QWidget*) &widget);

    return application.exec();
}
