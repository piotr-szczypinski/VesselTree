#include "mainWindow.h"
//#include "../qtcreator-build/ui_mainWindow.h"
#include "../build-vesseltreeg-Qt4-Debug/ui_mainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Dodanie widgeta do g³ównego okna programu
    //QWidget *wi = new widget();
    //ui->verticalLayout->addWidget(wi);

    wi = new widget();
    QWidget *miniWidget = wi;
    ui->verticalLayout->addWidget(miniWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString filePath =  QFileDialog::getOpenFileName(this, tr("Select 3D Image"), QString(), tr("All 3D Images (*.img *.nii);;Analyze (*.img);;Nifti (*.nii);;All files(*.*)"));
    if (filePath.isEmpty())
    {
        std::cout << "No File detected " << std::endl;
        return;
    }
    itkFunctions itek;
    mojaKlasa = new Obraz();
    mojaKlasa->fillStructure(itek.openAnalyzeImage(filePath.toStdString()));
    wi->dodajObr(mojaKlasa);
    wi->renderNewImage();
}

void MainWindow::on_actionGaussian_Filter_triggered()
{
    //Przyklad uzycia filtra itk'owego
    itkFunctions itek;
    mojaKlasa->fillStructure(itek.gaussianFilter(mojaKlasa->returnStruct(), 2));
    wi->update();
}

void MainWindow::on_actionHessian_Filter_triggered()
{
    //Przyklad uzycia filtra itk'owego
    itkFunctions itek;
    mojaKlasa->fillStructure(itek.hessianFilter(mojaKlasa->returnStruct(), 1));
    wi->update();
}

void MainWindow::on_actionChange_Background_Color_triggered()
{
    //Przyklad zniamy t³a
    wi->changeBackgroundColor((float)(rand()%256)/256,(float)(rand()%256)/256,(float)(rand()%256)/256);
}

void MainWindow::on_actionRemove_Raster_Image_triggered()
{
    //Usuniêcie obrazu z wievera
    wi->usunObr();
}

void MainWindow::on_actionChange_Image_Color_triggered()
{
    //Przyklad zmiany koloru obrazu
    wi->changeDataColor((float)(rand()%256)/256,(float)(rand()%256)/256,(float)(rand()%256)/256);
}

void MainWindow::on_actionSet_Transparency_triggered()
{
    //Ustawienie reczne przezroczystosci (0 niewidoczne, 1 solid)
    wi->SetTransparencyLevels(0,0.1);
}


void MainWindow::on_actionOn_Off_Transparency_triggered(bool checked)
{
    //Ustawienie przezroczystoci TAK/NIE
    wi->SetTransparentOpticity(checked);
}
