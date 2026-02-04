#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkCastImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkNormalizeImageFilter.h"
#include "QuickView.h"
#include <iostream>
#include <cmath>

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

int
main(int argc, char* argv[])
{
  // Check arguments
  if ( argc != 2) {
    std::cerr << "USAGE:\n " << std::endl;
    std::cerr << argv[0] << " <ImageFileName>" << std::endl;
    return EXIT_FAILURE;
  }
  
  // Define the types of the input Pixel and output Pixel
  typedef unsigned char InputPixelType;
  typedef float OutputPixelType;

  // Define the types of the input and output images
  typedef itk::Image<InputPixelType,2> InputImageType;
  typedef itk::Image<OutputPixelType,2> OutputImageType;
  
  // Define the types for the five filters we are going to use, reader filter included.
  typedef itk::ImageFileReader< InputImageType >  ReaderType;
  typedef itk::CastImageFilter< InputImageType, OutputImageType >  CastFilterType;
  typedef itk::RescaleIntensityImageFilter< InputImageType, OutputImageType >  RescaleFilterType;
  typedef itk::ShiftScaleImageFilter< InputImageType, OutputImageType >  ShiftScaleFilterType;
  typedef itk::NormalizeImageFilter< InputImageType, OutputImageType >  NormalizeFilterType;
  typedef itk::ImageRegionConstIterator<OutputImageType> ConstIterator;

  // Instantiate and set the reader filter
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  // Instantiate the cast filter
  CastFilterType::Pointer castFilter = CastFilterType::New();

  // Instantiate and set the rescale filter
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetOutputMinimum(10);
  rescaleFilter->SetOutputMaximum(100);
  

  // Instantiate and set the shift scale filter
  ShiftScaleFilterType::Pointer shiftFilter = ShiftScaleFilterType::New();
  shiftFilter->SetScale( 1.2 );
  shiftFilter->SetShift( 25 );

  // Instantiate the normalize filter
  NormalizeFilterType::Pointer normalizeFilter = NormalizeFilterType::New();

  /* Connect the pipeline.
     Notice that we have four sinks, hence we have to update each one to
     trigger the pipeline.
  */
  castFilter->SetInput(reader->GetOutput());
  shiftFilter->SetInput(reader->GetOutput());
  rescaleFilter->SetInput(reader->GetOutput());
  normalizeFilter->SetInput(reader->GetOutput());
  
  // Send an update message to each filter in order to the pipeline executes.
  try {
    castFilter->Update();
  }
  catch (itk::ExceptionObject & error) {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }

  try {
    shiftFilter->Update();
  }
  catch ( itk::ExceptionObject & error ) {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }

  try {
    rescaleFilter->Update();
  }
  catch ( itk::ExceptionObject & error ){
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }

  try {
    normalizeFilter->Update();
  }
  catch ( itk::ExceptionObject & error ){
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }

  OutputImageType::Pointer inputImage1 = castFilter->GetOutput();
  ConstIterator in(inputImage1,inputImage1->GetRequestedRegion());

  OutputImageType::Pointer inputImage2 = normalizeFilter->GetOutput();
  ConstIterator out(inputImage2,inputImage2->GetRequestedRegion());

  float sumPixel = 0;
  int count = 0;
  for( in.GoToBegin(),out.GoToBegin(); !in.IsAtEnd(); ++in ,++ out ) {
    float inputValue = in.Get();
    float outputValue = out.Get();
    //std::cout << "in -> " << inputValue <<" out -> "<< outputValue << std::endl ;
    sumPixel += in.Get(); 
    count += 1;
  }
  float media = sumPixel/count, varianza = 0.0;
  
  for( in.GoToBegin(),out.GoToBegin(); !in.IsAtEnd(); ++in ,++ out ) {
    
    float rango = 0.0;
    rango = pow(in.Get() - media,2);
    varianza += rango;

    float inputValue = in.Get();
    float outputValue = out.Get();
  }
  varianza /= count;

  for( in.GoToBegin(),out.GoToBegin(); !in.IsAtEnd(); ++in ,++ out ) {
    float inputValue = in.Get();
    float outputValue = out.Get();
    std::cout << "in -> " << inputValue <<" out -> "<< outputValue <<" MiOut-> " << (in.Get() - media)/sqrt(varianza)<<std::endl ;
  }
  std::cout << "media -> "<< sumPixel/count << std::endl ;
  std::cout << "varianza -> "<< varianza << std::endl ;

  // Use QuickView to show the resulting images
  QuickView viewer;

  // Visualise the image read from the file
  viewer.AddImage(reader->GetOutput(),true,"Original");
  //viewer.AddImage(shiftFilter->GetOutput(),true,"shiftFilter");
  //viewer.AddImage(rescaleFilter->GetOutput(),true,"rescaleFilter");
  viewer.AddImage(normalizeFilter->GetOutput(),true,"normalizeFilter");

  viewer.Visualize();

  return EXIT_SUCCESS;

}
