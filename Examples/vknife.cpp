/** @example createtree.cpp
 * Przykład pokazuje jak załadować obraz, zbinaryzować go, wyznaczyć szkielet,
 * zamienić obraz rastrowy na opis drzewa i zapisać wynik.
 */

#include "itkImageIOFactoryRegisterManager.h"
//#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
//#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)
//#include "renderit.h"
#include "tree.h"
#include "buildtree.h"
#include "image.h"
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
    printf("  copy s_input s_output\n");
    printf("  rescaleIntensity s_input s_output f_min_intensity f_max_intensity\n");
    printf("  gaussianFilter s_input s_output f_sigma\n");
    printf("  hessianFilter s_input s_output f_sigma\n");
    printf("  maxIntensity s_input\n");
    printf("  minIntensity s_input\n");
    printf("  floodFill s_input s_output f_min_intensity f_max_intensity i_x i_y i_z\n");
    printf("  multiscaleHessian s_input s_output f_sigma_min f_sigma_max i_scales\n");
    printf("  upscaleForCenteredSkeleton s_input s_output\n");
    printf("  skeletonFromBinary s_input s_output\n");
    printf("  skeletonToTreeIntSpace s_input s_output i_output_format\n");
    printf("  skeletonToTree s_input s_output i_output_format\n");


    printf("\n");
    printf("  /? displays help and exits\n\n");
}

#include<itkImageIOFactory.h>
void test(void)
{
	typedef std::list< itk::LightObject::Pointer > RegisteredObjectsContainerType;
    RegisteredObjectsContainerType registeredIOs = itk::ObjectFactoryBase::CreateAllInstance( "itkImageIOBase" );
    std::cout << "ImageFileReader components (" << registeredIOs.size() << "):" <<std::endl;
	for (RegisteredObjectsContainerType::const_iterator ci = registeredIOs.begin(); ci != registeredIOs.end(); ++ci)
	{
		std::cout << "  " << ci->GetPointer()->GetNameOfClass() << std::endl;
	}
}

/*
void nameParsing(char* mode, char* input, char* output)
{
	int f; 
	char* separator = (char*)",;\t";
	int length = strlen(mode);
    if(length <= 0) return;

	float params[8];
	for(int f = 0; f < 8; f++) params[f] = 0.0;

	std::string ssmode = mode;

	size_t found = ssmode.find_first_of(separator);
    while (found != std::string::npos)
    {
		ssmode.replace(found, 1, " ");
        found=ssmode.find_first_of(separator, found+1);
    }

	std::string command;
	std::istringstream stream(ssmode);
	stream >> command;
	for(int f = 0; f < 8; f++) stream >> params[f];

	try
	{
		if(command.compare("copy") == 0) copy(stream);

		ImageStructure BuildTree::gaussianFilter(ImageStructure par1, float par2)


		else std::cout << "Incorrect command" << std::endl;
	}
	catch(...)
	{
		std::cout << "Exception in " << ssmode << " command" << std::endl;
	}
}
*/

void copy(char* input, char* output)
{
	Image image;
	image.fillStructure(BuildTree::openAnalyzeImage(input));
	BuildTree::saveImage(image.returnStruct(), output);
}

void rescaleIntensity(char* input, char* output, double min, double max)
{
	Image image;
	ImageStructure outimage;
	image.fillStructure(BuildTree::openAnalyzeImage(input));
	outimage = ImageFilters::rescaleIntensity(image.returnStruct(), min, max);
	BuildTree::saveImage(outimage, output);
}

void gaussianFilter(char* input, char* output, double std)
{
	Image image;
	ImageStructure outimage;
	image.fillStructure(BuildTree::openAnalyzeImage(input));
	outimage = ImageFilters::gaussianFilter(image.returnStruct(), std);
	BuildTree::saveImage(outimage, output);
}

void hessianFilter(char* input, char* output, double std)
{
	Image image;
	ImageStructure outimage;
	image.fillStructure(BuildTree::openAnalyzeImage(input));
	outimage = ImageFilters::hessianFilter(image.returnStruct(), std);
	BuildTree::saveImage(outimage, output);
}

void maxIntensity(char* input)
{
	int x, y, z;
	Image image;
	ImageStructure outimage;
	image.fillStructure(BuildTree::openAnalyzeImage(input));
	double val = ImageFilters::maxIntensity(image.returnStruct(), &x, &y, &z);
	std::cout << x << " " << y << " " << z << " " << val << std::endl;
}

void minIntensity(char* input)
{
	int x, y, z;
	Image image;
	ImageStructure outimage;
	image.fillStructure(BuildTree::openAnalyzeImage(input));
	double val = ImageFilters::minIntensity(image.returnStruct(), &x, &y, &z);
	std::cout << x << " " << y << " " << z << " " << val << std::endl;
}

void floodFill(char* input, char* output, double min, double max, int x, int y, int z)
{
	Image image;
	ImageStructure outimage;
	image.fillStructure(BuildTree::openAnalyzeImage(input));

	std::vector<unsigned int> coord;
	coord.push_back(x); 	
	coord.push_back(y);
	coord.push_back(z);
	outimage = ImageFilters::RegionGrowing(image.returnStruct(), min, max, coord);
	BuildTree::saveImage(outimage, output);
}

void multiscaleHessian(char* input, char* output, double stdmin, double stdmax, int scales)
{
	Image image;
	ImageStructure outimage;
	image.fillStructure(BuildTree::openAnalyzeImage(input));
	outimage = ImageFilters::MultiscaleHessianAlgorithm(image.returnStruct(), stdmin, stdmax, scales);
	BuildTree::saveImage(outimage, output);
}

void upscaleForCenteredSkeleton(char* input, char* output)
{
    Image image;
    ImageStructure outimage;
    image.fillStructure(BuildTree::openAnalyzeImage(input));
    outimage = BuildTree::upscaleForCenteredSkeleton(image.returnStruct());
    BuildTree::saveImage(outimage, output);
}

void skeletonFromBinary(char* input, char* output)
{
    Image image;
    ImageStructure outimage;
    image.fillStructure(BuildTree::openAnalyzeImage(input));
    outimage = BuildTree::skeletonFromBinary(image.returnStruct());
    BuildTree::saveImage(outimage, output);
}

void skeletonToTree(char* input, char* output, unsigned int mode)
{
    Image image;
    Tree outimage;
    image.fillStructure(BuildTree::openAnalyzeImage(input));
    outimage = BuildTree::skeletonToTree(image.returnStruct());
    outimage.saveTree(output, mode%2);
}

void skeletonToTreeIntSpace(char* input, char* output, unsigned int mode)
{
    Image image;
    Tree outimage;
    image.fillStructure(BuildTree::openAnalyzeImage(input));
    outimage = BuildTree::skeletonToTreeIntSpace(image.returnStruct());
    outimage.saveTree(output, mode%2);
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
		if(strcmp(argv[1], "copy") == 0 && argc > 3) copy(argv[2], argv[3]);
		else if(strcmp(argv[1], "rescaleIntensity") == 0 && argc > 5) rescaleIntensity(argv[2], argv[3], atof(argv[4]), atof(argv[5]));
		else if(strcmp(argv[1], "gaussianFilter") == 0 && argc > 4) gaussianFilter(argv[2], argv[3], atof(argv[4]));
		else if(strcmp(argv[1], "hessianFilter") == 0 && argc > 4) hessianFilter(argv[2], argv[3], atof(argv[4]));
		else if(strcmp(argv[1], "floodFill") == 0 && argc > 8) floodFill(argv[2], argv[3], atof(argv[4]), atof(argv[5]), atoi(argv[6]), atoi(argv[7]), atoi(argv[8]));
		else if(strcmp(argv[1], "multiscaleHessian") == 0 && argc > 6) multiscaleHessian(argv[2], argv[3], atof(argv[4]), atof(argv[5]), atoi(argv[6]));
		else if(strcmp(argv[1], "minIntensity") == 0 && argc > 2) minIntensity(argv[2]);
		else if(strcmp(argv[1], "maxIntensity") == 0 && argc > 2) maxIntensity(argv[2]);
        else if(strcmp(argv[1], "upscaleForCenteredSkeleton") == 0 && argc > 3) upscaleForCenteredSkeleton(argv[2], argv[3]);
        else if(strcmp(argv[1], "skeletonFromBinary") == 0 && argc > 3) skeletonFromBinary(argv[2], argv[3]);
        else if(strcmp(argv[1], "skeletonToTreeIntSpace") == 0 && argc > 4) skeletonToTreeIntSpace(argv[2], argv[3], atoi(argv[4]));
        else if(strcmp(argv[1], "skeletonToTree") == 0 && argc > 4) skeletonToTree(argv[2], argv[3], atoi(argv[4]));


        else if(strcmp(argv[1], "test") == 0 && argc > 1) test();
        else std::cout << "Unknown command " << argv[1] << "or invalid arguments" << std::endl;
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
