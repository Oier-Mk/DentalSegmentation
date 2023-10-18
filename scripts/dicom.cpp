#include "dicom.hpp"

// Function that receives a dicom filename and return a a vector of dicom images
std::vector<DicomImage*> loadAllDicom(std::string directory){
    //Find the first .dcm file in the directory and get its filename
    OFString ofdirectory = directory.c_str();
    OFList<OFString> files;
    OFStandard::searchDirectoryRecursively(ofdirectory, files, "*.dcm");
    std::string firstFilename = files.front().c_str();
    
    //Gets the files. The first file ends with '_0.dcm' and the last file will end with '_num_files-1.dcm'
    std::string path = firstFilename.substr(0, firstFilename.find_last_of("/\\"));
    std::string filename = firstFilename.substr(firstFilename.find_last_of("/\\")+1, firstFilename.find_last_of("_")-firstFilename.find_last_of("/\\")-1);
    
    std::cout << "Path: " << path << std::endl;
    std::cout << "Filename: " << filename << std::endl;

    std::vector<DicomImage*> images;
    int iter = 0;
    while(true){
        std::string filename2 = path + "/" + filename + "_" + std::to_string(iter) + ".dcm";
        DicomImage *image;
        //Handle the exception. When there is no more images 'E:can't read file' will show up.
        try{
            image = new DicomImage(filename2.c_str());
            if (image != NULL){
                if (image->getStatus() == EIS_Normal){
                    images.push_back(image);
                    //std::cout << "Loaded " << filename2 << " succesfuly" << std::endl;
                    iter ++;
                }
                else{
                    std::cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << std::endl;
                    break;
                }
            }
        }
        catch(...){
            std::cout << "Loaded all the images" << std::endl;
            break;
        }
        
    }

    return images;
    
}

// Function that receives a vector of dicom images and return a 3d array of the whole 3d image
unsigned short*** preprocessDicom(std::vector<DicomImage*> images)
{
    // Get the shape of the 3d image: vector size is the z (slices)
    int z = images.size();
    // The x and y are the same for all the images
    int x = images[0]->getWidth();
    int y = images[0]->getHeight();

    // The 3d array should be indexed as [x][y][z]
    unsigned short*** image3d = new unsigned short**[x];
    for (int i = 0; i < x; i++)
    {
        image3d[i] = new unsigned short*[y];
        for (int j = 0; j < y; j++)
        {
            image3d[i][j] = new unsigned short[z];
        }
    }

    // Fill the array with the values from the image vector
    for (int k = 0; k < z; k++)
    {
        DicomImage *dicomImage = images[k];
        if (dicomImage->getStatus() == EIS_Normal)
        {
            const void *pixelData = dicomImage->getOutputData(16); // Assuming 16-bit data, adjust as per your data type
            for (int j = 0; j < y; j++)
            {
                for (int i = 0; i < x; i++)
                {
                    // Adjust the byte order based on your system's endianness
                    // You might need to consider endianness when extracting pixel values
                    unsigned short pixelValue = *((unsigned short*)pixelData);
                    image3d[i][j][k] = pixelValue;

                    // Move to the next pixel in the pixel data
                    pixelData = (const unsigned short*)pixelData + 1;
                }
            }
        }
        else
        {
            std::cerr << "Error: cannot load DICOM image (" << DicomImage::getString(dicomImage->getStatus()) << ")" << std::endl;
            // Handle error or return an appropriate value
        }
    }

    return image3d;
}




// std::vector<DicomImage*> loadAllDicom(std::string fileDirectory)
// {
//     OFString path = fileDirectory.c_str();
//     OFList<OFString> files;
//     OFStandard::searchDirectoryRecursively(path, files, "*.dcm");

//     std::vector<DicomImage*> images;
//     for (OFListIterator(OFString) it = files.begin(); it != files.end(); ++it)
//         {
//             DicomImage *image = new DicomImage(it->c_str());
//             if (image != NULL){
//                 if (image->getStatus() == EIS_Normal)
//                 {
//                     images.push_back(image);
//                     //std::cout << "Loaded " << it->c_str() << " succesfuly" << std::endl;
//                 }
//                 else
//                 {
//                     std::cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << std::endl;
//                 }
//             }
//         }
//     std::cout << fileDirectory << " loaded succesfuly"<<std::endl;

//     return images;
// }
