#include "itkImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"
#include "QuickView.h"

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

 
 
#include "itkImage.h"
 
 
int
main(int argc, char ** argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile " << std::endl;
    return EXIT_FAILURE;
  }

  using InputPixelType = float;
  using OutputPixelType = unsigned char;
  constexpr unsigned int Dimension = 2;
 
  using InputImageType = itk::Image<InputPixelType, Dimension>;
  using OutputImageType = itk::Image<OutputPixelType, Dimension>;

  using ReaderType = itk::ImageFileReader<InputImageType>;

  using FilterType = itk::RescaleIntensityImageFilter<InputImageType, OutputImageType>;

  FilterType::Pointer filter = FilterType::New();
  filter->SetOutputMinimum(0);
  filter->SetOutputMaximum(255);

  ReaderType::Pointer reader = ReaderType::New();
 
  filter->SetInput(reader->GetOutput());

  const char * inputFilename = argv[1];

  reader->SetFileName(inputFilename);

  QuickView viewer;

	std::string description;

	description = "Original";
	viewer.AddImage(reader->GetOutput(), true, description);

  description = "Filtrada";
	viewer.AddImage(filter->GetOutput(), true, description);
  viewer.Visualize();

 
 
  return EXIT_SUCCESS;
}