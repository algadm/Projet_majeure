import cv2
import numpy as np
import matplotlib.pyplot as plt
from osgeo import gdal

# I = cv2.imread('data/output_SRTMGL3.tif', cv2.IMREAD_GRAYSCALE)

ds = gdal.Open('data/output_SRTMGL3.tif')
print(ds.RasterCount)
gray = ds.GetRasterBand(1)
array = gray.ReadAsArray()
print(array)
print(array.shape)

# gdal.Translate('File1.png', 'data/output_SRTMGL3.tif', format='PNG', outputType=gdal.GDT_Byte, scaleParams=[[0,1]])

# np.savez("data/height.npz", my_array=array)
array.tofile('data/heightmap.bin')
plt.figure()
plt.imshow(array,'gray')
plt.show()