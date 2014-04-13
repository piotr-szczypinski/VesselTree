#ifndef widget_H
#define widget_H

#include <QWidget>
#include "gapbuildtreehelper.h"

#include <vtkPolyDataMapper.h>
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

namespace Ui {
    class widget;
}

//! K L A S A    W I D G E T
/*!
Klasa widget służy do wyświetlania obrazów rastrowych w postaci obiektów klasy Obraz
jak również obiektów geometrycznych podanch jako vtkSmatrPointer<vtkActor>.
W przeciwieństwie do obrazu rastrowego ilość wyświetlanych aktorów jest nieograniczona.
Przechowywane są one w wektorze co umożliwia ich sensowne dodawanie i usuwanie.
Proste funkcje umożliwiają ponadto zmieniać takie parametry jak kolor i przezroczystość wyświetlanych danych.
*/

class widget : public QWidget
{
    Q_OBJECT

public:
    vtkSmartPointer<vtkActor> returnActor (TreeSkeleton tree, float par1);

   //! K O N S T R U K T O R
   /*!
   Przygotowanie okna i renderera (narazie pustego)
   */
    explicit widget(QWidget *parent = 0);
    //! D E S T R U K T O R
    /*!
    Usunięcie GUI, nic więcej
    */
    ~widget();

    //! Przyjęcie wskaźnika do klasy obraz a następnie wywołanie funkcji renderNewImage()
    /*!
    \param *ob wskaźnik do klasy przechowującej obraz rastrowy.
    */
    void dodajObr(Obraz *ob);
    //! Usunięcie jedynego obrazu rastrowego
    void usunObr();
    //! Wyświetlenie obrazu
    void renderNewImage();
    //! Dodanie aktora do sceny
    /*!
      Aktor zostaje dodany do vektora aktorów a następnie wyświetlony
    \param a vtkSmartPointer<vtkActor>.
    */
    void AddActor(vtkSmartPointer<vtkActor> a);
    //! Usunięcie aktora ze sceny
    /*!
      Aktor zostaje usunięty zarówno ze sceny jak i z vektora aktorów.
    \param id numer aktora do usunięcia (domyślnie -1 = usuwany ostatnio dodany)
    */
    void usunActor(int id = -1);
    //! Odświerzenie obrazu rastrowego
    /*!
    Jeśli obraz nie jest wczytywany kompletnie nowy obraz, a jedynie efekt filtracji
    wywołanie tej funkcji spowoduje podmianę obrazu wejsciowego bez ingerencji w preferencje sceny
    */
    void update();
    //! Reset pozycji i ustawienia Kamery
    /*!
    po wczytaniu nwego obrazu lub dodaniu aktora do sceny występuje konieczność ponownego ustawienia
    kamery, aby w obszarze okna widoczne były wszystkie elementy
    */
    void resetCamera();
    //! Zmiana koloru tła
    /*!
    \param r składowa czerwona  (0-1)
    \param g składowa zielona   (0-1)
    \param b składowa niebieska (0-1)
    */
    void changeBackgroundColor(float r, float g, float b);
    //! Zmiana odcienia obrazu rastrowego
    /*!
    \param r składowa czerwona  (0-1)
    \param g składowa zielona   (0-1)
    \param b składowa niebieska (0-1)
    */
    void changeDataColor(float r, float g, float b);
    //! Zmiana koloru aktora
    /*!
    \param r składowa czerwona  (0-1)
    \param g składowa zielona   (0-1)
    \param b składowa niebieska (0-1)
    \param id numer aktora (domyślnie -1 = ostatnio dodany aktor)
    */
    void changeActorColor(float r, float g, float b, int id = -1);
    //! Zmiana ustawień przezroczystości aktora
    /*!
    \param o wartość przezroczystości  (0 niewidoczny - 1 nieprzezroczysty)
    \param id numer aktora (domyślnie -1 = ostatnio dodany aktor)
    */
    void changeActorOpacity(float o, int id = -1);
    //! Zezwolenie na przezroczystość obrazu rastrowego
    /*!
    \param flag jeśli wartość jest true to przezroczystość jest domyślna, jeśli false to każdy woksel jest nieprzezroczysty
    */
    void SetTransparentOpticity(bool flag);
    //! Zmiana ustawień przezroczystości obrazu rastrowego
    /*!
    \param min wartość minimalna przezroczystości w obrazie (domyślnie 0 = najciemniejsze woksele są niewidoczne)
    \param max wartość maxymalna przezroczystości w obrazie (domyślnie 1 = najjaśniejsze woksele są nieprzezroczyste)
    */
    void SetTransparencyLevels(float min=0, float max=1);

private:
    Ui::widget *ui;             /*!< Wskaźnik do UI */
    Obraz *obr;                 /*!< Wskaźnik do klasy OBRAZ */
    TreeSkeleton *ske;          /*!< Wskaźnik do klasy ADAMA (jeśli wyświetlamy actor to nie jest on potrzebny) */

    //! Pointery VTK'owe które dzięki temu, że są widoczne w klasie mogą być szybko modyfikowane
    Connector::Pointer                                          connector;
    vtkSmartPointer<vtkImageCast>                               castFilter;
    vtkSmartPointer<vtkPiecewiseFunction>                       opticityTransferFunction;
    vtkSmartPointer<vtkColorTransferFunction>                   colorTransferFunction;
    vtkSmartPointer<vtkVolumeProperty>                          volumeProperty;
    vtkSmartPointer<vtkVolumeRayCastCompositeFunction>          compositeFunction;
    vtkSmartPointer<vtkVolume>                                  volume;
    vtkSmartPointer<vtkVolumeRayCastMapper>                     volumeMapper;
    vtkSmartPointer<vtkRenderer>                                renderer;
    vtkSmartPointer<vtkPolyDataMapper>                          mapper;
    std::vector<vtkSmartPointer<vtkActor> >                     vectorOfActors;

private slots:

};

#endif // widget_H
