#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkMeanImageFilter.h"
#include "itkMedianImageFilter.h"
#include "QuickView.h"
#include <iostream>
#include <sstream>

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);



int main
(int argc, char* argv[])
{
  // Check command line arguments
  if( argc < 2 || argc > 3) {
    std::cerr << "USAGE: " << std::endl;
    std::cerr << argv[0] << " <ImageFileName> [kernelSupport=1]" << std::endl;
    return EXIT_FAILURE;
  }

  // Parse command line arguments
  std::string inputFilename = argv[1];
  
  // Define the pixel type
  typedef unsigned char PixelType;
    
  // Define the types of the input and output images
  typedef itk::Image<PixelType,2> InputImageType;
  typedef itk::Image<PixelType,2> OutputImageType;
  
  // Define the types of the filters we are going to use
  typedef itk::ImageFileReader<InputImageType> ReaderType;
  typedef itk::MeanImageFilter<InputImageType,OutputImageType> MeanFilterType;
  typedef itk::MedianImageFilter<InputImageType,OutputImageType> MedianFilterType;

  // Instantiate and setup the reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename.c_str());
  
  // Instantiate and setup a mean filter
  MeanFilterType::Pointer meanFilter = MeanFilterType::New();

  //// Instantiate a SizeType for setting the kernel support for mean filter 
  InputImageType::SizeType support;

  //// Set kernel support if argc==3 to argv[2], else default values.
  if(argc == 3) {
    support[0]= support[1]= atoi(argv[2]);
  } else {    
    support[0] = 2; // radius along x
    support[1] = 2; // radius along y
  }
  meanFilter->SetRadius(support);

  // Connect the filter to the reader
  meanFilter->SetInput(reader->GetOutput());

  // Instantiate and setup a median filter
  MedianFilterType::Pointer medianFilter = MedianFilterType::New();

  // Median filter uses the same kernel support than mean filter above
  medianFilter->SetRadius(support);

  // Connect the filter to the reader
  medianFilter->SetInput(reader->GetOutput());

  // Send an update message to each sink filter in order to the pipeline executes.
  try {
    meanFilter->Update();
  }
  catch ( itk::ExceptionObject & error ){
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }
  
  try {
    medianFilter->Update();
  }
  catch ( itk::ExceptionObject & error ){
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }
  
  // Use QuickView to show the different images
  QuickView viewer;

  // Visualise the image read from the file and display the filename
  viewer.AddImage<InputImageType>(reader->GetOutput(), true, argv[1]);
  
  std::ostringstream oss;
  oss << "Mean filter\n kernel size=" << support[0] << "x" << support[1];
  viewer.AddImage<OutputImageType>(meanFilter->GetOutput(), true, oss.str().c_str());

  oss.str("");
  oss << "Median filter\n kernel size=" << support[0] << "x" << support[1];
  viewer.AddImage<OutputImageType>(medianFilter->GetOutput(), true, oss.str().c_str());

  viewer.Visualize();

  return EXIT_SUCCESS;

}

