# Lab01 - Task 03

In this task we will write a program to extract information from a image that is provided as an
argument and then we will display it. Concretely, we will show the following data: image size for each
dimension, origin, spacing, and direction matrix. Look at the code in image4.cxx to know how to write
the program. We will be working with the following images both as arguments to our program and for
editing them:
BrainProtonDensity3Slices.mha
BrainProtonDensity3Slices.raw
BrainProtonDensitySliceBorder20.mhd
BrainProtonDensitySliceBorder20.raw
BrainProtonDensitySliceBorder20DirectionPlus30.mhd
BrainProtonDensitySliceBorder20.zraw
BrainProtonDensitySliceBorder20DirectionPlus30.nhdr
BrainProtonDensitySliceBorder20DirectionPlus30.raw
The images above have been extracted from the Data directory which is inside the ITK
installation directory. You can read the files BrainProtonDensitySliceBorder20.mhd and
BrainProtonDensitySliceBorder20DirectionPlus30.mhd in a text editor, gedit for instance. This
two files are metadata files, which describe information associated with the real image data.
For example, in BrainProtonDensitySliceBorder20.mhd you can see “ NDims = 2 ” data field
that represents the image dimension, the “ ElementSpacing = 1 1 ” that shows the spacing in each
dimension, the “ DimSize = 221 257 ” that holds the number of pixels in each dimension, and the “
ElementType = MET_UCHAR ” that represent the data type used to save the pixel values in the data file.
OK, we have two extra fields particularly reserved to hold information about the file that actually saves
the pixel values: “ CompressedData = False ” and “ ElementDataFile = BrainProtonDensitySliceBorder20.raw”
The BrainProtonDensitySliceBorder20DirectionPlus30.nhdr is another kind of metadata file (
.nhdr ) commonly used in medical imaging. You can read it using gedit and compare the metadata
information with the .mhd format. Try out the program you have written with the above images and
compare the information it display with the information saved in .mha , .mhd and .nhdr formats.
