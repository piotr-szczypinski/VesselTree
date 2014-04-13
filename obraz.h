/*
 * VesselTree - MRI image segmentation and characterization
 *
 * Copyright 2014  Piotr M. SzczypiÅ„ski <piotr.szczypinski@p.lodz.pl>
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

#include <iostream>
#include <vector>

//! Typ wyliczeniowy
/*! Typ danych reprezentuj¹cych obraz */
enum type
{
    BOOL1, UCHAR8, INT16, FLOAT32, DOUBLE64
};
//! S T R U K T U R A    O B R A Z U
/*!
Struktura zawieraj¹ca niezbêdne informacje o obrazie.
*/
struct strukturaObrazu
{
    std::vector<unsigned int> size; /*!< vektor - rozmiar obrazu w danym kierunku  */
    std::vector<double> spacing;    /*!< vektor - spacing w danym kierunku   */
    std::vector<double> origin;     /*!< vektor - origin w danym kierunku   */
    type fileType;                  /*!< typ obrazu wybrany z typu wyliczeniowego enum   */
    std::string shortNote;          /*!< opcjonalna informacja o obrazie   */
    void *imageData;                /*!< wskaŸnik do danych   */
};

//! K L A S A    O B R A Z
/*!
Klasa Obraz zawiera podstawowe funkcje dzia³aj¹ce na strukturze obrazu. Wszystkie z nich napisane zosta³y w taki sposób
aby u¿ytkownik nie musia³ ko¿ystaæ z zewnêtrznych bibliotek do przetwarzania obrazów.
*/
class Obraz : strukturaObrazu
{
public:
    //! Informacja o iloœci wymiarów obrazu
    /*!
    \return liczba wymiarów.
    */
    int dimensionality();
    //! Typ obrazu
    /*!
    \return typ obrazu w postaci pozycji z enum type
    */
    type getType();
    //! Informacje o obrazie
    /*!
    \return informacja tekstowa o obrazie
    */
    std::string getShortNote();
    //! Rozmiar obrazu
    /*!
    \return rozmiar obrazu we wszystkich dostêpnych kierunkach
    */
    std::vector<unsigned int> getSize();
    //! Spacing obrazu
    /*!
    \return spacing obrazu we wszystkich dostêpnych kierunkach
    */
    std::vector<double> getSpacing();
    //! Pocz¹tek obrazu
    /*!
    \return origin obrazu we wszystkich dostêpnych kierunkach
    */
    std::vector<double> getOrigin();
    //! Pobierz wartoœæ voksela
    /*!
    \param coord wspó³rzêdne woksela
    \return wartoœæ danego woksela
    */
    double getValue(std::vector<int> coord);
    //! Pobierz wartoœæ voksela (tylko obraz 3d)
    /*!
    \param x wspó³rzêdna 0
    \param y wspó³rzêdna 1
    \param z wspó³rzêdna 2
    \return wartoœæ danego woksela
    */
    double getValue(int x, int y, int z);
    //! Pobierz wskaŸnik do linni
    /*!
    \param coord wspó³rzêdne woksela
    \param direction kierunek linni
    \return wskaŸnik do pocz¹tku lini
    */
    void* getLinePointer(std::vector<int> coord, int direction);
    //! Pobierz wskaŸnik do slica
    /*!
    \param coord wspó³rzêdne woksela
    \param direction kierunek slica
    \return wskaŸnik do pocz¹tku slica
    */
    void* getSlice(std::vector<int> coord, int direction);
    //! Ustaw wartoœæ voksela
    /*!
    \param coord wspó³rzêdne woksela
    \param value rz¹dana wartoœæ
    */
    void setValue(std::vector<int> coord, double value);
    //! Ustaw wartoœæ voksela (tylko obraz 3d)
    /*!
    \param x wspó³rzêdna 0
    \param y wspó³rzêdna 1
    \param z wspó³rzêdna 2
    \param value rz¹dana wartoœæ
    */
    void setValue(int x, int y, int z, double value);
    //! Wype³nij strukturê danymi
    /*!
    \param image struktura obrazu
    */
    void fillStructure(strukturaObrazu image);
    //! Zwróæ strukturê obrqazu
    /*!
    \return struktura obrazu
    */
    strukturaObrazu returnStruct();
};
