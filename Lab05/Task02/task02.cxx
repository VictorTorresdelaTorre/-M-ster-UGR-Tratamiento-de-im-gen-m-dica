#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVTKImageIO.h"
 
 
#include "itkImage.h"
#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
 
 
int
main(int argc, char ** argv)
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    return EXIT_FAILURE;
  }
  using PixelType = unsigned char;
  constexpr unsigned int Dimension = 2;
  using ImageType = itk::Image<PixelType, Dimension>;
  using ReaderType = itk::ImageFileReader<ImageType>;
  using WriterType = itk::ImageFileWriter<ImageType>;
  using ImageIOType = itk::VTKImageIO;
  ReaderType::Pointer  reader = ReaderType::New();
  WriterType::Pointer  writer = WriterType::New();
  ImageIOType::Pointer vtkIO = ImageIOType::New();
  const char * inputFilename = argv[1];
  const char * outputFilename = argv[2];
  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  writer->SetInput(reader->GetOutput());
  vtkIO->SetFileTypeToASCII();
  writer->SetImageIO(vtkIO);
  try
  {
    writer->Update();
  }
  catch (const itk::ExceptionObject & err)
  {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}