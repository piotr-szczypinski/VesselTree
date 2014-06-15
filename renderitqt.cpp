#include "renderitqt.h"

RenderITQt::RenderITQt(QWidget* parent, Qt::WFlags f):RenderIT(),QVTKWidget(parent, f)
{
    vtkRenderWindow *renderWindow = vtkOpenGLRenderWindow::New();
    renderWindow->AddRenderer(renderer);
    renderWindow->SetStereoTypeToDresden();
    this->SetRenderWindow(renderWindow);
}
