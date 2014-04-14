#include "mainWindow.h"
#include "imagefilters.h"

// W projekcie dodac sciezke do folderu z ui_widget.h do INCLUDEPATH
// bo inaczej bedzie wojna miedzy systemowa
#include "ui_mainWindow.h"
//#include "../qtcreator-build/ui_mainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Dodanie widgeta do g³ównego okna programu
    //QWidget *wi = new widget();
    //ui->verticalLayout->addWidget(wi);

    wi = new RenderITQt();
    setCentralWidget((QWidget*) wi);
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
    //itkFunctions itek;
    mojaKlasa = new Image();
    mojaKlasa->fillStructure(ImageFilters::openAnalyzeImage(filePath.toStdString()));
    wi->setImage(mojaKlasa);
    //wi->renderNewImage();
}

void MainWindow::on_actionGaussian_Filter_triggered()
{
    //itkFunctions itek;
    mojaKlasa->fillStructure(ImageFilters::gaussianFilter(mojaKlasa->returnStruct(), 2));
    wi->updateImage(mojaKlasa);
}

void MainWindow::on_actionHessian_Filter_triggered()
{
    //itkFunctions itek;
   // mojaKlasa->fillStructure(itek.hessianFilter(mojaKlasa->returnStruct(), 1));
   // wi->update();
}

void MainWindow::on_actionChange_Background_Color_triggered()
{
    wi->setBackgroundColor((float)(rand()%256)/256,(float)(rand()%256)/256,(float)(rand()%256)/256);
}

void MainWindow::on_actionRemove_Raster_Image_triggered()
{
    wi->removeImage();
}

void MainWindow::on_actionChange_Image_Color_triggered()
{
    wi->setImageColor((float)(rand()%256)/256,(float)(rand()%256)/256,(float)(rand()%256)/256);
}

/*
void MainWindow::on_actionDraw_Model_triggered()
{
    Obraz obr;
    TreeSkeleton out;
    TreeSkeleton out2;

   obr.fillStructure(openAnalyzeImage("E:/Obrazy_Adama/thinning.img"));
   //obr.fillStructure(openAnalyzeImage("E:/Dziwna_Data/thinning.img"));
   out = skeletonToTree(obr.returnStruct());

   obr.fillStructure(openAnalyzeImage("E:/Obrazy_Adama/ctoutput.img"));
   //obr.fillStructure(openAnalyzeImage("E:/Dziwna_Data/ctoutput.img"));
   out2 = szacowanie_srednicy ( obr.returnStruct(), out);

   //TreeSkeleton *ptr;
   //ptr = &out2;
   //wi->AddActor(ptr);
   wi->AddActor(wi->returnActor(out2, 0.15));
}
*/


void MainWindow::on_actionDraw_Model_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load tree model"),
                                                    NULL,//domyslna nazwa pliku
                                                    tr("Text file (*.txt) (*.txt);;All Files (*)"));

    if (!fileName.isEmpty())
    {
        Tree out;
        out.loadTree(fileName.toStdString().c_str());
//        out.saveTree("aaaaaa.txt", 1);
        wi->addTree(out, 0);
//        wi->AddActor(wi->returnActor(out, 0.15));
    }
}


void MainWindow::on_actionChange_Model_Color_triggered()
{
    wi->setTreeColor((float)(rand()%256)/256,(float)(rand()%256)/256,(float)(rand()%256)/256);
}

void MainWindow::on_actionOnOff_Transparency_triggered(bool checked)
{
    if(checked) wi->setImageOpacity();
    else wi->setImageOpacity(0.0, 1.0);
}

void MainWindow::on_actionSet_Transparency_triggered()
{
    wi->setImageOpacity(0,0.1);
}

void MainWindow::on_actionRemove_Model_triggered()
{
    wi->removeTree(0);
}

void MainWindow::on_actionChange_Model_Opacity_triggered()
{
    wi->setTreeOpacity(0.2, 0);
}
