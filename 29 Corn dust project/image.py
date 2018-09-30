import numpy as np
import sys
from scipy.ndimage import imread
from scipy.misc import imsave
import matplotlib.pyplot as plt

M1 = imread( sys.argv[1] )[:,:,:3]
M2 = imread( sys.argv[2] )[:,:,:3]

M1 = np.average( M1, axis=2 )
M2 = np.average( M2, axis=2 )

M = np.fabs(M2-M1)

''' cropping the picture '''

x1 = 0
y1 = 0

x2 = 1920
y2 = 1080

cropped = M[ y1:y2, x1:x2 ]

''' making a greyscale histogram '''

Hx = np.average( cropped, axis=0 )

plt.plot( Hx )
np.savetxt( 'hist-x.csv', Hx, fmt='%.3f' )
plt.savefig( 'hist-x.png' )
plt.show()

Hy = np.average( cropped, axis=1 )
plt.plot( Hy )
np.savetxt( 'hist-y.csv', Hy, fmt='%.3f' )
plt.savefig( 'hist-y.png' )
plt.show()

imsave( 'BW.png', M )
imsave( 'BW-cropped.png', cropped )

np.savetxt( 'BW-cropped.csv', cropped, delimiter=',', fmt='%4d' )

