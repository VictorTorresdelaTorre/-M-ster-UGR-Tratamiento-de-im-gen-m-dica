/*
Autor: Víctor Torres de la Torre
Nombre: Task02
Descripción: Se pretende visualizar una imagen usando la biblioteca QuickView, además, 
visualizaremos también dicha imagen con el filtro shrink de ITK con un factor de 2.
*/

//Bibliotecas
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "QuickView.h"
#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

#include <iostream>
#include <sstream>
#include <itkShrinkImageFilter.h>

//Funcion que visualiza por pantalla un vector de imagenes pasadas como parámetro
void visualizar(int num_imagenes, itk::Image<unsigned char, 2>* imagenes[], std::string descripciones[])
{
  QuickView viewer;
  for(int i = 0; i < num_imagenes; i++){
    viewer.AddImage(imagenes[i],true,descripciones[i]);
  }
  viewer.Visualize();
}


int main( int argc, char* argv[])
{
  // Comprobamos que recibimos dos parámetros, el ejecutable y el nombre de la imagen a mostrar y tratar
  if ( argc != 2) {
    std::cerr << "USAGE:" << std::endl;
    std::cerr << argv[0] << " <Image Filename>" << std::endl;
    return EXIT_FAILURE;
  }


  // Guardamos la ruta de la imagen
  std::string imageFilename = argv[1];

  // Definimos el tipo de imagen
  typedef itk::Image<unsigned char,2> ImageType;

  // Definimos el reader de imagen correspondiente
  typedef itk::ImageFileReader<ImageType> ReaderType;

  // Instanciamos el reader y asignamos la ruta de la imagen que vamos a tratar
  ReaderType::Pointer reader= ReaderType::New();
  reader->SetFileName(argv[1]);

  //Incluimos el using para el filtro shrink de ITK e instanciamos un objeto de la clase
  using ShrinkImageFilterType = itk::ShrinkImageFilter<ImageType, ImageType>;
  ShrinkImageFilterType::Pointer shrinkFilter = ShrinkImageFilterType::New();
  ShrinkImageFilterType::Pointer shrinkFilter2 = ShrinkImageFilterType::New();

  //Usamos la imagen cargada en el reader para incluirlo en el filtro (PipeLine) y ponemos el factor a 2 y a 4
  shrinkFilter->SetInput(reader->GetOutput());
  shrinkFilter->SetShrinkFactor(0, 2);

  shrinkFilter2->SetInput(reader->GetOutput());
  shrinkFilter2->SetShrinkFactor(0, 4);

  // Actualizamos el PipeLine
  try {
    reader->Update();
  }
  catch( itk::ExceptionObject & error ){
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }
  
  //Visualizamos el vector de imágenes que pasamos como parámetro
  itk::Image<unsigned char, 2>* imagenes[3] = {reader->GetOutput(), shrinkFilter->GetOutput(), shrinkFilter2->GetOutput()};
  std::string descripciones[3] = {"Original", "Filtro con factor 2", "Filtro con factor 4"};
  visualizar(3, imagenes, descripciones);
  
  return EXIT_SUCCESS;
  
}
