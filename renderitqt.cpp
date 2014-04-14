#include "renderitqt.h"

RenderITQt::RenderITQt(QWidget* parent, Qt::WFlags f):RenderIT(),QVTKWidget(parent, f)
{
    vtkRenderWindow *renderWindow = vtkRenderWindow::New();
    renderWindow->AddRenderer(renderer);
    renderWindow->SetStereoTypeToDresden();
    this->SetRenderWindow(renderWindow);
}
