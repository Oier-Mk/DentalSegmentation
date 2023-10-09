import tensorflow as tf
from tensorflow import keras
from models import FCN8Model, UNETModel
from initial_processing import create_tf_record, load_all_files
import numpy as np

def load_tfrecord(filename): #FIXME: seguramente haya que hacer más preprocesamiento
    #Load the tf record
    return tf.data.TFRecordDataset(filename)

def main():

    #Load the files, cap = 3 for testing purposes
    data_dir = 'HeartDataset/imagesTr'
    label_dir = 'HeartDataset/labelsTr'
    data, labels = load_all_files(data_dir, label_dir, cap=3)

    # Create both FCN8 and UNET models
    #fcn8_model = FCN8Model()
    unet_model = UNETModel()

    # # Compile the models
    #fcn8_model.compile(optimizer='adam', loss='binary_crossentropy')
    unet_model.compile(optimizer='adam', loss='binary_crossentropy')

    # Separate the dataset into train and test
    test_size = 0.2
    train_size = 1 - test_size
    split = int(train_size * len(data))
    train_data = data[:split]
    train_labels = labels[:split]
    test_data = data[split:]
    test_labels = labels[split:]

    # Train the models
    #fcn8_model.fit(train_data, train_labels, epochs=5)
    unet_model.fit(train_data, train_labels, epochs=5)

    # Evaluate the models and print the results
    #print('FCN8 Model')
    #fcn8_model.evaluate(test_data, test_labels)
    print('UNET Model')
    unet_model.evaluate(test_data, test_labels)






if __name__ == '__main__':
    main()


