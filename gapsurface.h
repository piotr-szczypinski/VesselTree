#ifndef GAPSURFACE_H
#define GAPSURFACE_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTrackball.h>
#include <vector>
#include "vtkProperty.h"
#include <vtkArrowSource.h>
#include <vtkConeSource.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkRegularPolygonSource.h>
#include <vtkParametricFunctionSource.h>
#include <vtkTupleInterpolator.h>
#include <vtkTubeFilter.h>
#include <vtkParametricSpline.h>
#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSTLWriter.h>
#include <vtkTriangleFilter.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include "treeskel.h"

/** \class GapGeometryHelpers
 *  \brief GapGeometryHelpers jest klasą funkcji pomocniczych do prezentacji obrazu raztrowego i drzewa naczyń krwionośnych.
 */
class GapGeometryHelpers
{
public:
/** \brief Tworzenie modelu geometrycznego na podstawie struktury galezi
 *  Przygotowanie vtkActor do wyświetlenia
 *  \param tree - struktura galezi
 *  \param par1 - ilosc punktow brana pod uwage przy budowie modelu (wszystkie - 1)
 *  \param par2 - true: rob sfery w rozgalezieniach, false: nie rob
 *  \param par3 - promien sfery (1: promieñ równy oszacowanemu w rozgalezieniu)
 *  \returns Zwraca vtkActor zawierający model
 */
  static vtkSmartPointer<vtkActor> returnActor(TreeSkeleton tree, float par1 = 0.2, bool par2 = true, float par3 = 1.1);
/**  \brief Tworzenie alternatywnego modelu geometrycznego (kule, linia centralna, bifurkacje i zakoñczenia) na podstawie struktury galezi
 *  Przygotowanie vtkActor do wyświetlenia
 *  \param tree - struktura galezi
 *  \param par1 - przezroczystość kul
 *  \param par2 - ilosc punktow brana pod uwage przy budowie linii srodkowej (wszystkie - 1)
 *  \returns Zwraca wektor zawierający wiele vtkActor
 */
  static std::vector< vtkSmartPointer<vtkActor> > returnActors(TreeSkeleton tree, float par1 = 0.05, float par2 = 0.5);
/**  \brief Tworzenie linii centralnej na podstawie struktury galezi
 *  Przygotowanie vtkActor do wyświetlenia
 *  \param tree - struktura galezi
 *  \param par1 - ilosc punktow brana pod uwage przy budowie linii srodkowej (wszystkie - 1)
 *  \returns Zwraca wektor zawierający wiele vtkActor
 */
  static std::vector< vtkSmartPointer<vtkActor> > returnActors2(TreeSkeleton tree, float par1 = 0.5);


/* \brief Function converts vessel tree boundatry to tiangle
 * mesh representation and saves it to an *.stl file.
 *  \param tree input TreeSkeleton class
 *  \param partial (0.0 - 1.0] fraction of nodes used in conversion
 *  \param file_name output file name
 */
/** \brief Tworzy model geometryczny ścian naczyń krwionośnych drzewa i zapisuje do pliku stl.
 * \param tree wejściowa klasa opisu drzewa
 * \param partial (0.0 - 1.0] frakcja węzłów wykorzystywanych
 * \param file_name nazwa pliku wyjściowego
 * \returns zwraca true gdy się uda albo false gdy się nie uda
 */
  static bool saveModel (TreeSkeleton tree, float par1, const char* par2);
/**  \brief Tworzenie modelu geometrycznego na podstawie struktury galezi
 *  Zapis modelu do pliku .stl
 *  tree - struktura galezi
 *  \param par1 - ilosc punktow brana pod uwage przy budowie modelu (wszystkie - 1)
 *  \param par2 - sciezka do pliku, ktory ma byc zapisany (format stl)
 *  \param par3 - true: rob sfery w rozgalezieniach, false: nie rob
 *  \param par4 - promien sfery (1: promień równy oszacowanemu w rozgalezieniu)
 *  \returns Zwraca wartosc true jezeli zapis do pliku sie powiodl
 */
  static bool saveModel (TreeSkeleton tree, float par1, const char* par2, bool par3, float par4 = 1.1);

};

#endif // GAPSURFACE_H
