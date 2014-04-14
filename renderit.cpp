#include "renderit.h"
#include "treerender.h"
#include "imagefilters.h"

#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>

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
    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
}
void RenderIT::setImage(Image* image)
{
    connector->SetInput(ImageFilters::StructureToItkImage(ImageFilters::rescaleIntensity(image->returnStruct())));
    connector->Modified();
    connector->Update();
    castFilter->SetOutputScalarTypeToUnsignedShort();
    castFilter->SetInput(connector->GetOutput());
    opticityTransferFunction->AddPoint(0,         0.0);
    opticityTransferFunction->AddPoint(255,       1.0);
    colorTransferFunction->AddRGBPoint(0,   0.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(255, 1.0, 1.0, 1.0);
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opticityTransferFunction);
    volumeMapper->SetVolumeRayCastFunction(compositeFunction);
    volumeMapper->SetInput(castFilter->GetOutput());
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
    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
}
void RenderIT::removeImage(void)
{
    renderer->RemoveVolume(volume);
    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
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
    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
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
        if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
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
    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();

}
void RenderIT::setBackgroundColor(float r, float g, float b)
{
    renderer->SetBackground(r, g, b);
    renderer->GetRenderWindow()->Render();
}
void RenderIT::setImageColor(float r, float g, float b)
{
    colorTransferFunction->RemoveAllPoints();
    colorTransferFunction->AddRGBPoint(0,   0.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(255, r, g, b);
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
    opticityTransferFunction->AddPoint(0,         min);
    opticityTransferFunction->AddPoint(255,       max);
    if(renderer->GetRenderWindow() != NULL) renderer->GetRenderWindow()->Render();
}

//----------------------------------------------------------------------------------------

RenderITWindow::RenderITWindow(int style):
    RenderIT()
{
    vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkOrientationMarkerWidget> widget =
      vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    vtkSmartPointer<vtkAxesActor> axes =
      vtkSmartPointer<vtkAxesActor>::New();

    renderWindow->AddRenderer(renderer);
    renderWindowInteractor->SetRenderWindow(renderWindow);

    widget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
    widget->SetOrientationMarker( axes );
    widget->SetInteractor( renderWindowInteractor );
    widget->SetViewport( 0.0, 0.0, 0.4, 0.4 );
    widget->SetEnabled( 1 );
    widget->InteractiveOn();

    renderWindow->Render();

    switch(style)
    {
    case 1:
    {
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
        vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    renderWindowInteractor->SetInteractorStyle(style);
    } break;

    case 2:
    {
    vtkSmartPointer<vtkInteractorStyleSwitch> style =
          vtkSmartPointer<vtkInteractorStyleSwitch>::New();
    renderWindowInteractor->SetInteractorStyle(style);
    } break;
    }
    renderWindowInteractor->Start();
}
