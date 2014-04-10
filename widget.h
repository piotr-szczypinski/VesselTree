#ifndef widget_H
#define widget_H

#include <QWidget>
//#include "mainWindow.h"
#include "itkFunctions.h"

#include <vtkSmartPointer.h>
#include <vtkImageCast.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeTextureMapper3D.h>
#include <vtkVolume.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

namespace Ui {
    class widget;
}

class widget : public QWidget
{
    Q_OBJECT

public:
    explicit widget(QWidget *parent = 0);
    ~widget();
    //MainWindow *mainW;
    void dodajObr(Obraz *ob);
    void usunObr();
    void renderNewImage();
    void update();
    void changeBackgroundColor(float r, float g, float b);
    void changeDataColor(float r, float g, float b);
    void SetTransparentOpticity(bool flag);
    void SetTransparencyLevels(float min=0, float max=255);

private:
    void resetSmartPointers();
    Ui::widget *ui;
    Obraz *obr;
    Connector::Pointer                                          connector;
    vtkSmartPointer<vtkImageCast>                               castFilter;
    vtkSmartPointer<vtkPiecewiseFunction>                       opticityTransferFunction;
    vtkSmartPointer<vtkColorTransferFunction>                   colorTransferFunction;
    vtkSmartPointer<vtkVolumeProperty>                          volumeProperty;
    vtkSmartPointer<vtkVolumeRayCastCompositeFunction>          compositeFunction;
    vtkSmartPointer<vtkVolume>                                  volume;
    vtkSmartPointer<vtkVolumeRayCastMapper>                     volumeMapper;
    vtkSmartPointer<vtkRenderer>                                renderer;

private slots:

};

#endif // widget_H
