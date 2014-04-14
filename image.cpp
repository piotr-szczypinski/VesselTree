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

#include "image.h"

int Image::dimensionality()
{
    return size.size();
}
type Image::getType()
{
    switch(fileType)
    {
       case BOOL1:     std::cout << "bool" << std::endl;     break;
       case UCHAR8:    std::cout << "uchar" << std::endl;    break;
       case INT16:     std::cout << "int16" << std::endl;    break;
       case FLOAT32:   std::cout << "float32" << std::endl;  break;
       case DOUBLE64:  std::cout << "double64" << std::endl; break;
    }
    return fileType;
}
std::string Image::getShortNote()
{
   return shortNote;
}
std::vector<unsigned int> Image::getSize()
{
    return size;
}
std::vector<double> Image::getSpacing()
{
    return spacing;
}
std::vector<double> Image::getOrigin()
{
    return origin;
}

double Image::getValue(std::vector<int> coord)
{
    unsigned int suma = 0, mnoznik = 1;
    for(int i = 0 ; i<coord.size() ; i++)
    {
        suma=suma + coord[i]*mnoznik;
        mnoznik = mnoznik*size[i];
    }
    float *dane = (float*)(void*)imageData;
    return dane[suma];
}
double Image::getValue(int x, int y, int z)
{
    std::vector<int> coord;
    coord.push_back(x);
    coord.push_back(y);
    coord.push_back(z);
    return getValue(coord);
}

void* Image::getLinePointer(std::vector<int> coord, int direction)
{
    coord[direction] = 0;
    unsigned int suma = 0, mnoznik = 1;
    for(int i = 0 ; i<coord.size() ; i++)
    {
        suma=suma + coord[i]*mnoznik;
        mnoznik = mnoznik*size[i];
    }
    float *dane = (float*)(void*)imageData;

    return  dane+suma;
}
void* Image::getSlice(std::vector<int> coord, int direction)
{
    unsigned int suma = 0, mnoznik = 1;
    for(int i = 0 ; i<coord.size() ; i++)
    {
        if(i==direction)    suma=suma + coord[i]*mnoznik;
        mnoznik = mnoznik*size[i];
    }
    float *dane = (float*)(void*)imageData;

    return  dane+suma;
}

void Image::setValue(std::vector<int> coord, double value)
{
    unsigned int suma = 0, mnoznik = 1;
    for(int i = 0 ; i<coord.size() ; i++)
    {
        suma=suma + coord[i]*mnoznik;
        mnoznik = mnoznik*size[i];
    }
    float *dane = (float*)(void*)imageData;
    dane[suma] = value;
}
void Image::setValue(int x, int y, int z, double value)
{
    std::vector<int> coord;
    coord.push_back(x);
    coord.push_back(y);
    coord.push_back(z);
    setValue(coord, value);

}

void Image::fillStructure(ImageStructure image)
{
    fileType = image.fileType;
    imageData = image.imageData;
    spacing =image.spacing;
    origin = image.origin;
    size = image.size;
    shortNote = image.shortNote;
}
ImageStructure Image::returnStruct()
{
    ImageStructure tempImg;
    tempImg.fileType = fileType;
    tempImg.imageData = imageData;
    tempImg.spacing = spacing;
    tempImg.origin = origin;
    tempImg.size = size;
    tempImg.shortNote = shortNote;

    return tempImg;
}
