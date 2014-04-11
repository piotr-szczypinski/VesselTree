#ifndef GAPSURFACE_H
#define GAPSURFACE_H

/*! \file gapsurface.h zawiera deklaracje funkcji pomocniczych klasy TreeSkeleton
 * wykorzystujących bibliotekę VTK.
 */ 

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

/*! \fn bool saveModel(TreeSkeleton tree, float partial, const char* file_name);
 * \brief Function converts vessel tree boundatry to tiangle
 * mesh representation and saves it to an *.stl file.
 *  \param tree input TreeSkeleton class
 *  \param partial (0.0 - 1.0] fraction of nodes used in conversion
 *  \param file_name output file name
 */
bool saveModel (TreeSkeleton tree, float par1, const char* par2);

#endif // GAPSURFACE_H
