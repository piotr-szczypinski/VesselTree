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

#include <iostream>
#include <vector>

enum type
{
    BOOL1, UCHAR8, INT16, FLOAT32, DOUBLE64
};

struct strukturaObrazu
{
    std::vector<unsigned int> size;
    std::vector<double> spacing;
    std::vector<double> origin;
    type fileType;
    std::string shortNote;
    void *imageData;
};

class Obraz : strukturaObrazu
{
public:
    int dimensionality();
    type getType();
    std::string getShortNote();
    std::vector<unsigned int> getSize();
    std::vector<double> getSpacing();
    std::vector<double> getOrigin();
    double getValue(std::vector<int> coord);
    double getValue(int x, int y, int z);
    void* getLinePointer(std::vector<int> coord, int direction);
    void* getSlice(std::vector<int> coord, int direction);
    void setValue(std::vector<int> coord, double value);
    void setValue(int x, int y, int z, double value);
    void fillStructure(strukturaObrazu image);
    strukturaObrazu returnStruct();
};
