#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QColorDialog>

#include "renderitqt.h"
#include "tree.h"
#include "image.h"
#include "imagefilters.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionLoad_image_triggered();

    void on_actionLoad_tree_triggered();

    void on_pushButtonBackground_clicked();

    void on_pushButtonImageColor_clicked();

    void on_pushButtonTreeColor_clicked();

    void on_sliderImageMaxOpacity_actionTriggered(int action);

    void on_sliderImageMinOpacity_actionTriggered(int action);

    void on_sliderImageMinWindow_actionTriggered(int action);

    void on_sliderImageMaxWindow_actionTriggered(int action);

    void on_sliderTreeOpacity_actionTriggered(int action);

private:
    Ui::MainWindow *ui;

    RenderITQt *renderer;
    Image *image;
    QColor imagecolor;
    Tree tree;
    QColor treecolor;
};

#endif // MAINWINDOW_H
