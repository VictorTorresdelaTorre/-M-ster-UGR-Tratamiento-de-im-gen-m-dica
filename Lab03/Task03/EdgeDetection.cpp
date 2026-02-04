#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkMedianImageFilter.h"
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"

#include "QuickView.h"

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
	typedef float PixelType;
	const unsigned int Dimension = 2;
	typedef itk::Image<PixelType, Dimension> ImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;
	typedef itk::CannyEdgeDetectionImageFilter<ImageType, ImageType> CannyEdgeDetectionFilterType;
	typedef itk::MedianImageFilter<ImageType, ImageType> MedianFilterType;

	//VARIABLES
	float variance = 1.5;
	float upper = 10;
	float lower = 1.5;

	//LECTOR
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);

	//Filtro sin eliminación de ruido
	CannyEdgeDetectionFilterType::Pointer cannyEdgeDetectionFilterNoFiltered = CannyEdgeDetectionFilterType::New();
	cannyEdgeDetectionFilterNoFiltered->SetInput(reader->GetOutput());
	cannyEdgeDetectionFilterNoFiltered->SetVariance(variance);
	cannyEdgeDetectionFilterNoFiltered->SetUpperThreshold(upper);
	cannyEdgeDetectionFilterNoFiltered->SetLowerThreshold(lower);

	//Eliminación de ruido 1
	
	/*MedianFilterType::Pointer median = MedianFilterType::New();

	ImageType::SizeType indexRadius;
	indexRadius[0] = 3;
	indexRadius[1] = 3;

	median->SetInput(reader->GetOutput());
	median->SetRadius(indexRadius);*/

	//Eliminacion de ruido 2

	typedef itk::DiscreteGaussianImageFilter<ImageType, ImageType> DiscreteGaussianFilterType;

	DiscreteGaussianFilterType::Pointer discreteGaussian = DiscreteGaussianFilterType::New();
	discreteGaussian->SetInput(reader->GetOutput());

	int discreteGaussianVariance = 4;
	int discreteGaussianMaximumKernelWidth = 5;

	discreteGaussian->SetVariance(discreteGaussianVariance);
	discreteGaussian->SetMaximumKernelWidth(discreteGaussianMaximumKernelWidth);
	discreteGaussian->Update();


	//filtro con eliminacion de ruido
	CannyEdgeDetectionFilterType::Pointer cannyEdgeDetectionFilterFiltered = CannyEdgeDetectionFilterType::New();
	cannyEdgeDetectionFilterFiltered->SetInput(discreteGaussian->GetOutput());
	cannyEdgeDetectionFilterFiltered->SetVariance(variance);
	cannyEdgeDetectionFilterFiltered->SetUpperThreshold(upper);
	cannyEdgeDetectionFilterFiltered->SetLowerThreshold(lower);

	QuickView viewer;
	viewer.SetNumberOfColumns(1);

	string description;

	description = "Original";
	viewer.AddImage(reader->GetOutput(), true, description);
	description = "Canny No Filtered\nVariance = " + to_string(variance) + ", Upper = " + to_string(upper) + ", Lower = " + to_string(lower);
	viewer.AddImage(cannyEdgeDetectionFilterNoFiltered->GetOutput(), true, description);
	description = "discreteGaussian";
	viewer.AddImage(discreteGaussian->GetOutput(), true, description);
	description = "Canny Filtered\nVariance = " + to_string(variance) + ", Upper = " + to_string(upper) + ", Lower = " + to_string(lower);
	viewer.AddImage(cannyEdgeDetectionFilterFiltered->GetOutput(), true, description);
	viewer.Visualize();
	
	return EXIT_SUCCESS;
}