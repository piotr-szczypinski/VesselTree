#include "obraz.h"

#include "itkImage.h"
typedef itk::Image< float, 3 >                                                  ImageType;
#include "itkImageFileReader.h"
typedef itk::ImageFileReader< ImageType >                                       ReaderType;
#include "itkImageFileWriter.h"
typedef itk::ImageFileWriter< ImageType >                                       WriterType;
#include "itkDiscreteGaussianImageFilter.h"
typedef itk::DiscreteGaussianImageFilter< ImageType, ImageType>                 GaussianFilterType;
#include "itkHessianRecursiveGaussianImageFilter.h"
typedef itk::HessianRecursiveGaussianImageFilter< ImageType >                   HessianFilterType;
#include "itkHessian3DToVesselnessMeasureImageFilter.h"
typedef itk::Hessian3DToVesselnessMeasureImageFilter< float >                   VesselnessMeasureFilterType;
#include "itkImageRegionConstIterator.h"
typedef itk::ImageRegionConstIterator< ImageType >                              ConstIteratorType;
#include "itkImageRegionIterator.h"
typedef itk::ImageRegionIterator< ImageType>                                    IteratorType;
#include "itkImageRegionConstIteratorWithIndex.h"
typedef itk::ImageRegionIteratorWithIndex< ImageType>                           IteratorTypeWI;
#include "itkConnectedThresholdImageFilter.h"
typedef itk::ConnectedThresholdImageFilter< ImageType, ImageType >              ConnectedFilterType;
#include "itkMinimumMaximumImageCalculator.h"
typedef itk::MinimumMaximumImageCalculator <ImageType>                          ImageCalculatorFilterType;
#include "itkImageToVTKImageFilter.h"
typedef itk::ImageToVTKImageFilter< ImageType >                                 Connector;
#include "itkRescaleIntensityImageFilter.h"
typedef itk::RescaleIntensityImageFilter< ImageType,ImageType>                  RescaleFilterType;

//! K L A S A    I T K F U N C T I O N S
/*!
Klasê itkFunctions tworz¹ funkcje przetwarzania obrazów. Zwarzywszy na fakt, ¿e ka¿da z nich
pobiera obraz wejœciowy jako argument funkcji, przeprowadza operacjê filtracji po czym zwraca obraz,
nale¿a³oby siê zastanowiæ czy istnieje potrzeba tworzenia klasy. Jeœli do³¹czy³oby siê te funkcje jako
"samodzielne metody" to umo¿liwi³oby to korzystanie z nich bez koniecznoœci inicjalizowania obiektu klasy.
Ta decyzja pozostawiona jest G³ównemu Twórcy oprogramowania.
----------------------------------------------------------------------------------------------------------
Dwie pierwsze funkcje (itkImageToStructure i StructureToItkImage) s³u¿¹ do konwersji miêdzy obrazami
w postaci struktury (obraz.cpp) i kontenerem obrazu u¿ywanym przez biblioteki Insight Toolkit. Jest to konieczne,
poniewa¿ wszystkie metody wykorzystuj¹ metody w³aœnie z tego œrodowiska (ITK).
*/
class GapImageHelpers
{
private:

public:
    //! Konwersja obrazu ITK do postaci struktury zdeklarowanej w obraz.cpp
    /*!
    \param par1 wskaŸnik do obrazu ITK.
    \return ten sam obraz reprezentowany ju¿ jako struktura.
    */
    strukturaObrazu itkImageToStructure(ImageType::Pointer par1);
    //! Konwersja obrazu w postaci struktury zdeklarowanej w obraz.cpp do typu akceptowalnego przez ITK
    /*!
    \param par1 obraz w postaci struktury.
    \return ten sam obraz reprezentowany ju¿ jako ITK image.
    */
    ImageType::Pointer StructureToItkImage(strukturaObrazu par1);
    //! Otwarcie obrazu zapisanego w plikach .nii, .img, .hdr
    /*!
    \param par1 œcie¿ka do pliku
    \return obraz w postaci struktury.
    */
    strukturaObrazu openAnalyzeImage(std::string par1);
    //! Zmiana zakresu jasnoœci reprezentuj¹cych obraz ("rozci¹ganie histogramu")
    /*!
    \param par1 obraz wejœciowy w postaci struktury
    \param min wartoœæ minimalna jasnoœci w obrazie wyjœciowym (domyœlnie 0)
    \param max wartoœæ maxymalna jasnoœci w obrazie wyjœciowym (domyœlnie 255)
    \return obraz wyjœciowy w postaci struktury.
    */
    strukturaObrazu rescaleIntensity( strukturaObrazu par1, float min = 0, float max = 255 );
    //! Filtr Gaussa o zadanym poziomie rozmycia
    /*!
    \param par1 obraz wejœciowy w postaci struktury
    \param par2 wartoœæ wariancji
    \return obraz wyjœciowy w postaci struktury.
    */
    strukturaObrazu gaussianFilter(strukturaObrazu par1, float par2);
    //! Filtracja Macie¿¹ Hessego i funkcja unnaczynniania Franghiego
    /*!
      Funkcja s³u¿y do wyeksponowania struktur cylindrycznych w obrazie. Wartoœæ rozmycia
      (par2) odpowiada za rozmiar obiektów które zostan¹ wyeksponowane.
    \param par1 obraz wejœciowy w postaci struktury
    \param par2 wartoœæ sigma odpowiedzialna na rozmycie
    \return obraz wyjœciowy w postaci struktury.
    */
    strukturaObrazu hessianFilter(strukturaObrazu par1, float par2);
    //! Z³o¿enie dwóch obrazów w jeden za zasadzie rzutowania najwiêkszych jasnoœci
    /*!
      W przyadku filtracji wieloskalowych (filtracja hessego) konieczne jest sumowanie kolejnych
      efektów filtracji. Funkcja ta porównuje wartoœci wokseli obu obrazów o tych samych wspó³rzêdnych.
      Do obrazu wynikowego trafia wy¿sza wartoœæ. Operacja powtarzana jest dla wszystkich punktów obrazu.
      Obrazy musz¹ byæ tych samych rozmiarów.
    \param par1 obraz wejœciowy w postaci struktury
    \param par2 obraz wejœciowy w postaci struktury
    \param par3 wartoœæ sigma odpowiedzialna na rozmycie
    \return obraz wyjœciowy w postaci struktury.
    */
    strukturaObrazu mipTwoImages(strukturaObrazu par1, strukturaObrazu par2, float par3);
    //! Rozrost obszaru od zarodka
    /*!
      Rozrost obszaru od danego punktu obrazu. Metoda iteracyjna polegaj¹ca na do³¹czaniu do obszaru s¹siednich punktów
      których wartoœci s¹ w przedziale (par2, par3).
    \param par1 obraz wejœciowy w postaci struktury
    \param par2 próg dolny
    \param par3 próg górny
    \param coord wspó³rzêdne punktu startu rozrostu obszaru
    \return obraz wyjœciowy w postaci struktury.
    */
    strukturaObrazu RegionGrowing(strukturaObrazu par1, float par2, float par3, std::vector<unsigned int> coord);
    //! Utworzenie pustego obrazu
    /*!
      Utworzenie pustego obrazu, o tej samej rozdzielczoœci, spacingu i typie co obraz wejœciowy.
    \param par1 obraz wejœciowy w postaci struktury
    \return obraz wyjœciowy w postaci struktury.
    */
    strukturaObrazu CreateEmptyStructure(strukturaObrazu par1);
    //! Wieloskalowa filtracja Hessego
    /*!
      Funkcja "hessianFilter" wykonywana wielokrotnie, a wyniki poszczególnych iteracji sk³adane s¹ przy
      pomocy funkcji "mipTwoImages".
    \param par1 obraz wejœciowy w postaci struktury
    \param sigmaMin wartoœæ minimalna rozmycia
    \param sigmaMax wartoœæ maksymalna rozmycia
    \param noOfScales iloœæ iteracji filtracji
    \return obraz wyjœciowy w postaci struktury.
    */
    strukturaObrazu MultiscaleHessianAlgorithm(strukturaObrazu par1, float sigmaMin, float sigmaMax, int noOfScales);
    //! Znajdowanie zarodka, punktu startu operacji rozrostu obszaru
    /*!
      Prosta funkcja znajduj¹ca najjaœniejszy punkt wewn¹trz obrazu
    \param par1 obraz wejœciowy w postaci struktury
    \return wspó³rzêdne w postaci wektora.
    */
    std::vector<unsigned int> FindSeed(strukturaObrazu par1);
    //! Wartoœæ maksymalna jasnoœci obrazu
    /*!
    \param par1 obraz wejœciowy w postaci struktury
    \return wartoœæ maksymana
    */
    double FindMaximumValue(strukturaObrazu par1);
    //! Wartoœæ minimalna jasnoœci obrazu
    /*!
    \param par1 obraz wejœciowy w postaci struktury
    \return wartoœæ minimalna
    */
    double FindMinimumValue(strukturaObrazu par1);
    //! Zapis obrazu do pliku (.nii, .img, .hdr)
    /*!
    \param par1 obraz wejœciowy w postaci struktury
    \param par2 œcie¿ka wraz z nazw¹ pliku
    */
    void saveImage(strukturaObrazu par1, std::string par2);
    //! Algorytm znajdowania naczyñ krwionoœnych w obrazach Time of Flight
    /*!
      Funkcja wykonuje wielokrotn¹ filtracjê hessego, oraz przeprowadza segmentacjê od automatycznie znalezionego
      punktu rozrostu obszaru.
    \param par1 obraz wejœciowy w postaci struktury
    \param noOfScales iloœæ iteracji filtracji hessego
    \param thresholdPercent wartoœæ progu wyra¿ona procentowo
    \return obraz wyjœciowy w postaci struktury.
    */
    strukturaObrazu HVSalgorithm(strukturaObrazu par1, int noOfScales, float thresholdPercent);
};
