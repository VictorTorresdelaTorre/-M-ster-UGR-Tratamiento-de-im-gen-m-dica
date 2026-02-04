# Lab01 - Task02

In this task we are going to learn how to read an image from a file and then show it using ItkVtkGlue
capabilities. Our code, showImage.cpp for instance, reads and image passed by argument and
displays it using QuickView 2 . We will replicate the structure of this code many times in order to show
the results of our pipelines. Read the code from Image2.cxx to get a clue of how to programming
showImage.cpp , generate the executable and use it to read and display the image FatMRISlice.png
included in the data directory. Remember to create a directory for Task 2, task02 , and inside it a
build directory, next modify adequately the file CMakeLists.txt for including the name of the new
executable and source code file.
Next, try to read BrainProtonDensity3Slices.raw . You get an error, especifically an exception, be-
cause ITK tried to create a reader object for the .raw image file format among a list of known formats—
ITK shows you the list!. Try out to run your program again with BrainProtonDensity3Slices.mha .
Now it works fine because this is a known file format for ITK. In a later session we will put on deeper
with reading and writing different image file formats.
Once you have read and executed showImage.cpp try to write a program that reads an image from
a file and Shrinks the image in a factor of 2, displaying both the read and the shrunk image into a
viewer. For understanding itk::ShrinkImageFilter seek in the documentation of ITK at itk.org.
