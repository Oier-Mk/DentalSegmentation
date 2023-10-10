import os
import pydicom
import nibabel as nib

def load_data_label(data_dir):
    # Step 1: Load the dcom files (data)
    # Get all the dcom files from the data directory
    data_files = [
        file for file in os.listdir(data_dir) if file.endswith('.dcm')
        ]
    # Read the dcom files each one is a slice of the 3D image.
    # The shape of each layer is (764,764) so the whole image is (764,764, number of slices)
    data = [
        pydicom.dcmread(os.path.join(data_dir, file)).pixel_array for file in data_files
        ]
   
    # Step 2: Load the nii.gz file (label)
    # Get the label file
    label_file = [
        file for file in os.listdir(data_dir) if file.endswith('.nii.gz')
        ][0]
    # Read the label file
    label = nib.load(os.path.join(data_dir, label_file)).get_fdata()
    # The shape of the data and the label is (764, 764, number of slices)

    return data, label


def create_tf_record(data_dir):
    
    # Step 1: Load the data and label
    data, label = load_data_label(data_dir)
    
    # TODO: Create a TFRecord
    # TODO: Write the data and label to the TFRecord 


def train():
    # TODO: Load the TFRecord
    # TODO: Create the UNET model
    # TODO: Compile the model
    # TODO: Separate the dataset into train and test
    # TODO: Train the model
    # TODO: Evaluate the model
    pass


def main():
    data_dir = 'data\Etiquetado_17682_20230302_152135'
    create_tf_record(data_dir)


if __name__ == '__main__':
    main()