/*
Autor: Víctor Torres de la Torre
Nombre: Task01
Descripción: Primera toma de contacto con ITK, Hello World
*/

//Bibliotecas
#include "itkImage.h"
#include <iostream>

int main()
{
  using ImageType = itk::Image<unsigned short, 3>;

  ImageType::Pointer image = ImageType::New();

  std::cout << "ITK Hello World !" << std::endl;

  return EXIT_SUCCESS;
}

