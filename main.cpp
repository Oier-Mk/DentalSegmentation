#include <iostream>
// #include <dcmtk/dcmdata/dctk.h>
// #include <dcmtk/config/osconfig.h>
// #include <dcmtk/dcmimgle/dcmimage.h>
#include <nifti/nifti1_io.h>

// std::vector<DicomImage*> loadAllDicom(std::string fileDirectory)
// {
//     OFString path = fileDirectory.c_str();
//     OFList<OFString> files;
//     OFStandard::searchDirectoryRecursively(path, files, "*.dcm");

//     std::vector<DicomImage*> images;
//     for (OFListIterator(OFString) it = files.begin(); it != files.end(); ++it)
//     {
//         DicomImage *image = new DicomImage(it->c_str());
//         if (image != NULL){
//             if (image->getStatus() == EIS_Normal)
//             {
//                 images.push_back(image);
//                 std::cout << "Loaded " << it->c_str() << " succesfuly" << std::endl;
//             }
//             else
//             {
//                 std::cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << std::endl;
//             }
//         }
//     }
//     return images;
// }

nifti_image loadNifti(std::string filename)
{
    // Leer el archivo .nii.gz usando NIfTI
    nifti_image *nii = nifti_image_read(filename, 1);
    if (nii != NULL)
    {
        // Obtener el número de dimensiones de la imagen
        int ndim = nii->ndim;
        // Obtener el tamaño de cada dimensión de la imagen
        int *dim = nii->dim;
        // Obtener el tipo de datos de la imagen
        int datatype = nii->datatype;
        // Obtener el puntero a los datos de la imagen
        void *data = nii->data;
        if (data != NULL)
        {
            // Mostrar algunas propiedades de la imagen
            std::cout << "Dimensiones: " << ndim << std::endl;
            std::cout << "Tamaño: ";
            for (int i = 0; i < ndim; i++)
            {std::cout << dim[i] << " ";
            }
            std::cout << std::endl;
            std::cout << "Tipo de datos: " << datatype << std::endl;
            
            return nii;
        }
    }
    return NULL;
}



int main()
{
    // std::string directory = "data/Etiquetado_17682_20230302_152135";

    // //Load all the dcm files
    // std::vector<DicomImage*> images = loadAllDicom(directory);

    // //Print the shape of the vector to check is loaded successfuly
    // std::cout << "Shape of the vector: " << images.size() << std::endl;
    // //shape of the first image (rows,columns)
    // std::cout << "Shape of the first image: " << images[0]->getHeight() << "," << images[0]->getWidth() << std::endl;

    // //Free the image vector
    // for (int i = 0; i < images.size(); i++)
    // {
    //     delete images[i];
    // }

    //Get the labels (is the only .nii.gz file in the directory selected)
    std::string lbl_filename = "data/Etiquetado_17682_20230302_152135/Untitled.nii.gz";
    nifti_image lbl = loadNifti(lbl_filename);

    //Free the memory
    nifti_image_free(lbl);

    std::cout << "Hello World!";
    return 0;
}