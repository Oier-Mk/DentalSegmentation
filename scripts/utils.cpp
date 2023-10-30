#include "utils.hpp"

unsigned short*** crop3D(unsigned short*** array, std::vector<unsigned short>* sizes, std::vector<unsigned short>* crop_sizes, std::vector<unsigned short> crop_coords){
    /*
    Crop a 3D array to the specified size
    array : 3D array to crop
    sizes : pointer to the vector containing the sizes of the original 3D array
    crop_sizes : vector containing the sizes of the cropped 3D array
    crop_coords : vector containing the coordinates of the 3D crop box. The coordinates are the coordinates of the top left front corner of the crop box.
    */

    //Get the crop sizes
    unsigned short crop_sx = crop_sizes->at(0);
    unsigned short crop_sy = crop_sizes->at(1);
    unsigned short crop_sz = crop_sizes->at(2);

    // Check if the crop sizes are smaller than the original sizes
    if (crop_sx > sizes->at(0) || crop_sy > sizes->at(1) || crop_sz > sizes->at(2)){
        std::cout << "Crop sizes are bigger than the original sizes" << std::endl;
        return NULL;
    }
    //Check if the crop coordinates are inside the original array
    if (crop_coords[0] > sizes->at(0) || crop_coords[1] > sizes->at(1) || crop_coords[2] > sizes->at(2)){
        std::cout << "Crop coordinates are outside the original array" << std::endl;
        return NULL;
    }


    // Allocate memory for the cropped array
    unsigned short*** cropped_array = new unsigned short**[crop_sx];
    for (unsigned short i = 0; i < crop_sx; i++){
        cropped_array[i] = new unsigned short*[crop_sy];
        for (unsigned short j = 0; j < crop_sy; j++){
            cropped_array[i][j] = new unsigned short[crop_sz];
        }
    }

    // Copy the values from the original array to the cropped array
    for (unsigned short i = 0; i < crop_sx; i++){
        for (unsigned short j = 0; j < crop_sy; j++){
            for (unsigned short k = 0; k < crop_sy; k++){
                unsigned short x = i + crop_coords[0];
                unsigned short y = j + crop_coords[1];
                unsigned short z = k + crop_coords[2];
                cropped_array[i][j][k] = array[x][y][z];
            }
        }
    }

    return cropped_array;

}

unsigned short** getSlice(unsigned short*** array, std::vector<unsigned short>* sizes, unsigned short slice, unsigned short axis, std::vector<unsigned short>* slice_sizes){
    /*
    Get a slice of a 3D array
    array : 3D array to get the slice from
    sizes : pointer to the vector containing the sizes of the 3D array
    slice : slice to get
    axis : axis along which the slice is taken
    slice_sizes : pointer to a NULL vector. The function will fill the vector with the sizes of the slice
    */

    if (axis == 0){
        // The slice is taken along the x axis so the slice sizes are the y and z sizes
        slice_sizes->push_back(sizes->at(1));
        slice_sizes->push_back(sizes->at(2));
        // Return the slice
        unsigned short** slice_array = new unsigned short*[sizes->at(1)];
        for (unsigned short i = 0; i < sizes->at(1); i++){
            slice_array[i] = new unsigned short[sizes->at(2)];
            for (unsigned short j = 0; j < sizes->at(2); j++){
                slice_array[i][j] = array[slice][i][j];
            }
        }
        return slice_array;
    }
    else if (axis == 1){
        // The slice is taken along the y axis so the slice sizes are the x and z sizes
        slice_sizes->push_back(sizes->at(0));
        slice_sizes->push_back(sizes->at(2));
        // Return the slice
        unsigned short** slice_array = new unsigned short*[sizes->at(0)];
        for (unsigned short i = 0; i < sizes->at(0); i++){
            slice_array[i] = new unsigned short[sizes->at(2)];
            for (unsigned short j = 0; j < sizes->at(2); j++){
                slice_array[i][j] = array[i][slice][j];
            }
        }
        return slice_array;
    }
    else if (axis == 2){
        // The slice is taken along the z axis so the slice sizes are the x and y sizes
        slice_sizes->push_back(sizes->at(0));
        slice_sizes->push_back(sizes->at(1));
        // Return the slice
        unsigned short** slice_array = new unsigned short*[sizes->at(0)];
        for (unsigned short i = 0; i < sizes->at(0); i++){
            slice_array[i] = new unsigned short[sizes->at(1)];
            for (unsigned short j = 0; j < sizes->at(1); j++){
                slice_array[i][j] = array[i][j][slice];
            }
        }
        return slice_array;
    }
    else{
        std::cout << "Invalid axis" << std::endl;
        return NULL;
    }

}

void print_array(unsigned short*** array, std::vector<unsigned short>* sizes){
    /*
    Print a 3D array
    array : 3D array to print
    sizes : pointer to the vector containing the sizes of the 3D array
    */

    for (unsigned short i = 0; i < sizes->at(0); i++){
        std::cout << "Slice " << i << std::endl;
        for (unsigned short j = 0; j < sizes->at(1); j++){
            for (unsigned short k = 0; k < sizes->at(2); k++){
                std::cout << array[i][j][k] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
void print_arr_slice(unsigned short*** array, std::vector<unsigned short>* sizes, unsigned short slice, unsigned short axis){
    /*
    Print a slice of a 3D array
    array : 3D array to print
    sizes : pointer to the vector containing the sizes of the 3D array
    slice : slice to print
    axis : axis along which the slice is taken
    */

    // Get the slice
    std::vector<unsigned short> slice_sizes;
    unsigned short** slice_array = getSlice(array, sizes, slice, axis, &slice_sizes);

    // Print the slice
    for (unsigned short i = 0; i < slice_sizes[0]; i++){
        for (unsigned short j = 0; j < slice_sizes[1]; j++){
            std::cout << slice_array[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Free the memory
    for (unsigned short i = 0; i < slice_sizes[0]; i++){
        delete[] slice_array[i];
    }
    delete[] slice_array;

}

void write_array(unsigned short*** array, std::vector<unsigned short>* sizes, std::string filename){
    /*
    Write a 3D array to a file
    array : 3D array to write
    sizes : pointer to the vector containing the sizes of the 3D array
    filename : name of the file to write the array to
    */

    // Open the file to write text
    FILE* file = fopen(filename.c_str(), "w");

    // Write the sizes of the array to the file
    for (unsigned short i = 0; i < sizes->size(); i++){
        fprintf(file, "%d\n", sizes->at(i));
    }

    // Write the array to the file  
    for (unsigned short i = 0; i < sizes->at(0); i++){
        fprintf(file, "Slice %d\n", i); // Write the slice number to the file to make it easier to read the file
        for (unsigned short j = 0; j < sizes->at(1); j++){
            for (unsigned short k = 0; k < sizes->at(2); k++){
                fprintf(file, "%d ", array[i][j][k]);
            }
            fprintf(file, "\n");
        }
    }

    // Close the file
    fclose(file);

}

void write_arr_slice(unsigned short*** array, std::vector<unsigned short>* sizes, unsigned short slice, unsigned short axis, std::string filename){
    /*
    Write a slice of a 3D array to a file
    array : 3D array to write
    sizes : pointer to the vector containing the sizes of the 3D array
    slice : slice to write
    axis : axis along which the slice is taken
    filename : name of the file to write the array to
    */
    
    // Get the slice
    std::vector<unsigned short> slice_sizes;
    unsigned short** slice_array = getSlice(array, sizes, slice, axis, &slice_sizes);

    // Open the file to write text
    FILE* file = fopen(filename.c_str(), "w");

    // Write the sizes and the axis to the file
    fprintf(file, "%d\n", slice_sizes[0]);
    fprintf(file, "%d\n", slice_sizes[1]);
    fprintf(file, "%d\n", axis);

    //Write the slice to the file
    for (unsigned short i = 0; i < slice_sizes[0]; i++){
        for (unsigned short j = 0; j < slice_sizes[1]; j++){
            fprintf(file, "%d ", slice_array[i][j]);
        }
        fprintf(file, "\n");
    }

    // Close the file
    fclose(file);

    // Free the memory
    for (unsigned short i = 0; i < slice_sizes[0]; i++){
        delete[] slice_array[i];
    }
    delete[] slice_array;

}


    