#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkViewImage.h"
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
    std::cerr << argv[0] << " input3DImageFile" << std::endl;
  }

  const char * inputFilename = argv[1];

  using PixelType = float;
  constexpr unsigned int Dimension = 3;
  using ImageType = itk::Image<PixelType, Dimension>;
  using ReaderType = itk::ImageFileReader<ImageType>;
  auto reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();
  itk::ViewImage<ImageType>::View(reader->GetOutput());
  
  return EXIT_FAILURE;
}
