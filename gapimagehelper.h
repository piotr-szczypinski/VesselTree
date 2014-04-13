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
Klasę itkFunctions tworzą funkcje przetwarzania obrazów. Zwarzywszy na fakt, że każda z nich
pobiera obraz wejściowy jako argument funkcji, przeprowadza operację filtracji po czym zwraca obraz,
należałoby się zastanowić czy istnieje potrzeba tworzenia klasy. Jeśli dołączyłoby się te funkcje jako
"samodzielne metody" to umożliwiłoby to korzystanie z nich bez konieczności inicjalizowania obiektu klasy.
Ta decyzja pozostawiona jest Głównemu Twórcy oprogramowania.
----------------------------------------------------------------------------------------------------------
Dwie pierwsze funkcje (itkImageToStructure i StructureToItkImage) służą do konwersji między obrazami
w postaci struktury (obraz.cpp) i kontenerem obrazu używanym przez biblioteki Insight Toolkit. Jest to konieczne,
ponieważ wszystkie metody wykorzystują metody właśnie z tego środowiska (ITK).
*/
class GapImageHelpers
{
private:

public:
    //! Konwersja obrazu ITK do postaci struktury zdeklarowanej w obraz.cpp
    /*!
    \param par1 wskaźnik do obrazu ITK.
    \return ten sam obraz reprezentowany już jako struktura.
    */
    strukturaObrazu itkImageToStructure(ImageType::Pointer par1);
    //! Konwersja obrazu w postaci struktury zdeklarowanej w obraz.cpp do typu akceptowalnego przez ITK
    /*!
    \param par1 obraz w postaci struktury.
    \return ten sam obraz reprezentowany już jako ITK image.
    */
    ImageType::Pointer StructureToItkImage(strukturaObrazu par1);
    //! Otwarcie obrazu zapisanego w plikach .nii, .img, .hdr
    /*!
    \param par1 ścieżka do pliku
    \return obraz w postaci struktury.
    */
    strukturaObrazu openAnalyzeImage(std::string par1);
    //! Zmiana zakresu jasności reprezentujących obraz ("rozciąganie histogramu")
    /*!
    \param par1 obraz wejściowy w postaci struktury
    \param min wartość minimalna jasności w obrazie wyjściowym (domyślnie 0)
    \param max wartość maxymalna jasności w obrazie wyjściowym (domyślnie 255)
    \return obraz wyjściowy w postaci struktury.
    */
    strukturaObrazu rescaleIntensity( strukturaObrazu par1, float min = 0, float max = 255 );
    //! Filtr Gaussa o zadanym poziomie rozmycia
    /*!
    \param par1 obraz wejściowy w postaci struktury
    \param par2 wartość wariancji
    \return obraz wyjściowy w postaci struktury.
    */
    strukturaObrazu gaussianFilter(strukturaObrazu par1, float par2);
    //! Filtracja Macieżą Hessego i funkcja unnaczynniania Franghiego
    /*!
      Funkcja służy do wyeksponowania struktur cylindrycznych w obrazie. Wartość rozmycia
      (par2) odpowiada za rozmiar obiektów które zostaną wyeksponowane.
    \param par1 obraz wejściowy w postaci struktury
    \param par2 wartość sigma odpowiedzialna na rozmycie
    \return obraz wyjściowy w postaci struktury.
    */
    strukturaObrazu hessianFilter(strukturaObrazu par1, float par2);
    //! Złożenie dwóch obrazów w jeden za zasadzie rzutowania największych jasności
    /*!
      W przyadku filtracji wieloskalowych (filtracja hessego) konieczne jest sumowanie kolejnych
      efektów filtracji. Funkcja ta porównuje wartości wokseli obu obrazów o tych samych współrzędnych.
      Do obrazu wynikowego trafia wyższa wartość. Operacja powtarzana jest dla wszystkich punktów obrazu.
      Obrazy muszą być tych samych rozmiarów.
    \param par1 obraz wejściowy w postaci struktury
    \param par2 obraz wejściowy w postaci struktury
    \param par3 wartość sigma odpowiedzialna na rozmycie
    \return obraz wyjściowy w postaci struktury.
    */
    strukturaObrazu mipTwoImages(strukturaObrazu par1, strukturaObrazu par2, float par3);
    //! Rozrost obszaru od zarodka
    /*!
      Rozrost obszaru od danego punktu obrazu. Metoda iteracyjna polegająca na dołączaniu do obszaru sąsiednich punktów
      których wartości są w przedziale (par2, par3).
    \param par1 obraz wejściowy w postaci struktury
    \param par2 próg dolny
    \param par3 próg górny
    \param coord współrzędne punktu startu rozrostu obszaru
    \return obraz wyjściowy w postaci struktury.
    */
    strukturaObrazu RegionGrowing(strukturaObrazu par1, float par2, float par3, std::vector<unsigned int> coord);
    //! Utworzenie pustego obrazu
    /*!
      Utworzenie pustego obrazu, o tej samej rozdzielczości, spacingu i typie co obraz wejściowy.
    \param par1 obraz wejściowy w postaci struktury
    \return obraz wyjściowy w postaci struktury.
    */
    strukturaObrazu CreateEmptyStructure(strukturaObrazu par1);
    //! Wieloskalowa filtracja Hessego
    /*!
      Funkcja "hessianFilter" wykonywana wielokrotnie, a wyniki poszczególnych iteracji składane są przy
      pomocy funkcji "mipTwoImages".
    \param par1 obraz wejściowy w postaci struktury
    \param sigmaMin wartość minimalna rozmycia
    \param sigmaMax wartość maksymalna rozmycia
    \param noOfScales ilość iteracji filtracji
    \return obraz wyjściowy w postaci struktury.
    */
    strukturaObrazu MultiscaleHessianAlgorithm(strukturaObrazu par1, float sigmaMin, float sigmaMax, int noOfScales);
    //! Znajdowanie zarodka, punktu startu operacji rozrostu obszaru
    /*!
      Prosta funkcja znajdująca najjaśniejszy punkt wewnątrz obrazu
    \param par1 obraz wejściowy w postaci struktury
    \return współrzędne w postaci wektora.
    */
    std::vector<unsigned int> FindSeed(strukturaObrazu par1);
    //! Wartość maksymalna jasności obrazu
    /*!
    \param par1 obraz wejściowy w postaci struktury
    \return wartość maksymana
    */
    double FindMaximumValue(strukturaObrazu par1);
    //! Wartość minimalna jasności obrazu
    /*!
    \param par1 obraz wejściowy w postaci struktury
    \return wartość minimalna
    */
    double FindMinimumValue(strukturaObrazu par1);
    //! Zapis obrazu do pliku (.nii, .img, .hdr)
    /*!
    \param par1 obraz wejściowy w postaci struktury
    \param par2 ścieżka wraz z nazwą pliku
    */
    void saveImage(strukturaObrazu par1, std::string par2);
    //! Algorytm znajdowania naczyń krwionośnych w obrazach Time of Flight
    /*!
      Funkcja wykonuje wielokrotną filtrację hessego, oraz przeprowadza segmentację od automatycznie znalezionego
      punktu rozrostu obszaru.
    \param par1 obraz wejściowy w postaci struktury
    \param noOfScales ilość iteracji filtracji hessego
    \param thresholdPercent wartość progu wyrażona procentowo
    \return obraz wyjściowy w postaci struktury.
    */
    strukturaObrazu HVSalgorithm(strukturaObrazu par1, int noOfScales, float thresholdPercent);
};
