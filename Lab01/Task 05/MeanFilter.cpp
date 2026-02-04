/*
Autor: Víctor Torres de la Torre
Nombre: Task05
Descripción: Se pretende aplicar y visualizar un filtro de media a la imagen pasada por parámetro, recibiendo también el valor de kernel
*/

//Bibliotecas
#include "itkMeanImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "QuickView.h"

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

int
main(int argc, char * argv[])
{
  //  Comprobamos que recibimos tres parámetros, el ejecutable, el nombre de la imagen a mostrar y el valor de kernel
  if (argc != 3)
  {
      std::cerr << "Usage: " << std::endl;
      std::cerr << argv[0] << " <InputImageFile> <kernel>" << std::endl;
      return EXIT_FAILURE;
  }

  //Definimos el número de dimensiones, y guardamos la ruta de la imagen a tratar y el valor de kernel
  const unsigned int Dimension = 2;
  const char * inputFileName = argv[1];
  const int    kernelValue = std::stoi(argv[2]);

  //Definimos el tipo de imagen 
  using ImageType = itk::Image<unsigned char, Dimension>;

  //Definimos el reader, creamos uno nuevo y apuntamos a la imagen pasada como parámetro
  using ReaderType = itk::ImageFileReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);

  //Definimos el tipo de filtro MeanFilter y creamos uno nuevo
  using FilterType = itk::MeanImageFilter<ImageType, ImageType>;
  FilterType::Pointer meanFilter = FilterType::New();

  //Asignamos el valor de kernel
  FilterType::InputSizeType kernel;
  kernel.Fill(kernelValue);

  //Creamos la imagen filtrada
  meanFilter->SetRadius(kernel);
  meanFilter->SetInput(reader->GetOutput());

  //Visualizamos ambas imágenes
  QuickView viewer;

  std::stringstream strs;
  viewer.AddImage(reader->GetOutput(),true,"Original");
  viewer.AddImage(meanFilter->GetOutput(),true,"MeanFilter");

  viewer.Visualize();

  return EXIT_SUCCESS;
}