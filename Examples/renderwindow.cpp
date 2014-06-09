/** \example renderwindow.cpp
 * Przykład pokazuje jak załadować obraz rastrowy i opis drzewa z plików a następnie wyświetlić je w oknie.
 * \image html renderwindow.jpg
 * \image latex renderwindow.eps
 */

#include "renderit.h"
#include "tree.h"
#include "buildtree.h"
#include "image.h"


//#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
//#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderer.h>
//#include <vtkPolyData.h>
//#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
//#include <vtkWindowToImageFilter.h>
//#include <vtkPNGWriter.h>
//#include <vtkGraphicsFactory.h>
//#include <vtkSmartPointer.h>
//#include <vtkImageCast.h>
//#include <vtkPiecewiseFunction.h>
//#include <vtkColorTransferFunction.h>
//#include <vtkVolumeProperty.h>
//#include <vtkVolumeRayCastMapper.h>
//#include <vtkVolumeRayCastCompositeFunction.h>
//#include <vtkVolumeTextureMapper3D.h>
//#include <vtkVolume.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTrackball.h>
//#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
//    #include <windows.h>
//#else
//    #include <unistd.h>
//#endif

int main(int argc, char *argv[])
{
//    typedef itk::Image<unsigned char, 3> VisualizingImageType;
//      typedef itk::ImageFileReader< VisualizingImageType >  ReaderType;
//      ReaderType::Pointer reader = ReaderType::New();
//      reader->SetFileName( argv[1] );
//      reader->Update();
//      VisualizingImageType::Pointer image=reader->GetOutput();

//      vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
//      vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
//      ren1->SetBackground(0.5f,0.5f,1.0f);

//      renWin->AddRenderer(ren1);
//      renWin->SetSize(1280,1024);
//      vtkSmartPointer<vtkRenderWindowInteractor> iren =
//          vtkSmartPointer<vtkRenderWindowInteractor>::New();
//      iren->SetRenderWindow(renWin);
//      renWin->Render(); // make sure we have an OpenGL context.

//      typedef itk::ImageToVTKImageFilter<VisualizingImageType> itkVtkConverter;
//      itkVtkConverter::Pointer conv=itkVtkConverter::New();
//      conv->SetInput(image);
//      conv->Update();

//      vtkSmartPointer<vtkImageData> image2=vtkSmartPointer<vtkImageData>::New();
//      image2->ShallowCopy(conv->GetOutput());
//      //shallow copy is vtk's equivalent of disconnect pipeline

//      vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper =
//          vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
//      volumeMapper->SetInputData(image2);

//      vtkSmartPointer<vtkVolumeProperty> volumeProperty =
//      vtkSmartPointer<vtkVolumeProperty>::New();

//      vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
//      vtkSmartPointer<vtkPiecewiseFunction>::New();
//      compositeOpacity->AddPoint(0.0,0.0);
//      compositeOpacity->AddPoint(80.0,1.0);
//      compositeOpacity->AddPoint(80.1,0.0);
//      compositeOpacity->AddPoint(255.0,0.0);
//      volumeProperty->SetScalarOpacity(compositeOpacity);

//      vtkSmartPointer<vtkColorTransferFunction> color =
//      vtkSmartPointer<vtkColorTransferFunction>::New();
//      color->AddRGBPoint(0.0  ,0.0,0.0,1.0);
//      color->AddRGBPoint(40.0  ,1.0,0.0,0.0);
//      color->AddRGBPoint(255.0,1.0,1.0,1.0);
//      volumeProperty->SetColor(color);

//      vtkSmartPointer<vtkVolume> volume =
//      vtkSmartPointer<vtkVolume>::New();
//      volume->SetMapper(volumeMapper);
//      volume->SetProperty(volumeProperty);

//      //Here we take care of position and orientation
//      //so that volume is in DICOM patient physical space
//      VisualizingImageType::DirectionType d=image->GetDirection();
//      vtkMatrix4x4 *mat=vtkMatrix4x4::New(); //start with identity matrix
//      for (int i=0; i<3; i++)
//          for (int k=0; k<3; k++)
//              mat->SetElement(i,k, d(i,k));

//      //counteract the built-in translation by origin
//      VisualizingImageType::PointType origin=image->GetOrigin();
//      volume->SetPosition(-origin[0], -origin[1], -origin[2]);

//      //add translation to the user matrix
//      for (int i=0; i<3; i++)
//          mat->SetElement(i,3, origin[i]);
//      volume->SetUserMatrix(mat);

//      //Add coordinate system axes, so we have a reference for position and orientation
//      vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
//      axes->SetTotalLength(250,250,250);
//      axes->SetShaftTypeToCylinder();
//      axes->SetCylinderRadius(0.01);
//      ren1->AddActor(axes);

//      ren1->AddVolume( volume );
//      ren1->ResetCamera();

//      renWin->Render();
//      renWin->Render();
//      renWin->Render();

//      iren->Start();




//    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
//    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//    mapper->SetInputConnection(sphereSource->GetOutputPort());
//    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
//    actor->SetMapper(mapper);

//    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//    renderWindow->AddRenderer(renderer);

//    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
//    renderWindowInteractor->SetInteractorStyle(style);
//    renderWindowInteractor->SetRenderWindow(renderWindow);

//    renderer->AddActor(actor);
//    renderer->SetBackground(0, 1, 1);
//    renderWindow->Render();
//    renderWindowInteractor->Start();






    Image image;
    Tree tree;
    if(argc < 3) return 1;
    image.fillStructure(BuildTree::openAnalyzeImage(argv[1]));
    //tree.loadTree(argv[2]);
    RenderITWindow window = RenderITWindow(1);
    window.setImage(&image);
    //window.addTree(tree, 1);
    //window.setTreeColor(1.0, 0.5, 0, 0);

    window.setBackgroundColor(0.8, 0.8, 0.5);
    //window.showAndGo();
    window.show();



    return 0;
}
