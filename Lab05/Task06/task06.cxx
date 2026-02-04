#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkExtractImageFilter.h"
 
 
#include "itkImage.h"
 
 
int
main(int argc, char ** argv)
{
  if (argc < 4)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " input3DImageFile output2DImageFile sliceNumber"
              << std::endl;
    return EXIT_FAILURE;
  }
  using InputPixelType = unsigned char;
  using OutputPixelType = unsigned char;
 
  using InputImageType = itk::Image<InputPixelType, 3>;
  using OutputImageType = itk::Image<OutputPixelType, 2>;
  using ReaderType = itk::ImageFileReader<InputImageType>;
  using WriterType = itk::ImageFileWriter<OutputImageType>;
  const char * inputFilename = argv[1];
  const char * outputFilename = argv[2];
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  using FilterType = itk::ExtractImageFilter<InputImageType, OutputImageType>;
  FilterType::Pointer filter = FilterType::New();
  filter->InPlaceOn();
  filter->SetDirectionCollapseToSubmatrix();
  reader->UpdateOutputInformation();
  InputImageType::RegionType inputRegion =
    reader->GetOutput()->GetLargestPossibleRegion();
  InputImageType::SizeType size = inputRegion.GetSize();
  size[2] = 0;
  InputImageType::IndexType start = inputRegion.GetIndex();
  const unsigned int        sliceNumber = std::stoi(argv[3]);
  start[2] = sliceNumber;
  InputImageType::RegionType desiredRegion;
  desiredRegion.SetSize(size);
  desiredRegion.SetIndex(start);
  filter->SetExtractionRegion(desiredRegion);
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