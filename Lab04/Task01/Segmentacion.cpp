#include "itkImage.h"
#include "itkImageFileReader.h"

#include "itkMedianImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkConnectedThresholdImageFilter.h"
#include "itkNeighborhoodConnectedImageFilter.h"
#include "itkConfidenceConnectedImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkThresholdImageFilter.h"

#include "QuickView.h"

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

using namespace std;

int main (int argc, char * argv[])
{
	if (argc < 6) {
		cerr << "Usage: " << endl;
		cerr << argv[0] << "   inputImageFile seedX seedY lowerThreshold upperThreshold " << endl;
		return EXIT_FAILURE;
	}


	//Creando tipo de imagen adecuado para BinaryThresholdImageFilter
	const unsigned int Dimension = 2;
	typedef unsigned char PixelType;
	typedef itk::Image<PixelType, Dimension> ImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType2;
	

	//Creando para El reader normal 
	typedef float InputPixelType;
	typedef float OutputPixelType;

	typedef itk::Image<InputPixelType, 2> InputImageType;
	typedef itk::Image<OutputPixelType, 2> OutputImageType;
	typedef itk::ImageFileReader<InputImageType> ReaderType;

	//Filtro Media
	typedef itk::MedianImageFilter<InputImageType, InputImageType> MedianImageFilterType;
	typedef itk::MedianImageFilter<ImageType, ImageType> MedianImageFilterType2;

	//Filtro 
	typedef itk::BinaryThresholdImageFilter<ImageType, ImageType> BinaryThresholdFilterType;

	//Capturando los parámetros por consola
	int seedX = atoi(argv[2]);
	int seedY = atoi(argv[3]);
	float lowerThreshold = atof(argv[4]);
	float upperThreshold = atof(argv[5]);

	InputImageType::IndexType index;
	index[0] = seedX;
	index[1] = seedY;


	//Reader normal
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);

	//Reader para BinaryThresholdFilterType
	ReaderType2::Pointer reader2 = ReaderType2::New();
	reader2->SetFileName(argv[1]);

	//Filtro de mediana Normal
	MedianImageFilterType::Pointer filter_M = MedianImageFilterType::New();
	filter_M->SetInput(reader->GetOutput());
	filter_M->SetRadius(2);

	//Filtro de mediana BinaryThresholdFilterType
	MedianImageFilterType2::Pointer filter_M2 = MedianImageFilterType2::New();
	filter_M2->SetInput(reader2->GetOutput());
	filter_M2->SetRadius(2);
	
	//Variables para BinaryThresholdFilterType
	int outside = 255; //valor de intensidad para valores por debajo del humbral
	int inside = 0; //valor de intensidad para valores superiores al humbral
	int lower = 70; //humbral inferior
	int upper = 160; //humbral superior

	//BinaryThresholdFilterType
	BinaryThresholdFilterType::Pointer filter = BinaryThresholdFilterType::New();
	filter->SetInput(filter_M2->GetOutput());
	filter->SetOutsideValue(outside);
	filter->SetInsideValue(inside);
	filter->SetLowerThreshold(lower);
	filter->SetUpperThreshold(upper);
	filter->Update();
	
	
	/*
	typedef itk::DiscreteGaussianImageFilter<InputImageType, InputImageType> DiscreteGaussianFilterType;
	DiscreteGaussianFilterType::Pointer filter = DiscreteGaussianFilterType::New();
	filter->SetInput(reader->GetOutput());

	int discreteGaussianVariance = 1;
	int discreteGaussianMaximumKernelWidth = 3;

	filter->SetVariance(discreteGaussianVariance);
	filter->SetMaximumKernelWidth(discreteGaussianMaximumKernelWidth);
	filter->Update();*/

	typedef itk::ConnectedThresholdImageFilter<InputImageType, InputImageType> ConnectedFilterType;
	//Creamos las dos imagenes
	ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
	ConnectedFilterType::Pointer connectedThresholdFiltered = ConnectedFilterType::New();
	//Linkamos la imagen
	connectedThreshold->SetInput(reader->GetOutput());
	connectedThresholdFiltered->SetInput(filter_M->GetOutput());

	connectedThreshold->SetLower(lowerThreshold);
	connectedThresholdFiltered->SetLower(lowerThreshold);

	connectedThreshold->SetUpper(upperThreshold);
	connectedThresholdFiltered->SetUpper(upperThreshold);

	connectedThreshold->SetReplaceValue(255);
	connectedThresholdFiltered->SetReplaceValue(255);

	connectedThreshold->SetSeed(index);
	connectedThresholdFiltered->SetSeed(index);

	typedef itk::NeighborhoodConnectedImageFilter<InputImageType, InputImageType> NeighborhoodConnectedFilterType;
	NeighborhoodConnectedFilterType::Pointer neighborhoodConnected = NeighborhoodConnectedFilterType::New();
	NeighborhoodConnectedFilterType::Pointer neighborhoodConnectedFiltered = NeighborhoodConnectedFilterType::New();
	neighborhoodConnected->SetInput(reader->GetOutput());
	neighborhoodConnectedFiltered->SetInput(filter_M->GetOutput());
	neighborhoodConnected->SetLower(lowerThreshold);
	neighborhoodConnectedFiltered->SetLower(lowerThreshold);
	neighborhoodConnected->SetUpper(upperThreshold);
	neighborhoodConnectedFiltered->SetUpper(upperThreshold);
	neighborhoodConnected->SetReplaceValue(255);
	neighborhoodConnectedFiltered->SetReplaceValue(255);
	neighborhoodConnected->SetSeed(index);
	neighborhoodConnectedFiltered->SetSeed(index);

	unsigned int radius = 5;
	double multiplier = 2;
	unsigned int iters = 50;

	typedef itk::ConfidenceConnectedImageFilter<InputImageType, InputImageType> ConfidenceConnectedFilterType;
	typedef itk::ConfidenceConnectedImageFilter<ImageType, ImageType> ConfidenceConnectedFilterType2;
	ConfidenceConnectedFilterType::Pointer confidenceConnected = ConfidenceConnectedFilterType::New();
	ConfidenceConnectedFilterType2::Pointer confidenceConnectedFiltered = ConfidenceConnectedFilterType2::New();
	confidenceConnected->SetInput(filter_M->GetOutput());
	confidenceConnectedFiltered->SetInput(filter->GetOutput());
	confidenceConnected->SetInitialNeighborhoodRadius(radius);
	confidenceConnectedFiltered->SetInitialNeighborhoodRadius(radius);
	confidenceConnected->SetMultiplier(multiplier);
	confidenceConnectedFiltered->SetMultiplier(multiplier);
	confidenceConnected->SetNumberOfIterations(iters);
	confidenceConnectedFiltered->SetNumberOfIterations(iters);
	confidenceConnected->SetReplaceValue(255);
	confidenceConnectedFiltered->SetReplaceValue(255);
	confidenceConnected->SetSeed(index);
	confidenceConnectedFiltered->SetSeed(index);

	QuickView viewer;
	//viewer.SetNumberOfColumns(4);

	string description;

	description = "Connected Threshold Image Filter \nLower: " + to_string(lowerThreshold) + ", Upper: " + to_string(upperThreshold);
	viewer.AddImage(connectedThreshold->GetOutput(), true, description);
	description = "Connected Threshold Image Filter Filtered \nLower: " + to_string(lowerThreshold) + ", Upper: " + to_string(upperThreshold);
	viewer.AddImage(connectedThresholdFiltered->GetOutput(), true, description);
	description = "Neigborhood Connected Image Filter \nLower: " + to_string(lowerThreshold) + ", Upper: " + to_string(upperThreshold);
	viewer.AddImage(neighborhoodConnected->GetOutput(), true, description);
	description = "Neigborhood Connected Image Filter Filtered \nLower: " + to_string(lowerThreshold) + ", Upper: " + to_string(upperThreshold);
	viewer.AddImage(neighborhoodConnectedFiltered->GetOutput(), true, description);
	description = "Confidence Connected Image Filter \nRadius: " + to_string(radius) + ", Multiplier: " + to_string(multiplier) + ", Iters: " + to_string(iters);
	viewer.AddImage(confidenceConnected->GetOutput(), true, description);
	description = "Confidence Connected Image Filter Filtered \nRadius: " + to_string(radius) + ", Multiplier: " + to_string(multiplier) + ", Iters: " + to_string(iters);
	viewer.AddImage(confidenceConnectedFiltered->GetOutput(), true, description);
	description = "Original";
	viewer.AddImage(reader->GetOutput(), true, description);
	description = "Filtrada";
	viewer.AddImage(filter->GetOutput(), true, description);
	viewer.Visualize();

	return EXIT_SUCCESS;
}