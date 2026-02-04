
cmake_minimum_required(VERSION 2.8)
 
project(Segmentacion)
 
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

if (ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})
else()
  find_package(ItkVtkGlue REQUIRED)
  include(${ItkVtkGlue_USE_FILE})
  set(Glue ItkVtkGlue)
endif()
 
add_executable(Segmentacion Segmentacion.cpp)
target_link_libraries(Segmentacion
  ${Glue}  ${VTK_LIBRARIES} ${ITK_LIBRARIES})
