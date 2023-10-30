#include "n_net.hpp"


// void printTensorflowVersion(){
//     std::cout << "Tensorflow version: " << TF_Version() << std::endl;
// }

unsigned short*** mockNNet(unsigned short*** dicom, std::vector<unsigned short>* sizes, unsigned short*** lbl){
    
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

// // Load the H5 model with Tensorflow C 'loadModel(std::string filename)'
// TF_Graph* loadModel(std::string filename){
//     // Create the graph
//     TF_Graph* graph = TF_NewGraph();
//     // Create the session
//     TF_SessionOptions* options = TF_NewSessionOptions();
//     TF_Session* session = TF_NewSession(graph, options, status);
//     // Create the model
//     TF_Buffer* graph_def = read_file(filename);
//     TF_ImportGraphDefOptions* graph_opts = TF_NewImportGraphDefOptions();
//     TF_GraphImportGraphDef(graph, graph_def, graph_opts, status);
//     // Check the status
//     if (TF_GetCode(status) != TF_OK) {
//         fprintf(stderr, "ERROR: Unable to import graph %s", TF_Message(status));
//         return NULL;
//     }
//     // Free the resources
//     TF_DeleteImportGraphDefOptions(graph_opts);
//     TF_DeleteBuffer(graph_def);
//     TF_DeleteSessionOptions(options);
//     return graph;
// }

// // Infere the model with Tensorflow C 'inference(unsigned short*** dicom, std::vector<unsigned short>* sizes, TF_Graph* graph)'
// unsigned short*** inference(unsigned short*** dicom, std::vector<unsigned short>* sizes, TF_Graph* graph){
//     // Get the sizes of the dicom
//     int img_x = sizes->at(0);
//     int img_y = sizes->at(1);
//     int img_z = sizes->at(2);
//     //Define the input dimensions
//     int64_t in_dims[] = {img_x, img_y, img_z};
//     // Define the input tensor
//     TF_Tensor* input_tensor = TF_AllocateTensor(TF_UINT16, in_dims, 3, img_x*img_y*img_z*sizeof(unsigned short));
    
//     // Fill the input tensor
//     unsigned short* input_tensor_data = (unsigned short*)TF_TensorData(input_tensor);
//     for (int i = 0; i < img_x; i++)
//     {
//         for (int j = 0; j < img_y; j++)
//         {
//             for (int k = 0; k < img_z; k++)
//             {
//                 input_tensor_data[i*img_y*img_z + j*img_z + k] = dicom[i][j][k];
//             }
//         }
//     }

//     // Run the TFGraph model inference
//     TF_Tensor* output_tensor = NULL;
//     TF_Output output_op = {TF_GraphOperationByName(graph, "output_node0"), 0};
//     TF_SessionRun(session, NULL, &input_op, &input_tensor, 1, &output_op, &output_tensor, 1, NULL, 0, NULL, status);
//     // Check the status
//     if (TF_GetCode(status) != TF_OK) {
//         fprintf(stderr, "ERROR: Unable to run graph %s", TF_Message(status));
//         return 1;
//     }
//     // Get the output tensor data
//     unsigned short* output_tensor_data = (unsigned short*)TF_TensorData(output_tensor);

//     // Define the output segmentation in the 3D array
//     unsigned short*** seg = new unsigned short**[img_x];
//     for (int i = 0; i < img_x; i++)
//     {
//         seg[i] = new unsigned short*[img_y];
//         for (int j = 0; j < img_y; j++)
//         {
//             seg[i][j] = new unsigned short[img_z];
//             for (int k = 0; k < img_z; k++)
//             {
//                 seg[i][j][k] = output_tensor_data[i*img_y*img_z + j*img_z + k];
//             }
//         }
//     }
    
//     // Free the resources
//     TF_DeleteTensor(input_tensor);
//     TF_DeleteTensor(output_tensor);

//     return seg;
// }


