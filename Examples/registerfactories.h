#ifndef REGISTERFACTORIES_H
#define REGISTERFACTORIES_H

//#include<itkAnalyzeImageIOFactory.h>
#include<itkBioRadImageIOFactory.h>
#include<itkHDF5ImageIOFactory.h>
#include<itkOFFMeshIOFactory.h>
#include<itkBMPImageIOFactory.h>
#include<itkHDF5TransformIOFactory.h>
#include<itkPNGImageIOFactory.h>
#include<itkBruker2DSEQImageIOFactory.h>
#include<itkImageIOFactory.h>
#include<itkSiemensVisionImageIOFactory.h>
#include<itkBYUMeshIOFactory.h>
#include<itkJPEG2000ImageIOFactory.h>
#include<itkStimulateImageIOFactory.h>
#include<itkFileListVideoIOFactory.h>
#include<itkJPEGImageIOFactory.h>
#include<itkTIFFImageIOFactory.h>
#include<itkFreeSurferAsciiMeshIOFactory.h>
#include<itkLSMImageIOFactory.h>
#include<itkTransformIOFactory.h>
#include<itkFreeSurferBinaryMeshIOFactory.h>
#include<itkMatlabTransformIOFactory.h>
#include<itkTransformIOFactory.hxx>
#include<itkGDCMImageIOFactory.h>
#include<itkMeshIOFactory.h>
#include<itkTxtTransformIOFactory.h>
#include<itkGE4ImageIOFactory.h>
#include<itkMetaImageIOFactory.h>
#include<itkVideoIOFactory.h>
#include<itkGE5ImageIOFactory.h>
#include<itkMRCImageIOFactory.h>
#include<itkVoxBoCUBImageIOFactory.h>
#include<itkGEAdwImageIOFactory.h>
#include<itkNiftiImageIOFactory.h>
#include<itkVTKImageIOFactory.h>
#include<itkGiftiMeshIOFactory.h>
#include<itkNrrdImageIOFactory.h>
#include<itkVTKPolyDataMeshIOFactory.h>
#include<itkGiplImageIOFactory.h>
#include<itkOBJMeshIOFactory.h>

void registerFactories(void)
{
    itk::MetaImageIOFactory::RegisterOneFactory();
    itk::PNGImageIOFactory::RegisterOneFactory();
    itk::NiftiImageIOFactory::RegisterOneFactory();
    itk::MatlabTransformIOFactory::RegisterOneFactory();
    itk::TIFFImageIOFactory::RegisterOneFactory();
    itk::SiemensVisionImageIOFactory::RegisterOneFactory();
    //itk::Bruker2DSEQImageIOFactory::RegisterOneFactory();
    itk::HDF5ImageIOFactory::RegisterOneFactory();
    itk::JPEGImageIOFactory::RegisterOneFactory();
    //itk::JPEG2000ImageIOFactory::RegisterOneFactory();


    typedef std::list< itk::LightObject::Pointer > RegisteredObjectsContainerType;
    RegisteredObjectsContainerType registeredIOs = itk::ObjectFactoryBase::CreateAllInstance( "itkImageIOBase" );
    std::cout << "Registered ImageFileReader IO objects: " << registeredIOs.size() << std::endl;
}

#endif // REGISTERFACTORIES_H
