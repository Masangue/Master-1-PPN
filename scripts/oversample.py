#kera tensorflow  scipy opencv-python

# import keras.api._v2.keras as keras
# from tensorflow.python.keras.layers import Dense

# example of horizontal flip image augmentation
from numpy import expand_dims
from  tensorflow.keras.preprocessing.image import load_img
from  tensorflow.keras.preprocessing.image import img_to_array
from  tensorflow.keras.preprocessing.image import ImageDataGenerator
import os
from matplotlib import pyplot

import random
import numpy as np
import scipy

import imageio
# import cv2
import matplotlib.pyplot as plt
from skimage.util import random_noise
from matplotlib.pyplot import figure

import scipy.misc

import hashlib

datagen = ImageDataGenerator(
 rotation_range=0.1,
 width_shift_range=0.1,
 height_shift_range=0.1,
 shear_range=0.1,
 zoom_range=0.1,
 horizontal_flip=True,
 fill_mode='nearest',
)

dataset_source_dir = "dataset/train/" 
dataset_out_dir = "dataset/test_modify/" 
class_dirs = {"MildDemented", "ModerateDemented", "NonDemented", "VeryMildDemented", }
 
for class_dir in class_dirs:
    current_dir = dataset_source_dir + class_dir 
    current_out_dir = dataset_out_dir + class_dir 
    os.makedirs(current_out_dir)

    for filename in os.listdir(current_dir):
        path = os.path.join(current_dir, filename)

        if os.path.isfile(path):
            print(path)

            img = load_img(path)
            data = img_to_array(img)
            samples = expand_dims(data, 0)
            
            it = datagen.flow(samples, batch_size=1)
            
            for i in range(9):
                pyplot.subplot(330 + 1 + i)
                batch = it.next()
                image = batch[0].astype('uint8')
                image = random_noise(image, mode='gaussian', seed=None, clip=True)
                # pyplot.imshow(image)
                hashname = hashlib.md5(image).hexdigest()
                filename_out =current_out_dir + "/" + str(hashname) + ".png"

                image_gray = (image[:,:,0]+ image[:,:,1]+ image[:,:,2]) / 3
                image_int = np.array(image_gray*255, np.uint8)
                imageio.imwrite(filename_out, image_int)

