#include <iostream>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/config/osconfig.h>

void loadDicomFile(std::string filename)
{
    DcmFileFormat fileformat;
    OFCondition status = fileformat.loadFile(filename.c_str());

    if (status.good())
    {
        // File loaded successfully

        // Access the DICOM dataset
        DcmDataset *dataset = fileformat.getDataset();

        // Print something related to the dataset to check if it is loaded correctly
        OFString patientName;
        if (dataset->findAndGetOFString(DCM_PatientName, patientName).good())
        {
            std::cout << "Patient Name: " << patientName << std::endl;
        }
        else
        {
            std::cerr << "Error: cannot access Patient Name!" << std::endl;
        }

        // TODO: Process the DICOM dataset as needed
    }
    else
    {
        // Error loading the file
        std::cerr << "Error: Unable to load DICOM file " << filename << std::endl;
    }
}

int main()
{
    std::string filename = "data/Etiquetado_17682_20230302_152135/17682_20230302_152135_0.dcm";
    loadDicomFile(filename);

    std::cout << "Hello World!";
    return 0;
}
