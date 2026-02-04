#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkRegionOfInterestImageFilter.h"
 
#include "itkImage.h"
 
 
int
main(int argc, char ** argv)
{
  if (argc < 7)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    std::cerr << " startX startY sizeX sizeY" << std::endl;
    return EXIT_FAILURE;
  }
  using InputPixelType = unsigned char;
  using OutputPixelType = unsigned char;
  constexpr unsigned int Dimension = 2;
 
  using InputImageType = itk::Image<InputPixelType, Dimension>;
  using OutputImageType = itk::Image<OutputPixelType, Dimension>;
  using ReaderType = itk::ImageFileReader<InputImageType>;
  using WriterType = itk::ImageFileWriter<OutputImageType>;

  using FilterType = itk::RegionOfInterestImageFilter<InputImageType, OutputImageType>;
 
  FilterType::Pointer filter = FilterType::New();
  OutputImageType::IndexType start;
  start[0] = std::stoi(argv[3]);
  start[1] = std::stoi(argv[4]);
  OutputImageType::SizeType size;
  size[0] = std::stoi(argv[5]);
  size[1] = std::stoi(argv[6]);
  OutputImageType::RegionType desiredRegion;
  desiredRegion.SetSize(size);
  desiredRegion.SetIndex(start);

  filter->SetRegionOfInterest(desiredRegion);

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char * inputFilename = argv[1];
  const char * outputFilename = argv[2];
  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
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