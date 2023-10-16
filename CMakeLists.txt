cmake_minimum_required(VERSION 3.5.0)
project(dental_seg VERSION 0.1.0 LANGUAGES C CXX)

# set(
# 	ENV{DCMDICTPATH}	"/Users/mentxaka/Dcmtk/dcmtk-3.6.7-install/usr/local/share/dcmtk/dicom.dic"
# )

# Include directories for external libraries
include_directories(
	# "/Users/mentxaka/Dcmtk/dcmtk-3.6.7-install/usr/local/include"
	"/Users/mentxaka/Nifti/nifti-install/usr/local/include"
)

 

# Include link directories for external libraries
# link_directories(
# 	# "/Users/mentxaka/Dcmtk/dcmtk-3.6.7-install/usr/local/lib"
# 	"/Users/mentxaka/Nifti/nifti-install/usr/local/lib"
# )

 

# Your executable
add_executable(dental_seg main.cpp)

 

# Link the libraries
target_link_libraries(dental_seg PRIVATE
	# dcmdata
	# ofstd
	# oflog
	# iconv
	# dcmimgle
	/Users/mentxaka/Nifti/nifti-install/usr/local/lib/libnifti2.a
	/Users/mentxaka/Nifti/nifti-install/usr/local/lib/libnifticdf.a
	/Users/mentxaka/Nifti/nifti-install/usr/local/lib/libniftiio.a
	/Users/mentxaka/Nifti/nifti-install/usr/local/lib/libznz.a
)

