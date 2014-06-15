#include "treerender.h"

#if VTK_MAJOR_VERSION <= 5
#define SetInputData SetInput
#define SetSourceData SetSource
#endif


//-------------------------------------------------------------------------------------
bool TreeRender::saveModel (Tree tree, float par1, const char* par2)
{
    //par1 - iloæ punktów brana pod uwagê przy budowie modelu (wszystkie - 1)
    //par2 - cie¿ka do pliku, który ma byæ zapisany
//???    vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
//???    booleanOperation->SetOperationToUnion();
    vtkSmartPointer<vtkPolyData> boolCopy = vtkSmartPointer<vtkPolyData>::New();
    int count_boolean = 0;
    for (int i = 0;i<tree.count();i++)
    {
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius = vtkSmartPointer<vtkTupleInterpolator> ::New();
        interpolatedRadius->SetInterpolationTypeToLinear();
        interpolatedRadius->SetNumberOfComponents(1);
        double rad;
        for(int v = 0;v < tree.count(i) ;v++)
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
        if (ures < 3){ures = 2;}
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
            tuber->SetInputData(polyData);
            tuber->SetNumberOfSides(20);
            tuber->SetVaryRadiusToVaryRadiusByAbsoluteScalar();
            tuber->CappingOn();

            vtkTriangleFilter *triangle = vtkTriangleFilter::New();
            triangle->SetInputData(tuber->GetOutput());
/*???
            if (count_boolean == 0)
            {
                booleanOperation->SetInput(0,triangle->GetOutput());
            }
            else if (count_boolean == 1)
            {
                booleanOperation->SetInput(1,triangle->GetOutput());
                boolCopy = booleanOperation->GetOutput();
                boolCopy->Update();
            }
            else
            {
                vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation2 = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
                booleanOperation2->SetOperationToUnion();
                booleanOperation2->AddInput(0,boolCopy);
                booleanOperation2->AddInput(1,triangle->GetOutput());
                boolCopy = booleanOperation2->GetOutput();
                boolCopy->Update();
            }
            */
            count_boolean++;
        }
    }
    //---------- stl

/*???
    vtkSmartPointer<vtkSTLWriter> stlWriter = vtkSmartPointer<vtkSTLWriter>::New();
    stlWriter->SetFileName(par2);
    stlWriter->SetFileTypeToASCII();
    stlWriter->SetInputConnection(boolCopy->GetProducerPort());
    stlWriter->Write();

    */
    return true;
}


//-------------------------------------------------------------------------------------
bool TreeRender::saveModel (Tree tree, float par1, const char* par2, bool par3, float par4)
{
//???    vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
//???    booleanOperation->SetOperationToUnion();
    vtkSmartPointer<vtkPolyData> boolCopy = vtkSmartPointer<vtkPolyData>::New();
    int count_boolean = 0;
    for (int i = 0;i<tree.count();i++)
    {
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius = vtkSmartPointer<vtkTupleInterpolator> ::New();
        interpolatedRadius->SetInterpolationTypeToLinear();
        interpolatedRadius->SetNumberOfComponents(1);
        double rad;
        float xxx,yyy,zzz,conn;
        for(int v = 0;v < tree.count(i) ;v++)
        {
            NodeIn3D node = tree.node(i, v);
            points->InsertPoint(v,node.x,node.y,node.z);
            xxx = node.x;
            yyy = node.y;
            zzz = node.z;
            rad = node.diameter;
            conn = node.connections;
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

        if (ures < 3){ures = 2;}

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
            tuber->SetInputData(polyData);
            tuber->SetNumberOfSides(20);
            tuber->SetVaryRadiusToVaryRadiusByAbsoluteScalar();
            tuber->CappingOn();

            vtkTriangleFilter *triangle = vtkTriangleFilter::New();
            triangle->SetInputData(tuber->GetOutput());
/*???
            if (count_boolean == 0)
            {
                booleanOperation->SetInput(0,triangle->GetOutput());
            }
            else if (count_boolean == 1)
            {
                booleanOperation->SetInput(1,triangle->GetOutput());
                boolCopy = booleanOperation->GetOutput();
                boolCopy->Update();
            }
            else
            {
                vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation2 = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
                booleanOperation2->SetOperationToUnion();
                booleanOperation2->AddInput(0,boolCopy);
                booleanOperation2->AddInput(1,triangle->GetOutput());
                boolCopy = booleanOperation2->GetOutput();
                boolCopy->Update();
            }
            */

            count_boolean++;
            vtkSmartPointer<vtkSphereSource> polygonSource = vtkSmartPointer<vtkSphereSource>::New();
            if(conn == 3 && par3 == true)
                {
                polygonSource->SetRadius(rad*par4);
                polygonSource->SetCenter(xxx,yyy,zzz);
                polygonSource->Update();

/*???
                if (count_boolean == 0)
                {
                    booleanOperation->SetInput(0,polygonSource->GetOutput());
                }
                else if (count_boolean == 1)
                {
                    booleanOperation->SetInput(1,polygonSource->GetOutput());
                    boolCopy = booleanOperation->GetOutput();
                    boolCopy->Update();
                }
                else
                {
                    vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation2 = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
                    booleanOperation2->SetOperationToUnion();
                    booleanOperation2->AddInput(0,boolCopy);
                    booleanOperation2->AddInput(1,polygonSource->GetOutput());
                    boolCopy = booleanOperation2->GetOutput();
                    boolCopy->Update();
                }
                */

                count_boolean++;
                }
    }
    //---------- stl
/*
    vtkSmartPointer<vtkSTLWriter> stlWriter = vtkSmartPointer<vtkSTLWriter>::New();
    stlWriter->SetFileName(par2);
    stlWriter->SetFileTypeToASCII();
    stlWriter->SetInputConnection(boolCopy->GetProducerPort());
    stlWriter->Write();
    */
    return true;
}
//-------------------------------------------------------------
vtkSmartPointer<vtkActor> TreeRender::returnActor(Tree tree, float par1, bool par2, float par3)
{
//???    vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
//???    booleanOperation->SetOperationToUnion();
    vtkSmartPointer<vtkPolyData> boolCopy = vtkSmartPointer<vtkPolyData>::New();
    int count_boolean = 0;
    for (int i = 0;i<tree.count();i++)
    {
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius = vtkSmartPointer<vtkTupleInterpolator> ::New();
        interpolatedRadius->SetInterpolationTypeToLinear();
        interpolatedRadius->SetNumberOfComponents(1);
        double rad;
        float xxx,yyy,zzz,conn;
        for(int v = 0;v < tree.count(i) ;v++)
        {
            NodeIn3D node = tree.node(i, v);
            points->InsertPoint(v,node.x,node.y,node.z);
            xxx = node.x;
            yyy = node.y;
            zzz = node.z;
            rad = node.diameter;
            conn = node.connections;
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

        if (ures < 3){ures = 2;}

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
            tuber->SetInputData(polyData);
            tuber->SetNumberOfSides(20);
            tuber->SetVaryRadiusToVaryRadiusByAbsoluteScalar();
            tuber->CappingOn();

            vtkTriangleFilter *triangle = vtkTriangleFilter::New();
            triangle->SetInputData(tuber->GetOutput());
/*???
            if (count_boolean == 0)
            {
                booleanOperation->SetInput(0,triangle->GetOutput());
            }
            else if (count_boolean == 1)
            {
                booleanOperation->SetInput(1,triangle->GetOutput());
                boolCopy = booleanOperation->GetOutput();
                boolCopy->Update();
            }
            else
            {
                vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation2 = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
                booleanOperation2->SetOperationToUnion();
                booleanOperation2->AddInput(0,boolCopy);
                booleanOperation2->AddInput(1,triangle->GetOutput());
                boolCopy = booleanOperation2->GetOutput();
                boolCopy->Update();
            }
            */
            count_boolean++;
            vtkSmartPointer<vtkSphereSource> polygonSource = vtkSmartPointer<vtkSphereSource>::New();
            if(conn == 3 && par2 == true)
            {
                polygonSource->SetRadius(rad*par3);
                polygonSource->SetCenter(xxx,yyy,zzz);
                polygonSource->Update();
/*???
                if (count_boolean == 0)
                {
                    booleanOperation->SetInput(0,polygonSource->GetOutput());
                }
                else if (count_boolean == 1)
                {
                    booleanOperation->SetInput(1,polygonSource->GetOutput());
                    boolCopy = booleanOperation->GetOutput();
                    boolCopy->Update();
                }
                else
                {
                    vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation2 = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
                    booleanOperation2->SetOperationToUnion();
                    booleanOperation2->AddInput(0,boolCopy);
                    booleanOperation2->AddInput(1,polygonSource->GetOutput());
                    boolCopy = booleanOperation2->GetOutput();
                    boolCopy->Update();
                }
                */

                count_boolean++;
           }
    }
       // Setup actors and mappers
       vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
       mapper->SetInputData(boolCopy);
       mapper->ScalarVisibilityOff();
       vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
       actor->SetMapper(mapper);
       actor->GetProperty()->SetColor(0.8,0,0);
       return actor;
}
//------------------------------------------------------------------------
std::vector< vtkSmartPointer<vtkActor> > TreeRender::returnActors(Tree tree, float par1, float par2)
{

    vtkSmartPointer<vtkPoints> pointsG = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsY = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR1 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR2 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR3 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR4 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR5 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR6 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR7 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR8 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR9 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR10 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR11 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR12 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR13 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR14 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR15 = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> pointsR16 = vtkSmartPointer<vtkPoints>::New();

    for (unsigned int i = 0;i <tree.nodeCount();i++)
    {
        NodeIn3D node = tree.node(i);
        if(node.connections == 1){pointsG->InsertNextPoint(node.x,node.y,node.z);}
        if(node.connections == 3){pointsY->InsertNextPoint(node.x,node.y,node.z);}
                   if(node.connections == 2)
                      {
                       if(node.diameter==1){pointsR1->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==2){pointsR2->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==3){pointsR3->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==4){pointsR4->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==5){pointsR5->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==6){pointsR6->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==7){pointsR7->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==8){pointsR8->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==9){pointsR9->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==10){pointsR10->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==11){pointsR11->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==12){pointsR12->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==13){pointsR13->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==14){pointsR14->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==15){pointsR15->InsertNextPoint(node.x,node.y,node.z);}
                  else if(node.diameter==16){pointsR16->InsertNextPoint(node.x,node.y,node.z);}
           }
    }

     vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
     sphereSource->SetRadius(.5);
     vtkSmartPointer<vtkSphereSource> polygonSource1 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource1->SetRadius(1);
     vtkSmartPointer<vtkSphereSource> polygonSource2 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource2->SetRadius(2);
     vtkSmartPointer<vtkSphereSource> polygonSource3 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource3->SetRadius(3);
     vtkSmartPointer<vtkSphereSource> polygonSource4 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource4->SetRadius(4);
     vtkSmartPointer<vtkSphereSource> polygonSource5 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource5->SetRadius(5);
     vtkSmartPointer<vtkSphereSource> polygonSource6 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource6->SetRadius(6);
     vtkSmartPointer<vtkSphereSource> polygonSource7 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource7->SetRadius(7);
     vtkSmartPointer<vtkSphereSource> polygonSource8 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource8->SetRadius(8);
     vtkSmartPointer<vtkSphereSource> polygonSource9 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource9->SetRadius(9);
     vtkSmartPointer<vtkSphereSource> polygonSource10 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource10->SetRadius(10);
     vtkSmartPointer<vtkSphereSource> polygonSource11 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource11->SetRadius(11);
     vtkSmartPointer<vtkSphereSource> polygonSource12 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource12->SetRadius(12);
     vtkSmartPointer<vtkSphereSource> polygonSource13 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource13->SetRadius(13);
     vtkSmartPointer<vtkSphereSource> polygonSource14 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource14->SetRadius(14);
     vtkSmartPointer<vtkSphereSource> polygonSource15 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource15->SetRadius(15);
     vtkSmartPointer<vtkSphereSource> polygonSource16 = vtkSmartPointer<vtkSphereSource>::New();
     polygonSource16->SetRadius(16);

     std::vector<vtkSmartPointer<vtkActor> > actors;

     vtkSmartPointer<vtkPolyData> polydataG = vtkSmartPointer<vtkPolyData>::New();polydataG->SetPoints(pointsG);
     vtkSmartPointer<vtkGlyph3D> glyph3DG = vtkSmartPointer<vtkGlyph3D>::New();glyph3DG->SetSourceData(sphereSource->GetOutput());glyph3DG->SetInputData(polydataG);
     glyph3DG->GeneratePointIdsOn();
     vtkSmartPointer<vtkPolyDataMapper> mapperG = vtkSmartPointer<vtkPolyDataMapper>::New();mapperG->SetInputConnection(glyph3DG->GetOutputPort());
     vtkSmartPointer<vtkActor> actorG = vtkSmartPointer<vtkActor>::New();actorG->SetMapper(mapperG);
     actorG->GetProperty()->SetColor(0,1,0);actors.push_back(actorG);

     vtkSmartPointer<vtkPolyData> polydataY = vtkSmartPointer<vtkPolyData>::New();polydataY->SetPoints(pointsY);
     vtkSmartPointer<vtkGlyph3D> glyph3DY = vtkSmartPointer<vtkGlyph3D>::New();glyph3DY->SetSourceData(sphereSource->GetOutput());glyph3DY->SetInputData(polydataY);
     glyph3DY->GeneratePointIdsOn();
     vtkSmartPointer<vtkPolyDataMapper> mapperY = vtkSmartPointer<vtkPolyDataMapper>::New();mapperY->SetInputConnection(glyph3DY->GetOutputPort());
     vtkSmartPointer<vtkActor> actorY = vtkSmartPointer<vtkActor>::New();actorY->SetMapper(mapperY);
     actorY->GetProperty()->SetColor(1,1,0);actors.push_back(actorY);

      float opacity = par1;

      vtkSmartPointer<vtkPolyData> polydataR1 = vtkSmartPointer<vtkPolyData>::New();polydataR1->SetPoints(pointsR1);
      vtkSmartPointer<vtkGlyph3D> glyph3DR1 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR1->SetSourceData(polygonSource1->GetOutput());glyph3DR1->SetInputData(polydataR1);
      glyph3DR1->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR1 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR1->SetInputConnection(glyph3DR1->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR1 = vtkSmartPointer<vtkActor>::New();actorR1->SetMapper(mapperR1);actorR1->GetProperty()->SetOpacity(opacity);
      actorR1->GetProperty()->SetColor(1,1,1);actors.push_back(actorR1);
      vtkSmartPointer<vtkPolyData> polydataR2 = vtkSmartPointer<vtkPolyData>::New();polydataR2->SetPoints(pointsR2);
      vtkSmartPointer<vtkGlyph3D> glyph3DR2 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR2->SetSourceData(polygonSource2->GetOutput());glyph3DR2->SetInputData(polydataR2);
      glyph3DR2->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR2 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR2->SetInputConnection(glyph3DR2->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR2 = vtkSmartPointer<vtkActor>::New();actorR2->SetMapper(mapperR2);actorR2->GetProperty()->SetOpacity(opacity);
      actorR2->GetProperty()->SetColor(1,1,1);actors.push_back(actorR2);
      vtkSmartPointer<vtkPolyData> polydataR3 = vtkSmartPointer<vtkPolyData>::New();polydataR3->SetPoints(pointsR3);
      vtkSmartPointer<vtkGlyph3D> glyph3DR3 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR3->SetSourceData(polygonSource3->GetOutput());glyph3DR3->SetInputData(polydataR3);
      glyph3DR3->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR3 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR3->SetInputConnection(glyph3DR3->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR3 = vtkSmartPointer<vtkActor>::New();actorR3->SetMapper(mapperR3);actorR3->GetProperty()->SetOpacity(opacity);
      actorR3->GetProperty()->SetColor(1,1,1);actors.push_back(actorR3);
      vtkSmartPointer<vtkPolyData> polydataR4 = vtkSmartPointer<vtkPolyData>::New();polydataR4->SetPoints(pointsR4);
      vtkSmartPointer<vtkGlyph3D> glyph3DR4 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR4->SetSourceData(polygonSource4->GetOutput());glyph3DR4->SetInputData(polydataR4);
      glyph3DR4->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR4 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR4->SetInputConnection(glyph3DR4->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR4 = vtkSmartPointer<vtkActor>::New();actorR4->SetMapper(mapperR4);actorR4->GetProperty()->SetOpacity(opacity);
      actorR4->GetProperty()->SetColor(1,1,1);actors.push_back(actorR4);
      vtkSmartPointer<vtkPolyData> polydataR5 = vtkSmartPointer<vtkPolyData>::New();polydataR5->SetPoints(pointsR5);
      vtkSmartPointer<vtkGlyph3D> glyph3DR5 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR5->SetSourceData(polygonSource5->GetOutput());glyph3DR5->SetInputData(polydataR5);
      glyph3DR5->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR5 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR5->SetInputConnection(glyph3DR5->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR5 = vtkSmartPointer<vtkActor>::New();actorR5->SetMapper(mapperR5);actorR5->GetProperty()->SetOpacity(opacity);
      actorR5->GetProperty()->SetColor(1,1,1);actors.push_back(actorR5);
      vtkSmartPointer<vtkPolyData> polydataR6 = vtkSmartPointer<vtkPolyData>::New();polydataR6->SetPoints(pointsR6);
      vtkSmartPointer<vtkGlyph3D> glyph3DR6 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR6->SetSourceData(polygonSource6->GetOutput());glyph3DR6->SetInputData(polydataR6);
      glyph3DR6->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR6 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR6->SetInputConnection(glyph3DR6->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR6 = vtkSmartPointer<vtkActor>::New();actorR6->SetMapper(mapperR6);actorR6->GetProperty()->SetOpacity(opacity);
      actorR6->GetProperty()->SetColor(1,1,1);actors.push_back(actorR6);
      vtkSmartPointer<vtkPolyData> polydataR7 = vtkSmartPointer<vtkPolyData>::New();polydataR7->SetPoints(pointsR7);
      vtkSmartPointer<vtkGlyph3D> glyph3DR7 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR7->SetSourceData(polygonSource7->GetOutput());glyph3DR7->SetInputData(polydataR7);
      glyph3DR7->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR7 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR7->SetInputConnection(glyph3DR7->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR7 = vtkSmartPointer<vtkActor>::New();actorR7->SetMapper(mapperR7);actorR7->GetProperty()->SetOpacity(opacity);
      actorR7->GetProperty()->SetColor(1,1,1);actors.push_back(actorR7);
      vtkSmartPointer<vtkPolyData> polydataR8 = vtkSmartPointer<vtkPolyData>::New();polydataR8->SetPoints(pointsR8);
      vtkSmartPointer<vtkGlyph3D> glyph3DR8 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR8->SetSourceData(polygonSource8->GetOutput());glyph3DR8->SetInputData(polydataR8);
      glyph3DR8->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR8 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR8->SetInputConnection(glyph3DR8->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR8 = vtkSmartPointer<vtkActor>::New();actorR8->SetMapper(mapperR8);actorR8->GetProperty()->SetOpacity(opacity);
      actorR8->GetProperty()->SetColor(1,1,1);actors.push_back(actorR8);
      vtkSmartPointer<vtkPolyData> polydataR9 = vtkSmartPointer<vtkPolyData>::New();polydataR9->SetPoints(pointsR9);
      vtkSmartPointer<vtkGlyph3D> glyph3DR9 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR9->SetSourceData(polygonSource9->GetOutput());glyph3DR9->SetInputData(polydataR9);
      glyph3DR9->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR9 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR9->SetInputConnection(glyph3DR9->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR9 = vtkSmartPointer<vtkActor>::New();actorR9->SetMapper(mapperR9);actorR9->GetProperty()->SetOpacity(opacity);
      actorR9->GetProperty()->SetColor(1,1,1);actors.push_back(actorR9);
      vtkSmartPointer<vtkPolyData> polydataR10 = vtkSmartPointer<vtkPolyData>::New();polydataR10->SetPoints(pointsR10);
      vtkSmartPointer<vtkGlyph3D> glyph3DR10 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR10->SetSourceData(polygonSource10->GetOutput());glyph3DR10->SetInputData(polydataR10);
      glyph3DR10->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR10 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR10->SetInputConnection(glyph3DR10->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR10 = vtkSmartPointer<vtkActor>::New();actorR10->SetMapper(mapperR10);actorR10->GetProperty()->SetOpacity(opacity);
      actorR10->GetProperty()->SetColor(1,1,1);actors.push_back(actorR10);
      vtkSmartPointer<vtkPolyData> polydataR11 = vtkSmartPointer<vtkPolyData>::New();polydataR11->SetPoints(pointsR11);
      vtkSmartPointer<vtkGlyph3D> glyph3DR11 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR11->SetSourceData(polygonSource11->GetOutput());glyph3DR11->SetInputData(polydataR11);
      glyph3DR11->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR11 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR11->SetInputConnection(glyph3DR11->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR11 = vtkSmartPointer<vtkActor>::New();actorR11->SetMapper(mapperR11);actorR11->GetProperty()->SetOpacity(opacity);
      actorR11->GetProperty()->SetColor(1,1,1);actors.push_back(actorR11);
      vtkSmartPointer<vtkPolyData> polydataR12 = vtkSmartPointer<vtkPolyData>::New();polydataR12->SetPoints(pointsR12);
      vtkSmartPointer<vtkGlyph3D> glyph3DR12 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR12->SetSourceData(polygonSource12->GetOutput());glyph3DR12->SetInputData(polydataR12);
      glyph3DR12->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR12 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR12->SetInputConnection(glyph3DR12->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR12 = vtkSmartPointer<vtkActor>::New();actorR12->SetMapper(mapperR12);actorR12->GetProperty()->SetOpacity(opacity);
      actorR12->GetProperty()->SetColor(1,1,1);actors.push_back(actorR12);
      vtkSmartPointer<vtkPolyData> polydataR13 = vtkSmartPointer<vtkPolyData>::New();polydataR13->SetPoints(pointsR13);
      vtkSmartPointer<vtkGlyph3D> glyph3DR13 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR13->SetSourceData(polygonSource13->GetOutput());glyph3DR13->SetInputData(polydataR13);
      glyph3DR13->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR13 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR13->SetInputConnection(glyph3DR13->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR13 = vtkSmartPointer<vtkActor>::New();actorR13->SetMapper(mapperR13);actorR13->GetProperty()->SetOpacity(opacity);
      actorR13->GetProperty()->SetColor(1,1,1);actors.push_back(actorR13);
      vtkSmartPointer<vtkPolyData> polydataR14 = vtkSmartPointer<vtkPolyData>::New();polydataR14->SetPoints(pointsR14);
      vtkSmartPointer<vtkGlyph3D> glyph3DR14 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR14->SetSourceData(polygonSource14->GetOutput());glyph3DR14->SetInputData(polydataR14);
      glyph3DR14->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR14 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR14->SetInputConnection(glyph3DR14->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR14 = vtkSmartPointer<vtkActor>::New();actorR14->SetMapper(mapperR14);actorR14->GetProperty()->SetOpacity(opacity);
      actorR14->GetProperty()->SetColor(1,1,1);actors.push_back(actorR14);
      vtkSmartPointer<vtkPolyData> polydataR15 = vtkSmartPointer<vtkPolyData>::New();polydataR15->SetPoints(pointsR15);
      vtkSmartPointer<vtkGlyph3D> glyph3DR15 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR15->SetSourceData(polygonSource15->GetOutput());glyph3DR15->SetInputData(polydataR15);
      glyph3DR15->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR15 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR15->SetInputConnection(glyph3DR15->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR15 = vtkSmartPointer<vtkActor>::New();actorR15->SetMapper(mapperR15);actorR15->GetProperty()->SetOpacity(opacity);
      actorR15->GetProperty()->SetColor(1,1,1);actors.push_back(actorR15);
      vtkSmartPointer<vtkPolyData> polydataR16 = vtkSmartPointer<vtkPolyData>::New();polydataR16->SetPoints(pointsR16);
      vtkSmartPointer<vtkGlyph3D> glyph3DR16 = vtkSmartPointer<vtkGlyph3D>::New();glyph3DR16->SetSourceData(polygonSource16->GetOutput());glyph3DR16->SetInputData(polydataR16);
      glyph3DR16->GeneratePointIdsOn();
      vtkSmartPointer<vtkPolyDataMapper> mapperR16 = vtkSmartPointer<vtkPolyDataMapper>::New();mapperR16->SetInputConnection(glyph3DR16->GetOutputPort());
      vtkSmartPointer<vtkActor> actorR16 = vtkSmartPointer<vtkActor>::New();actorR16->SetMapper(mapperR16);actorR16->GetProperty()->SetOpacity(opacity);
      actorR16->GetProperty()->SetColor(1,1,1);actors.push_back(actorR16);

      for (int i = 0;i<tree.count();i++)
      {
          vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
          vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius = vtkSmartPointer<vtkTupleInterpolator> ::New();
          interpolatedRadius->SetInterpolationTypeToLinear();
          interpolatedRadius->SetNumberOfComponents(1);
          double rad;
          float xxx,yyy,zzz,conn;
          for(int v = 0;v < tree.count(i) ;v++)
          {
              NodeIn3D node = tree.node(i, v);
              points->InsertPoint(v,node.x,node.y,node.z);
              xxx = node.x;
              yyy = node.y;
              zzz = node.z;
              rad = node.diameter;
              conn = node.connections;
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
          float ures = par2 * points->GetNumberOfPoints(); //.15

          if (ures < 3){ures = 2;}

              functionSource->SetUResolution( ures);
              functionSource->Update();

          vtkSmartPointer<vtkPolyData> tubePolyData = vtkSmartPointer<vtkPolyData>::New();
          tubePolyData = functionSource->GetOutput();
          vtkSmartPointer<vtkPolyDataMapper> lineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
          lineMapper->SetInputData(tubePolyData);
          vtkSmartPointer<vtkActor> lineActor = vtkSmartPointer<vtkActor>::New();
          lineActor->SetMapper(lineMapper);
          lineActor->GetProperty()->SetColor(0,0,0);
          actors.push_back(lineActor);
      }

      return actors;
}
//------------------------------------------------------------------------------
std::vector< vtkSmartPointer<vtkActor> > TreeRender::returnActors2(Tree tree, float par1)
{
    std::vector<vtkSmartPointer<vtkActor> > actors;
      for (int i = 0;i<tree.count();i++)
      {
          vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
          vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius = vtkSmartPointer<vtkTupleInterpolator> ::New();
          interpolatedRadius->SetInterpolationTypeToLinear();
          interpolatedRadius->SetNumberOfComponents(1);
          double rad;
          float xxx,yyy,zzz,conn;
          for(int v = 0;v < tree.count(i) ;v++)
          {
              NodeIn3D node = tree.node(i, v);
              points->InsertPoint(v,node.x,node.y,node.z);
              xxx = node.x;
              yyy = node.y;
              zzz = node.z;
              rad = node.diameter;
              conn = node.connections;
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

          if (ures < 3){ures = 2;}

              functionSource->SetUResolution( ures);
              functionSource->Update();

          vtkSmartPointer<vtkPolyData> tubePolyData = vtkSmartPointer<vtkPolyData>::New();
          tubePolyData = functionSource->GetOutput();
          vtkSmartPointer<vtkPolyDataMapper> lineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
          lineMapper->SetInputData(tubePolyData);
          vtkSmartPointer<vtkActor> lineActor = vtkSmartPointer<vtkActor>::New();
          lineActor->SetMapper(lineMapper);
          lineActor->GetProperty()->SetColor(0,0,0);
          actors.push_back(lineActor);
      }

      return actors;
}

