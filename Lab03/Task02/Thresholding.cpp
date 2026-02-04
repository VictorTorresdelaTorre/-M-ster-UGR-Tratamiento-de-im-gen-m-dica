#include "itkImage.h"
#include "itkImageFileReader.h"

#include "itkBinaryThresholdImageFilter.h"
#include "itkThresholdImageFilter.h"

#include "QuickView.h"

#include "itkMedianImageFilter.h"

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

using namespace std;

int main(int argc, char * argv[])
{
	if (argc < 2) {
		cerr << "Usage: " << endl;
		cerr << argv[0] << " inputImageFile" << endl;
		return EXIT_FAILURE;
	}

	//DEFINICION DE TIPOS
	const unsigned int Dimension = 2;
	typedef unsigned char PixelType;
	typedef itk::Image<PixelType, Dimension> ImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;
	typedef itk::BinaryThresholdImageFilter<ImageType, ImageType> BinaryThresholdFilterType;
	typedef itk::ThresholdImageFilter<ImageType> ThresholdFilterType;
  	typedef itk::MedianImageFilter<ImageType,ImageType> MedianFilterType;
	//VARIABLES
	int outside = 0; //valor de intensidad para valores por debajo del humbral
	int inside = 255; //valor de intensidad para valores superiores al humbral
	int lower = 90; //humbral inferior
	int upper = 180; //humbral superior
	int below = 90;
	ImageType::SizeType support = {2,2};
	
	//LECTOR
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);

	//FILTRO DE MEDIA
	MedianFilterType::Pointer medianFilter = MedianFilterType::New();
	medianFilter->SetRadius(support);
	medianFilter->SetInput(reader->GetOutput());

	//FILTRO 1
	BinaryThresholdFilterType::Pointer binaryThresholdFilter = BinaryThresholdFilterType::New();
	binaryThresholdFilter->SetInput(reader->GetOutput());
	binaryThresholdFilter->SetOutsideValue(outside);
	binaryThresholdFilter->SetInsideValue(inside);
	binaryThresholdFilter->SetLowerThreshold(lower);
	binaryThresholdFilter->SetUpperThreshold(upper);
	binaryThresholdFilter->Update();

	//FILTRO 2
	ThresholdFilterType::Pointer thresholdFilter = ThresholdFilterType::New();
	thresholdFilter->SetInput(reader->GetOutput());
	thresholdFilter->SetOutsideValue(outside);
	thresholdFilter->ThresholdBelow(below);
	thresholdFilter->Update();

	//FILTRO 1 con filtro de ruido
	BinaryThresholdFilterType::Pointer binaryThresholdFilter_r = BinaryThresholdFilterType::New();
	binaryThresholdFilter_r->SetInput(medianFilter->GetOutput());
	binaryThresholdFilter_r->SetOutsideValue(outside);
	binaryThresholdFilter_r->SetInsideValue(inside);
	binaryThresholdFilter_r->SetLowerThreshold(lower);
	binaryThresholdFilter_r->SetUpperThreshold(upper);
	binaryThresholdFilter_r->Update();

	//FILTRO 2 con filtro de ruido
	ThresholdFilterType::Pointer thresholdFilter_r = ThresholdFilterType::New();
	thresholdFilter_r->SetInput(medianFilter->GetOutput());
	thresholdFilter_r->SetOutsideValue(outside);
	thresholdFilter_r->ThresholdBelow(below);
	thresholdFilter_r->Update();

	//VISUALIZACION
	QuickView viewer;
	viewer.SetNumberOfColumns(2);

	string description;

	description = "Original";
	viewer.AddImage(reader->GetOutput(), true, description);
	description = "Binary Threshold -> Outside = " + to_string(outside) + ", Inside = " + to_string(inside) + "\n Lower = " + to_string(lower) + ", Upper = " + to_string(upper);
	viewer.AddImage(binaryThresholdFilter->GetOutput(), true, description);
	description = "Threshold\nOutside = " + to_string(outside) + ", Below = " + to_string(below);
	viewer.AddImage(thresholdFilter->GetOutput(), true, description);

	description = "Median filter\n kernel size= " + to_string(support[0]) + "x" + to_string(support[1]);
	viewer.AddImage(medianFilter->GetOutput(), true, description);
	description = "Binary Threshold -> Outside = " + to_string(outside) + ", Inside = " + to_string(inside) + "\n Lower = " + to_string(lower) + ", Upper = " + to_string(upper);
	viewer.AddImage(binaryThresholdFilter_r->GetOutput(), true, description);
	description = "Threshold\nOutside = " + to_string(outside) + ", Below = " + to_string(below);
	viewer.AddImage(thresholdFilter_r->GetOutput(), true, description);
	viewer.Visualize();
	
	return EXIT_SUCCESS;
}