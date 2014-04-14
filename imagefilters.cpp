#include "image.h"
#include "imagefilters.h"

ImageStructure ImageFilters::itkImageToStructure(ImageType::Pointer par1)
{
    //--Konwersja obrazu w formacie ITK do postaci struktury--//
    ImageStructure tempImage;

    for(int i = 0; i<par1->GetLargestPossibleRegion().GetImageDimension(); i++)
    {
        tempImage.size.push_back(par1->GetLargestPossibleRegion().GetSize()[i]);
        tempImage.spacing.push_back(par1->GetSpacing()[i]);
        tempImage.origin.push_back(par1->GetOrigin()[i]);
    }
    tempImage.fileType = FLOAT32;
    float *imgArray = par1->GetPixelContainer()->GetImportPointer();
    tempImage.imageData = imgArray;
    par1->GetPixelContainer()->SetContainerManageMemory(false);
    return tempImage;
}

ImageType::Pointer ImageFilters::StructureToItkImage(ImageStructure par1)
{
    //--Konwersja struktury obrazu do formatu ITK--//
    ImageType::Pointer emptyImage = ImageType::New();
    ImageType::RegionType Region;
    ImageType::RegionType::IndexType Start;
    ImageType::RegionType::SizeType Size;

    double spac[3], orgi[3];
    for(int i = 0; i<3; i++)
    {
        Start[i] = 0;
        Size[i]  = par1.size[i];
        spac[i] = par1.spacing[i];
        orgi[i] = par1.origin[i];
    }
    Region.SetIndex( Start );
    Region.SetSize( Size );

    emptyImage->SetRegions( Region );
    emptyImage->SetOrigin(orgi);
    emptyImage->SetSpacing(spac);

    float *dane = (float*)(void*)par1.imageData;
    size_t numberOfPixels = emptyImage->GetLargestPossibleRegion().GetNumberOfPixels();
    emptyImage->GetPixelContainer()->SetImportPointer(dane, numberOfPixels, false);

    return emptyImage;
}

ImageStructure ImageFilters::openAnalyzeImage(std::string par1)
{
    //--Wczytanie obrazu w formacie .nii, .hdr, .img--//
    ReaderType::Pointer      reader = ReaderType::New();
    reader->SetFileName( par1 );
    reader->Update();

    return itkImageToStructure(reader->GetOutput());
}

ImageStructure ImageFilters::rescaleIntensity( ImageStructure par1, float min, float max )
{
    RescaleFilterType::Pointer rescale = RescaleFilterType::New();
    rescale->SetInput( StructureToItkImage(par1) );
    rescale->SetOutputMinimum( min );
    rescale->SetOutputMaximum( max );
    rescale->Update();
    return itkImageToStructure(rescale->GetOutput());
}

ImageStructure ImageFilters::gaussianFilter(ImageStructure par1, float par2)
{
    //--Filtracja gausowska, par2 odpowiedzialny za rozmycie--//
    GaussianFilterType::Pointer     gaussian = GaussianFilterType::New();
    gaussian->SetInput(StructureToItkImage(par1));
    gaussian->SetVariance( par2 );
    gaussian->Update();
    return itkImageToStructure(gaussian->GetOutput());
}

ImageStructure ImageFilters::hessianFilter(ImageStructure par1, float par2)
{
    //-- Filtracja hessego z dan¹ wartoœci¹ sigma (par2) --//
    HessianFilterType::Pointer hessian = HessianFilterType::New();
    hessian->SetInput(StructureToItkImage(par1));
    hessian->SetSigma( par2 );
    VesselnessMeasureFilterType::Pointer vesselnessFilter = VesselnessMeasureFilterType::New();
    vesselnessFilter->SetInput( hessian->GetOutput() );
    vesselnessFilter->Update();
    return itkImageToStructure(vesselnessFilter->GetOutput());
}

ImageStructure ImageFilters::mipTwoImages(ImageStructure par1, ImageStructure par2, float par3)
{
    //-- z³o¿enie dwóch obrazów w jeden za zasadzie rzutowania najwiêkszych jasnoœci, (par3 jest wspó³czynnikiem sumowania) --//
    ImageType::Pointer mipImage = ImageType::New();
    mipImage = StructureToItkImage(par2);
    ImageType::Pointer inputImage = ImageType::New();
    inputImage = StructureToItkImage(par1);

    ConstIteratorType inputIterator(inputImage, inputImage->GetRequestedRegion());
    IteratorType mipIterator(mipImage, mipImage->GetRequestedRegion());
    for ( inputIterator.GoToBegin(), mipIterator.GoToBegin(); !inputIterator.IsAtEnd(); ++inputIterator, ++mipIterator)
    {
        if (par3*(inputIterator.Get())> mipIterator.Get()){mipIterator.Set(inputIterator.Get());}
    }
    return itkImageToStructure(mipImage);
}

ImageStructure ImageFilters::RegionGrowing(ImageStructure par1, float par2, float par3, std::vector<unsigned int> coord)
{
    //-- rozrost obszaru od zarodka (coord), par2 i par3 to odpowiednio próg dolny i górny
    ConnectedFilterType::Pointer ct = ConnectedFilterType::New();
    ImageType::IndexType seed;
    seed[0] = coord[0];   seed[1] = coord[1];    seed[2] = coord[2];
    ct->SetInput(StructureToItkImage(par1));
    ct->SetSeed(seed);
    ct->SetLower(par2);
    ct->SetUpper(par3);
    ct->SetReplaceValue(1);
    ct->Update();
    return itkImageToStructure(ct->GetOutput());
    std::cout<<"region growing"<<std::endl;
}

ImageStructure ImageFilters::CreateEmptyStructure(ImageStructure par1)
{
    //-- Tworzenie pustego obrazu ktory poslozy do operacji MIP kolejnych operacji filtracji --//
    ImageType::RegionType Region;
    ImageType::Pointer emptyImage = ImageType::New();
    ImageType::RegionType::IndexType Start;
    Start[0] = 0;
    Start[1] = 0;
    Start[2] = 0;
    Region.SetIndex( Start );
    ImageType::RegionType::SizeType Size;
    Size[0]  = par1.size[0];
    Size[1]  = par1.size[1];
    Size[2]  = par1.size[2];
    Region.SetSize( Size );
    emptyImage->SetRegions( Region );
    emptyImage->Allocate();
    emptyImage->FillBuffer(0);
    emptyImage->SetOrigin(StructureToItkImage(par1)->GetOrigin());
    emptyImage->SetSpacing(StructureToItkImage(par1)->GetSpacing());
    return itkImageToStructure(emptyImage);
}

ImageStructure ImageFilters::MultiscaleHessianAlgorithm(ImageStructure par1, float sigmaMin, float sigmaMax, int noOfScales)
{
    //wieloskalowa filtracja hessego (noOfScales - iloœæ powtórzeñ), od warotœci minimalnej (sigmaMin) do maksymalnej (sigmaMax)--//
    ImageStructure img;

    img = CreateEmptyStructure(par1);

    for(int i = 0; i<noOfScales; i++)
    {
        float sigma;
        if(noOfScales > 1)  sigma=sigmaMin+(((sigmaMax-sigmaMin)/(sigmaMax-1))*i);
        else                sigma=sigmaMin;
        img = mipTwoImages( hessianFilter(par1, sigma), img, 1);
    }
    return img;
}

std::vector<unsigned int> ImageFilters::FindSeed(ImageStructure par1)
{
    //--Automatyczne znalezienie punktu startowego do rozrostu obszaru
    ImageType::RegionType MiniRegion;
    ImageType::RegionType::IndexType start;
    start[0] = (par1.size[0]/2)-(par1.size[0]/8);
    start[1] = (par1.size[1]/2)-(par1.size[1]/8);
    start[2] = (par1.size[2]/2)-(par1.size[2]/8);
    MiniRegion.SetIndex( start );

    ImageType::RegionType::SizeType size;
    size[0]  = par1.size[0]/4;
    size[1]  = par1.size[1]/4;
    size[2]  = par1.size[2]/4;
    MiniRegion.SetSize( size );

    ImageType::IndexType  position;
    float pvmax = 0;
    IteratorTypeWI searchIt( StructureToItkImage(par1), MiniRegion );
    for( searchIt.GoToBegin();!searchIt.IsAtEnd();++searchIt)
    {
     if(searchIt.Get()> pvmax)
        {
         pvmax = searchIt.Get();
         position = searchIt.GetIndex();
        }
    }
    std::vector<unsigned int> point;
    point.push_back(position[0]); point.push_back(position[0]); point.push_back(position[0]);
    std::cout<<"seed"<<point[0]<<point[1]<<point[2]<<std::endl;
    return point;
}

double ImageFilters::FindMaximumValue(ImageStructure par1)
{
    //--Wartoœæ maksymalna
    ImageCalculatorFilterType::Pointer imageCalculatorFilter = ImageCalculatorFilterType::New ();
    imageCalculatorFilter->SetImage(StructureToItkImage(par1));
    imageCalculatorFilter->Compute();
    return imageCalculatorFilter->GetMaximum();
}

double ImageFilters::FindMinimumValue(ImageStructure par1)
{
    //--Wartoœæ minimalna
    ImageCalculatorFilterType::Pointer imageCalculatorFilter = ImageCalculatorFilterType::New ();
    imageCalculatorFilter->SetImage(StructureToItkImage(par1));
    imageCalculatorFilter->Compute();
    return imageCalculatorFilter->GetMinimum();
}

void ImageFilters::saveImage(ImageStructure par1, std::string par2)
{
    //--Zapis obrazu na dysk, par2 to nazwa pliku--//
    WriterType::Pointer      writer = WriterType::New();
    writer->SetFileName( par2 );
    writer->SetInput( StructureToItkImage(par1)  );
    writer->Update();
}

ImageStructure ImageFilters::HVSalgorithm(ImageStructure par1, int noOfScales, float thresholdPercent)
{
    //--Pe³ny algorytm detekcji naczyñ wyko¿ystuj¹cy wieloskalow¹filtracje i rozrost obszaru
    ImageStructure img;
    img = MultiscaleHessianAlgorithm(par1, 0.1, 2.0, noOfScales);
    saveImage(img, "plikTEMP.nii");
    std::vector<unsigned int> point2;
    point2.push_back(50); point2.push_back(50); point2.push_back(50);
    img = RegionGrowing(img, FindMaximumValue(img)*thresholdPercent, FindMaximumValue(img), FindSeed(img));
    return img;
}
