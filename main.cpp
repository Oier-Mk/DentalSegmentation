#include <iostream>
#include "scripts/dicom.hpp"
#include "scripts/nifti.hpp"
#include "scripts/n_net.hpp"
#include "scripts/utils.hpp"

void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " <option(s)> SOURCES"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-d,--dicom DICOM_DIR\tSpecify the directory of the DICOM files (.dcm)\n"
              << "\t-n,--nifti NIFTI_FILE\tSpecify the NIFTI file with the original mask (.nii.gz)\n"
              << "\t-m,--model MODEL_FILE\tSpecify the model file (.h5)\n"
              << "\t-o,--output OUTPUT_FILE\tSpecify the filename of the NIFTI fiel of the resulting mask (.nii.gz)\n"
              << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 7) {
        show_usage(argv[0]);
        return 1;
    }
    std::string dicom_directory;
    std::string nifti_file;
    std::string exit_filename;
    std::string model_filename;
    for (int i = 1; i < argc; ++i){
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")){
            show_usage(argv[0]);
            return 0;
        }
        else if ((arg == "-d") || (arg == "--dicom")){
            if (i + 1 < argc){
                dicom_directory = argv[++i];
            }
            else{
                std::cerr << "--dicom option requires one argument." << std::endl;
                return 1;
            }
        }
        else if ((arg == "-n") || (arg == "--nifti")){
            if (i + 1 < argc){
                nifti_file = argv[++i];
            }
            else{
                std::cerr << "--nifti option requires one argument." << std::endl;
                return 1;
            }
        }
        else if ((arg == "-m") || (arg == "--model")){
            if (i + 1 < argc){
                model_filename = argv[++i];
            }
            else{
                std::cerr << "--model option requires one argument." << std::endl;
                return 1;
            }
        }
        else if ((arg == "-o") || (arg == "--output")){
            if (i + 1 < argc){
                exit_filename = argv[++i];
            }
            else{
                std::cerr << "--output option requires one argument." << std::endl;
                return 1;
            }
        }
        else{
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }
    }

    if (dicom_directory.empty() || nifti_file.empty() || exit_filename.empty() || model_filename.empty()){
        std::cerr << "Missing arguments" << std::endl;
        return 1;
    }
    
    // std::string dicom_directory = "data/Etiquetado_17682_20230302_152135";
    // std::string nifti_file = "data/Etiquetado_17682_20230302_152135/Untitled.nii.gz";
    // std::string exit_filename = "data/res/135.nii.gz";
    // std::string model_filename = "model/unet.h5"; 

    // build/dental_seg  -d data/Etiquetado_17682_20230302_152135 -n data/Etiquetado_17682_20230302_152135/Untitled.nii.gz -m model/unet.h5 -o data/res/135.nii.gz

    // print the 3 argvs
    std::cout << "Selected DICOM directory: " << dicom_directory << std::endl;
    std::cout << "NIFTI file of the mask: " << nifti_file << std::endl;
    std::cout << "Output_NIFIT_filename: " << exit_filename << std::endl;
    std::cout << "Model filename: " << model_filename << std::endl;

    // /* ----------------------------------- TEST ----------------------------------- */

    // Call for the version of Tensorflow
    // printTensorflowVersion();

    /* ----------------------------------- DICOM ----------------------------------- */

    //Load all the dcm files
    std::vector<DicomImage*> images = loadAllDicom(dicom_directory);

    //Print the shape of the vector to check is loaded successfuly
    std::cout << "Shape of the vector: " << images.size() << std::endl;
    //shape of the first image (rows,columns)
    std::cout << "Shape of the first image: " << images[0]->getHeight() << "," << images[0]->getWidth() << std::endl;

    //Create a vector of unsigned short for the size of the images
    std::vector<unsigned short> img_sizes;

    unsigned short *** img3darr = preprocessDicom(images, &img_sizes);
    
    //Free the image vector
    for (int i = 0; i < images.size(); i++)
    {
        delete images[i];
    }
    images.clear();

    /* ----------------------------------- NIFTI ----------------------------------- */

    nifti_image* lbl = loadNifti(nifti_file);

    //Create a vector of unsigned short for the size of the images
    std::vector<unsigned short> lbl_sizes;

    unsigned short*** lbl3darr = preprocessNifti(lbl, &lbl_sizes);

    //Free the memory
    nifti_image_free(lbl);

    /* ----------------------------------- CROPPING ----------------------------------- */

    // Crop the image and the label to a 300x300x300 cube starting in the 1000x100x100 coordinates
    
    std::vector<unsigned short> img_crop_sizes;
    img_crop_sizes.push_back(300);
    img_crop_sizes.push_back(300);
    img_crop_sizes.push_back(300);
    std::vector<unsigned short> img_crop_coords;
    img_crop_coords.push_back(100);
    img_crop_coords.push_back(100);
    img_crop_coords.push_back(100);
    unsigned short*** cropped_img = crop3D(img3darr, &img_sizes, &img_crop_sizes, img_crop_coords);
    unsigned short*** cropped_lbl = crop3D(lbl3darr, &lbl_sizes, &img_crop_sizes, img_crop_coords);

    //Free the image and label 3d arrays
    for (int i = 0; i < img_sizes[0]; i++)
    {
        for (int j = 0; j < img_sizes[1]; j++)
        {
            delete[] img3darr[i][j];
            delete[] lbl3darr[i][j];
        }
        delete[] img3darr[i];
        delete[] lbl3darr[i];
    }

    // Write the 160 slice in the z axis of the cropped image
    write_arr_slice(cropped_img, &img_crop_sizes, 160, 2, "data/res/img_slice.txt");
    // Same slice for the cropped label
    write_arr_slice(cropped_lbl, &img_crop_sizes, 160, 2, "data/res/lbl_slice.txt");

    // /* ----------------------------------- RED ----------------------------------- */

    // unsigned short*** seg3darr = mockNNet(img3darr, &img_sizes, lbl3darr); //FIXME: As the network is not implemented, actually it just copies the input mask

    // //Free the image and label 3d arrays
    // for (int i = 0; i < img_sizes[0]; i++)
    // {
    //     for (int j = 0; j < img_sizes[1]; j++)
    //     {
    //         delete[] img3darr[i][j];
    //         delete[] lbl3darr[i][j];
    //     }
    //     delete[] img3darr[i];
    //     delete[] lbl3darr[i];
    // }
    
    // //Free the size vector of the label
    // lbl_sizes.clear();

    // /* ----------------------------------- OUTPUT ----------------------------------- */

    // nifti_image* seg = postprocessNifti(seg3darr, &img_sizes);

    // //Free the segmentation 3d array
    // for (int i = 0; i < img_sizes[0]; i++)
    // {
    //     for (int j = 0; j < img_sizes[1]; j++)
    //     {
    //         delete[] seg3darr[i][j];
    //     }
    //     delete[] seg3darr[i];
    // }

    // //Free the size vector of the image
    // img_sizes.clear();

    // writeNifti(seg, exit_filename);
    
    // //Free the nifti image memory
    // nifti_image_free(seg);
    

    return 0;
}