#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkBMPImageIO.h"
 
int main(int argc, char ** argv)
{
  if (argc < 4)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0]
              << " firstSliceValue lastSliceValue  outputImageFile "
              << std::endl;
    return EXIT_FAILURE;
  }
  using PixelType = unsigned char;
  constexpr unsigned int Dimension = 3;
 
  using ImageType = itk::Image<PixelType, Dimension>;
  using ReaderType = itk::ImageSeriesReader<ImageType>;
  using WriterType = itk::ImageFileWriter<ImageType>;
 
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
 
 
  const unsigned int first = std::stoi(argv[1]);
  const unsigned int last = std::stoi(argv[2]);
 
  const char * outputFilename = argv[3];
  using NameGeneratorType = itk::NumericSeriesFileNames;
 
  NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
  nameGenerator->SetSeriesFormat("t%02d.bmp");
 
  nameGenerator->SetStartIndex(first);
  nameGenerator->SetEndIndex(last);
  nameGenerator->SetIncrementIndex(1);
  reader->SetImageIO(itk::BMPImageIO::New());
  reader->SetFileNames(nameGenerator->GetFileNames());



  writer->SetFileName(outputFilename);
  writer->SetInput(reader->GetOutput());
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