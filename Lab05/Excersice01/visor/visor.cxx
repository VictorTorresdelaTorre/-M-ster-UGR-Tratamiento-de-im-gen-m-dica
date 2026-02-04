#include "itkImageFileReader.h"

#include "QuickView.h"

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

using namespace std;

int main (int argc, char * argv[])
{
	if (argc < 3) {
		cerr << "Usage: " << endl;
		cerr << argv[0] << "   Image1 Image2 " << endl;
		return EXIT_FAILURE;
	}

using ImageType = itk::Image<unsigned char,2>;
using ReaderType = itk::ImageFileReader<ImageType>;

ReaderType::Pointer reader1= ReaderType::New(); 
reader1->SetFileName(argv[1]);

 try {
    reader1->Update();
  }
  catch( itk::ExceptionObject & error ){
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }

ReaderType::Pointer reader2= ReaderType::New();
reader2->SetFileName(argv[2]);

 try {
    reader2->Update();
  }
  catch( itk::ExceptionObject & error ){
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }

	QuickView viewer;
	//viewer.SetNumberOfColumns(4);

	string description;

	description = argv[1];
	viewer.AddImage(reader1->GetOutput(), true, description);
	description = argv[2];
	viewer.AddImage(reader2->GetOutput(), true, description);
	viewer.Visualize();



  
  return EXIT_SUCCESS;
}

