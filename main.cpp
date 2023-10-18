#include <iostream>
#include "scripts/dicom.hpp"
#include "scripts/nifti.hpp"
#include "scripts/n_net.hpp"

// //Function that plots a slice of the 3d array on an image
// void plotSlice(unsigned short*** image3d, int slice, std::string filename) {
//     // Tamaño del arreglo tridimensional
//     int size_x = 764;
//     int size_y = 764;
//     int size_z = 603;

//     // Verificar si el número de rebanada (slice) está en el rango correcto
//     if (slice < 0 || slice >= size_z) {
//         std::cerr << "Error: Slice fuera de rango." << std::endl;
//         return;
//     }

//     // Abrir el archivo en modo de escritura
//     std::ofstream outFile(filename);
//     if (!outFile.is_open()) {
//         std::cerr << "Error al abrir el archivo." << std::endl;
//         return;
//     }

//     // Guardar la rebanada en el archivo
//     for (int i = 0; i < size_x; i++) {
//         for (int j = 0; j < size_y; j++) {
//             // Guardar el valor en el archivo, separado por '\t'
//             outFile << image3d[i][j][slice];
            
//             // Si no es el último valor de la fila, añadir '\t'
//             if (j < size_y - 1) {
//                 outFile << '\t';
//             }
//         }
//         // Al final de cada fila, añadir '\n'
//         outFile << std::endl;
//     }

//     // Cerrar el archivo después de escribir
//     outFile.close();

//     std::cout << "Slice guardada en: " << filename << std::endl;
// }

int main()
{
    /* ----------------------------------- DICOM ----------------------------------- */
    // std::string directory = "data/Etiquetado_17682_20230302_152135";
    // std::string directory = "data/Etiquetado_17689_20230307_084844";
    std::string directory = "data/Etiquetado_17691_20230307_135251";
    // std::string directory = "data/Etiquetado_17705_20230313_144230";

    //Load all the dcm files
    std::vector<DicomImage*> images = loadAllDicom(directory);

    //Print the shape of the vector to check is loaded successfuly
    std::cout << "Shape of the vector: " << images.size() << std::endl;
    //shape of the first image (rows,columns)
    std::cout << "Shape of the first image: " << images[0]->getHeight() << "," << images[0]->getWidth() << std::endl;

    unsigned short *** img3darr = preprocessDicom(images);

    //Free the image vector
    for (int i = 0; i < images.size(); i++)
    {
        delete images[i];
    }

    /* ----------------------------------- NIFTI ----------------------------------- */

    // std::string lbl_filename = "data/Etiquetado_17682_20230302_152135/Untitled.nii.gz";
    // std::string lbl_filename = "data/Etiquetado_17689_20230307_084844/Untitled.nii.gz";
    std::string lbl_filename = "data/Etiquetado_17691_20230307_135251/Label.nii.gz";
    // std::string lbl_filename = "data/Etiquetado_17705_20230313_144230/Untitled.nii.gz";

    nifti_image* lbl = loadNifti(lbl_filename);

    unsigned short*** lbl3darr = preprocessNifti(lbl);

    //Free the memory
    nifti_image_free(lbl);

    /* ----------------------------------- RED ----------------------------------- */

    unsigned short*** seg3darr = nNet(img3darr, lbl3darr);

    //Free the image and label 3d arrays
    for (int i = 0; i < 764; i++)
    {
        for (int j = 0; j < 764; j++)
        {
            delete[] img3darr[i][j];
            delete[] lbl3darr[i][j];
        }
        delete[] img3darr[i];
        delete[] lbl3darr[i];
    }

    /* ----------------------------------- OUTPUT ----------------------------------- */

    nifti_image* seg = postprocessNifti(seg3darr);

    //Free the segmentation 3d array
    for (int i = 0; i < 764; i++)
    {
        for (int j = 0; j < 764; j++)
        {
            delete[] seg3darr[i][j];
        }
        delete[] seg3darr[i];
    }


    // writeNifti(seg, "/Users/mentxaka/Github/DentalSegmentation/data/res/135.nii.gz");
    // writeNifti(seg, "/Users/mentxaka/Github/DentalSegmentation/data/res/884.nii.gz");
    writeNifti(seg, "/Users/mentxaka/Github/DentalSegmentation/data/res/251.nii.gz");
    // writeNifti(seg, "/Users/mentxaka/Github/DentalSegmentation/data/res/230.nii.gz");

    //Free the nifti image memory
    nifti_image_free(seg);

    return 0;
}