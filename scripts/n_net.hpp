
#ifndef N_NET_HPP
#define N_NET_HPP

#include <iostream>
extern "C" {
    #include <tensorflow/c/c_api.h>
}


unsigned short*** mockNNet(unsigned short*** dicom, std::vector<unsigned short>* sizes, unsigned short*** lbl);
//void printTensorflowVersion();
//TF_Graph* loadModel(std::string filename);
//unsigned short*** inference(unsigned short*** dicom, std::vector<unsigned short>* sizes, TF_Graph* graph);

#endif // !N_NET_HPP