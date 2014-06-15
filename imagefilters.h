#ifndef IMAGEFILTERS_H
#define IMAGEFILTERS_H

#include "image.h"
#include "typedefsti.h"

/*!
ImageFilters jest opakowaniem dla funkcji przetwarzania obrazów. Funkcje pobierają obraz wejściowy jako argument,
przeprowadzają operację filtracji po czym zwracają obraz wynikowy. Wszystkie funkcje klasy zadeklarowane są jako
statyczne co pozwala korzystać z nich bez konieczności tworzenia obiektu tej klasy.
Dwie pierwsze funkcje (itkImageToStructure i StructureToItkImage) służą do konwersji między obrazami
w postaci struktury ImageStructure i kontenerem obrazu używanym przez biblioteki Insight Toolkit. Jest to konieczne,
ponieważ wszystkie metody korzystają z tego środowiska (ITK).
*/
class ImageFilters
{
public:
    //! Konwersja obrazu ITK do postaci struktury zdeklarowanej w obraz.cpp
    /*!
    \param par1 wskaźnik do obrazu ITK.
    \return ten sam obraz reprezentowany już jako struktura.
    */
    static ImageStructure itkImageToStructure(ImageType::Pointer par1);
    //! Konwersja obrazu w postaci struktury zdeklarowanej w obraz.cpp do typu akceptowalnego przez ITK
    /*!
    \param par1 obraz w postaci struktury.
    \return ten sam obraz reprezentowany już jako ITK image.
    */
    static ImageType::Pointer StructureToItkImage(ImageStructure par1);
    //! Otwarcie obrazu zapisanego w plikach .nii, .img, .hdr
    /*!
    \param par1 ścieżka do pliku
    \return obraz w postaci struktury.
    */
    static ImageStructure openAnalyzeImage(std::string par1);
    //! Zmiana zakresu jasności reprezentujących obraz ("rozciąganie histogramu")
    /*!
    \param par1 obraz wejściowy w postaci struktury
    \param min wartość minimalna jasności w obrazie wyjściowym (domyślnie 0)
    \param max wartość maxymalna jasności w obrazie wyjściowym (domyślnie 255)
    \return obraz wyjściowy w postaci struktury.
    */
    static ImageStructure rescaleIntensity( ImageStructure par1, float min = 0, float max = 255 );
    //! Filtr Gaussa o zadanym poziomie rozmycia
    /*!
    \param par1 obraz wejściowy w postaci struktury
    \param par2 wartość wariancji
    \return obraz wyjściowy w postaci struktury.
    */
    static ImageStructure gaussianFilter(ImageStructure par1, float par2);
    //! Filtracja Macieżą Hessego i funkcja unnaczynniania Franghiego
    /*!
      Funkcja służy do wyeksponowania struktur cylindrycznych w obrazie. Wartość rozmycia
      (par2) odpowiada za rozmiar obiektów które zostaną wyeksponowane.
    \param par1 obraz wejściowy w postaci struktury
    \param par2 wartość sigma odpowiedzialna na rozmycie
    \return obraz wyjściowy w postaci struktury.
    */
    static ImageStructure hessianFilter(ImageStructure par1, float par2);
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
    static ImageStructure mipTwoImages(ImageStructure par1, ImageStructure par2, float par3);
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
    static ImageStructure RegionGrowing(ImageStructure par1, float par2, float par3, std::vector<unsigned int> coord);
    //! Utworzenie pustego obrazu
    /*!
      Utworzenie pustego obrazu, o tej samej rozdzielczości, spacingu i typie co obraz wejściowy.
    \param par1 obraz wejściowy w postaci struktury
    \return obraz wyjściowy w postaci struktury.
    */
    static ImageStructure CreateEmptyStructure(ImageStructure par1);
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
    static ImageStructure MultiscaleHessianAlgorithm(ImageStructure par1, float sigmaMin, float sigmaMax, int noOfScales);
    //! Znajdowanie zarodka, punktu startu operacji rozrostu obszaru
    /*!
      Prosta funkcja znajdująca najjaśniejszy punkt wewnątrz obrazu
    \param par1 obraz wejściowy w postaci struktury
    \return współrzędne w postaci wektora.
    */
    static std::vector<unsigned int> FindSeed(ImageStructure par1);
    //! Wartość maksymalna jasności obrazu
    /*!
    \param par1 obraz wejściowy w postaci struktury
    \return wartość maksymana
    */
    static double FindMaximumValue(ImageStructure par1);
    //! Wartość minimalna jasności obrazu
    /*!
    \param par1 obraz wejściowy w postaci struktury
    \return wartość minimalna
    */
    static double FindMinimumValue(ImageStructure par1);


    static double minIntensity(ImageStructure par1, int* x, int* y, int* z);
    static double maxIntensity(ImageStructure par1, int* x, int* y, int* z);


    //! Zapis obrazu do pliku (.nii, .img, .hdr)
    /*!
    \param par1 obraz wejściowy w postaci struktury
    \param par2 ścieżka wraz z nazwą pliku
    */
    static void saveImage(ImageStructure par1, std::string par2);
    //! Algorytm znajdowania naczyń krwionośnych w obrazach Time of Flight
    /*!
      Funkcja wykonuje wielokrotną filtrację hessego, oraz przeprowadza segmentację od automatycznie znalezionego
      punktu rozrostu obszaru.
    \param par1 obraz wejściowy w postaci struktury
    \param noOfScales ilość iteracji filtracji hessego
    \param thresholdPercent wartość progu wyrażona procentowo
    \return obraz wyjściowy w postaci struktury.
    */
    static ImageStructure HVSalgorithm(ImageStructure par1, int noOfScales, float thresholdPercent);
};

#endif //IMAGEFILTERS_H
