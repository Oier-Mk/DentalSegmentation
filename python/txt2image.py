import os
import numpy as np
#function that passing a txt with the pixel value of the image, it creates a numpy array
def txt2nparray(txt_file):
    with open(txt_file, 'r') as f:
        data = f.readlines()
    data = [line.strip() for line in data]
    data = [line.split() for line in data]
    data = np.array(data, dtype=np.float32)
    return data



def plot_data_label(data, label):
    import matplotlib.pyplot as plt
    # use plt.imshow(ds.pixel_array, cmap=plt.cm.bone) to plot the image

    #Plot the data and the label and save them in png files
    save_dir = 'cfig'
    if not os.path.exists(save_dir):
        os.mkdir(save_dir)

    # make the label be plotted on top of the data in red color
    # the label be a mask on top of the data (not plot if the label is 0)
    plt.imshow(data, cmap=plt.cm.bone)
    plt.imshow(label, cmap=plt.cm.Reds, alpha=0.4)
    plt.axis('off')
    plt.savefig(os.path.join(save_dir, f'C211.png'))
    plt.close()

def main():
    mask = txt2nparray('/Users/mentxaka/Github/DentalSegmentation/mask.txt')
    image = txt2nparray('/Users/mentxaka/Github/DentalSegmentation/image.txt')

    plot_data_label(image, mask)



if __name__ == '__main__':
    main()