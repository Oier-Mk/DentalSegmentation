# Dental Segmentation
This repository contains the code for the pre and post processing of the dental segmentation project. The code is written in C++. The code provides the following functionalities:
- Preprocessing of the DICOM files into short 3D arrays to feed the neural network.
- Preprocessing of the ground truth masks (NIIFTI format) into short 3D arrays to feed the neural network.
- Postprocessing of the neural network output (short 3D arrays) into the final segmentation masks in NIFTI format.
- Utils for the pre and post processing of the data. e.g. Cropping the data.

## How to use
The code is compiled with CMake (recommended to build it in /build ). The executable dental_seg is created. The executable dental_seg has the following parameters:
- -d, --dicom: Path to the directory containing the DICOM files.
- -n, --nifti: Path to the NIFTI file containing the ground truth mask.
- -m, --model: Path to the .H5 file containing the neural network model.
- -o, --output: Path to the output NIFTI file containing the segmentation mask.
An example on how to use the executable dental_seg is the following:
```
build/dental_seg  -d data/Etiquetado_17682_20230302_152135 -n data/Etiquetado_17682_20230302_152135/Untitled.nii.gz -m model/unet.h5 -o data/res/135.nii.gz
```

## Required libraries
The main 3 libraries used in this project are DMCTK for the DICOM files, nifticlib for the NIFTI files and Tensorflow C for using the AI model. The code is written in C++ and compiled with CMake. The libraries can be found in the following links:
- [DMCTK](https://dicom.offis.de/dcmtk.php.en)
- [nifticlib](https://github.com/NIFTI-Imaging/nifti_clib)
- [Tensorflow](https://www.tensorflow.org/install/lang_c)
  
The 2 first libraries must be compiled and installed in the system before compiling the code of the dental segmentation project.

## How to compile
The code is compiled with CMake. A similar CMakeLists.txt file to this is neccessary to compile the code:
```
cmake_minimum_required(VERSION 3.5.0)
project(dental_seg VERSION 0.1.0 LANGUAGES C CXX)

file( GLOB  SOURCES
	"scripts/*.cpp"
	"scripts/*.hpp")

# Include directories for external libraries
include_directories(
	"installation_dir_of_dcmtk_lib/include"
	"installation_dir_of_nifti_lib/include"
	"installation_dir_of_tensorflow_lib/include"
)

# Include link directories for external libraries
link_directories(
	"installation_dir_of_dcmtk_lib/lib"
	"installation_dir_of_nifti_lib/lib"
	"installation_dir_of_tensorflow_lib/lib"
)


# Your executable
add_executable(dental_seg main.cpp ${SOURCES})


# Link the libraries
target_link_libraries(dental_seg PRIVATE
	dcmdata
	ofstd
	oflog
	iconv
	dcmimgle
	niftiio
	tensorflow
)
```
## Extra: Python files
There are some extra python files that are not necessary but provide some insight during development.

## Fixes & TO - DO
- [ ] Fix the std:: error. The code seems to not find the std namespace in some installations of the project.
- [ ] Fix the issue with the Tensorflow library. The Tensorflow library is not working properly on the MACOS system due to incompatibility with newer versions of the MACOS system. (Try using a Docker container with a Linux system).
- [ ] TO DO: Test thoroughly the Tensorflow H5 model loading and inference.

