#ifndef DICOM_HPP
#define DICOM_HPP

#include <iostream>
#include <filesystem>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include "dcmtk/ofstd/oflist.h"
#include "dcmtk/ofstd/ofstring.h"
#include "dcmtk/ofstd/ofstd.h"


std::vector<DicomImage*> loadAllDicom(std::string directory);
unsigned short*** preprocessDicom(std::vector<DicomImage*> images, std::vector<unsigned short>* sizes);
#endif // !DICOM_HPP