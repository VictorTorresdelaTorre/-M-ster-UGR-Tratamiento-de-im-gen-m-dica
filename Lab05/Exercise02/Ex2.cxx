#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
 
int
main(int argc, char * argv[])
{
 
  if (argc < 2)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " DicomDirectory  outputFileName"
              << std::endl;
    return EXIT_FAILURE;
  }
  using PixelType = unsigned char;
  constexpr unsigned int Dimension = 3;
 
  using ImageType = itk::Image<PixelType, Dimension>;
  using ReaderType = itk::ImageSeriesReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();
  using ImageIOType = itk::GDCMImageIO;
  ImageIOType::Pointer dicomIO = ImageIOType::New();
 
  reader->SetImageIO(dicomIO);
  using NamesGeneratorType = itk::GDCMSeriesFileNames;
  NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
 
  nameGenerator->SetUseSeriesDetails(true);
  nameGenerator->AddSeriesRestriction("0008|0021");
 
  nameGenerator->SetDirectory(argv[1]);
 
 
  try
  {
    std::cout << std::endl << "The directory: " << std::endl;
    std::cout << std::endl << argv[1] << std::endl << std::endl;
    std::cout << "Contains the following DICOM Series: ";
    std::cout << std::endl << std::endl;

    using SeriesIdContainer = std::vector<std::string>;
 
    const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
 
    auto seriesItr = seriesUID.begin();
    auto seriesEnd = seriesUID.end();
    while (seriesItr != seriesEnd)
    {
      std::cout << seriesItr->c_str() << std::endl;
      ++seriesItr;
    }
    std::string seriesIdentifier;
 
    if (argc > 3) 
    {
      seriesIdentifier = argv[3];
    }
    else
    {
      seriesIdentifier = seriesUID.begin()->c_str();
    }
 
 
    std::cout << std::endl << std::endl;
    std::cout << "Now reading series: " << std::endl << std::endl;
    std::cout << seriesIdentifier << std::endl;
    std::cout << std::endl << std::endl;
    using FileNamesContainer = std::vector<std::string>;
    FileNamesContainer fileNames;
 
    fileNames = nameGenerator->GetFileNames(seriesIdentifier);
    reader->SetFileNames(fileNames);
    try
    {
      reader->Update();
    }
    catch (const itk::ExceptionObject & ex)
    {
      std::cout << ex << std::endl;
      return EXIT_FAILURE;
    }
    using WriterType = itk::ImageFileWriter<ImageType>;
    WriterType::Pointer writer = WriterType::New();
 
    writer->SetFileName(argv[2]);
 
    writer->SetInput(reader->GetOutput());
 
    std::cout << "Writing the image as " << std::endl << std::endl;
    std::cout << argv[2] << std::endl << std::endl;
    try
    {
      writer->Update();
    }
    catch (const itk::ExceptionObject & ex)
    {
      std::cout << ex << std::endl;
      return EXIT_FAILURE;
    }
  }
  catch (const itk::ExceptionObject & ex)
  {
    std::cout << ex << std::endl;
    return EXIT_FAILURE;
  }
 
  return EXIT_SUCCESS;
}