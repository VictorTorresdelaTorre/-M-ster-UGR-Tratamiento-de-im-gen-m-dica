#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkGDCMImageIO.h"
 
#include <list>
#include <fstream>
 
int
main(int argc, char * argv[])
{
 
  if (argc < 3)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " DicomImage OutputImage ";
    return EXIT_FAILURE;
  }
  using InputPixelType = signed short;
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
  using WritePixelType = unsigned char;
 
  using WriteImageType = itk::Image<WritePixelType, 2>;
  using RescaleFilterType = itk::RescaleIntensityImageFilter<InputImageType, WriteImageType>;
 
  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
 
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  using Writer2Type = itk::ImageFileWriter<WriteImageType>;
 
  Writer2Type::Pointer writer2 = Writer2Type::New();
 
  writer2->SetFileName(argv[2]);
 
  rescaler->SetInput(reader->GetOutput());
  writer2->SetInput(rescaler->GetOutput());
 
  try
  {
    writer2->Update();
  }
  catch (const itk::ExceptionObject & e)
  {
    std::cerr << "exception in file writer " << std::endl;
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
  }
 
  return EXIT_SUCCESS;
}
