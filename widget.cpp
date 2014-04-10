// Problem w linii 131 137 187 193-194 257-269

#include "widget.h"

// W projekcie dodac sciezke do folderu z ui_widget.h do INCLUDEPATH
// bo inaczej bedzie wojna miedzy systemowa
#include "ui_widget.h"
#include <iostream>

widget::widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget)
{
    ui->setupUi(this);

    connector = Connector::New();
    castFilter = vtkSmartPointer<vtkImageCast>::New();
    opticityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
    colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    compositeFunction = vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();
    volumeMapper = vtkSmartPointer<vtkVolumeRayCastMapper>::New();
    volume = vtkSmartPointer<vtkVolume>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();

    renderer->SetBackground(.3, .2, .1);
    ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
    renderer->GetRenderWindow()->Render();
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
   connector->SetInput(StructureToItkImage(rescaleIntensity(obr->returnStruct())));
   connector->Modified();
   connector->Update();
   renderer->GetRenderWindow()->Render();
}

void widget::renderNewImage()
{
    //--Connector do ³¹czenia itk'a z vtk'em
    connector->SetInput(StructureToItkImage(rescaleIntensity(obr->returnStruct())));
    connector->Modified();
    connector->Update();
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
    resetCamera();
    renderer->GetRenderWindow()->Render();
}
void widget::changeBackgroundColor(float r, float g, float b)
{
    renderer->SetBackground(r, g, b);
    renderer->GetRenderWindow()->Render();
}
void widget::changeDataColor(float r, float g, float b)
{
    colorTransferFunction->RemoveAllPoints();
    colorTransferFunction->AddRGBPoint(0,   0.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(255, r, g, b);
    renderer->GetRenderWindow()->Render();
}
void widget::SetTransparentOpticity(bool flag)
{
    opticityTransferFunction->RemoveAllPoints();
    if(flag) opticityTransferFunction->AddPoint(0,         0.0);
    else     opticityTransferFunction->AddPoint(0,         1.0);
    opticityTransferFunction->AddPoint(255,       1.0);
    renderer->GetRenderWindow()->Render();
}
void widget::SetTransparencyLevels(float min, float max)
{
    opticityTransferFunction->RemoveAllPoints();
    opticityTransferFunction->AddPoint(0,         min);
    opticityTransferFunction->AddPoint(255,       max);
    renderer->GetRenderWindow()->Render();
}

void widget::AddActor(vtkSmartPointer<vtkActor> a)
{
    vectorOfActors.push_back(a);
    renderer->AddActor(vectorOfActors[vectorOfActors.size()-1]);
    renderer->GetRenderWindow()->Render();
    resetCamera();
}

void widget::usunActor(int id)
{
    if(id == -1) id = vectorOfActors.size()-1;
    renderer->RemoveActor(vectorOfActors[id]);
    renderer->GetRenderWindow()->Render();
    vectorOfActors.erase(vectorOfActors.begin() + id);
}
void widget::changeActorColor(float r, float g, float b, int id)
{
    if(id == -1) id = vectorOfActors.size()-1;
//    vectorOfActors[id]->GetProperty()->SetColor(r,g,b);
    renderer->GetRenderWindow()->Render();
}
void widget::changeActorOpacity(float o, int id)
{
    if(id == -1) id = vectorOfActors.size()-1;
//    vectorOfActors[id]->GetProperty()->SetOpacity(o);
    renderer->GetRenderWindow()->Render();
}
void widget::resetCamera()
{
    double fp[3];
    double p[3];
    double dist;
    renderer->ResetCamera();
    fp[0] = renderer->GetActiveCamera()->GetFocalPoint()[0];
    fp[1] = renderer->GetActiveCamera()->GetFocalPoint()[1];
    fp[2] = renderer->GetActiveCamera()->GetFocalPoint()[2];
    p[0] = renderer->GetActiveCamera()->GetPosition()[0];
    p[1] = renderer->GetActiveCamera()->GetPosition()[1];
    p[2] = renderer->GetActiveCamera()->GetPosition()[2];
    dist = sqrt((p[0]-fp[0])*(p[0]-fp[0]) + (p[1]-fp[1])*(p[1]-fp[1]) + (p[2]-fp[2])*(p[2]-fp[2]));
    renderer->GetActiveCamera()->SetPosition(fp[0], fp[1], fp[2]+dist);
    renderer->GetActiveCamera()->SetViewUp(0.0, 1.0, 0.0);
    renderer->GetRenderWindow()->Render();
}

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
//#include <vtkBooleanOperationPolyDataFilter.h>



vtkSmartPointer<vtkActor> widget::returnActor (TreeSkeleton tree, float par1)
{
    vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
    booleanOperation->SetOperationToUnion();
    vtkSmartPointer<vtkPolyData> boolCopy = vtkSmartPointer<vtkPolyData>::New();
    int count_boolean = 0;
    for (int i = 0; i<tree.count(); i++)
    {
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius = vtkSmartPointer<vtkTupleInterpolator> ::New();
        interpolatedRadius->SetInterpolationTypeToLinear();
        interpolatedRadius->SetNumberOfComponents(1);
        double rad;
        for(int v = 0; v < tree.count(i); v++)
        {
            NodeIn3D node = tree.node(i, v);

            points->InsertPoint(v,node.x,node.y,node.z);
            rad = node.diameter;
            interpolatedRadius->AddTuple(v,&rad);
        }
        vtkSmartPointer<vtkParametricSpline> spline = vtkSmartPointer<vtkParametricSpline>::New();
        spline->SetPoints(points);
        spline->ParameterizeByLengthOff();
        spline->SetLeftConstraint(3);
        spline->SetRightConstraint(3);
        // Interpolate the points
        vtkSmartPointer<vtkParametricFunctionSource> functionSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
        functionSource->SetParametricFunction(spline);

        float ures = par1 * points->GetNumberOfPoints(); //.15
        int pff = 1;
        if (ures < 3){ures = 2;}     //if (ures < 3){ures = 2;}
        if (pff == 1)
        {
            functionSource->SetUResolution( ures);
            functionSource->Update();
            // Generate the radius scalars
            vtkSmartPointer<vtkDoubleArray> tubeRadius = vtkSmartPointer<vtkDoubleArray>::New();
            tubeRadius->SetName("TubeRadius");
            unsigned int n = functionSource->GetOutput()->GetNumberOfPoints();
            tubeRadius->SetNumberOfTuples(n);
            double tMin = interpolatedRadius->GetMinimumT();
            double tMax = interpolatedRadius->GetMaximumT();
            double r[1];
            for (unsigned int i = 0; i < n; ++i)
            {
                double t = (tMax - tMin) / (n - 1) * i + tMin;
                interpolatedRadius->InterpolateTuple(t, r);
                tubeRadius->SetTuple1(i, r[0]);
            }

            vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
            polyData = functionSource->GetOutput();
            polyData->GetPointData()->AddArray(tubeRadius);
            polyData->GetPointData()->SetActiveScalars("TubeRadius");

            vtkTubeFilter* tuber = vtkTubeFilter::New();
            tuber->SetInput(polyData);
            tuber->SetNumberOfSides(20);
            tuber->SetVaryRadiusToVaryRadiusByAbsoluteScalar();
            tuber->CappingOn();

            vtkTriangleFilter *triangle = vtkTriangleFilter::New();
            triangle->SetInput(tuber->GetOutput());

             if (count_boolean == 0){booleanOperation->SetInput(0,triangle->GetOutput());}
             else if (count_boolean == 1){booleanOperation->SetInput(1,triangle->GetOutput());
                                          boolCopy = booleanOperation->GetOutput();
                                          boolCopy->Update();}
             else
                 {
                 vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation2 = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
                 booleanOperation2->SetOperationToUnion();
                 booleanOperation2->AddInput(0,boolCopy);
                 booleanOperation2->AddInput(1,triangle->GetOutput());
                 boolCopy = booleanOperation2->GetOutput();
                 boolCopy->Update();
                 }

            count_boolean++;
        }
    }
    //Setup actors and mappers
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInput(boolCopy);
    mapper->ScalarVisibilityOff();
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0,1,0);

    return actor;

}
/*
vtkSmartPointer<vtkActor> widget::returnActor (TreeSkeleton tree, float par1)
{
vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
 booleanOperation->SetOperationToUnion();
 vtkSmartPointer<vtkPolyData> boolCopy = vtkSmartPointer<vtkPolyData>::New();
  int count_boolean = 0;
 for (int i = 0;i<branches.size();i++)
      {
      vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
            vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius = vtkSmartPointer<vtkTupleInterpolator> ::New();
            interpolatedRadius->SetInterpolationTypeToLinear();
            interpolatedRadius->SetNumberOfComponents(1);
            double rad;
            for(int v = 0;v < branches[i].nodeIndex.size() ;v++)
                  {
                   points->InsertPoint(v,nodes[branches[i].nodeIndex[v]].x,nodes[branches[i].nodeIndex[v]].y,nodes[branches[i].nodeIndex[v]].z);
                   rad = nodes[branches[i].nodeIndex[v]].diameter;
                   interpolatedRadius->AddTuple(v,&rad);
                   }
             vtkSmartPointer<vtkParametricSpline> spline = vtkSmartPointer<vtkParametricSpline>::New();
             spline->SetPoints(points);
             spline->ParameterizeByLengthOff();
             spline->SetLeftConstraint(3);
             spline->SetRightConstraint(3);
             // Interpolate the points
             vtkSmartPointer<vtkParametricFunctionSource> functionSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
             functionSource->SetParametricFunction(spline);

             float ures = par1 * points->GetNumberOfPoints(); //.15
             int pff = 1;
             if (ures < 3){ures = 2;}     //if (ures < 3){ures = 2;}
             if (pff == 1)
             {
             functionSource->SetUResolution( ures);
             functionSource->Update();
             // Generate the radius scalars
             vtkSmartPointer<vtkDoubleArray> tubeRadius = vtkSmartPointer<vtkDoubleArray>::New();
             tubeRadius->SetName("TubeRadius");
             unsigned int n = functionSource->GetOutput()->GetNumberOfPoints();
             tubeRadius->SetNumberOfTuples(n);
             double tMin = interpolatedRadius->GetMinimumT();
             double tMax = interpolatedRadius->GetMaximumT();
             double r[1];
             for (unsigned int i = 0; i < n; ++i)
               {
               double t = (tMax - tMin) / (n - 1) * i + tMin;
               interpolatedRadius->InterpolateTuple(t, r);
               tubeRadius->SetTuple1(i, r[0]);
               }

             vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
             polyData = functionSource->GetOutput();
             polyData->GetPointData()->AddArray(tubeRadius);
             polyData->GetPointData()->SetActiveScalars("TubeRadius");

             vtkTubeFilter* tuber = vtkTubeFilter::New();
             tuber->SetInput(polyData);
             tuber->SetNumberOfSides(20);
             tuber->SetVaryRadiusToVaryRadiusByAbsoluteScalar();
             tuber->CappingOn();

             vtkTriangleFilter *triangle = vtkTriangleFilter::New();
             triangle->SetInput(tuber->GetOutput());

             if (count_boolean == 0){booleanOperation->SetInput(0,triangle->GetOutput());}
             else if (count_boolean == 1){booleanOperation->SetInput(1,triangle->GetOutput());
                                          boolCopy = booleanOperation->GetOutput();
                                          boolCopy->Update();}
             else
                 {
                 vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation2 = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
                 booleanOperation2->SetOperationToUnion();
                 booleanOperation2->AddInput(0,boolCopy);
                 booleanOperation2->AddInput(1,triangle->GetOutput());
                 boolCopy = booleanOperation2->GetOutput();
                 boolCopy->Update();
                 }

             count_boolean++;
             }
         }
                //Setup actors and mappers
               vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
               mapper->SetInput(boolCopy);
               mapper->ScalarVisibilityOff();
               vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
               actor->SetMapper(mapper);
               actor->GetProperty()->SetColor(0,1,0);

// Create a sphere
//  vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
//  sphereSource->SetCenter(0.0, 0.0, 0.0);
//  sphereSource->SetRadius(5.0);

//  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//  mapper->SetInputConnection(sphereSource->GetOutputPort());

//  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
//  actor->SetMapper(mapper);
//  actor->GetProperty()->SetColor(0,1,0);

               return actor;

}
*/
