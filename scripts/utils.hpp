#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

unsigned short** getSlice(unsigned short*** array, std::vector<unsigned short>* sizes, unsigned short slice, unsigned short axis, std::vector<unsigned short>* slice_sizes);
void print_array(unsigned short*** array, std::vector<unsigned short>* sizes);
void print_arr_slice(unsigned short*** array, std::vector<unsigned short>* sizes, unsigned short slice, unsigned short axis);
void write_array(unsigned short*** array, std::vector<unsigned short>* sizes, std::string filename);
unsigned short*** crop3D(unsigned short*** array, std::vector<unsigned short>* sizes, std::vector<unsigned short>* crop_sizes, std::vector<unsigned short> crop_coords);
void write_arr_slice(unsigned short*** array, std::vector<unsigned short>* sizes, unsigned short slice, unsigned short axis, std::string filename);

#endif // !UTILS_HPP