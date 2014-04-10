#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>

#include "widget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    widget *wi;
    Obraz *mojaKlasa;

private slots:


private slots:
    void on_actionChange_Model_Opacity_triggered();
    void on_actionRemove_Model_triggered();
    void on_actionSet_Transparency_triggered();
    void on_actionOnOff_Transparency_triggered(bool checked);
    void on_actionChange_Model_Color_triggered();
    void on_actionDraw_Model_triggered();
    void on_actionChange_Image_Color_triggered();
    void on_actionHessian_Filter_triggered();
    void on_actionRemove_Raster_Image_triggered();
    void on_actionGaussian_Filter_triggered();
    void on_actionChange_Background_Color_triggered();
    void on_actionOpen_Image_triggered();
};

#endif // MAINWINDOW_H
