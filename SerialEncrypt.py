import os                   # Path setting and file-retrieval
import cv2                  # OpenCV
import time                 # Timing Execution
import numpy as np          # See above
import CONFIG as cfg        # Debug flags and constants
from shutil import rmtree   # Directory removal
import secrets              # CSPRNG
import warnings             # Ignore integer overflow during diffusion

warnings.filterwarnings("ignore", category=RuntimeWarning)

os.chdir(cfg.PATH)

# Path-check and image reading
def Init():
    if not os.path.exists(cfg.SRC):
        print("Input directory does not exist!")
        raise SystemExit(0)
    else:
        if os.path.isfile(cfg.ENC_OUT):
            os.remove(cfg.ENC_OUT)
        if os.path.isfile(cfg.DEC_OUT):
            os.remove(cfg.DEC_OUT)

    if os.path.exists(cfg.TEMP):
        rmtree(cfg.TEMP)
    os.makedirs(cfg.TEMP)

    # Open Image
    img = cv2.imread(cfg.ENC_IN,-1)
    if img is None:
        print("File does not exist!")
        raise SystemExit(0)
    return img, img.shape[0], img.shape[1]

# Generate and return rotation vector of length n containing values < m
def genRelocVec(m, n, logfile):
    # Initialize constants
    secGen = secrets.SystemRandom()
    a = secGen.randint(2,cfg.PERMINTLIM)
    b = secGen.randint(2,cfg.PERMINTLIM)
    c = 1 + a*b
    x = secGen.uniform(0.0001,1.0)
    y = secGen.uniform(0.0001,1.0)
    offset = secGen.randint(2,cfg.PERMINTLIM)

    # Log parameters for decryption
    with open(logfile, 'w+') as f:
        f.write(str(a) +"\n")
        f.write(str(b) +"\n")
        f.write(str(x) +"\n")
        f.write(str(y) +"\n")
        f.write(str(offset) + "\n")

    # Skip first <offset> values
    for i in range(offset):
        x = (x + a*y)%1 
        y = (b*x + c*y)%1
    
    # Start writing intermediate values
    ranF = np.zeros((m*n),dtype=float)
    for i in range(m*n//2):
        x = (x + a*y)%1
        y = (b*x + c*y)%1
        ranF[2*i] = x
        ranF[2*i+1] = y
        if x==0 or y==0:
            null = "null"
            return null, null
    
    # Generate column-relocation vector
    r = secGen.randint(1,m*n-n)
    exp = 10**14
    vec = np.zeros((n),dtype=int)
    for i in range(n):
        vec[i] = int((ranF[r+i]*exp)%m)

    with open(logfile, 'a+') as f:
        f.write(str(r))

    return ranF, vec

# Column rotation
def rotateColumn(img, col, colID, offset):
    colLen = len(col)
    for i in range(img.shape[0]): # For each row
        img[i][colID] = col[(i+offset)%colLen]
        
# Row rotation
def rotateRow(img, row, rowID, offset):
    rowLen = len(row)
    for j in range(img.shape[1]): # For each column
        img[rowID][j] = row[(j+offset)%rowLen]

def Encrypt():
    # Read image
    img, m, n = Init()

    # Col-rotation | len(U)=n, values from 0->m
    P1, U = genRelocVec(m,n,"temp\\P1.txt") 
    while type(U) is str:
        P1, U = genRelocVec(m,n,"temp\\P1.txt")

    # Row-rotation | len(V)=m, values from 0->n
    P2, V = genRelocVec(n,m,"temp\\P2.txt") 
    while type(V) is str:
        P2, V = genRelocVec(n,m,"temp\\P2.txt")

    for i in range(cfg.PERM_ROUNDS):
        # For each column
        for j in range(n):
            if U[j]!=0:
                rotateColumn(img, np.copy(img[:,j]), j, U[j])
        
        # For each row
        for i in range(m):
            if V[i]!=0:
                rotateRow(img, np.copy(img[i,:]), i, V[i])

    if cfg.DEBUG_IMAGES==True:
        cv2.imwrite(cfg.PERM, img)

    '''PERMUTATION PHASE COMPLETE'''

    # Convert image to grayscale and flatten it
    imgBW = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    imgVec = np.asarray(imgBW).reshape(-1)
    fDiff = np.zeros_like(imgVec)
    rDiff = np.zeros_like(imgVec)
    
    # Initiliaze diffusion constants
    secGen = secrets.SystemRandom()
    alpha = secGen.randint(1,cfg.DIFFINTLIM)
    beta = secGen.randint(1,cfg.DIFFINTLIM)
    mn = len(imgVec)
    mid = mn//2
    f, r = cfg.f, cfg.r
    
    # Forward Diffusion
    fDiff[0] = f + imgVec[0] + alpha*(P1[0] if f&1==0 else P1[1]) # 0

    for i in range(1, mid): # 1->(mid-1)
        fDiff[i] = int(fDiff[i-1] + imgVec[i] + alpha*(P1[2*i] if fDiff[i-1]&1==0 else P1[2*i + 1]))
    j = 0
    for i in range(mid, mn): # mid->(mn-1)
        fDiff[i] = fDiff[i-1] + imgVec[i] + alpha*(P1[2*j] if fDiff[i-1]&1==0 else P1[2*j + 1])
        j += 1
    
    # Reverse Diffusion
    rDiff[mn-1] = r + fDiff[mn-1] + beta*(P2[mn-2] if r&1==0 else P2[mn-1]) # (mn-1)
    
    j = mid-1
    for i in range(mn-2, mid-1, -1): # (mn-2)->mid
        rDiff[i] = rDiff[i+1] + fDiff[i] + beta*(P2[2*j] if rDiff[i+1]&1==0 else P2[2*j + 1])
        j -= 1
    
    for i in range(mid-1, -1, -1): # (mid-1)->0
        rDiff[i] = rDiff[i+1] + fDiff[i] + beta*(P2[2*i] if rDiff[i+1]&1==0 else P2[2*i + 1])

    
    # Log diffusion parameters for decryption
    with open("temp\\diff.txt","w+") as f:
        f.write(str(alpha) + "\n")
        f.write(str(beta) + "\n")

    img = (np.reshape(rDiff,imgBW.shape)).astype(np.uint8)

    if cfg.DEBUG_IMAGES==True:
        cv2.imwrite(cfg.DIFF, imgBW)

    '''DIFFUSION PHASE COMPLETE'''
    
    cv2.imwrite(cfg.ENC_OUT, img)

Encrypt()
cv2.waitKey(0)
cv2.destroyAllWindows()