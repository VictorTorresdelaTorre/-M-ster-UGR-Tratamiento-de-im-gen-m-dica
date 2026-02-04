// Software Guide : BeginCodeSnippet
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
// Software Guide : EndCodeSnippet
 
 
#include "itkImage.h"

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
 
int
main(int argc, char ** argv)
{
  // Verify the number of parameters in the command line
  if (argc < 3)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    return EXIT_FAILURE;
  }
 
 
  // Software Guide : BeginCodeSnippet
  using PixelType = unsigned char;
  constexpr unsigned int Dimension = 2;
  using ImageType = itk::Image<PixelType, Dimension>;
  // Software Guide : EndCodeSnippet
 
  // Software Guide : BeginCodeSnippet
  using ReaderType = itk::ImageFileReader<ImageType>;
  using WriterType = itk::ImageFileWriter<ImageType>;
  // Software Guide : EndCodeSnippet
 
  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  // Software Guide : EndCodeSnippet
 
 
  // Here we recover the file names from the command line arguments
  //
  const char * inputFilename = argv[1];
  const char * outputFilename = argv[2];
 
 
  // Software Guide : BeginCodeSnippet
  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  // Software Guide : EndCodeSnippet
 
  // Software Guide : BeginCodeSnippet
  writer->SetInput(reader->GetOutput());
  // Software Guide : EndCodeSnippet
 
 
  // Software Guide : BeginCodeSnippet
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
