#include "n_net.hpp"

unsigned short*** nNet(unsigned short*** dicom, unsigned short*** lbl){
    
    std::cout<<"Neural Network doing its job!!"<<std::endl;

    // Deep copy of lbl in seg
    unsigned short*** seg = new unsigned short**[764];
    for (int i = 0; i < 764; i++)
    {
        seg[i] = new unsigned short*[764];
        for (int j = 0; j < 764; j++)
        {
            seg[i][j] = new unsigned short[603];
            for (int k = 0; k < 603; k++)
            {
                seg[i][j][k] = lbl[i][j][k];
            }
        }
    }

    return seg;
}
