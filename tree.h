/*
 * VesselTree - MRI image segmentation and characterization
 * 
 * Copyright 2014  Piotr M. Szczypiński <piotr.szczypinski@p.lodz.pl>
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

#ifndef TREE_H
#define TREE_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <math.h>

/* \struct Coordinates3d
 *  \brief Coordinates3d stores coordinates x, y and z.
 *
 * This is simply a vector of coordinates.
 */
/** \struct Coordinates3d
 *  \brief struktura Coordinates3d przechowuje współrzędne (x, y, z) przestrzeni trójwymiarowej.
 */
struct Coordinates3d
{
    double x;
    double y;
    double z;
};

/* \struct NodeIn3D
 *  \brief NodeIn3D stores node coordinates, number
 *  of neighbors and local diameter of a vessel.
 *
 * This structure derives from the Coordinates3d.
 * It stores coordinates of the node, number of nodes connected to
 * it and diameter of a vessel arround the node.
 */
/** \struct NodeIn3D
 *  \brief struktura NodeIn3D przechowuje współrzędne węzła, liczbę węzłów połączonych i średnicę naczynia.
 *
 * Struktura dziedziczy z Coordinates3d i dodatkowo deklaruje zmienne
 * \param connections do przechowywania liczby połączeń z węzłami sąsiednimi
 * oraz \param diameter określający średnicę naczynia krwionośnego
 * w miejscu położenia węzła.
 */
struct NodeIn3D:Coordinates3d
{
    /** Liczba sąziednich węzłów połączonych*/
    unsigned int connections;
    /** Średnica naczynia krwionośnego w położeniu węzła*/
    unsigned int diameter;
};

/* \struct BasicBranch
 *  \brief BasicBranch stores indexes of nodes building the branch.
 */
/** \struct BasicBranch
 *  \brief struktura BasicBranch zawiera wektor indeksów węzłów tworzących
 *  gałąź drzewa.
 *
 * Struktura zawiera wektor indeksów węzłów tworzących gałąź drzewa.
 * Współrzędne i inne informacje o węzłach muszą być przechowywane
 * w innym wektorze danych. Przykładem zastosowania jest struktura
 * TreeSkeletonStructure.
 */
struct BasicBranch
{
    std::vector<unsigned int> nodeIndex;
};

/* \struct TreeStructure
 *  \brief TreeStructure stores information on a vessel tree.
 *
 * TreeStructure stores indexes of nodes building individual
 * branches in a vector of BasicBranch structures.
 * The corresponding nodes coordinate, connectivity and diameter are stored
 * in a vector of NodeIn3D structures.
 * User should not operate directly on the TreeStructure.
 * Instead it is preferred to use functions of Tree class
 * to add or remove branches.
 */
/** \struct TreeSkeletonStructure
 *  \brief struktura TreeSkeletonStructure przechowuje informację o budowie drzewa.
 *
 * TreeSkeletonStructure przechowuje informację o budowie drzewa.
 * wszystkie węzły tworzące drzewo przechowywane są w wektorze nodes
 * natomiast to, które z tych węzłów tworzą poszczególne gałęzie
 * zapisywane jest w wektorze branches.
 * Użytkownik nie powinien bezpośrednio korzystać z pól tej struktury.
 * Zamiast tego należy korzystać z klasy TreeSkeleton implementującej
 * odpowiednie funkcje.
 */
struct TreeStructure
{
    /** Lista węzłów*/
    std::vector <NodeIn3D> nodes;
    /** Lista gałęzi*/
    std::vector <BasicBranch> branches;
};

//-------------------------------------------------------------------------------------
/* \classt Tree
 *  \brief Tree derives from TreeStructure.
 *
 * Tree defines functions to safely add and remove branches
 * in Tree structure, query the number of nodes and branches,
 * to save and load data, etc.
 */
/** \classt TreeSkeleton
 *  \brief TreeSkeleton dziedziczy z TreeSkeletonStructure i implementuje
 *  funkcje dostępu.
 *
 * TreeSkeleton definiuje funkcje do bezpiecznego korzystania ze struktury
 * TreeSkeletonStructure. Między innymi są to funkcje dodawania i usuwania
 * gałęzi, zapisu do pliku i odczytu z pliku.
 *
 * \author Piotr M. Szczypiński
 */
class Tree:TreeStructure
{
public:
/** Konstruktor klasy. Parametr mindistance decyduje o łączeniu węzłów.
 * Jeśli odległość węzłą końcowego dodawanej gałęzi od istniejącego węzła
 * drzewa jest mniejsza od mindistance to węzły są ze sobą łączone w jeden.*/
    Tree(double mindistance = 0.5);

/** Zapisuje drzewo do pliku o podanej nazwie i w jednym z kilku dostępnych
 * formatów
 * \param fileName jest nazwą pliku
 * \param format jest numerem oznaczającym format zapisu:
 * 0 - oddzielnie węzły i indeksy węzłów w gałęziach
 * 1 - gałęzie z pełną informacją o węzłach
*/
    bool saveTree(const char *fileName, unsigned int format);
/** Odczytuje dane z pliku
 * \param fileName jest nazwą pliku
*/
    bool loadTree(const char *fileName);

/* Returns count of branches.*/
/** Liczba gałęzi drzewa.
 * \returns zwraca liczbę gałęzi drzewa*/
    unsigned int count(void);
/* Returns count of nodes in a branch.
 * \param branchIndex is an index of the branch
 * \returns count of nodes or negative value if fails*/
/** Liczba węzłów w gałęzi.
 * \param ib indeks gałęzi
 * \returns zwraca liczbę węzłów tworzących gałąź o indeksie ib.*/
    int count(unsigned int ib);
/** Liczba węzłów drzewa.
 * \returns zwraca liczbę węzłów tworzących całe drzewo.*/
    unsigned int nodeCount(void);

/** Wektor węzłów gałęzi.
 * \param ib indeks gałęzi
 * \returns zwraca wektor węzłów tworzących gałąź o indeksie ib.*/
    std::vector<NodeIn3D> branch(unsigned int ib);
/* Returns a node of the branch.
 * \param branchI is an index of the branch
 * \param nodeI is an index of subsequent node of the branch*/
/** Węzeł gałęzi.
 * \param ib indeks gałęzi
 * \param in indeks kolejny węzła gałęzi
 * \returns węzeł gałęzi*/
    NodeIn3D node(unsigned int ib, unsigned int in);
/* Returns a node of a tree.
  * \param index is a global index of node in the tree*/
/** Węzeł drzewa.
 * \param i indeks węzła w drzewie
 * \returns węzeł drzewa*/
    NodeIn3D node(unsigned int i);

/* Exchanges a node in the tree with a new one.
  * \param index is an index in the tree of the node to overwrite
  * \param newNode is a structure with a new node specification
  * \returns true on success*/
/** Podmienia węzeł w drzewie na nowy.
 * \param i indeks węzła w drzewie
 * \param node nowy węzeł
 * \returns zwrace true jeśli się uda albo false jeśli się nie uda*/
    bool setNode(NodeIn3D node, unsigned int i);
/* Adds a new branch newBranch to the tree.
  * \param newBranch vector of the new branch nodes
  * \returns true on success*/
/** Dodaje nową gałąź do drzewa.
 * \param branch lista węzłów tworzących dodawaną gałąź
 * \returns zwrace true jeśli się uda albo false jeśli się nie uda*/
    bool addBranch(std::vector<NodeIn3D> branch);
/* Removes a branch of an index index from the tree.
    * \param index is an index in the branch to remove
    * \returns true on success*/
/** Usuwa gałąź z drzewa.
 * \param ib indeks gałęzi do usunięcia
 * \returns zwrace true jeśli się uda albo false jeśli się nie uda*/
    bool removeBranch(unsigned int ib);

private:
/* Distance to join nodes, set by the constructor.*/
/** Parametr ustawiany przez konstruktor klasy, decyduje o łączeniu węzłów.
*/
    double joinDistance;
};

#endif // TREE_H
