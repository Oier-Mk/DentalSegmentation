import os
import pydicom
import nibabel as nib
import numpy as np
from models import UNETModel

def create_tf_record(data_dir):
    # TODO: FINISH THIS FUNCTION
    # Step 1: Load the data and label
    data, label = load_data_label(data_dir)
    
    # TODO: Create a TFRecord
    # TODO: Write the data and label to the TFRecord 

def load_tf_record(filename):
    # TODO: FINISH THIS FUNCTION
    return None

def load_data_label(data_dir):
    # Step 1: Load the dcom files (data)
    # Get all the dcom files from the data directory
    data_files = [
        file for file in os.listdir(data_dir) if file.endswith('.dcm')
        ]
    # Sort the files by the number in the file name (the filename ends with a number after an underscore)
    data_files.sort(key=lambda x: int(x.split('_')[-1].split('.')[0]))

    n_slices = len(data_files)
    # Read the dcom files each one is a slice of the 3D image.
    # The shape of each layer is (764,764) so the whole image should be (764,764, number of slices)
    data = []
    for file in data_files:
        ds = pydicom.dcmread(os.path.join(data_dir, file))
        data.append(ds.pixel_array)
    # Convert the list to a numpy array
    data = np.array(data)
    # data is in axis (z,y,x) so we need to transpose it to (x,y,z)
    data = np.transpose(data, (2, 1, 0))

    # Step 2: Load the nii.gz file (label)
    # Get the label file
    label_file = [
        file for file in os.listdir(data_dir) if file.endswith('.nii.gz')
        ][0]
    # Read the label file
    label = nib.load(os.path.join(data_dir, label_file)).get_fdata()
    # The shape of the data and the label is (764, 764, number of slices)

    return data, label

def plot_data_label(data, label):
    import matplotlib.pyplot as plt
    # use plt.imshow(ds.pixel_array, cmap=plt.cm.bone) to plot the image

    #Plot the data and the label and save them in png files
    save_dir = 'figures'
    if not os.path.exists(save_dir):
        os.mkdir(save_dir)

    for i in range(data.shape[2]):
        # make the label be plotted on top of the data in red color
        # the label be a mask on top of the data (not plot if the label is 0)
        plt.imshow(data[:, :, i], cmap=plt.cm.bone)
        plt.imshow(label[:, :, i], cmap=plt.cm.Reds, alpha=0.4)
        plt.axis('off')
        plt.savefig(os.path.join(save_dir, f'{i}.png'))
        plt.close()


def train(data, label):
    # TODO: Create the UNET model

    # TODO: Compile the model
    # TODO: Separate the dataset into train and test
    # TODO: Train the model
    # TODO: Evaluate the model
    pass


def main():
    data_dir = 'data\Etiquetado_17682_20230302_152135'
    data, label = load_data_label(data_dir)

    #Print the shape of the data and the label
    print(f'The shape of the data is {data.shape}')
    print(f'The shape of the label is {label.shape}')

    #plot_data_label(data, label)
    train(data, label)


if __name__ == '__main__':
    main()