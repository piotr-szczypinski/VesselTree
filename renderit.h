#ifndef RENDERIT_H
#define RENDERIT_H

//#include <vtkPolyDataMapper.h>
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
#include <vtkCamera.h>

#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>
typedef itk::Image< float,3 > ImageType;

#include "image.h"
#include "tree.h"

/*!
RenderIT klasa zawiera funkcje do wyświetlania obrazu rastrowego klasy Image z nałożonymi obiektami
klasy Tree. Użytkownik nie powinien tworzyć obiektów tej klasy. Zamiast tego należy utworzyć obiekt
klasy dziedziczącej RenderITWindow lub RenderItQt.
*/
class RenderIT
{
public:
    RenderIT();
    void setImage(Image* image);
    void updateImage(Image* image);
    void removeImage(void);
    int addTree(Tree tree, int mode);
    int removeTree(int id = -1);
    void cameraReset();
    void setBackgroundColor(float r, float g, float b);
    void setImageColor(float r, float g, float b, float min = 0.0, float max = 255.0);
    void setImageOpacity(float min=1.0, float max=1.0);
    int setTreeColor(float r, float g, float b, unsigned int id = -1);
    int setTreeOpacity(float o, unsigned int id);

protected:
    vtkSmartPointer<vtkRenderer>                                renderer;

private:
    itk::ImageToVTKImageFilter< ImageType >::Pointer            connector;
    vtkSmartPointer<vtkImageCast>                               castFilter;
    vtkSmartPointer<vtkPiecewiseFunction>                       opticityTransferFunction;
    vtkSmartPointer<vtkColorTransferFunction>                   colorTransferFunction;
    vtkSmartPointer<vtkVolumeProperty>                          volumeProperty;
    vtkSmartPointer<vtkVolumeRayCastCompositeFunction>          compositeFunction;
    vtkSmartPointer<vtkVolume>                                  volume;
    vtkSmartPointer<vtkVolumeRayCastMapper>                     volumeMapper;
//    vtkSmartPointer<vtkPolyDataMapper>                          mapper;
//    std::vector<vtkSmartPointer<vtkActor> >                     vectorOfActors;
//    std::vector<unsigned int>                                   numActorsPerTree;
    std::vector<std::vector<vtkSmartPointer<vtkActor> > >       treeActors;
};

/*! RenderITWindow tworzy okno wyświetlania dla klasy RenderIT, po której dziedziczy*/
class RenderITWindow:public RenderIT
{
    RenderITWindow(int style);
};

#endif // RENDERIT_H
