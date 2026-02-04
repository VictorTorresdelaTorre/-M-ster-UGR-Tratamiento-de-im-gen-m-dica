#include "itkImage.h"
#include "itkImageFileReader.h"

#include "itkGradientMagnitudeImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"

#include "QuickView.h"

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

using namespace std;

int main (int argc, char * argv[])
{
	if (argc < 2) {
		cerr << "Usage: " << endl;
		cerr << argv[0] << "   inputImageFile " << endl;
		return EXIT_FAILURE;
	}
	//DEFINICIÓN DE TIPOS
	typedef float InputPixelType;
	typedef float OutputPixelType;
	typedef itk::Image<InputPixelType, 2> InputImageType;
	typedef itk::Image<OutputPixelType, 2> OutputImageType;
	typedef itk::ImageFileReader<InputImageType> ReaderType;
	typedef itk::GradientMagnitudeImageFilter<InputImageType, OutputImageType> GradientMagnitudeFilterType;
	typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<InputImageType, OutputImageType> GradientMagnitudeRecursiveGaussianFilterType;
	
	//Creamos el lector
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);

	//Creamos el filtro de tipo 1 al que pasamos la imagen leida en el lector
	GradientMagnitudeFilterType::Pointer gradientMagnitudeFilter = GradientMagnitudeFilterType::New();
	gradientMagnitudeFilter->SetInput(reader->GetOutput());
	gradientMagnitudeFilter->Update();
	
	//Creamos el filtro de tipo 2 al que pasamos la imagen leida en el lector
	GradientMagnitudeRecursiveGaussianFilterType::Pointer gradientMagnitudeRecursiveGaussianFilter1 = GradientMagnitudeRecursiveGaussianFilterType::New();
	gradientMagnitudeRecursiveGaussianFilter1->SetInput(reader->GetOutput());
	
	//Cambiamos el valor de sigma en el filtro
	const double sigma1 = 0.01;
	gradientMagnitudeRecursiveGaussianFilter1->SetSigma(sigma1);
	gradientMagnitudeRecursiveGaussianFilter1->Update();

	/*GradientMagnitudeRecursiveGaussianFilterType::Pointer gradientMagnitudeRecursiveGaussianFilter2 = GradientMagnitudeRecursiveGaussianFilterType::New();
	gradientMagnitudeRecursiveGaussianFilter2->SetInput(reader->GetOutput());
	//Cambiamos el valor de sigma en el filtro
	const double sigma2 = 0.1;
	gradientMagnitudeRecursiveGaussianFilter2->SetSigma(sigma2);
	gradientMagnitudeRecursiveGaussianFilter2->Update();

	GradientMagnitudeRecursiveGaussianFilterType::Pointer gradientMagnitudeRecursiveGaussianFilter3 = GradientMagnitudeRecursiveGaussianFilterType::New();
	gradientMagnitudeRecursiveGaussianFilter3->SetInput(reader->GetOutput());
	//Cambiamos el valor de sigma en el filtro
	const double sigma3 = 0.2;
	gradientMagnitudeRecursiveGaussianFilter3->SetSigma(sigma3);
	gradientMagnitudeRecursiveGaussianFilter3->Update();

	GradientMagnitudeRecursiveGaussianFilterType::Pointer gradientMagnitudeRecursiveGaussianFilter4 = GradientMagnitudeRecursiveGaussianFilterType::New();
	gradientMagnitudeRecursiveGaussianFilter4->SetInput(reader->GetOutput());
	//Cambiamos el valor de sigma en el filtro
	const double sigma4 = 0.35;
	gradientMagnitudeRecursiveGaussianFilter4->SetSigma(sigma4);
	gradientMagnitudeRecursiveGaussianFilter4->Update();*/

	//VISUALIZACION
	QuickView viewer;
	viewer.SetNumberOfColumns(2);
	string description;
	description = "Original";
	viewer.AddImage(reader->GetOutput(), true, description);
	
	description = "Gradient Magnitude";
	viewer.AddImage(gradientMagnitudeFilter->GetOutput(), true, description);

	description = "Gradient Magnitude Recursive Gausssian\nSigma = " + to_string(sigma1);
	viewer.AddImage(gradientMagnitudeRecursiveGaussianFilter1->GetOutput(), true, description);

	/*description = "Gradient Magnitude Recursive Gausssian\nSigma = " + to_string(sigma2);
	viewer.AddImage(gradientMagnitudeRecursiveGaussianFilter2->GetOutput(), true, description);

	description = "Gradient Magnitude Recursive Gausssian\nSigma = " + to_string(sigma3);
	viewer.AddImage(gradientMagnitudeRecursiveGaussianFilter3->GetOutput(), true, description);

	description = "Gradient Magnitude Recursive Gausssian\nSigma = " + to_string(sigma4);
	viewer.AddImage(gradientMagnitudeRecursiveGaussianFilter4->GetOutput(), true, description);*/
	
	viewer.Visualize();

	return EXIT_SUCCESS;
}