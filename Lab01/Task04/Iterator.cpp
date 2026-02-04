/*
Autor: Víctor Torres de la Torre
Nombre: Task04
Descripción: Se pretende entender los itradores de ITK, además, mostraremos la imgen que se crea.
*/

//Bibliotecas
#include <iostream>
#include <sstream>
#include "itkImage.h"
#include <itkRandomImageSource.h>
#include "QuickView.h"
#include "itkImageFileReader.h"
#include "vtkAutoInit.h"
#include "itkViewImage.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

int main( int argc, char* argv[])
{
    //  Comprobamos que no recibimos parámetros
    if ( argc != 1) {
    std::cerr << "USAGE:" << std::endl;
    std::cerr << "The execution of this program does not receive parameters" << std::endl;
    return EXIT_FAILURE;
    }

    // Instanciamos una imagen 2D
    typedef float PixelType ;
    typedef itk::Image<PixelType,2> ImageType2D;

    // declaramos un puntero aleatorio 
    itk::RandomImageSource<ImageType2D>::Pointer random ;

    // Creamos una nueva imagen 2D
    random = itk::RandomImageSource<ImageType2D>::New();

    // Establecemos los valores máximo y mino para pixel
    random->SetMin(0.0) ;
    random->SetMax(1.0) ;

    // Definimos el tamaño de imagen
    ImageType2D::SizeValueType size[2];
    size[0]=200;
    size[1]= 200;
    random->SetSize(size);

    // Actualizamos el Pipeline
    random->Update() ;

    // Declaramos un puntero para apuntar a un nuevo objeto de tipo ImageType2D
    ImageType2D::Pointer outputImage2D = ImageType2D::New();

    // Definimos el tamaño de imagen
    ImageType2D::IndexType region2DIndex;
    region2DIndex[0] = 0; 
    region2DIndex[1] = 0; 

    // Definimos el número de pixel por dimensión
    ImageType2D::SizeType region2DSize;
    region2DSize[0] = 200;
    region2DSize[1] = 200;

    // Creamos la region
    ImageType2D::RegionType region2D;
    region2D.SetIndex(region2DIndex);
    region2D.SetSize(region2DSize);

    // Damos una región válida a la imagen y asignamos en memoria
    outputImage2D->SetRegions(region2D);
    outputImage2D->Allocate();

    // Declaramos dos iteradores 
    typedef itk::ImageRegionConstIterator<ImageType2D> ConstIterator2DType ;
    typedef itk::ImageRegionIterator<ImageType2D> Iterator2DType ;

    // apuntamos un puntero a la salida de un filtro aleatorio
    ImageType2D::Pointer inputImage2D = random->GetOutput();

    // Definimos los iteradores sobre el input y el output
    ConstIterator2DType in(inputImage2D,inputImage2D->GetRequestedRegion());
    Iterator2DType out(outputImage2D,inputImage2D->GetRequestedRegion());

    for( in.GoToBegin(),out.GoToBegin(); !in.IsAtEnd(); ++in ,++ out ) {
    out.Set(in.Get()*in.Get()) ;
    // Imprimimos los valores del punto actual en input y output
    float inputValue = in.Get();
    float outputValue = out.Get();
    std :: cout << inputValue << "\t" << outputValue << std :: endl ;
    }

    //Visualizamos el input,el output y random por pantalla, input y random deben imprimir lo mismo
    QuickView viewer;

    viewer.AddImage<ImageType2D>(outputImage2D, true, "output");
    viewer.AddImage<ImageType2D>(inputImage2D,true, "input");
    viewer.AddImage(random->GetOutput(), true, "random");

    viewer.Visualize();

    return EXIT_SUCCESS ;
}