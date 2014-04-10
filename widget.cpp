#include "widget.h"
//#include "../qtcreator-build/ui_widget.h"
#include "../build-vesseltreeg-Qt4-Debug/ui_widget.h"
#include <iostream>

widget::widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget)
{
    //mainW = mw;
    ui->setupUi(this);

}

widget::~widget()
{
    delete ui;
}

void widget::dodajObr(Obraz *ob)
{
    obr = ob;
    renderNewImage();
}
void widget::usunObr()
{
    renderer->RemoveVolume(volume);
    renderer->GetRenderWindow()->Render();
}

void widget::update()
{
    //Update obrazu (np po filtracji)
   itkFunctions itkOperation;
   connector->SetInput(itkOperation.StructureToItkImage(itkOperation.rescaleIntensity(obr->returnStruct())));
   connector->Modified();
   connector->Update();
   renderer->GetRenderWindow()->Render();
}
void widget::resetSmartPointers()
{
    //inijalizacja pointerów
    connector = Connector::New();
    castFilter = vtkSmartPointer<vtkImageCast>::New();
    opticityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
    colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    compositeFunction = vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();
    volumeMapper = vtkSmartPointer<vtkVolumeRayCastMapper>::New();
    volume = vtkSmartPointer<vtkVolume>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();
}

void widget::renderNewImage()
{
    //Wywietlenie nowego obrazu
    resetSmartPointers();
    itkFunctions itkOperation;
    //--Connector do ³¹czenia itk'a z vtk'em
    connector->SetInput(itkOperation.StructureToItkImage(itkOperation.rescaleIntensity(obr->returnStruct())));
    //--Rzutowanie jasnoci obrazu do Unsigned Short 
    castFilter->SetOutputScalarTypeToUnsignedShort();
    castFilter->SetInput(connector->GetOutput());
    //--Definicja funkcji przezroczystoci    
    opticityTransferFunction->AddPoint(0,         0.0);
    opticityTransferFunction->AddPoint(255,       1.0);
    //--Definicja skali kolorów
    colorTransferFunction->AddRGBPoint(0,   0.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(255, 1.0, 1.0, 1.0);
    //--Grupowanie w³¹ciwoci które pos³u¿¹ przy budowie "wolumenu" 
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opticityTransferFunction);
    //--Stworzenie mapera
    volumeMapper->SetVolumeRayCastFunction(compositeFunction);
    volumeMapper->SetInput(castFilter->GetOutput());
    //--Definicja "wolumenu" - wybór mapera i w³aciwoci zdefiniowanych wczeniej   
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);
    volume->Update();
    //Dodanie "wolumenu" do renderera
    renderer->AddVolume(volume);
    renderer->SetBackground(.3, .2, .1);
    //--Rozpoczêcie wywietlania
    ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
    renderer->GetRenderWindow()->Render();
}
void widget::changeBackgroundColor(float r, float g, float b)
{
    //zmiana koloru tla
    renderer->SetBackground(r, g, b);
    renderer->GetRenderWindow()->Render();
}
void widget::changeDataColor(float r, float g, float b)
{
    //zmiana koloru rastra
    colorTransferFunction->RemoveAllPoints();
    colorTransferFunction->AddRGBPoint(0,   0.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(255, r, g, b);
    renderer->GetRenderWindow()->Render();
}
void widget::SetTransparentOpticity(bool flag)
{
    //w³ /wy³ przezroczystoæ
    opticityTransferFunction->RemoveAllPoints();
    if(flag) opticityTransferFunction->AddPoint(0,         0.0);
    else     opticityTransferFunction->AddPoint(0,         1.0);
    opticityTransferFunction->AddPoint(255,       1.0);
    renderer->GetRenderWindow()->Render();
}
void widget::SetTransparencyLevels(float min, float max)
{
    //ustaw przezroczystosc
    opticityTransferFunction->RemoveAllPoints();
    opticityTransferFunction->AddPoint(0,         min);
    opticityTransferFunction->AddPoint(255,       max);
    renderer->GetRenderWindow()->Render();
}
