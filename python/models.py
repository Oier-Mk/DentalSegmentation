from tensorflow import keras

'''
Build model UNET with
Final Output Layer: “Sigmoid”
Loss: “Binary CrossEntropy”
Optimizer: “Adam”
Side Note: These structures can also be used for
           multi-class segmentation problem but changes
           need to apply accordingly
'''


class UNETModel(keras.Model):
    def __init__(self):
        super(UNETModel, self).__init__()
    
    def build(self, input_shape):
        
        # The encoder has 4 blocks of 2 conv3d layers
        # The input shape is 764x764x603
        self.encoder = keras.Sequential([
            # TODO
        ])
        # The middle is a bottleneck of 2 conv3d layers
        self.middle = keras.Sequential([
            # TODO
        ])

        # The decoder has 4 blocks of 2 conv3d layers
        # The output shape is a mask of 764x764x603
        self.decoder = keras.Sequential([
            # TODO
        ])

    def __call__(self, inputs):
        x1 = self.encoder(inputs)
        x2 = self.middle(x1)
        x3 = self.decoder(x2)
        output = self.output_layer(x3)
        return output
