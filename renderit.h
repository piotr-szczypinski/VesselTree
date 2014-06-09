#ifndef RENDERIT_H
#define RENDERIT_H

#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)

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
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>


#include <itkImageFileReader.h>
#include "itkImageToVTKImageFilter.h"
#include <vtkSmartPointer.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkMatrix4x4.h>
#include <vtkAxesActor.h>

#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>
typedef itk::Image< float,3 > ImageType;

#include "image.h"
#include "tree.h"

/**
RenderIT klasa zawiera funkcje do wyświetlania obrazu rastrowego klasy Image z nałożonymi obiektami
klasy Tree. Użytkownik nie powinien tworzyć obiektów tej klasy. Zamiast tego należy utworzyć obiekt
klasy dziedziczącej RenderITWindow lub RenderItQt.
*/
class RenderIT
{
public:
    RenderIT();
/** \brief Ustawia obraz trójwymiarowy do wyświetlenia w scenie i resetuje parametry wyświetlania
 * \param image wskaźnik opbrazu
 */
    void setImage(Image* image);
/** \brief Ustawia obraz trójwymiarowy do wyświetlenia w scenie bez resetowania parametrów wyświetlania
 * \param image wskaźnik opbrazu
 */
    void updateImage(Image* image);
/** \brief Usuwa obraz trójwymiarowy ze sceny
 */
    void removeImage(void);

/** \brief Dodaje drzewo do wyświetlenia w scenie.
 * \param tree opis drzewa
 * \param mode liczba 0..3 okreslająca sposób wyświetlania drzewa
 * \returns indeks dodanego drzewa na liście wyswietlanych drzew
 */
    int addTree(Tree tree, int mode);

/** \brief Usuwa drzewo o danym indeksie ze sceny
 * \param id index drzewa, jeśli id<0 to usuwane są wszystkie drzewa
 * \returns indeks ostatniego drzewa na liście wyswietlanych drzew
 */
    int removeTree(int id = -1);

/** \brief Resetuje ustawienia sceny
 */
    void cameraReset();

/** \brief Zmienia kolor tła
 * \param r składowa czerwona koloru
 * \param g składowa zielona koloru
 * \param b składowa niebieska koloru
 */
    void setBackgroundColor(float r, float g, float b);

/** \brief Zmienia kolor obrazu rastrowego
 * \param r składowa czerwona koloru
 * \param g składowa zielona koloru
 * \param b składowa niebieska koloru
 * \param min wartość dla minimalnej jasności (czerni)
 * \param max wartość dla maksymalnej jasności
 */
    void setImageColor(float r, float g, float b, float min = 0.0, float max = 255.0);

/** \brief Zmienia przeźroczystość obrazu rastrowego
 * \param min wartość dla pełnej przeźroczystości
 * \param max wartość dla pełnej widoczności, nieprzeźroczystości
 */
    void setImageOpacity(float min, float max);

/** \brief Zmienia kolor drzewa
 * \param r składowa czerwona koloru
 * \param g składowa zielona koloru
 * \param b składowa niebieska koloru
 * \param id index drzewa
 * \returns indeks ostatniego drzewa na liście wyswietlanych drzew
 */
    int setTreeColor(float r, float g, float b, unsigned int id);

/** Zmienia przeźroczystość drzewa
 * \param o poziom przeźroczystości od 0.0 do 1.0
 * \param id index drzewa
 * \returns indeks ostatniego drzewa na liście wyswietlanych drzew
 */
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
    std::vector<std::vector<vtkSmartPointer<vtkActor> > >       treeActors;
};

/** \class RenderITWindow
 *  \brief dziedziczy po klasie RenderIT. Jej zadaniem jest
 * utworzenie niezaleznego okna do wyświetlania danych 3D.
 *
 * \author Piotr M. Szczypiński
 */
class RenderITWindow:public RenderIT
{
public:
/** Konstruktor klasy
 * \param style 0 lub 1 określa sposób obsługi okna myszą
 */
    RenderITWindow(int style);
/** wyświetla okno i czeka na jego zamknięcie */
    void show(void);
/** wyświetla okno i wykonuje dalsze elementy programu*/
    void showAndGo(void);

private:
    int style;
};

#endif // RENDERIT_H
