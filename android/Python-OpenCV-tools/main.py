import cv2
import numpy as np
scrimg=cv2.imread('1.jpg')

Blur=cv2.GaussianBlur(scrimg,(3,3),10)
Gray=cv2.cvtColor(Blur,cv2.COLOR_BGR2GRAY)
cannyimg=cv2.Canny(Gray,20,20)
kernel= cv2.getStructuringElement(cv2.MORPH_RECT,ksize=(2,2))
imgDila= cv2.dilate(cannyimg,kernel)
contour =cv2.findContours(imgDila,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
for con in contour:
    perimeter=cv2.arcLength(con,True)
    approx=cv2.approxPolyDP(con,perimeter*0.02,True)
   
output=np.zeros(scrimg)
output.fill(255)
cv2.drawContours(output,approx,-1,(0),1)
cv2.imshow(output)
cv2.waitKey(0)