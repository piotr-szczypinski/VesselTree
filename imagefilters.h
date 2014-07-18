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

#ifndef IMAGEFILTERS_H
#define IMAGEFILTERS_H

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageDuplicator.h>
#include <itkDiscreteGaussianImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkConnectedThresholdImageFilter.h>
#include <itkHessianRecursiveGaussianImageFilter.h>
#include <itkHessian3DToVesselnessMeasureImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkBinaryThinningImageFilter3D.h>
#include <itkWhiteTopHatImageFilter.h>
#include <itkBlackTopHatImageFilter.h>
#include <itkGrayscaleDilateImageFilter.h>
#include <itkGrayscaleErodeImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryFillholeImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>


//TODO: Zrobic/przeniesc dokumentacje zgodna z wymaganiami doxygen


template <class T> class ImageFilters
{
public:
    typedef itk::Image< T, 3 > ImageType;
    //! Wczytanie obrazu zapisanego w pojedynczym pliku do zadanego obszaru pamięci
    /*!
      Funkcja wczytująca obraz z pojedynczego pliku o nazwie podanej jako parametr. Obraz zostaje zapisany w pamięci wskazanej przez drugi parametr.
    \param file_name nazwa lub pełna ścieżka pliku do wczytania
    \param *image wskaźnik do adresu pamięci pod którym zostanie przechowany obraz
    */
    static void open(std::string file_name, typename ImageType::Pointer* image)
    {
        typename itk::ImageFileReader< ImageType >::Pointer reader = itk::ImageFileReader< ImageType >::New();
        reader->SetFileName(file_name);
        reader->Update();
        *image = reader->GetOutput();

//        (*image)->SetReleaseDataFlag(false);
//        reader->Delete();
    }
    //! Wczytanie obrazu zapisanego w pojedynczym pliku
    /*!
      Funkcja wczytująca obraz z pojedynczego pliku o nazwie podanej jako parametr i zwraca wskaźnik do pamięci.
    \param file_name nazwa lub pełna ścieżka pliku do wczytania
    \return wskaźnik do obrazu
    */
    static typename ImageType::Pointer open(std::string file_name)
    {
        typename itk::ImageFileReader< ImageType >::Pointer reader = itk::ImageFileReader< ImageType >::New();
        reader->SetFileName(file_name);
        reader->Update();

//        typename ImageType::Pointer image;
//        image = reader->GetOutput();
//        image->SetReleaseDataFlag(false);
//        reader->Delete();
//        return image;

        return reader->GetOutput();
    }
    //! Zapis obrazu do pojedynczego pliku
    /*!
      Funkcja umożliwiająca zapis danych obrazowych do pliku
    \param image wskaźnik do obrazu który ma zostać zapisany
    \param file_name nazwa lub pełna ścieżka pliku do zapisu
    */
    static void save(typename ImageType::Pointer image, std::string file_name)
    {
        typename itk::ImageFileWriter< ImageType >::Pointer writer = itk::ImageFileWriter< ImageType >::New();
        writer->SetFileName(file_name);
        writer->SetInput(image);
        writer->Update();
    }
    //! Utworzenie kopi pliku na dysku
    /*!
      Funkcja umożliwia utworzenie kopi podanego pliku i zapisanie go pod inną nazwą lub w innej lokalizacji
    \param in_file_name nazwa lub pełna ścieżka pliku do odczytu
    \param out_file_name nazwa lub pełna ścieżka pliku do zapisu
    */
    static void copy(std::string in_file_name, std::string out_file_name)
    {
        typename itk::ImageFileReader< ImageType >::Pointer reader = itk::ImageFileReader< ImageType >::New();
        reader->SetFileName(in_file_name);
        reader->Update();
        typename itk::ImageFileWriter< ImageType >::Pointer writer = itk::ImageFileWriter< ImageType >::New();
        writer->SetFileName(out_file_name);
        writer->SetInput(reader->GetOutput());
        writer->Update();
    }
    //! Zmiana zakresu wartości jasności użytych w obrazie ("rozciąganie histogramu")
    /*!
      Zastosowanie liniowej transformacji do poziomów jasności w obrazie. Zakres zmian definiowany jest przez użytkownika poprzez podanie
      wartości minimalnej i maksymalnej jasności w obrazie wyjściowym
    \param input_image wskaźnik do przetwarzanego obrazu
    \param min wartość minimalna jasności w obrazie wyjściowym (domyślnie 0)
    \param max wartość maksymalna jasności w obrazie wyjściowym (domyślnie 255)
    \return wskaźnik do obrazu
    */
    static typename ImageType::Pointer rescaleIntensity(typename ImageType::Pointer input_image, float min = 0, float max = 255)
    {
        typedef typename itk::RescaleIntensityImageFilter< ImageType, ImageType > F;
        typedef typename F::Pointer P;
        P filter = F::New();
        filter->SetInput(input_image);
        filter->SetOutputMinimum(min);
        filter->SetOutputMaximum(max);
        filter->Update();
        return filter->GetOutput();
    }
    //! Filtracja Gaussowska o zadanym poziomie rozmycia
    /*!
      Filtracja uśredniająca uzyskana poprzez splot obrazu i funkcji Gaussa
    \param input_image wskaźnik do przetwarzanego obrazu
    \param sigma wartość wariancji
    \return wskaźnik do obrazu
    */
    static typename ImageType::Pointer gaussianFilter(typename ImageType::Pointer input_image, float sigma)
    {
        typedef typename itk::DiscreteGaussianImageFilter< ImageType, ImageType > F;
        typedef typename F::Pointer P;
        P filter = F::New();
        filter->SetInput(input_image);
        filter->SetVariance(sigma*sigma);
        filter->Update();
        return filter->GetOutput();
    }
    //! Filtracja Macierzą Hessego i funkcja unnaczynniania Franghiego
    /*!
      Funkcja służy do wyeksponowania struktur cylindrycznych w obrazie. Wartość rozmycia
      (sigma) odpowiada za rozmiar obiektów które zostaną wykryte.
    \param input_image wskaźnik do przetwarzanego obrazu
    \param sigma wartość wariancji odpowiedzialna na rozmycie obrazu
    \return wskaźnik do obrazu
    */
    static typename ImageType::Pointer hessianFilter(typename ImageType::Pointer input_image, float sigma)
    {
        typedef typename itk::HessianRecursiveGaussianImageFilter< ImageType > F;
        typedef typename F::Pointer P;
        typedef typename itk::Hessian3DToVesselnessMeasureImageFilter< T > F2;
        typedef typename F2::Pointer P2;
        P filter = F::New();
        filter->SetInput(input_image);
        filter->SetSigma(sigma);
        //filter->Update();
        P2 filter2 = F2::New();
        filter2->SetInput(filter->GetOutput());
        filter2->Update();
        return filter2->GetOutput();
    }

    //! Rzutowanie dwóch trójwymiarowych obrazów metodą MIP (Maximum intensity projection)
        /*!
          W przyadku filtracji wieloskalowych (filtracja hessego) konieczne jest sumowanie kolejnych
          efektów filtracji. Funkcja ta porównuje wartości jasności wokseli obu obrazach wejściowych.
          Do obrazu wynikowego trafia więkasza wartość z każdej pary punktów. Obrazy wejściowe muszą być tych samych rozmiarów.
          Przy pomocy parametru multiplier możliwa jest zmiana wag obu obrazów (wartości obrazu nr 2 zostają mnożone przez wartość współczynnika).
          Jeśli multiplier = 1 to wagi jasności obu obrazów są jednakowe.
          Jeśli multiplier > 1 to wartości jasności obrazu nr 2 mają większą wagę
          Jeśli multiplier < 1 to wartości jasności obrazu nr 2 mają mniejszą wagę
        \param mip_image wskaĹşnik do przetwarzanego obrazu nr 1
        \param input_image wskaĹşnik do przetwarzanego obrazu nr 2
        \param multiplier współczynnik wagowy
        \return wskaĹşnik do obrazu
        */
    static typename ImageType::Pointer mipTwoImages(typename ImageType::Pointer mip_image, typename ImageType::Pointer input_image, float multiplier)
    {
        typedef typename itk::ImageRegionConstIterator< ImageType > ConstIteratorType;
        typedef typename itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;

        ConstIteratorType inputIterator(input_image, input_image->GetRequestedRegion());
        IteratorType mipIterator(mip_image, mip_image->GetRequestedRegion());
        for (inputIterator.GoToBegin(), mipIterator.GoToBegin(); !inputIterator.IsAtEnd(); ++inputIterator, ++mipIterator)
            if (multiplier*(inputIterator.Get()) > mipIterator.Get())
                mipIterator.Set(inputIterator.Get());
        return mip_image;
    }

    //! Rozrost obszaru od zarodka
        /*!
          Rozrost obszaru od danego punktu obrazu. Metoda iteracyjna polegająca na dołączaniu do obszaru sąsiednich punktów
          których wartości jasności są w przedziale (min, max).
        \param input_image wskaźnik do przetwarzanego obrazu
        \param min próg dolny
        \param max próg górny
        \param coords współrzędne punktu startu rozrostu obszaru
        \return wskażnik do obrazu
        */
    static typename ImageType::Pointer regionGrowing(typename ImageType::Pointer input_image, float min, float max, typename ImageType::IndexType coords)
    {
        typedef itk::ConnectedThresholdImageFilter< ImageType, ImageType > ConnectedFilterType;
        typename ConnectedFilterType::Pointer ct = ConnectedFilterType::New();
        ct->SetInput(input_image);
        ct->SetSeed(coords);
        ct->SetLower(min);
        ct->SetUpper(max);
        ct->SetReplaceValue(1);
        ct->Update();
        return ct->GetOutput();
    }

    //! Progowanie obrazu z progiem globalnym
        /*!
          Zamiana obrazu wejściowego na obraz binary. Wokselom których wartość jasności znajduje się
          w przedziale (min, max) przypisana zostaje wartość 255. Punkty które nie spełniają kryterium
          otrzymują wartości zerowe.
        \param input_image wskaźnik do przetwarzanego obrazu
        \param min próg dolny
        \param max próg górny
        \return wskaźnik do obrazu
        */
    static typename ImageType::Pointer thresholdFilter(typename ImageType::Pointer input_image, float min, float max)
    {
        typedef typename itk::BinaryThresholdImageFilter< ImageType, ImageType > F;
        typedef typename F::Pointer P;
        P filter = F::New();
        filter->SetInput(input_image);
        filter->SetLowerThreshold(min);
        filter->SetUpperThreshold(max);
        filter->SetInsideValue(255);
        filter->SetOutsideValue(0);
        filter->Update();
        return filter->GetOutput();
    }
    //! Wieloskalowa filtracja Hessego (ciąg arytmetyczny)
     /*!
       Funkcja "hessianFilter" wykonywana wielokrotnie, a wyniki poszczególnych iteracji są sumowane przy
       pomocy funkcji "mipTwoImages". Użytkownik podaje minimalną (sigmaMin) i maksymalną (sigmaMax) wartość rozmycia
       oraz ilość filtracji macierzą Hessego które chce wykonać (noOfScales). Brakujące wartości skal zostają dobrane zgodnie z zasadami
       ciągu arytmetycznego.
     \param input_image wskaźnik do przetwarzanego obrazu
     \param sigmaMin wartość minimalna rozmycia
     \param sigmaMax wartość maksymalna rozmycia
     \param noOfScales ilość iteracji filtracji
     \return wskaźnik do obrazu
     */
    static typename ImageType::Pointer multiscaleHessianAlgorithmA(typename ImageType::Pointer input_image, float sigmaMin, float sigmaMax, int noOfScales)
    {
        typename ImageType::Pointer clone = input_image->Clone();
        for(int i = 0; i<noOfScales; i++)
        {
            float sigma;
            if(noOfScales > 1)  sigma=sigmaMin+(((sigmaMax-sigmaMin)/(sigmaMax-1))*i);
            else                sigma=sigmaMin;
            clone = mipTwoImages( hessianFilter(input_image, sigma), clone, 1);
        }
        return clone;
    }
    //! Wieloskalowa filtracja Hessego (ciąg geometryczny)
     /*!
       Funkcja "hessianFilter" wykonywana wielokrotnie, a wyniki poszczególnych iteracji są sumowane przy
       pomocy funkcji "mipTwoImages". Użytkownik podaje minimalną (sigmaMin) i maksymalną (sigmaMax) wartość rozmycia
       oraz ilość filtracji macierzą Hessego które chce wykonać (noOfScales). Brakujące wartości skal zostają dobrane zgodnie z zasadami
       ciągu geometrycznego.
     \param input_image wskaźnik do przetwarzanego obrazu
     \param sigmaMin wartość minimalna rozmycia
     \param sigmaMax wartość maksymalna rozmycia
     \param noOfScales ilość iteracji filtracji
     \return wskaźnik do obrazu
     */
    static typename ImageType::Pointer multiscaleHessianAlgorithmG(typename ImageType::Pointer input_image, float sigmaMin, float sigmaMax, int noOfScales)
    {
        typename ImageType::Pointer clone = input_image->Clone();

        double q;
        q = pow(sigmaMax/sigmaMin,(1/double(noOfScales-1)));

        for(int i = 0; i<noOfScales; i++)
        {
            float sigma;
            sigma=sigmaMin*pow(q, i);
            clone = mipTwoImages( hessianFilter(input_image, sigma), clone, 1);
        }
        return clone;
    }
    //! Wartość minimalna jasności obrazu
        /*!
          Funkcja zwraca minimalną wartość jasności w obrazie
        \param input_image wskaźnik do przetwarzanego obrazu
        \param *coords wskaźnik do współrzędnych znalezionej wartości minimalnej
        \return wartość minimalna
        */
    static double minIntensity(typename ImageType::Pointer input_image, typename ImageType::IndexType * coords = NULL)
    {
        typedef typename itk::MinimumMaximumImageCalculator < ImageType > F;
        typedef typename F::Pointer P;
        P filter = F::New ();
        filter->SetImage(input_image);
        filter->Compute();
        if(coords != NULL) *coords = filter->GetIndexOfMinimum();
        return filter->GetMinimum();
    }
    //! Wartość maksymalna jasności obrazu
        /*!
          Funkcja zwraca maksymalną wartość jasności w obrazie
        \param input_image wskaźnik do przetwarzanego obrazu
        \param *coords wskaźnik do współrzędnych znalezionej wartości maksymalnej
        \return wartość maksymalna
        */
    static double maxIntensity(typename ImageType::Pointer input_image, typename ImageType::IndexType * coords = NULL)
    {
        typedef typename itk::MinimumMaximumImageCalculator < ImageType > F;
        typedef typename F::Pointer P;
        P filter = F::New ();
        filter->SetImage(input_image);
        filter->Compute();
        if(coords != NULL) *coords = filter->GetIndexOfMaximum();
        return filter->GetMaximum();
    }
    //! Algorytm znajdowania naczyń krwionośnych w obrazach Time of Flight
    /*!
      Funkcja wykonuje wielokrotną filtrację hessego, oraz przeprowadza segmentację od automatycznie znalezionego
      punktu rozrostu obszaru.
    \param input_image wskaźnik do przetwarzanego obrazu
    \param noOfScales ilość iteracji filtracji hessego
    \param thresholdPercent wartość progu wyrażona procentowo (gdzie 1.0 to wrtość maksymalna obrazu)
    \return wskaźnik do obrazu
    */
    static typename ImageType::Pointer hvsAlgorithm(typename ImageType::Pointer input_image, int noOfScales, float thresholdPercent)
    {
        typename ImageType::Pointer clone;
        clone = multiscaleHessianAlgorithmG(input_image, 0.1, 2.0, noOfScales);
        typename ImageType::IndexType coords;
        double maximum = FindMaximumValue(clone, &coords);
        clone = regionGrowing(clone, maximum*thresholdPercent, maximum, coords);
        return clone;
    }

    static typename ImageType::Pointer dilate(typename ImageType::Pointer input_image, float radius)
    {
        typedef typename itk::BinaryBallStructuringElement< T, 3 > SE;
        typedef typename itk::GrayscaleDilateImageFilter < ImageType, ImageType, SE > F;
        typedef typename F::Pointer P;
        SE se;
        se.SetRadius(radius);
        se.CreateStructuringElement();
        P filter = F::New();
        filter->SetInput(input_image);
        filter->SetKernel(se);
        filter->Update();
        return filter->GetOutput();
    }

    static typename ImageType::Pointer erode(typename ImageType::Pointer input_image, float radius)
    {
        typedef typename itk::BinaryBallStructuringElement< T, 3 > SE;
        typedef typename itk::GrayscaleErodeImageFilter < ImageType, ImageType, SE > F;
        typedef typename F::Pointer P;
        SE se;
        se.SetRadius(radius);
        se.CreateStructuringElement();
        P filter = F::New();
        filter->SetInput(input_image);
        filter->SetKernel(se);
        filter->Update();
        return filter->GetOutput();
    }

    static typename ImageType::Pointer blackTopHat(typename ImageType::Pointer input_image, float radius)
    {
        typedef typename itk::BinaryBallStructuringElement< T, 3 > SE;
        typedef typename itk::BlackTopHatImageFilter < ImageType, ImageType, SE > F;
        typedef typename F::Pointer P;
        SE se;
        se.SetRadius(radius);
        se.CreateStructuringElement();
        P filter = F::New();
        filter->SetInput(input_image);
        filter->SetKernel(se);
        filter->Update();
        return filter->GetOutput();
    }

    static typename ImageType::Pointer whiteTopHat(typename ImageType::Pointer input_image, float radius)
    {
        typedef typename itk::BinaryBallStructuringElement< T, 3 > SE;
        typedef typename itk::WhiteTopHatImageFilter < ImageType, ImageType, SE > F;
        typedef typename F::Pointer P;
        SE se;
        se.SetRadius(radius);
        se.CreateStructuringElement();
        P filter = F::New();
        filter->SetInput(input_image);
        filter->SetKernel(se);
        filter->Update();
        return filter->GetOutput();
    }

    static typename ImageType::Pointer holeFill(typename ImageType::Pointer input_image, float foreground)
    {
        typedef typename itk::BinaryFillholeImageFilter < ImageType > F;
        typedef typename F::Pointer P;
        P filter = F::New();
        filter->SetInput(input_image);
        filter->SetForegroundValue(foreground);
        filter->Update();
        return filter->GetOutput();
    }

    static typename ImageType::Pointer skeletonFromBinary(typename ImageType::Pointer input_image)
    {
        typedef typename itk::BinaryThinningImageFilter3D < ImageType, ImageType > F;
        typedef typename F::Pointer P;
        P filter = F::New();
        filter->SetInput(input_image);
        filter->Update();
        return filter->GetOutput();
    }

    static typename ImageType::Pointer upscaleForCenteredSkeleton(typename ImageType::Pointer inputitk)
    {
        typename ImageType::SizeType size;
        typename ImageType::SpacingType spacing;
        for(int i = 0; i<inputitk->GetLargestPossibleRegion().GetImageDimension(); i++)
        {
            size[i] = inputitk->GetLargestPossibleRegion().GetSize()[i] * 2;
            spacing[i] = inputitk->GetSpacing()[i] / 2.0;
        }

        itk::Index<3> start; start.Fill(0);
        typename ImageType::Pointer outputitk = ImageType::New();
        typename ImageType::RegionType region(start, size);
        outputitk->SetRegions(region);
        outputitk->SetSpacing(spacing);
        outputitk->Allocate();
        outputitk->FillBuffer(0);

        int maxx, maxy, maxz;

        typename ImageType::IndexType pixelIndex;
        typename ImageType::IndexType opixelIndex;
        maxx = inputitk->GetLargestPossibleRegion().GetSize()[0];
        maxy = inputitk->GetLargestPossibleRegion().GetSize()[1];
        maxz = inputitk->GetLargestPossibleRegion().GetSize()[2];

        for(pixelIndex[2] = 0; pixelIndex[2] < maxz; pixelIndex[2]++)
        {
            opixelIndex[2] = pixelIndex[2]*2;
            for(pixelIndex[1] = 0; pixelIndex[1] < maxz; pixelIndex[1]++)
            {
                opixelIndex[1] = pixelIndex[1]*2;
                for(pixelIndex[0] = 0; pixelIndex[0] < maxz; pixelIndex[0]++)
                {
                    opixelIndex[0] = pixelIndex[0]*2;

                    char pixel = inputitk->GetPixel(pixelIndex);
                    outputitk->SetPixel(opixelIndex, pixel);
                }
            }
        }

        maxz *= 2;
        maxy *= 2;
        maxx *= 2;

        for(pixelIndex[2] = 0; pixelIndex[2] < maxz; pixelIndex[2]+=2)
        {
            opixelIndex[2] = pixelIndex[2];
            for(pixelIndex[1] = 0; pixelIndex[1] < maxz; pixelIndex[1]+=2)
            {
                opixelIndex[1] = pixelIndex[1];
                for(pixelIndex[0] = 1; pixelIndex[0] < maxz; pixelIndex[0]+=2)
                {
                    opixelIndex[0] = pixelIndex[0]-1;
                    char pixel = outputitk->GetPixel(opixelIndex);
                    if(pixel)
                    {
                        opixelIndex[0] -= 2;
                        pixel = outputitk->GetPixel(opixelIndex);
                        if(pixel)
                        {
                            outputitk->SetPixel(pixelIndex, pixel);
                        }
                    }
                }
            }
        }

        for(pixelIndex[2] = 0; pixelIndex[2] < maxz; pixelIndex[2]+=2)
        {
            opixelIndex[2] = pixelIndex[2];
            for(pixelIndex[0] = 0; pixelIndex[0] < maxx; pixelIndex[0]++)
            {
                opixelIndex[0] = pixelIndex[0];
                for(pixelIndex[1] = 1; pixelIndex[1] < maxy; pixelIndex[1]+=2)
                {
                    opixelIndex[1] = pixelIndex[1]-1;
                    char pixel = outputitk->GetPixel(opixelIndex);
                    if(pixel)
                    {
                        opixelIndex[1] -= 2;
                        pixel = outputitk->GetPixel(opixelIndex);
                        if(pixel)
                        {
                            outputitk->SetPixel(pixelIndex, pixel);
                        }
                    }
                }
            }
        }

        for(pixelIndex[1] = 0; pixelIndex[1] < maxy; pixelIndex[1]++)
        {
            opixelIndex[1] = pixelIndex[1];
            for(pixelIndex[0] = 0; pixelIndex[0] < maxx; pixelIndex[0]++)
            {
                opixelIndex[0] = pixelIndex[0];
                for(pixelIndex[2] = 1; pixelIndex[2] < maxz; pixelIndex[2]+=2)
                {
                    opixelIndex[2] = pixelIndex[2]-1;
                    char pixel = outputitk->GetPixel(opixelIndex);
                    if(pixel)
                    {
                        opixelIndex[2] -= 2;
                        pixel = outputitk->GetPixel(opixelIndex);
                        if(pixel)
                        {
                            outputitk->SetPixel(pixelIndex, pixel);
                        }
                    }
                }
            }
        }
        return outputitk;
    }
};

#endif //IMAGEFILTERS_H
