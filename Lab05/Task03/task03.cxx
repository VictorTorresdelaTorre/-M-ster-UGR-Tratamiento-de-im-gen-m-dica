#include "itkRGBPixel.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "QuickView.h"

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
 
 
int
main(int argc, char ** argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputRGBImageFile"
              << std::endl;
    return EXIT_FAILURE;
  }
  using PixelType = itk::RGBPixel<unsigned char>;
  using ImageType = itk::Image<PixelType, 2>;

  using ReaderType = itk::ImageFileReader<ImageType>;
 
  ReaderType::Pointer reader = ReaderType::New();
 
 
  const char * inputFilename = argv[1];
  const char * outputFilename = argv[2];

  reader->SetFileName(inputFilename);
 
 
  ImageType::Pointer image = reader->GetOutput();

  QuickView viewer;

	std::string description;

	description = "Imagen RGB";
	viewer.AddImage(reader->GetOutput(), true, description);
  viewer.Visualize();

  return EXIT_SUCCESS;
}