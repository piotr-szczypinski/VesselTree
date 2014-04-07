#include "gapsurface.h"

//-------------------------------------------------------------------------------------
bool saveModel (TreeSkeleton tree, float par1, const char* par2)
{
    //par1 - iloæ punktów brana pod uwagê przy budowie modelu (wszystkie - 1)
    //par2 - cie¿ka do pliku, który ma byæ zapisany
    vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
    booleanOperation->SetOperationToUnion();
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
            tuber->SetInput(polyData);
            tuber->SetNumberOfSides(20);
            tuber->SetVaryRadiusToVaryRadiusByAbsoluteScalar();
            tuber->CappingOn();

            vtkTriangleFilter *triangle = vtkTriangleFilter::New();
            triangle->SetInput(tuber->GetOutput());

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
            count_boolean++;
        }
    }
    //---------- stl
    vtkSmartPointer<vtkSTLWriter> stlWriter = vtkSmartPointer<vtkSTLWriter>::New();
    stlWriter->SetFileName(par2);
    stlWriter->SetFileTypeToASCII();
    stlWriter->SetInputConnection(boolCopy->GetProducerPort());
    stlWriter->Write();
    return true;
}
