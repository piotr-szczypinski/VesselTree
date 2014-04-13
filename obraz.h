/*
 * VesselTree - MRI image segmentation and characterization
 *
 * Copyright 2014  Piotr M. SzczypiĹ„ski <piotr.szczypinski@p.lodz.pl>
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
/*! Typ danych reprezentujących obraz */
enum type
{
    BOOL1, UCHAR8, INT16, FLOAT32, DOUBLE64
};
//! S T R U K T U R A    O B R A Z U
/*!
Struktura zawierająca niezbędne informacje o obrazie.
*/
struct strukturaObrazu
{
    std::vector<unsigned int> size; /*!< vektor - rozmiar obrazu w danym kierunku  */
    std::vector<double> spacing;    /*!< vektor - spacing w danym kierunku   */
    std::vector<double> origin;     /*!< vektor - origin w danym kierunku   */
    type fileType;                  /*!< typ obrazu wybrany z typu wyliczeniowego enum   */
    std::string shortNote;          /*!< opcjonalna informacja o obrazie   */
    void *imageData;                /*!< wskaźnik do danych   */
};

//! K L A S A    O B R A Z
/*!
Klasa Obraz zawiera podstawowe funkcje działające na strukturze obrazu. Wszystkie z nich napisane zostały w taki sposób
aby użytkownik nie musiał kożystać z zewnętrznych bibliotek do przetwarzania obrazów.
*/
class Obraz : strukturaObrazu
{
public:
    //! Informacja o ilości wymiarów obrazu
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
    \return rozmiar obrazu we wszystkich dostępnych kierunkach
    */
    std::vector<unsigned int> getSize();
    //! Spacing obrazu
    /*!
    \return spacing obrazu we wszystkich dostępnych kierunkach
    */
    std::vector<double> getSpacing();
    //! Początek obrazu
    /*!
    \return origin obrazu we wszystkich dostępnych kierunkach
    */
    std::vector<double> getOrigin();
    //! Pobierz wartość voksela
    /*!
    \param coord współrzędne woksela
    \return wartość danego woksela
    */
    double getValue(std::vector<int> coord);
    //! Pobierz wartość voksela (tylko obraz 3d)
    /*!
    \param x współrzędna 0
    \param y współrzędna 1
    \param z współrzędna 2
    \return wartość danego woksela
    */
    double getValue(int x, int y, int z);
    //! Pobierz wskaźnik do linni
    /*!
    \param coord współrzędne woksela
    \param direction kierunek linni
    \return wskaźnik do początku lini
    */
    void* getLinePointer(std::vector<int> coord, int direction);
    //! Pobierz wskaźnik do slica
    /*!
    \param coord współrzędne woksela
    \param direction kierunek slica
    \return wskaźnik do początku slica
    */
    void* getSlice(std::vector<int> coord, int direction);
    //! Ustaw wartość voksela
    /*!
    \param coord współrzędne woksela
    \param value rządana wartość
    */
    void setValue(std::vector<int> coord, double value);
    //! Ustaw wartość voksela (tylko obraz 3d)
    /*!
    \param x współrzędna 0
    \param y współrzędna 1
    \param z współrzędna 2
    \param value rządana wartość
    */
    void setValue(int x, int y, int z, double value);
    //! Wypełnij strukturę danymi
    /*!
    \param image struktura obrazu
    */
    void fillStructure(strukturaObrazu image);
    //! Zwróć strukturę obrqazu
    /*!
    \return struktura obrazu
    */
    strukturaObrazu returnStruct();
};
