#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    imagecolor = QColor(255, 255, 255);
    treecolor = QColor(255, 255, 255);
    renderer = new RenderITQt();
    setCentralWidget((QWidget*) renderer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load image"),
                                                    NULL,
                                                    tr("Images (*.img *.nii);;All files(*)"));
    if (!fileName.isEmpty())
    {
        image = new Image();
        image->fillStructure(ImageFilters::openAnalyzeImage(fileName.toStdString()));
        renderer->setImage(image);
    }
}

void MainWindow::on_actionLoad_tree_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load tree model"),
                                                    NULL,
                                                    tr("Text file (*.txt) (*.txt);;All Files (*)"));
    if (!fileName.isEmpty())
    {
        tree.loadTree(fileName.toStdString().c_str());
        renderer->addTree(tree, 0);
    }
}

void MainWindow::on_pushButtonBackground_clicked()
{
    QColor color = QColorDialog::getColor();
    renderer->setBackgroundColor(color.red()/255.0, color.green()/255.0, color.blue()/255.0);
}

void MainWindow::on_pushButtonImageColor_clicked()
{
    imagecolor = QColorDialog::getColor();
    on_sliderImageMinWindow_actionTriggered(0);
}

void MainWindow::on_pushButtonTreeColor_clicked()
{
    int op = ui->sliderTreeOpacity->value();
    treecolor = QColorDialog::getColor();
    renderer->setTreeColor(treecolor.red()/255.0,
                           treecolor.green()/255.0,
                           treecolor.blue()/255.0, 0);
}

void MainWindow::on_sliderImageMinOpacity_actionTriggered(int action)
{
    int min = ui->sliderImageMinOpacity->value();
    int max = ui->sliderImageMaxOpacity->value();
    renderer->setImageOpacity(min/1024.0, max/1024.0);
}

void MainWindow::on_sliderImageMaxOpacity_actionTriggered(int action)
{
    on_sliderImageMinOpacity_actionTriggered(action);
}

void MainWindow::on_sliderImageMinWindow_actionTriggered(int action)
{
    int min = ui->sliderImageMinWindow->value();
    int max = ui->sliderImageMaxWindow->value();
    renderer->setImageColor(imagecolor.red()/255.0,
                            imagecolor.green()/255.0,
                            imagecolor.blue()/255.0,
                            255.0*min/1024.0, 255.0*max/1024.0);
}

void MainWindow::on_sliderImageMaxWindow_actionTriggered(int action)
{
   on_sliderImageMinWindow_actionTriggered(action);
}

void MainWindow::on_sliderTreeOpacity_actionTriggered(int action)
{
    on_pushButtonTreeColor_clicked();
}
