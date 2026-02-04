#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkGDCMImageIO.h"

#include "QuickView.h"

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
 
#include <list>
#include <fstream>
 
int
main(int argc, char * argv[])
{
 
  if (argc < 2)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "DicomImage";
    return EXIT_FAILURE;
  }
  using InputPixelType = unsigned char;
  constexpr unsigned int InputDimension = 2;
 
  using InputImageType = itk::Image<InputPixelType, InputDimension>;
  using ReaderType = itk::ImageFileReader<InputImageType>;
 
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  using ImageIOType = itk::GDCMImageIO;
 
  ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
 
  reader->SetImageIO(gdcmImageIO);
  try
  {
    reader->Update();
  }
  catch (const itk::ExceptionObject & e)
  {
    std::cerr << "exception in file reader " << std::endl;
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
  }
	QuickView viewer;
	//viewer.SetNumberOfColumns(4);

	std::string description;
	description = argv[1];
	viewer.AddImage(reader->GetOutput(), true, description);
	viewer.Visualize();
 
  return EXIT_SUCCESS;
}