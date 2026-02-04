/*
Autor: Víctor Torres de la Torre
Nombre: Task03
Descripción: Se pretende mostrar los slices de un raw, además, imprimiremos la metainformación de la imagen,
*/

//Bibliotecas
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkExtractImageFilter.h"
#include "QuickView.h"
#include "vtkAutoInit.h" 
#include "itkViewImage.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
#include <iostream>
#include <sstream>
#include <itkShrinkImageFilter.h>


int main( int argc, char* argv[])
{
  //  Comprobamos que recibimos dos parámetros, el ejecutable y el nombre de la imagen a mostrar y tratar
  if ( argc != 2) {
    std::cerr << "USAGE:" << std::endl;
    std::cerr << argv[0] << " <Image3D Filename>" << std::endl;
    return EXIT_FAILURE;
  }

  // Guardamos la ruta de la imagen
  std::string imageFilename = argv[1];
  
  // Definimos los tipos de imagenes
  using ImageType3D = itk::Image<unsigned char,3>;
  using ImageType2D = itk::Image<unsigned char,2>;
  
  // Definimos el reader de imagen correspondiente
  using ReaderType = itk::ImageFileReader<ImageType3D>;

  // Definimos el extractor 
  using FilterType = itk::ExtractImageFilter<ImageType3D,ImageType2D>;
  
  // Instanciamos un reader
  ReaderType::Pointer reader= ReaderType::New();
  reader->SetFileName(argv[1]);

  // Instanciamos y definimos el filter
  FilterType::Pointer filter = FilterType::New();
  filter->InPlaceOn();
  filter->SetDirectionCollapseToSubmatrix();
  
  reader->UpdateOutputInformation();
  ImageType3D::RegionType inputRegion = reader->GetOutput()->GetLargestPossibleRegion();

  //  colapsamos en Z
  ImageType3D::SizeType size = inputRegion.GetSize();
  size[2] = 0;

  // Indicamos el inicio para visualizar 
  ImageType3D::IndexType start = inputRegion.GetIndex();
  start[2] = 0;

  // Establecemos la región que uqeremos visualizar de la imagen 3D
  ImageType3D::RegionType desiredRegion;
  desiredRegion.SetSize(size);
  desiredRegion.SetIndex(start);

  filter->SetExtractionRegion( desiredRegion );
  filter->SetInput( reader->GetOutput() );
  
  //  Actualizamos el PipeLine
  try {
    filter->Update();
  }
  catch( itk::ExceptionObject & error ){
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }

  //Declaramos las variables que contendrán los metadatos, Spacin, Origin,Direction, número de dimensiones y el tamaño de las mismas
  const ImageType3D::SpacingType & sp = reader->GetOutput()->GetSpacing();
  const ImageType3D::PointType & origin = reader->GetOutput()->GetOrigin();
  const ImageType3D::DirectionType & direct = reader->GetOutput()->GetDirection();
  const ImageType3D::ImageDimensionType & nDimensions = reader->GetOutput()->GetImageDimension();
  const ImageType3D::SizeType & sizeDimensions = reader->GetOutput()->GetLargestPossibleRegion().GetSize();


  //Imprimimos los metadatos
  std::cout << "Spacing = ";
  std::cout << sp[0] << ", " << sp[1] << ", " << sp[2] << std::endl;

  std::cout << "Origin = ";
  std::cout << origin[0] << ", " << origin[1] << ", " << origin[2]<< std::endl;

  std::cout << "Direction = " << std::endl;
  std::cout << direct;

  std::cout << "Number of dimensions = ";
  std::cout << nDimensions << std::endl;
  
  std::cout << "Size of dimensions = ";
  std::cout << sizeDimensions << std::endl;


  // Visulizamos los slices
  QuickView viewer;
  
  std::stringstream strs;
  for (int i = 0; i < sizeDimensions[2]; i++) {
    strs << imageFilename << "\nSlide " << i;
    viewer.AddImage(filter->GetOutput(),true,strs.str());
    strs.str("");
    strs.clear();
  }
  viewer.Visualize();


  
  return EXIT_SUCCESS;
}
