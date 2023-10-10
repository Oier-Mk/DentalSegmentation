import os
import nibabel as nib
import tensorflow as tf
# import numpy as np


def load_nifti(filename):
    '''
    Loads a nifti file and returns the data as a numpy array
    '''
    nifti = nib.load(filename)
    data = nifti.get_fdata()
    return data


def load_all_files(data_dir, label_dir, cap=None):
    # Get all the nii.gz files from the data and label directories
    data_files = [
        file for file in os.listdir(data_dir) if file.endswith('.nii.gz')
        ]
    label_files = [
        file for file in os.listdir(label_dir) if file.endswith('.nii.gz')
        ]

    # The shape of the data and the label is (320, 320, 130)

    # Sort the files so that the data and label files match
    data_files.sort()
    label_files.sort()

    # Cap the number of files to load if needed
    if cap:
        data_files = data_files[:cap]
        label_files = label_files[:cap]

    # Load all the data and labels into numpy arrays
    data = [
        load_nifti(os.path.join(data_dir, file)) for file in data_files
        ]
    labels = [
        load_nifti(os.path.join(label_dir, file)) for file in label_files
        ]

    return data, labels


def write_single_tf_record(data, label, writer):
    '''
    Writes a single data and label to a TFRecord
    '''
    # Convert data and labels to bytes
    data_raw = data.tobytes()
    label_raw = label.tobytes()

    # Create a feature dictionary
    feature = {
        'data': tf.train.Feature(
            bytes_list=tf.train.BytesList(value=[data_raw])
            ),
        'label': tf.train.Feature(
            bytes_list=tf.train.BytesList(value=[label_raw])
            )
    }

    # Create an example protocol buffer
    example = tf.train.Example(features=tf.train.Features(feature=feature))

    # Serialize the example to a string and write to the TFRecord
    writer.write(example.SerializeToString())


def create_tf_record(data, labels, filename='data.tfrecord'):
    '''
    Creates a tensorflow record from the train data and labels 
    '''
    # Create a TFRecordWriter
    writer = tf.io.TFRecordWriter(filename)

    # Loop through all files and load them as numpy arrays
    # Then write them to the TFRecord

    for i in range(len(data)):
        # print the progress
        print('Processing {}/{} files'.format(i+1, len(data)))
        write_single_tf_record(data[i], labels[i], writer)
    
    writer.close()


def main():
    data_dir = 'HeartDataset/imagesTr'
    label_dir = 'HeartDataset/labelsTr'
    data, labels = load_all_files(data_dir, label_dir)
    create_tf_record(data, labels)


if __name__ == '__main__':
    main()