'''Configure algorithm operation via this file'''
import os

# Path to set working directory
PATH = os.path.dirname(os.path.abspath( __file__ )) + "\\"

# Input image name and extension
IMG = "lena"
EXT = ".png"

# Key paths
TEMP = "temp\\"             # Folder used to store intermediary results
SRC  = "images\\"           # Folder containing input and output

# Input/Output images
ENC_IN =  SRC + IMG + EXT               # Input image
ENC_OUT= SRC + IMG + "_encrypted.png"   # Encrypted Image
DEC_OUT= SRC + IMG + "_decrypted.png"   # Decrypted Image
PERM   = TEMP + IMG + "_1permuted.png"    # Permuted Image
DIFF   = TEMP + IMG + "_2diffused.png"    # Diffused Image
UNDIFF = TEMP + IMG + "_3undiffused.png"  # UnDiffused Image
UNPERM = TEMP + IMG + "_4unpermuted.png"  # UnPermuted Image

# Flags
DEBUG_TIMER  = True # Print timing statistics in console
DEBUG_IMAGES = True # Store intermediary results

# Control Parameters
PERM_ROUNDS= 2
PERMINTLIM = 32
DIFFINTLIM = 16
f = 0
r = 0