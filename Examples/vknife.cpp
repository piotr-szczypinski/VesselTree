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


#include <itkImageIOFactoryRegisterManager.h>
#include <itkImageIOFactory.h>
//#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
//#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)
#include "imagefilters.h"




//=============================================================================
// Print help
void printhelp(void)
{
//------------------------------------------------------------------------------------------v
    printf("Usage: vknife command [OPTION]...\n");
    printf("Runs volume (3D image) processing procedures from VesselTree library.\n");
    printf("Version 2014.06.10 by Piotr M. Szczypinski\n");
    printf("Commands:\n");
    printf("  copyU8 s_input s_output\n");
    printf("  copyF32 s_input s_output\n");
    printf("  rescaleIntensity s_input s_output f_min_intensity f_max_intensity\n");
    printf("  gaussianFilter s_input s_output f_sigma\n");
    printf("  hessianFilter s_input s_output f_sigma\n");
    printf("  maxIntensity s_input\n");
    printf("  minIntensity s_input\n");
    printf("  floodFill s_input s_output f_min_intensity f_max_intensity i_x i_y i_z\n");
    printf("  threshold s_input s_output f_min_intensity f_max_intensity\n");
    printf("  multiscaleHessian s_input s_output f_sigma_min f_sigma_max i_scales\n");
    printf("  upscaleForCenteredSkeleton s_input s_output\n");
    printf("  skeletonFromBinary s_input s_output\n");
    printf("  centeredSkeleton s_input s_output\n");
    printf("  whiteForegroundHoleFill s_input s_output\n");
    printf("  blackForegroundHoleFill s_input s_output\n");
    printf("  whiteTopHat s_input s_output f_radius\n");
    printf("  blackTopHat s_input s_output f_radius\n");
    printf("  binaryDilate s_input s_output f_radius\n");
    printf("  binaryErode s_input s_output f_radius\n");

    printf("  skeletonToTreeIntSpace s_input s_output i_output_format\n");
    printf("  skeletonToTree s_input s_output i_output_format\n");
    printf("\n");
    printf("  /? displays help and exits\n\n");
}

void test(void)
{
    std::cout << "ITK Version: " <<
                 ITK_VERSION_MAJOR << "." <<
                 ITK_VERSION_MINOR << "." <<
                 ITK_VERSION_PATCH << std::endl;

	typedef std::list< itk::LightObject::Pointer > RegisteredObjectsContainerType;
    RegisteredObjectsContainerType registeredIOs = itk::ObjectFactoryBase::CreateAllInstance( "itkImageIOBase" );
    std::cout << "ImageFileIO components (" << registeredIOs.size() << "):" <<std::endl;
	for (RegisteredObjectsContainerType::const_iterator ci = registeredIOs.begin(); ci != registeredIOs.end(); ++ci)
	{
		std::cout << "  " << ci->GetPointer()->GetNameOfClass() << std::endl;
	}
}

void copy(char* input, char* output, int type)
{
    switch(type)
    {
    case 8: ImageFilters< u_int8_t >::copy(input, output); break;
    case 9: ImageFilters< int8_t >::copy(input, output); break;
    case 16: ImageFilters< u_int16_t >::copy(input, output); break;
    case 17: ImageFilters< int16_t >::copy(input, output); break;
    case 32: ImageFilters< float >::copy(input, output); break;
    case 64: ImageFilters< double >::copy(input, output); break;
    default: ImageFilters< float >::copy(input, output);
    }
//    ImageFilters< float >::ImageType::Pointer image;
//    ImageFilters< float >::open(input, &image);
//    ImageFilters< float >::ImageType::Pointer image =
//    ImageFilters< float >::open(input);
//    ImageFilters< float >::save(image, output);
}

void rescaleIntensity(char* input, char* output, double min, double max)
{
    ImageFilters< float >::save(
                ImageFilters< float >::rescaleIntensity(
                    ImageFilters< float >::open(input),
                    min, max),
                output);
}

void gaussianFilter(char* input, char* output, double std)
{
    ImageFilters< float >::save(
                ImageFilters< float >::gaussianFilter(
                    ImageFilters< float >::open(input),
                    std),
                output);
}

void hessianFilter(char* input, char* output, double std)
{
    ImageFilters< float >::save(
                ImageFilters< float >::hessianFilter(
                    ImageFilters< float >::open(input),
                    std),
                output);
}

void maxIntensity(char* input)
{
    ImageFilters< float >::ImageType::IndexType coords;
    double val =
    ImageFilters< float >::maxIntensity(
        ImageFilters< float >::open(input),
        &coords);
    std::cout << coords[0] << " " << coords[1] << " " << coords[2] << " " << val << std::endl;
}

void minIntensity(char* input)
{
    ImageFilters< float >::ImageType::IndexType coords;
    double val =
    ImageFilters< float >::minIntensity(
        ImageFilters< float >::open(input),
        &coords);
    std::cout << coords[0] << " " << coords[1] << " " << coords[2] << " " << val << std::endl;
}

void floodFill(char* input, char* output, double min, double max, int x, int y, int z)
{
    ImageFilters< float >::ImageType::IndexType coords;
    coords[0] = x;
    coords[1] = y;
    coords[2] = z;
    ImageFilters< float >::save(
                ImageFilters< float >::regionGrowing(
                    ImageFilters< float >::open(input),
                    min, max, coords),
                output);
}

void threshold(char* input, char* output, double min, double max)
{
    ImageFilters< float >::save(
                ImageFilters< float >::thresholdFilter(
                    ImageFilters< float >::open(input),
                    min, max),
                output);
}

void multiscaleHessian(char* input, char* output, double stdmin, double stdmax, int scales)
{
    ImageFilters< float >::save(
                ImageFilters< float >::multiscaleHessianAlgorithm(
                    ImageFilters< float >::open(input),
                    stdmin, stdmax, scales),
                output);
}

void upscaleForCenteredSkeleton(char* input, char* output)
{
    ImageFilters< char >::save(
                ImageFilters< char >::upscaleForCenteredSkeleton(
                    ImageFilters< char >::open(input)),
                output);
}

void skeletonFromBinary(char* input, char* output)
{
    ImageFilters< char >::save(
                ImageFilters< char >::skeletonFromBinary(
                    ImageFilters< char >::open(input)),
                output);
}

void centeredSkeleton(char* input, char* output)
{
    ImageFilters< char >::save(
        ImageFilters< char >::skeletonFromBinary(
            ImageFilters< char >::upscaleForCenteredSkeleton(
                ImageFilters< char >::open(input))),
                output);
}

void whiteForegroundHoleFill(char* input, char* output)
{
    ImageFilters< char >::ImageType::Pointer image = ImageFilters< char >::open(input);
    char foreground = ImageFilters< char >::maxIntensity(image);
    ImageFilters< char >::ImageType::Pointer image2 = ImageFilters< char >::holeFill(image, foreground);
    ImageFilters< char >::save(image2, output);
}

void blackForegroundHoleFill(char* input, char* output)
{
    ImageFilters< char >::ImageType::Pointer image = ImageFilters< char >::open(input);
    char foreground = ImageFilters< char >::minIntensity(image);
    ImageFilters< char >::ImageType::Pointer image2 = ImageFilters< char >::holeFill(image, foreground);
    ImageFilters< char >::save(image2, output);
}

void whiteTopHat(char* input, char* output, double radius)
{
    ImageFilters< float >::save(
                ImageFilters< float >::whiteTopHat(
                    ImageFilters< float >::open(input),
                    radius),
                output);
}

void blackTopHat(char* input, char* output, double radius)
{
    ImageFilters< float >::save(
                ImageFilters< float >::blackTopHat(
                    ImageFilters< float >::open(input),
                    radius),
                output);
}

void dilate(char* input, char* output, double radius)
{
    ImageFilters< char >::save(
                ImageFilters< char >::dilate(
                    ImageFilters< char >::open(input),
                    radius),
                output);
}

void erode(char* input, char* output, double radius)
{
    ImageFilters< char >::save(
                ImageFilters< char >::erode(
                    ImageFilters< char >::open(input),
                    radius),
                output);
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printhelp();
		return 0;
    }

	try
	{
        //if(strcmp(argv[1], "copy") == 0 && argc > 3) copy(argv[2], argv[3], 32);
        if(strcmp(argv[1], "copyU8") == 0 && argc > 3) copy(argv[2], argv[3], 8);
        else if(strcmp(argv[1], "copyS16") == 0 && argc > 3) copy(argv[2], argv[3], 17);
        else if(strcmp(argv[1], "copyU16") == 0 && argc > 3) copy(argv[2], argv[3], 16);
        else if(strcmp(argv[1], "copyF32") == 0 && argc > 3) copy(argv[2], argv[3], 32);
        else if(strcmp(argv[1], "copyF64") == 0 && argc > 3) copy(argv[2], argv[3], 64);
        else if(strcmp(argv[1], "rescaleIntensity") == 0 && argc > 5) rescaleIntensity(argv[2], argv[3], atof(argv[4]), atof(argv[5]));
        else if(strcmp(argv[1], "gaussianFilter") == 0 && argc > 4) gaussianFilter(argv[2], argv[3], atof(argv[4]));
        else if(strcmp(argv[1], "hessianFilter") == 0 && argc > 4) hessianFilter(argv[2], argv[3], atof(argv[4]));
        else if(strcmp(argv[1], "floodFill") == 0 && argc > 8) floodFill(argv[2], argv[3], atof(argv[4]), atof(argv[5]), atoi(argv[6]), atoi(argv[7]), atoi(argv[8]));
        else if(strcmp(argv[1], "threshold") == 0 && argc > 5) threshold(argv[2], argv[3], atof(argv[4]), atof(argv[5]));
        else if(strcmp(argv[1], "multiscaleHessian") == 0 && argc > 6) multiscaleHessian(argv[2], argv[3], atof(argv[4]), atof(argv[5]), atoi(argv[6]));
        else if(strcmp(argv[1], "minIntensity") == 0 && argc > 2) minIntensity(argv[2]);
        else if(strcmp(argv[1], "maxIntensity") == 0 && argc > 2) maxIntensity(argv[2]);
        else if(strcmp(argv[1], "upscaleForCenteredSkeleton") == 0 && argc > 3) upscaleForCenteredSkeleton(argv[2], argv[3]);
        else if(strcmp(argv[1], "skeletonFromBinary") == 0 && argc > 3) skeletonFromBinary(argv[2], argv[3]);
        else if(strcmp(argv[1], "centeredSkeleton") == 0 && argc > 3) centeredSkeleton(argv[2], argv[3]);
        else if(strcmp(argv[1], "whiteForegroundHoleFill") == 0 && argc > 3) whiteForegroundHoleFill(argv[2], argv[3]);
        else if(strcmp(argv[1], "blackForegroundHoleFill") == 0 && argc > 3) blackForegroundHoleFill(argv[2], argv[3]);
        else if(strcmp(argv[1], "whiteTopHat") == 0 && argc > 4) whiteTopHat(argv[2], argv[3], atof(argv[4]));
        else if(strcmp(argv[1], "blackTopHat") == 0 && argc > 4) blackTopHat(argv[2], argv[3], atof(argv[4]));
        else if(strcmp(argv[1], "dilate") == 0 && argc > 4) dilate(argv[2], argv[3], atof(argv[4]));
        else if(strcmp(argv[1], "erode") == 0 && argc > 4) erode(argv[2], argv[3], atof(argv[4]));

//        else if(strcmp(argv[1], "skeletonToTreeIntSpace") == 0 && argc > 4) skeletonToTreeIntSpace(argv[2], argv[3], atoi(argv[4]));
//        else if(strcmp(argv[1], "skeletonToTree") == 0 && argc > 4) skeletonToTree(argv[2], argv[3], atoi(argv[4]));

        else if(strcmp(argv[1], "test") == 0 && argc > 1) test();
        else std::cout << "Unknown command " << argv[1] << " or invalid arguments" << std::endl;
	}
    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ITK exception" << std::endl;
        std::cerr << err << std::endl;
        return -2;
    }
	catch(...)
	{
        std::cerr << "Unknown exception" << std::endl;
		return -1;
	}

    return 0;
}
