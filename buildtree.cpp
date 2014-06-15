/*
 * VesselTree - MRI image segmentation and characterization
 *
 * Copyright 2014  Piotr M. Szczypiński <piotr.szczypinski@p.lodz.pl>
 *                 Adam Sankowski <sankowski@gmail.com>
 *                 Grzegorz Dwojakowski <grzegorz.dwojakowski@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "buildtree.h"

#include "itkImageDuplicator.h"
#include <sstream>
#include <iostream>
#include <fstream>


typedef itk::Image< float,3 > ImageType;
typedef itk::ImageFileReader< ImageType > ReaderType;
typedef itk::ConstantBoundaryCondition< ImageType > BoundaryConditionType;
typedef itk::NeighborhoodIterator< ImageType > NeighborhoodIteratorType1;
typedef itk::NeighborhoodIterator< ImageType, BoundaryConditionType> NeighborhoodIteratorType;
typedef itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;
typedef itk::ImageFileWriter< ImageType >  WriterType;
typedef itk::DiscreteGaussianImageFilter< ImageType, ImageType> GaussianFilterType;
typedef itk::RescaleIntensityImageFilter< ImageType,ImageType> RescaleFilterType;
typedef itk::BinaryBallStructuringElement< float, 3> StructuringElementType;
typedef itk::WhiteTopHatImageFilter< ImageType, ImageType, StructuringElementType >WhiteFilterType;
typedef itk::BlackTopHatImageFilter< ImageType, ImageType, StructuringElementType >BlackFilterType;
typedef itk::GrayscaleDilateImageFilter <ImageType, ImageType, StructuringElementType>GrayscaleDilateImageFilterType;
typedef itk::GrayscaleErodeImageFilter <ImageType, ImageType, StructuringElementType>GrayscaleErodeImageFilterType;
typedef itk::ConnectedThresholdImageFilter< ImageType, ImageType > ConnectedFilterType;
typedef itk::VotingBinaryIterativeHoleFillingImageFilter<ImageType > HoleFilterType;
typedef itk::BinaryThinningImageFilter3D< ImageType, ImageType > ThinningFilterType;
typedef itk::ImageToVTKImageFilter< ImageType > Connector;
//----------------------------------------------------------------------------------------
ImageStructure BuildTree::itkImageToStructure(itk::Image< float,3 >::Pointer par1)
{
    //--Konwersja obrazu w formacie ITK do postaci struktury--//
    ImageStructure tempImage;

    for(int i = 0; i<par1->GetLargestPossibleRegion().GetImageDimension(); i++)
    {
        tempImage.size.push_back(par1->GetLargestPossibleRegion().GetSize()[i]);
        tempImage.spacing.push_back(par1->GetSpacing()[i]);
        tempImage.origin.push_back(par1->GetOrigin()[i]);
    }
    tempImage.fileType = VFLOAT32;
    float *imgArray = par1->GetPixelContainer()->GetImportPointer();
    tempImage.imageData = imgArray;
    par1->GetPixelContainer()->SetContainerManageMemory(false);
    return tempImage;
}
//----------------------------------------------------------------------------------------
itk::Image< float,3 >::Pointer BuildTree::StructureToItkImage(ImageStructure par1)
{
    //--Konwersja struktury obrazu do formatu ITK--//
    itk::Image< float,3 >::Pointer emptyImage = itk::Image< float,3 >::New();
    itk::Image< float,3 >::RegionType Region;
    itk::Image< float,3 >::RegionType::IndexType Start;
    itk::Image< float,3 >::RegionType::SizeType Size;

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
//----------------------------------------------------------------------------------------
ImageStructure BuildTree::openAnalyzeImage(std::string par1)
{
    //--Wczytanie obrazu w formacie .nii, .hdr, .img--//
    ReaderType::Pointer      reader = ReaderType::New();
    reader->SetFileName( par1 );
    reader->Update();

    return itkImageToStructure(reader->GetOutput());
}
//----------------------------------------------------------------------------------------
ImageStructure BuildTree::gaussianFilter(ImageStructure par1, float par2)
{
    //--Filtracja gausowska, par2 odpowiedzialny za rozmycie--//
    GaussianFilterType::Pointer     gaussian = GaussianFilterType::New();
    gaussian->SetInput(StructureToItkImage(par1));
    gaussian->SetVariance( par2 );
    gaussian->Update();
    return itkImageToStructure(gaussian->GetOutput());
}
//----------------------------------------------------------------------------------------
void BuildTree::saveImage(ImageStructure par1, std::string par2)
{
    //--Zapis obrazu na dysk, par2 to nazwa pliku--//
    WriterType::Pointer      writer = WriterType::New();
    writer->SetFileName( par2 );
    writer->SetInput( StructureToItkImage(par1)  );
    writer->Update();
}
//----------------------------------------------------------------------------------------
ImageStructure BuildTree::reskalowanie (ImageStructure par1, float par2, float par3)
{
    //par2 - dolna wartoæ jasnoci
    //par3 - górna wartoæ jasnoci
    RescaleFilterType::Pointer rescale = RescaleFilterType::New();
    rescale->SetInput( StructureToItkImage(par1) );
    rescale->SetOutputMinimum( par2 );
    rescale->SetOutputMaximum( par3 );
    rescale->Update();
    return itkImageToStructure(rescale->GetOutput());
}
//----------------------------------------------------------------------------------------
ImageStructure BuildTree::wth (ImageStructure par1, float par2)
{
    //par2 - promieñ elementu strukturuj¹cego
    StructuringElementType ball;
    ball.SetRadius(par2);
    ball.CreateStructuringElement();
    WhiteFilterType::Pointer wtophat = WhiteFilterType::New();
    wtophat->SetKernel( ball );
    wtophat->SetInput( StructureToItkImage(par1) );
    wtophat->Update();
    return itkImageToStructure(wtophat->GetOutput());
}
//----------------------------------------------------------------------------------------
ImageStructure BuildTree::bth (ImageStructure par1, float par2)
{
    //par2 - promieñ elementu strukturuj¹cego
    StructuringElementType ball;
    ball.SetRadius(par2);
    ball.CreateStructuringElement();
    BlackFilterType::Pointer btophat = BlackFilterType::New();
    btophat->SetKernel( ball );
    btophat->SetInput( StructureToItkImage(par1) );
    btophat->Update();
    return itkImageToStructure(btophat->GetOutput());
}
//----------------------------------------------------------------------------------------
ImageStructure BuildTree::dylatacja (ImageStructure par1, float par2)
{
    //par2 - promieñ elementu strukturuj¹cego
    StructuringElementType structuringElement;
    structuringElement.SetRadius(par2);
    structuringElement.CreateStructuringElement();
    GrayscaleDilateImageFilterType::Pointer dilateFilter = GrayscaleDilateImageFilterType::New();
    dilateFilter->SetInput(StructureToItkImage(par1));
    dilateFilter->SetKernel(structuringElement);
    dilateFilter->Update();
    return itkImageToStructure(dilateFilter->GetOutput());
}
//----------------------------------------------------------------------------------------
ImageStructure BuildTree::erozja (ImageStructure par1, float par2)
{
    //par2 - promieñ elementu strukturuj¹cego
    StructuringElementType structuringElement;
    structuringElement.SetRadius(par2);
    structuringElement.CreateStructuringElement();
    GrayscaleErodeImageFilterType::Pointer erodeFilter = GrayscaleErodeImageFilterType::New();
    erodeFilter->SetInput(StructureToItkImage(par1));
    erodeFilter->SetKernel(structuringElement);
    erodeFilter->Update();
    return itkImageToStructure(erodeFilter->GetOutput());
}
//----------------------------------------------------------------------------------------
ImageStructure BuildTree::rozrost (ImageStructure par1, float par2, float par3, float par4, float par5, float par6)
{
    //par2 - wartoæ dolnego progu
    //par3 - wartoæ górnego progu
    //par4 - wspó³rzêdna x punktu startowego
    //par5 - wspó³rzêdna y punktu startowego
    //par6 - wspó³rzêdna z punktu startowego
    ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
    connectedThreshold->SetInput( StructureToItkImage(par1) );
    connectedThreshold->SetLower(  par2 );
    connectedThreshold->SetUpper(  par3  );
    connectedThreshold->SetReplaceValue( 1 );
    ImageType::IndexType  i;
    i[0]=par4;i[1]=par5;i[2]=par6;
    connectedThreshold->SetSeed( i );
    connectedThreshold->Update();
    return itkImageToStructure(connectedThreshold->GetOutput());
}
//----------------------------------------------------------------------------------------
ImageStructure BuildTree::rozrost_automatyczny (ImageStructure par1, float par2, float par3)
{
    //par2 - dolny zakres progu
    //par3 - górny zakres progu
    //----------regiony
   ImageType::RegionType Region;
   ImageType::RegionType::IndexType Start;
   Start[0] = 0;
   Start[1] = 0;
   Start[2] = 0;
   Region.SetIndex( Start );
   ImageType::RegionType::SizeType size;
   size[0]  = StructureToItkImage(par1)->GetRequestedRegion().GetSize()[0];
   size[1]  = StructureToItkImage(par1)->GetRequestedRegion().GetSize()[1];
   size[2]  = StructureToItkImage(par1)->GetRequestedRegion().GetSize()[2];
   Region.SetSize( size );

   ImageType::RegionType MiniRegion;
   ImageType::RegionType::IndexType start;
   start[0] = (size[0]/2)-(size[0]/8);
   start[1] = (size[1]/2)-(size[1]/8);
   start[2] = (size[2]/2)-(size[2]/8);
   MiniRegion.SetIndex( start );

   ImageType::RegionType::SizeType size2;
   size2[0]  = size[0]/4;
   size2[1]  = size[1]/4;
   size2[2]  = size[2]/4;
   MiniRegion.SetSize( size2 );
    //----------wybor punktu rozrostu obszaru
    ImageType::IndexType  i;
    int pvmax = 0;
    IteratorType searchIt(   StructureToItkImage(par1), MiniRegion );
    for( searchIt.GoToBegin();!searchIt.IsAtEnd();++searchIt)
    {
     if(searchIt.Get()> pvmax)
        {
         pvmax = searchIt.Get();
         i = searchIt.GetIndex();
        }
    }
    pvmax = 0;
    IteratorType searchIt2(   StructureToItkImage(par1), Region );
    for( searchIt2.GoToBegin();!searchIt2.IsAtEnd();++searchIt2)
    {
     if(searchIt2.Get()> pvmax)
        {
         pvmax = searchIt2.Get();
        }
    }
    //----------rozrosty obszaru i zliczanie wokseli dla ka¿dego progu
    int min=par2,max=par3;
    int diff = max - min;
    ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
    int tablica_progow [diff];
    for ( int licz=0; licz< diff; licz++)
    {
           tablica_progow [licz] = licz+1;
    }

    int tablica_wokseli [diff];
    for ( int licz=0; licz< diff; licz++)
    {
           tablica_wokseli [licz] = 0;
    }

   for ( int licz = min; licz <= max; licz++)
   {
    connectedThreshold->SetInput( StructureToItkImage(par1) );
    connectedThreshold->SetLower(  licz  );
    connectedThreshold->SetUpper(  pvmax  );
    connectedThreshold->SetReplaceValue( 1 );
    connectedThreshold->SetSeed( i );
    connectedThreshold->Update();
    ImageType::Pointer newImage =  connectedThreshold->GetOutput();
    ImageType::IndexType pixel2Index;
    pixel2Index[0] = 0;
    pixel2Index[1] = 0;
    pixel2Index[2] = 0;
    unsigned int a,b,c;
    int licznik=0;
    for(a=0 ; a<size[0] ; a++)
     {
           for(b=0 ; b<size[1] ; b++)
           {
                   for(c=0; c<size[2] ; c++)
                   {
                   pixel2Index[0] = a;
                   pixel2Index[1] = b;
                   pixel2Index[2] = c;
                   ImageType::PixelType pixel2Value = newImage->GetPixel( pixel2Index );

                   if(pixel2Value==1)
                           {
                                   licznik+=1;
                           }

                   }
           }
     }
     tablica_wokseli [licz] = licznik;
   }

   //---------------oblicznie ró¿nic miêdzy progami i wybór najwiêkszej

   int tablica_roznic [diff];
   for ( int licz=0; licz< diff; licz++)
   {
           tablica_roznic [licz] = 0;
   }

   for ( int licz = min; licz <= max; licz++)
   {
        tablica_roznic [licz] = tablica_wokseli[licz-1] - tablica_wokseli[licz];
   }
   int najwieksza_roznica=tablica_roznic[min];
   int indeks_najwiekszej_roznicy;
   for ( int licz = min; licz <= max; licz++)
   {
           if(tablica_roznic[licz]>najwieksza_roznica)
           {
               najwieksza_roznica=tablica_roznic[licz];
               indeks_najwiekszej_roznicy=licz;
           }
   }
   std::cout << "best threshold value :"<<indeks_najwiekszej_roznicy<<std::endl;

   //------------------rozrost obszaru z najlepszym progiem

    connectedThreshold->SetInput( StructureToItkImage(par1) );
    connectedThreshold->SetLower(  indeks_najwiekszej_roznicy );
    connectedThreshold->SetUpper(  pvmax  );
    connectedThreshold->SetReplaceValue( 1 );
    connectedThreshold->SetSeed( i );
    connectedThreshold->Update();

    return itkImageToStructure(connectedThreshold->GetOutput());
}
//----------------------------------------------------------------------------------------
ImageStructure BuildTree::wypelnianie (ImageStructure par1, float par2, float par3)
{
    //par2 - promieñ elementu strukturuj¹cego
    //par3 - maksymalna liczba iteracji
    HoleFilterType::Pointer holefilling = HoleFilterType::New();
    ImageType::SizeType indexRadius;
    indexRadius[0] = par2;
    indexRadius[1] = par2;
    indexRadius[2] = par2;
    holefilling->SetRadius( indexRadius );
    holefilling->SetBackgroundValue( 0 );
    holefilling->SetForegroundValue( 1 );
    holefilling->SetMajorityThreshold( 0 );
    holefilling->SetMaximumNumberOfIterations( par3 );
    holefilling->SetInput( StructureToItkImage(par1) );
    holefilling->Update();
    return itkImageToStructure(holefilling->GetOutput());
}
//----------------------------------------------------------------------------------------
ImageStructure BuildTree::pocienianie (ImageStructure par1)
{
    ThinningFilterType::Pointer thinningFilter = ThinningFilterType::New();
    thinningFilter->SetInput( StructureToItkImage(par1) );
    thinningFilter->Update();
    return itkImageToStructure(thinningFilter->GetOutput());
}

//----------------------------------------------------------------------------------------
Tree BuildTree::skeletonToTree(ImageStructure image)
{
    ImageType::Pointer inputKImage = StructureToItkImage(image);

// Find tip of whatever brunch to start
    NeighborhoodIteratorType::RadiusType radius;
    radius[0]=1; radius[1]=1; radius[2]=1;
    ImageType::IndexType regionIndex;
    regionIndex[0]=0;regionIndex[1]=0;regionIndex[2]=0;
    ImageType::SizeType regionSize = inputKImage->GetRequestedRegion().GetSize();
    ImageType::RegionType region;
    region.SetSize(regionSize);
    region.SetIndex(regionIndex);
    itk::NeighborhoodIterator<ImageType> iteratorStart(radius, inputKImage, region);

    int sc = iteratorStart.Size() / 2;
    int sy = iteratorStart.GetStride(1);
    int sz = iteratorStart.GetStride(2);

    for(iteratorStart.GoToBegin(); !iteratorStart.IsAtEnd(); ++iteratorStart)
    {
        int neighbors = 0;
        unsigned int i = 1;
        if(iteratorStart.GetCenterPixel() <= 0) continue;
        for(; i < 7; i++)
        {
            const int* nss = neighborhoodScanSequence[i];
            if(iteratorStart.GetPixel(sc+nss[0]+nss[1]*sy+nss[2]*sz) > 0) neighbors++;
        }
        if(neighbors == 1) break;
        neighbors = 0;
        for(; i < 19; i++)
        {
            const int* nss = neighborhoodScanSequence[i];
            if(iteratorStart.GetPixel(sc+nss[0]+nss[1]*sy+nss[2]*sz) > 0) neighbors++;
        }
        if(neighbors == 1) break;
        neighbors = 0;
        for(; i < 27; i++)
        {
            const int* nss = neighborhoodScanSequence[i];
            if(iteratorStart.GetPixel(sc+nss[0]+nss[1]*sy+nss[2]*sz) > 0) neighbors++;
        }
        if(neighbors == 1) break;
    }
    ImageType::IndexType startNodeIndex = iteratorStart.GetIndex();
// Now branch tip is available from startNodeIndex
//    std::cout << startNodeIndex << std::endl;

// Copy image to a buffer
    typedef itk::ImageDuplicator< ImageType > DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(inputKImage);
    duplicator->Update();
    ImageType::Pointer bufferImage = duplicator->GetOutput();

// Branch tracking based on flood-fill algorithm with stack
    Tree treeToCreate;
    std::vector<NodeIn3D> newBranch;
    std::vector<Index3DNeighbor> bifurStack;
    NodeIn3D newNode;
    Index3DNeighbor newNodeIndex;
    newNodeIndex.i = startNodeIndex;
    newNodeIndex.n = 1;
    newNode.connections = 0;
    newNode.diameter = 0;
    bifurStack.push_back(newNodeIndex);
    bufferImage->SetPixel(newNodeIndex.i, 0);

    while(bifurStack.size() > 0)
    {
        if(bifurStack.back().n >= 27)
        {
            bifurStack.pop_back();
            if(newBranch.size() > 0)
            {
                treeToCreate.addBranch(newBranch);
                newBranch.clear();
            }
        }
        else
        {
            regionIndex = bifurStack.back().i;
            region.SetIndex(regionIndex);
            itk::NeighborhoodIterator<ImageType> iteratorN(radius, bufferImage, region);

            for(; bifurStack.back().n < 27; bifurStack.back().n++)
            {
                const int* nss = neighborhoodScanSequence[bifurStack.back().n];
                int offset = sc+nss[0]+nss[1]*sy+nss[2]*sz;
                if(iteratorN.GetPixel(offset) > 0)
                {
                    if(newBranch.size() <= 0)
                    {
                        newNodeIndex.i = iteratorN.GetIndex(sc);
                        newNode.x = newNodeIndex.i[0];
                        newNode.y = newNodeIndex.i[1];
                        newNode.z = newNodeIndex.i[2];
                        newBranch.push_back(newNode);
//                        std::cout << newNodeIndex.i << std::endl;
                    }

                    newNodeIndex.i = iteratorN.GetIndex(offset);
                    newNodeIndex.n = 1;
                    bifurStack.push_back(newNodeIndex);

                    ImageType::IndexType pindex = iteratorN.GetIndex(offset);
                    bufferImage->SetPixel(pindex, 0);
                    newNode.x = pindex[0];
                    newNode.y = pindex[1];
                    newNode.z = pindex[2];
                    newBranch.push_back(newNode);
//                    std::cout << newNodeIndex.i << std::endl;
                    break;
                }
            }
        }
    }
    return treeToCreate;
}

//----------------------------------------------------------------------------------------

/*
TreeStructure BuildTree::szacowanie_polaczen(ImageStructure par1)
{
    //par1 - obraz scieniony(zawieraj�cy linie centralne)
    BasicBranch sb;
    Tree tr;

     //----------regiony
    ImageType::RegionType Region;
    ImageType::RegionType::IndexType Start;
    Start[0] = 0;
    Start[1] = 0;
    Start[2] = 0;
    Region.SetIndex( Start );
    ImageType::RegionType::SizeType size;
    size[0]  = StructureToItkImage(par1)->GetRequestedRegion().GetSize()[0];
    size[1]  = StructureToItkImage(par1)->GetRequestedRegion().GetSize()[1];
    size[2]  = StructureToItkImage(par1)->GetRequestedRegion().GetSize()[2];
    Region.SetSize( size );
    const ImageType::SpacingType& spacing = StructureToItkImage(par1)->GetSpacing();
    const ImageType::PointType& inputOrigin = StructureToItkImage(par1)->GetOrigin();
    ImageType::Pointer Conv = ImageType::New();
    Conv->SetRegions(Region);
    Conv->SetSpacing(spacing);
    Conv->SetOrigin(inputOrigin);
    Conv->Allocate();
    Conv->FillBuffer(0);
    IteratorType      Itaa(StructureToItkImage(par1),Region);
    IteratorType      Itqq(Conv,Region);
    for (Itaa.GoToBegin(),Itqq.GoToBegin();!Itaa.IsAtEnd(),!Itqq.IsAtEnd();++Itaa,++Itqq){Itqq.Set(Itaa.Get());}

    ImageType::Pointer Image = ImageType::New();
    Image->SetRegions(Region);
    Image->SetSpacing(spacing);
    Image->SetOrigin(inputOrigin);
    Image->Allocate();
    Image->FillBuffer(0);
    IteratorType      It(Conv,Region);
    IteratorType      Itq(Image,Region);
    for (It.GoToBegin(),Itq.GoToBegin();!It.IsAtEnd(),!Itq.IsAtEnd();++It,++Itq){Itq.Set(It.Get());}

    //------------estymacja zakonczen i bifurkacji
   NeighborhoodIteratorType::RadiusType radius;
   radius [0]=1;
   radius [1]=1;
   radius [2]=1;
   int center = 13;
   int neigh_num = 26;
    //-----------wykrywanie ko�c�wek -przeszukujemy ca�y ROI(nit3),
    //Szukamy voxela nale��cego do szkieletu(=1), kt�ry ma tylko jednego s�siada(ip=1),oznaczamy go jako ko�c�wk�(=3)
    NeighborhoodIteratorType nit3 ( radius, Image, Region );
    for (nit3.GoToBegin(); !nit3.IsAtEnd(); ++nit3)
        {
        int ip = 0;
        if(nit3.GetCenterPixel() == 1)
            {
            for(int i = 0; i<=neigh_num; i++)
               {
               if (i == center){}
               else {ip += nit3.GetPixel(i);}
               }
            if(ip == 1){nit3.SetCenterPixel(3);} //**zapisywanie warto�ci 3 jako zako�czenie
            }
        }
    //-------------"zjadanie" szkieletu,wykrywanie bifurkacji -przeszukujemy ca�y ROI(it) szukaj�c ko�c�wek(=3), dla ka�dek ko�c�wki rozpoczynamy �ledzenie.
    //zmieniamy tymczasowo warto�� ko�c�wki startowej na 3.1, �eby odr��ni� j� od pozosta�ych wewn�trz p�tli �ledzenia while.
    //Po zako�czeniu pojedy�czego �ledzenia nast�puje ponowna zmiana(=3)
    //Je�li kt�rykolwiek s�siad badanego voxela =3 lub =1 stanie si� on nast�pnym badanym voxelem(next).
    //P�tla ko�czy si� kiedy badany voxel jest ko�c�wk� (=3) lub nie znaleziono nast�pnego s�siada (wtedy badany voxel oznaczany jest jako zako�czenie (=4))
    //Droga pokonana przy ka�dym �ledzeniu oznaczana jest =2, �eby nie by�a widoczna dla kolejnych �ledze�.
    IteratorType it(Image,Region);
    ImageType::IndexType next;
    NeighborhoodIteratorType nit0 ( radius, Image, Region );
    for ( it.GoToBegin(); !it.IsAtEnd(); ++it)
       {
   //     start z zakonczenia, zamiast od razu zamiana na 2 to kolejka i zamiana na koncu sledzenia,
   //     przed szukaniem s�siada 1 szukanie s�siada 2, je�li znaleziony zamiana s�siada 2 na 4
   //     oraz centralnego na 4, potem normalna kontynuacja, czyli szukanie s�siada 1
        std::queue<int> que;
        ImageType::IndexType queidx,idx1,idx2;
        if(it.Get()==3)
            {
            it.Set(3.1);
            next = it.GetIndex();
            int stop = 0;

            while (stop == 0)
                {
                nit0.SetLocation(next);

                bool x = false;
                for(int i = 0; i<=neigh_num; i++)
                   {
                   if (i == center){}
                   else if(nit0.GetPixel(i) == 4){x = false; break;}
                   else if(nit0.GetPixel(i) == 2 && nit0.GetCenterPixel() == 1){idx1 = nit0.GetIndex();idx2 = nit0.GetIndex(i);x = true;}
                   }

                if(nit0.GetCenterPixel() == 1){queidx = nit0.GetIndex(); que.push(queidx[0]);que.push(queidx[1]);que.push(queidx[2]);nit0.SetCenterPixel(66);}

                bool nie_znaleziono = true;

                for(int i = 0; i<=neigh_num; i++)
                   {
                   if (i == center){}
                   else if(nit0.GetPixel(i) == 3){next = nit0.GetIndex(i);nie_znaleziono = false;}
                   }

                if (nie_znaleziono == true)
                   {
                   for(int i = 0; i<=neigh_num; i++)
                       {
                       if (i == center){}
                       else if(nit0.GetPixel(i) == 1){next = nit0.GetIndex(i);nie_znaleziono = false;break;}
                       }
                   }
                if (nie_znaleziono == false){}
                else if(nit0.GetCenterPixel() == 3) {stop = 1;}
                else{nit0.SetCenterPixel(4);stop = 1;x = false;}//**zapisywanie warto�ci 4 jako bifurkacji
                if (x == true){nit0.SetLocation(idx1);nit0.SetCenterPixel(4);nit0.SetLocation(idx2);nit0.SetCenterPixel(4);}//**zapisywanie warto�ci 4 jako bifurkacji

                }

            while (que.size() != 0)
                {
                queidx[0]=que.front();que.pop();queidx[1]=que.front();que.pop();queidx[2]=que.front();que.pop();
                nit0.SetLocation(queidx);
                if (nit0.GetCenterPixel() == 66){nit0.SetCenterPixel(2);}//**zapisywanie warto�ci 2 jako cz��ci ga��zi
                }
            it.Set(3);
            }
       }
    //--------------------------
    IteratorType      Iti(Image,Region);
    ImageType::Pointer ImageDirection = ImageType::New();
    ImageDirection->SetRegions(Region);
    ImageDirection->SetSpacing(spacing);
    ImageDirection->SetOrigin(inputOrigin);
    ImageDirection->Allocate();
    //powtarzanie wykrywania do skutku
    for(int i=0;i<4;i++)
    {
    ImageDirection->FillBuffer(0);
    IteratorType      It2(ImageDirection,Region);
    for (Iti.GoToBegin(),It2.GoToBegin();!Iti.IsAtEnd(),!It2.IsAtEnd();++Iti,++It2){if(Iti.Get() == 1){It2.Set(1);}}
    //-----------wykrywanie reszty bifurkacji -ten sam spos�b co przywykrywaniu ko�c�wek
    NeighborhoodIteratorType nit4 ( radius, ImageDirection, Region ); //obraz zawieraj�cy voxele pomini�te w �ledzeniu
    IteratorType itx ( Image, Region );
    for (nit4.GoToBegin(),itx.GoToBegin(); !nit4.IsAtEnd(),!itx.IsAtEnd(); ++nit4,++itx)
        {
        if(nit4.GetCenterPixel() == 1)
            {
            int ip = 0;
            for(int i = 0; i<=neigh_num; i++)
               {
               if (i == center){}
               else {ip += nit4.GetPixel(i);}
               }
            if(ip <= 1){itx.Set(5);}// 4- poprzestan na tym, 5 - wykrywanie ostatnich bifurkacji
            }
        }
    //-----------wykrywanie ostatnich bifurkacji
    for ( it.GoToBegin(); !it.IsAtEnd(); ++it)
       {
        if(it.Get() == 5)
            {
            it.Set(6);
            next = it.GetIndex();
            int stop = 0;
            while (stop == 0)
                {
                nit0.SetLocation(next);
                if(nit0.GetCenterPixel() == 1){nit0.SetCenterPixel(2);}//**zapisywanie warto�ci 2 jako cz��ci ga��zi

                             bool nie_znaleziono = true;
                             for(int i = 0; i<=neigh_num; i++)
                                {
                                if (i == center){}
                                else if(nit0.GetPixel(i) == 5){next = nit0.GetIndex(i);nie_znaleziono = false;}
                                }

                             if (nie_znaleziono == true)
                                {
                                for(int i = 0; i<=neigh_num; i++)
                                    {
                                    if (i == center){}
                                    else if(nit0.GetPixel(i) == 1){next = nit0.GetIndex(i);nie_znaleziono = false;}
                                    }
                                }
                             if (nie_znaleziono == false){}
                             else if(nit0.GetCenterPixel() == 5 || nit0.GetCenterPixel() == 3) {stop = 1;}
                             else{nit0.SetCenterPixel(4);stop = 1;} //**zapisywanie warto�ci 4 jako bifurkacji
                }
            it.Set(4);
            }
       }
    }
    //-----------
    IteratorType itek(Image,Region);
    int licznik_koncowek = 0;
    int licznik_bifurkacji = 0;
    for ( itek.GoToBegin(); !itek.IsAtEnd(); ++itek)
       {
        if(itek.Get() == 3){licznik_koncowek++;}
        if(itek.Get() == 4){licznik_bifurkacji++;}
       }
    //-----------korekta bifurkacji
    NeighborhoodIteratorType nit ( radius, Image, Region );
    ImageType::IndexType  indx;
    int licznik=0;
    for (nit.GoToBegin(); !nit.IsAtEnd(); ++nit)
        {
         if(nit.GetCenterPixel() == 4)
            {
             int ls = 0;// liczba s�siad�w

             for(int i = 0; i<=neigh_num; i++)
                {
                if (i == center){}
                else if(nit.GetPixel(i) > 0){ls++;}
                }

             if(ls == 2)//kiedy bifurkacja jest wykryta obok prawdziwej
                {//dla ka�dej bifurkacji, kt�ra posiada 2 s�siad�w szkieletu sprawdzane s� wszystkie s�siednie voxele. Je�li kt�ry� z nich ma 3 s�siad�w,
                 //jest oznaczany jako bifurkacja(=4). Natomiast poprzednio wykryty jako bifurkacja voxel zamieniany jest na zwyk�y voxel szkieletu(=2)
                 indx = nit.GetIndex();
                 licznik++;

                 for(int i = 0; i<=neigh_num; i++)
                    {
                    if (i == center){}
                    else
                       {
                       nit.SetLocation(nit.GetIndex(i));
                       ls = 0;
                       for(int i = 0; i<=neigh_num; i++)
                           {
                           if (i == center){}
                           else if(nit.GetPixel(i) > 0){ls++;}
                           if(nit.GetIndex(i) != indx && nit.GetPixel(i) == 4){ls = 0;}
                           }
                       if(ls >= 3 && nit.GetCenterPixel() == 2){nit.SetCenterPixel(4);nit.SetLocation(indx);nit.SetCenterPixel(2);goto end;}//**modyfikacja zakonczenia na cz��� ga��zi i odwrotnie
                       else {nit.SetLocation(indx);}
                       }
                     }
                 end:
                 nit.GetIndex();
                }
            }
        }
    //----------------estymacja kieruku lokalnego
     ImageDirection->FillBuffer(0);
    IteratorType it1(Image,Region);
    ImageType::IndexType sreidx;
    for(it1.GoToBegin();!it1.IsAtEnd();++it1)
    {
        if (it1.Get() == 3)
        {
            sreidx = it1.GetIndex();
        }
    }
    //dodajemy indeks ko�c�wki do kolejki
    std::queue<int> q;
    q.push(sreidx[0]);q.push(sreidx[1]);q.push(sreidx[2]);
    //odwrotnie ni� dla bifurkacji prze�ledzona droga oznaczana z 2 na 1
    licznik=0;
    NeighborhoodIteratorType nitbif ( radius, Image, Region );//obraz bifurkacji
    ImageType::Pointer Imagebif = ImageType::New();
    Imagebif->SetRegions(Region);
    Imagebif->SetSpacing(spacing);
    Imagebif->SetOrigin(inputOrigin);
    Imagebif->Allocate();
    Imagebif->FillBuffer(0);
    IteratorType      Ito(Image,Region);
    IteratorType      Itl(Imagebif,Region);
    for (Ito.GoToBegin(),Itl.GoToBegin();!Ito.IsAtEnd(),!Itl.IsAtEnd();++Ito,++Itl){Itl.Set(Ito.Get());}
    double parx;
    double pary;
    double parz;
    unsigned int parconnections;
    unsigned int pardiameter = 0;
    unsigned int pardirection;
    ImageType::IndexType idx;
    ImageType::IndexType idx_bif;
    int counter = -1;
    while (q.size() != 0)
    {
        idx[0]=q.front();q.pop();idx[1]=q.front();q.pop();idx[2]=q.front();q.pop(); //pobieranie indeksu z kolejki
        bool stop = false;
        while(stop != true)
        {
            counter+=1;
            nitbif.SetLocation(idx);
            if(nitbif.GetCenterPixel() == 3)
            {
               parx = idx[0];
               pary = idx[1];
               parz = idx[2];
               parconnections = 1;
               sb.nodeIndex.push_back(counter);
                nitbif.SetCenterPixel(1);
                bool nie_znaleziono = true;
                for(int i = 0; i<=neigh_num; i++)//albo pocz�tek �ledzenia (wtedy tylko pobiera si� nast�pny punkt �ledzenia)
                   {
                   if (i == center){}
                   else if (nitbif.GetPixel(i)>1){idx = nitbif.GetIndex(i);nie_znaleziono = false;}
                   }
                if(nie_znaleziono == true){stop = true;tr.branches.push_back(sb);sb.nodeIndex.clear();}//albo to drugie zako�czenie �ledzenia i nie ma dalszej drogi(wtedy ko�czymy �ledzenie)

            }
            else if(nitbif.GetCenterPixel() == 2 || nitbif.GetCenterPixel() == 5)//w pierwszej kolejno�ci poszukujemy punkt�w =2, dopiero jak takich nie ma szukamy wi�kszych
            {
                if (nitbif.GetCenterPixel() == 5)
                   {
                    int d = 1;
                    for(int i = 0; i<=neigh_num; i++)
                       {
                       if (i == center){d = 0;}
                       else if(nitbif.GetPixel(i) == 1.5)
                           {
                           for (unsigned int h = 0;h<tr.nodes.size();h++)
                               {
                               if (tr.nodes[h].connections == 3)
                                   {
                                   idx_bif = nitbif.GetIndex(i);
                                   if (tr.nodes[h].x == idx_bif[0] && tr.nodes[h].y == idx_bif[1] && tr.nodes[h].z == idx_bif[2])
                                       {
                                       sb.nodeIndex.push_back(h);
                                       }
                                   }
                               }
                           }
                       }
                   }
                parx = idx[0];
                pary = idx[1];
                parz = idx[2];
                parconnections = 2;
                sb.nodeIndex.push_back(counter);
                nitbif.SetCenterPixel(1);
                bool nie_znaleziono = true;
                int d = 1;
                for(int i = 0; i<=neigh_num; i++)
                   {
                   if (i == center){d = 0;}
                   else if(nitbif.GetPixel(i) == 4){idx = nitbif.GetIndex(i);nie_znaleziono = false;pardirection = i+d;}//**zapisywanie kierunku lokalnego
                   }
                d = 1;
                if(nie_znaleziono == true)
                   {
                    for(int i = 0; i<=neigh_num; i++)
                       {
                       if (i == center){d = 0;}
                       else
                           {
                           if(nitbif.GetPixel(i) == 2){idx = nitbif.GetIndex(i);nie_znaleziono = false;pardirection = i+d;}//**zapisywanie kierunku lokalnego
                           }
                       }
                   }
                d = 1;
                if(nie_znaleziono == true)
                   {
                    for(int i = 0; i<=neigh_num; i++)
                       {
                       if (i == center){d = 0;}
                       else if(nitbif.GetPixel(i) == 3){idx = nitbif.GetIndex(i);nie_znaleziono = false;pardirection = i+d;}//**zapisywanie kierunku lokalnego
                       }
                   }
            }
            else if(nitbif.GetCenterPixel() == 4)// na pocz�tku tak samo jak dla =2, nast�pnie szukamy punkt�w r��nych od ju� znalezionego i dodajemy je do kolejki
            {
                parx = idx[0];
                pary = idx[1];
                parz = idx[2];
                parconnections = 3;
                sb.nodeIndex.push_back(counter);
                tr.branches.push_back(sb);
                sb.nodeIndex.clear();
                sb.nodeIndex.push_back(counter);

                nitbif.SetCenterPixel(1.5);
                bool nie_znaleziono = true;
                for(int i = 0; i<=neigh_num; i++)
                   {
                   if (i == center){}
                   else if(nitbif.GetPixel(i) == 2){idx = nitbif.GetIndex(i);nie_znaleziono = false;}

                   }
                if(nie_znaleziono == true)
                   {
                    for(int i = 0; i<=neigh_num; i++)
                       {
                       if (i == center){}
                       else
                           {
                           if(nitbif.GetPixel(i) == 4){idx = nitbif.GetIndex(i);}
                           }
                       }
                   }

                //---
                for(int i = 0; i<=neigh_num; i++)
                   {
                   if (i == center){}
                   else
                       {
                       if((nitbif.GetPixel(i) == 2) && nitbif.GetIndex(i) != idx){nitbif.SetPixel(i,5);q.push(nitbif.GetIndex(i)[0]);q.push(nitbif.GetIndex(i)[1]);q.push(nitbif.GetIndex(i)[2]);}
                       if((nitbif.GetPixel(i) == 4) && nitbif.GetIndex(i) != idx){nitbif.SetPixel(i,4);q.push(nitbif.GetIndex(i)[0]);q.push(nitbif.GetIndex(i)[1]);q.push(nitbif.GetIndex(i)[2]);}
                       }
                   }
            }
            else {stop = true;}
            tr.addPoint(parx,pary,parz,parconnections,pardiameter);
        }
        if(q.size() == 0)
        {
            NeighborhoodIteratorType qnit ( radius, Image, Region );//obraz bifurkacji
            int ls,sremax;
            bool jest = false;
            for (qnit.GoToBegin();!qnit.IsAtEnd();++qnit)
            {
                sremax = 0;
                if(qnit.GetCenterPixel() == 2)
                {
                    //ls - liczba s�siad�w r�wnych 2
                    ls = 0;
                    for(int i = 0; i <= neigh_num; i++)
                       {
                       if (i == center){}
                       else if(qnit.GetPixel(i) == 2){ls++;}
                       }
                    if(ls == 1)
                    {
                        jest = true;
                    }
                }
            }
            if(jest == true){q.push(idx[0]);q.push(idx[1]);q.push(idx[2]);++licznik;}
        }
    }
    return tr;
}
*/

//----------------------------------------------------------------------------------------
Tree BuildTree::szacowanie_srednicy(ImageStructure par1, Tree par2)
{
    //par1 - obraz binarny po segmentacji
    //par2 - czêciowo uzupe³niona struktura zwrócona przez funkcjê szacowanie_polaczen
    //----------regiony
    ImageType::RegionType Region;
    ImageType::RegionType::IndexType Start;
    Start[0] = 0;
    Start[1] = 0;
    Start[2] = 0;
    Region.SetIndex( Start );
    ImageType::RegionType::SizeType size;
    size[0]  = StructureToItkImage(par1)->GetRequestedRegion().GetSize()[0];
    size[1]  = StructureToItkImage(par1)->GetRequestedRegion().GetSize()[1];
    size[2]  = StructureToItkImage(par1)->GetRequestedRegion().GetSize()[2];
    Region.SetSize( size );
    const ImageType::SpacingType& spacing = StructureToItkImage(par1)->GetSpacing();
    const ImageType::PointType& inputOrigin = StructureToItkImage(par1)->GetOrigin();
    ImageType::Pointer Image = ImageType::New();
    Image->SetRegions(Region);
    Image->SetSpacing(spacing);
    Image->SetOrigin(inputOrigin);
    Image->Allocate();
    Image->FillBuffer(0);
    IteratorType      It(StructureToItkImage(par1),Region);
    IteratorType      Itq(Image,Region);
    for (It.GoToBegin(),Itq.GoToBegin();!It.IsAtEnd(),!Itq.IsAtEnd();++It,++Itq){Itq.Set(It.Get());}

    for (unsigned int i = 0; i<par2.nodeCount(); i++)
    {
        ImageType::IndexType  ind;

        NodeIn3D node = par2.node(i);
        ind[0]=node.x;
        ind[1]=node.y;
        ind[2]=node.z;
        float procent;
        int tab[10];
        for (int i = 0;i < 10;i++){tab[i]=0;}
        bool end = false;
        for (int r = 1;r < 10;r++)
            {
            int ip;
            if(r==1){ip=19;}else if(r==2){ip=81;}else if(r==3){ip=179;}else if(r==4){ip=389;}else if(r==5){ip=739;}
            else if(r==6){ip=1189;}else if(r==7){ip=1791;}else if(r==8){ip=2553;}else if(r==9){ip=3695;}
            StructuringElementType binaryball;
            binaryball.SetRadius( r );
            binaryball.CreateStructuringElement();
            NeighborhoodIteratorType1::RadiusType radius = binaryball.GetRadius();
            NeighborhoodIteratorType1 it( radius, Image, Region );
            itk::NeighborhoodInnerProduct<ImageType> innerProduct;
            it.SetLocation(ind);
            procent = ((innerProduct( it, binaryball ))/ip)*100;
            tab [r] = procent;
            if (end == true){break;}
            if (innerProduct( it, binaryball ) < ip){end = true;}
            }
        int diameter;
        if(tab[1] < 30){diameter = 1;}
        if(tab[1] > 30){diameter = 2;}
        if(tab[2] > 30){diameter = 3;}
        if(tab[2] > 50){diameter = 4;}
        if(tab[3] > 40){diameter = 5;}
        if(tab[3] > 60){diameter = 6;}
        if(tab[4] > 70){diameter = 7;}
        if(tab[4] > 80){diameter = 8;}
        if(tab[5] > 70){diameter = 9;}
        if(tab[5] > 80){diameter = 10;}
        if(tab[6] > 70){diameter = 11;}
        if(tab[6] > 80){diameter = 12;}
        if(tab[7] > 70){diameter = 13;}
        if(tab[7] > 80){diameter = 14;}
        if(tab[8] > 70){diameter = 15;}
        if(tab[8] > 80){diameter = 16;}
        node.diameter = diameter;
        par2.setNode(node, i);

        }
    std::cout<<std::endl;
    return par2;
}
//----------------------------------------------------------------------------------------

ImageStructure BuildTree::rescaleIntensity( ImageStructure par1, float min, float max )
{
    RescaleFilterType::Pointer rescale = RescaleFilterType::New();
    rescale->SetInput( StructureToItkImage(par1) );
    rescale->SetOutputMinimum( min );
    rescale->SetOutputMaximum( max );
    rescale->Update();
    return itkImageToStructure(rescale->GetOutput());
}
