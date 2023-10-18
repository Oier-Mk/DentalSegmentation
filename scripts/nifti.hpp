#ifndef NIFTI_HPP
#define NIFTI_HPP

#include <iostream>
extern "C" {
    #include <nifti/nifti1_io.h>
}


nifti_image* loadNifti(std::string filename);
unsigned short*** preprocessNifti(nifti_image* nii);
nifti_image* postprocessNifti(unsigned short*** data);
void writeNifti(nifti_image* nii, std::string filename);

#endif // !NIFTI_HPP