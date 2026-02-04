#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkExtractImageFilter.h"
 
 
#include "itkImage.h"

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
    std::cerr << argv[0] << " input3DImageFile"
              << std::endl;
    return EXIT_FAILURE;
  }

  using InputPixelType = unsigned char;
  using OutputPixelType = unsigned char;
 
  using InputImageType = itk::Image<InputPixelType, 3>;
  using OutputImageType = itk::Image<OutputPixelType, 2>;

  using ReaderType = itk::ImageFileReader<InputImageType>;

  const char * inputFilename = argv[1];

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);

  using FilterType = itk::ExtractImageFilter<InputImageType, OutputImageType>;
  FilterType::Pointer filter = FilterType::New();
  filter->InPlaceOn();
  filter->SetDirectionCollapseToSubmatrix();

  reader->UpdateOutputInformation();
  InputImageType::RegionType inputRegion =
  reader->GetOutput()->GetLargestPossibleRegion();

  InputImageType::SizeType size = inputRegion.GetSize();
  size[2] = 0;

 
  const InputImageType::SizeType & sizeDimensions = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  int sliceSize = sizeDimensions[2];

  QuickView viewer;
  std::string description;

  for(int i = 0; i < sliceSize; i++){
  InputImageType::IndexType start = inputRegion.GetIndex();

  start[2] = i;

  InputImageType::RegionType desiredRegion;
  desiredRegion.SetSize(size);
  desiredRegion.SetIndex(start);

  filter->SetExtractionRegion(desiredRegion);
  filter->SetInput(reader->GetOutput());

  description = "Slice "+ std::to_string(i);
  viewer.AddImage(filter->GetOutput(), true, description);

  }
  viewer.Visualize();
 
  return EXIT_SUCCESS;
}