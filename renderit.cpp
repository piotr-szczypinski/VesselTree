#include "renderit.h"
#include "treerender.h"
#include "imagefilters.h"

#if VTK_MAJOR_VERSION <= 5
#define SetInputData SetInput
#define SetSourceData SetSource
#endif

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    #include <windows.h>
#else
    #include <unistd.h>
#endif

//#include <vtkOrientationMarkerWidget.h>
//#include <vtkAxesActor.h>

RenderIT::RenderIT()
{
    connector = itk::ImageToVTKImageFilter< ImageType >::New();
    castFilter = vtkSmartPointer<vtkImageCast>::New();
    opticityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
    colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    compositeFunction = vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();
    volumeMapper = vtkSmartPointer<vtkVolumeRayCastMapper>::New();
    volume = vtkSmartPointer<vtkVolume>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(.8, .8, .8);
    //if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
}


// http://www.vtk.org/Wiki/VTK/ExamplesBoneYard/Cxx/VolumeRendering/itkVtkImageConvert
void RenderIT::setImage(Image* image)
{
//    connector->SetInput(ImageFilters::StructureToItkImage(ImageFilters::rescaleIntensity(image->returnStruct())));
//    vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
//#if VTK_MAJOR_VERSION <= 5
//    actor->SetInput(connector->GetOutput());
//#else
//    connector->Update();
//    actor->GetMapper()->SetInputData(connector->GetOutput());
//#endif
//    renderer->AddActor(actor);
//    renderer->ResetCamera();


    connector->SetInput(ImageFilters::StructureToItkImage(ImageFilters::rescaleIntensity(image->returnStruct())));
    connector->Modified();
    connector->Update();
    castFilter->SetOutputScalarTypeToUnsignedShort();
    castFilter->SetInputData(connector->GetOutput());
    opticityTransferFunction->AddPoint(0,         0.0);
    opticityTransferFunction->AddPoint(255,       1.0);
    colorTransferFunction->AddRGBPoint(0,   0.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(255, 1.0, 1.0, 1.0);
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opticityTransferFunction);
    volumeMapper->SetVolumeRayCastFunction(compositeFunction);
    volumeMapper->SetInputData(castFilter->GetOutput());
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);
    volume->Update();
    renderer->AddVolume(volume);
    cameraReset();
    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
}
void RenderIT::updateImage(Image* image)
{
    connector->SetInput(ImageFilters::StructureToItkImage(ImageFilters::rescaleIntensity(image->returnStruct())));
    connector->Modified();
    connector->Update();
//    if(renderer->GetRenderWindow() != NULL)
//        renderer->GetRenderWindow()->Render();
}
void RenderIT::removeImage(void)
{
    renderer->RemoveVolume(volume);
//    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
}
int RenderIT::addTree(Tree tree, int mode)
{
    std::vector< vtkSmartPointer<vtkActor> > actors;
    switch(mode)
    {
    case 0: actors = TreeRender::returnActors2(tree, 0.5); break;
    case 1: actors = TreeRender::returnActors2(tree, 0.5); break;
    case 2: actors = TreeRender::returnActors2(tree); break;
    }
    int amax = actors.size();
    for(int a = 0; a < amax; a++)
    {
        renderer->AddActor(actors[a]);
    }
    treeActors.push_back(actors);
    cameraReset();
//    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
    return treeActors.size()-1;
}
int RenderIT::removeTree(int id)
{
    if(id < 0)
    {
        while(treeActors.size() > 0) removeTree(treeActors.size()-1);
    }
    else
    {
        if(treeActors.size() <= id) return treeActors.size()-1;
        int amax = treeActors[id].size();
        for(int a = 0; a < amax; a++)
        {
            renderer->RemoveActor(treeActors[id][a]);
        }
        treeActors.erase(treeActors.begin() + id);
//        if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
    }
    return treeActors.size()-1;
}
void RenderIT::cameraReset()
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
//    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();

}
void RenderIT::setBackgroundColor(float r, float g, float b)
{
    renderer->SetBackground(r, g, b);
    //renderer->GetRenderWindow()->Render();
    //renderer->Render();
}
void RenderIT::setImageColor(float r, float g, float b, float min, float max)
{
    colorTransferFunction->RemoveAllPoints();
    colorTransferFunction->AddRGBPoint(min,   0.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(max, r, g, b);
    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
}
int RenderIT::setTreeColor(float r, float g, float b, unsigned int id)
{
    if(treeActors.size() <= id) return treeActors.size()-1;
    int amax = treeActors[id].size();
    for(int a = 0; a < amax; a++)
    {
        treeActors[id][a]->GetProperty()->SetColor(r,g,b);
    }
    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
    return treeActors.size()-1;
}
int RenderIT::setTreeOpacity(float o, unsigned int id)
{
    if(treeActors.size() <= id) return treeActors.size()-1;
    int amax = treeActors[id].size();
    for(int a = 0; a < amax; a++)
    {
        treeActors[id][a]->GetProperty()->SetOpacity(o);
    }
    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
    return treeActors.size()-1;
}
void RenderIT::setImageOpacity(float min, float max)
{
    opticityTransferFunction->RemoveAllPoints();
    opticityTransferFunction->AddPoint(255, max);
    opticityTransferFunction->AddPoint(0, min);
    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
}

//----------------------------------------------------------------------------------------

RenderITWindow::RenderITWindow(int style):RenderIT()
{
    this->style = style;
}

void RenderITWindow::show(void)
{
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    switch(style)
    {
    case 1:
    {
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> tstyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    renderWindowInteractor->SetInteractorStyle(tstyle);
    } break;

    case 2:
    {
    vtkSmartPointer<vtkInteractorStyleSwitch> mstyle = vtkSmartPointer<vtkInteractorStyleSwitch>::New();
    renderWindowInteractor->SetInteractorStyle(mstyle);
    } break;
    }

    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindow->Render();
    renderWindowInteractor->Initialize();
    renderWindowInteractor->Start();
}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
RenderITWindow* threadPointerRenderITWindow;
DWORD WINAPI threadFunctionShowRenderITWindow(LPVOID lpParam)
{
    threadPointerRenderITWindow->show();
    return 0;
}
void RenderITWindow::showAndGo(void)
{
    threadPointerRenderITWindow = this;
    CreateThread(
                NULL,                               // default security attributes
                0,                                  // use default stack size
                threadFunctionShowRenderITWindow,   // thread function name
                0,                                  // argument to thread function
                0,                                  // use default creation flags
                NULL);                              // returns the thread identifier
}
#else
void RenderITWindow::showAndGo(void)
{
    pid_t child_pid;
    child_pid = fork();
    if(child_pid == 0)
    {
        show();
        exit(0);
    }
}
#endif
