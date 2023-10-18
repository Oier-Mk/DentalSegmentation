#include "nifti.hpp"

//Funcction that receives a string with the path of the nifti file and returns a pointer to the nifti_image
nifti_image* loadNifti(std::string filename)
{
    // Leer el archivo .nii.gz usando NIfTI
    nifti_image* nii = nifti_image_read(filename.c_str(), 1);
    
    //Check if is loaded correctly
    if (nii->data != NULL)
    {
        std::cout << "Loaded " << filename << " succesfuly" << std::endl;
        //Print some info from the nii image loaded
        std::cout << "Size of the image: " << nii->nvox << std::endl;
        std::cout << "Dimensions of the image: " << nii->nx << "," << nii->ny << "," << nii->nz << std::endl;
        std::cout << "Data type of the image: " << nii->datatype << std::endl;
        std::cout << "Number of bytes per voxel: " << nii->nbyper << std::endl;
        
        return nii;
    }
    return NULL;
}


//Function that receives a pointer nifti_image and return a 3d array of the 3d mask
unsigned short*** preprocessNifti(nifti_image* nii)
{
    //Get the shape of the 3d image the vector size is the z (slices)
    int z = nii->nz;
    //The x and y are the same for all the images
    int x = nii->nx;
    int y = nii->ny;

    std::cout << "Size of the array: " << x << "," << y << "," << z << std::endl;

    //The 3d array should be indexed as [x][y][z]
    unsigned short*** image3d = new unsigned short**[x];
    for (int i = 0; i < x; i++)
    {
        image3d[i] = new unsigned short*[y];
        for (int j = 0; j < y; j++)
        {
            image3d[i][j] = new unsigned short[z];
        }
    }

    //Fill the array with the values on the image vector
    for (int k = 0; k < z; k++)
    {
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++){
                image3d[i][j][k] = ((unsigned short*)nii->data)[i + j*x + k*x*y];
            }
        }
    }
    return image3d;
}

nifti_image* postprocessNifti(unsigned short*** data){
    std::cout << "Postprocessing nifti" << std::endl;

    //create a nifti_image
    nifti_image* nii = nifti_simple_init_nim();

    //fill the nifti_image with the data
    int x = 764;
    int y = 764;
    int z = 603;
    nii->nx = x;
    nii->ny = y;
    nii->nz = z;
    nii->nvox = x*y*z;
    nii->nbyper = sizeof(unsigned short);
    nii->datatype = DT_UINT16;
    nii->data = calloc(nii->nvox, nii->nbyper);

    //iterate over the array data
    for( int i = 0; i < x; i++){
        for( int j = 0; j < y; j++){
            for( int k = 0; k < z; k++){
                ((unsigned short*)nii->data)[i + j*x + k*x*y] = data[i][j][k];
            }
        }
    }
    return nii;

}


void writeNifti(nifti_image* nii, std::string filename){
    //Add the filename to the nifti_image
    nii->fname = strdup(filename.c_str());
    std::cout << "Writing " << filename << std::endl;
    nifti_image_write(nii);
}