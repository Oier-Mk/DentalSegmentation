#include "n_net.hpp"

unsigned short*** nNet(unsigned short*** dicom, std::vector<unsigned short>* sizes, unsigned short*** lbl){
    
    std::cout<<"Neural Network doing its job!!"<<std::endl;

    //Get the sizes of the lbl
    int img_x = sizes->at(0);
    int img_y = sizes->at(1);
    int img_z = sizes->at(2);

    // Deep copy of lbl in seg
    unsigned short*** seg = new unsigned short**[img_x];
    for (int i = 0; i < img_x; i++)
    {
        seg[i] = new unsigned short*[img_y];
        for (int j = 0; j < img_y; j++)
        {
            seg[i][j] = new unsigned short[img_z];
            for (int k = 0; k < img_z; k++)
            {
                seg[i][j][k] = lbl[i][j][k];
            }
        }
    }

    return seg;
}
