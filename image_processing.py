import nibabel as nib
import tensorflow as tf
from tensorflow import keras

def load():
    '''Returns a tuple with (train_data, label_data)'''

    image = nib.load("HeartDataset/imagesTr/la_003.nii.gz")
    image_data = image.get_fdata()
    image_shape = image_data.shape
    print(image_shape)

    label = nib.load("HeartDataset/labelsTr/la_003.nii.gz")
    label_data = label.get_fdata()
    label_shape = label_data.shape
    print(label_shape)
    
    return (image_data, label_data)

def to_tensorflow_record(data, labels, filename):
    '''Expects numpy arrays and converts it to a tensorflow record'''

    writer = tf.io.TFRecordWriter(filename)

    for i in range(data.shape[0]):
        # Convert data and labels to bytes
        data_raw = data[i].tobytes()
        label_raw = labels[i].tobytes()

        # Create a feature dictionary
        feature = {
            'data': tf.train.Feature(bytes_list=tf.train.BytesList(value=[data_raw])),
            'label': tf.train.Feature(bytes_list=tf.train.BytesList(value=[label_raw]))
        }

        # Create an example protocol buffer
        example = tf.train.Example(features=tf.train.Features(feature=feature))

        # Serialize the example to a string and write to the TFRecord
        writer.write(example.SerializeToString())

    writer.close()


'''
Build models, FCN-8 and UNET with
Final Output Layer: “Sigmoid”
Loss: “Binary CrossEntropy”
Optimizer: “Adam”
Side Note: These structures can also be used for multi-class segmentation problem but changes need to apply accordingly
'''
class FCN8Model(keras.Model):
    def __init__(self):
        super(FCN8Model, self).__init__()

        # Define the architecture
        self.conv1 = keras.layers.Conv3D(64, (3, 3, 3), activation='relu', padding='same')
        self.conv2 = keras.layers.Conv3D(64, (3, 3, 3), activation='relu', padding='same')
        self.pool1 = keras.layers.MaxPooling3D(pool_size=(2, 2, 2))

        self.conv3 = keras.layers.Conv3D(128, (3, 3, 3), activation='relu', padding='same')
        self.conv4 = keras.layers.Conv3D(128, (3, 3, 3), activation='relu', padding='same')
        self.pool2 = keras.layers.MaxPooling3D(pool_size=(2, 2, 2))

        self.conv5 = keras.layers.Conv3D(256, (3, 3, 3), activation='relu', padding='same')
        self.conv6 = keras.layers.Conv3D(256, (3, 3, 3), activation='relu', padding='same')
        self.conv7 = keras.layers.Conv3D(256, (3, 3, 3), activation='relu', padding='same')
        self.pool3 = keras.layers.MaxPooling3D(pool_size=(2, 2, 2))

        self.conv8 = keras.layers.Conv3D(512, (3, 3, 3), activation='relu', padding='same')
        self.conv9 = keras.layers.Conv3D(512, (3, 3, 3), activation='relu', padding='same')
        self.conv10 = keras.layers.Conv3D(512, (3, 3, 3), activation='relu', padding='same')
        self.pool4 = keras.layers.MaxPooling3D(pool_size=(2, 2, 2))

        self.conv11 = keras.layers.Conv3D(512, (3, 3, 3), activation='relu', padding='same')
        self.conv12 = keras.layers.Conv3D(512, (3, 3, 3), activation='relu', padding='same')
        self.conv13 = keras.layers.Conv3D(512, (3, 3, 3), activation='relu', padding='same')
        self.pool5 = keras.layers.MaxPooling3D(pool_size=(2, 2, 2))

        self.fc14 = keras.layers.Conv3D(4096, (7, 7, 7), activation='relu', padding='valid')
        self.fc15 = keras.layers.Conv3D(4096, (1, 1, 1), activation='relu', padding='valid')

        self.score_fr = keras.layers.Conv3D(1, (1, 1, 1), activation='linear', padding='valid')
        self.sigmoid = keras.layers.Activation('sigmoid')

    def call(self, inputs):
        x = self.conv1(inputs)
        x = self.conv2(x)
        x = self.pool1(x)

        x = self.conv3(x)
        x = self.conv4(x)
        x = self.pool2(x)

        x = self.conv5(x)
        x = self.conv6(x)
        x = self.conv7(x)
        x = self.pool3(x)

        x = self.conv8(x)
        x = self.conv9(x)
        x = self.conv10(x)
        x = self.pool4(x)

        x = self.conv11(x)
        x = self.conv12(x)
        x = self.conv13(x)
        x = self.pool5(x)

        x = self.fc14(x)
        x = self.fc15(x)

        x = self.score_fr(x)
        x = self.sigmoid(x)  # Apply sigmoid activation

        return x


class UNETModel(keras.Model):
    def __init__(self):
        super(UNETModel, self).__init__()

        # Define the encoder part of the UNet
        self.encoder = keras.Sequential([
            keras.layers.Conv3D(64, 3, activation='relu', padding='same', input_shape=(128, 128, 128, 1)),
            keras.layers.Conv3D(64, 3, activation='relu', padding='same'),
            keras.layers.MaxPooling3D(pool_size=(2, 2, 2)),
            keras.layers.Conv3D(128, 3, activation='relu', padding='same'),
            keras.layers.Conv3D(128, 3, activation='relu', padding='same'),
            keras.layers.MaxPooling3D(pool_size=(2, 2, 2)),
            keras.layers.Conv3D(256, 3, activation='relu', padding='same'),
            keras.layers.Conv3D(256, 3, activation='relu', padding='same'),
            keras.layers.MaxPooling3D(pool_size=(2, 2, 2)),
        ])

        # Define the middle part of the UNet
        self.middle = keras.Sequential([
            keras.layers.Conv3D(512, 3, activation='relu', padding='same'),
            keras.layers.Conv3D(512, 3, activation='relu', padding='same'),
            keras.layers.Conv3DTranspose(256, 2, strides=(2, 2, 2), padding='same'),
        ])

        # Define the decoder part of the UNet
        self.decoder = keras.Sequential([
            keras.layers.Conv3D(256, 3, activation='relu', padding='same'),
            keras.layers.Conv3D(256, 3, activation='relu', padding='same'),
            keras.layers.Conv3DTranspose(128, 2, strides=(2, 2, 2), padding='same'),
            keras.layers.Conv3D(128, 3, activation='relu', padding='same'),
            keras.layers.Conv3D(128, 3, activation='relu', padding='same'),
            keras.layers.Conv3DTranspose(64, 2, strides=(2, 2, 2), padding='same'),
            keras.layers.Conv3D(64, 3, activation='relu', padding='same'),
            keras.layers.Conv3D(64, 3, activation='relu', padding='same'),
        ])

        # Define the output layer
        self.output_layer = keras.layers.Conv3D(1, 1, activation='sigmoid', padding='same')

    def call(self, inputs):
        x1 = self.encoder(inputs)
        x2 = self.middle(x1)
        x3 = self.decoder(x2)
        output = self.output_layer(x3)
        return output




# Usage
image_data, label_data = load()
to_tensorflow_record(image_data, label_data, "data.tfrecord")

# Create both FCN8 and UNET models
fcn8_model = FCN8Model()
unet_model = UNETModel()

# Load the data
raw_dataset = tf.data.TFRecordDataset("data.tfrecord")

# Define the feature description
feature_description = {
    'data': tf.io.FixedLenFeature([], tf.string),
    'label': tf.io.FixedLenFeature([], tf.string)
}


