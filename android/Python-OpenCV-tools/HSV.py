#coding=utf-8
from array import array
import cv2
import numpy as np

def nothing(*arg):
    pass

def stackImages(img,Arry):
    # 模糊化
    frameBGR = cv2.GaussianBlur(img, (7, 7), 0)
    # HSV
    hsv = cv2.cvtColor(frameBGR, cv2.COLOR_BGR2HSV)
    colorLow = (Arry[0],Arry[1],Arry[2])
    colorHigh = (Arry[3], Arry[4], Arry[5])
    mask = cv2.inRange(hsv, colorLow, colorHigh)

    kernal = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (7, 7))
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernal)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernal)
    return mask

#颜色HSV值
#icol = (44,89,219,99,255,255)  # Green
#icol = (18, 0, 196, 140, 255, 255)  # Yellow
#icol = (89, 0, 0, 125, 255, 255)  # Blue
#icol = (156, 100, 234, 180, 255, 255)   # Red

#icol = (140,43,40,180,255,255)

icol =(156, 100, 234, 180, 255, 255)

#write=(80,30,160,160,255,255) #TFT 白色底色1
#icol=(60,5,40,145,140,255) #TFT 白色底色2
cv2.namedWindow('colorTest',0)

#滑块下限
cv2.createTrackbar('lowHue', 'colorTest', icol[0], 180, nothing)
cv2.createTrackbar('lowSat', 'colorTest', icol[1], 255, nothing)
cv2.createTrackbar('lowVal', 'colorTest', icol[2], 255, nothing)
#上限
cv2.createTrackbar('highHue', 'colorTest', icol[3], 180, nothing)
cv2.createTrackbar('highSat', 'colorTest', icol[4], 255, nothing)
cv2.createTrackbar('highVal', 'colorTest', icol[5], 255, nothing)

#cap = cv2.VideoCapture('http://192.168.16.102:81/videostream.cgi?user=admin&pwd=888888')
#ret, frame = cap.read()
frame=cv2.imread("temp.jpg")
while True:
    #frame = cv2.flip(frame,1)
    #获取滑块值
    lowHue = cv2.getTrackbarPos('lowHue', 'colorTest')
    lowSat = cv2.getTrackbarPos('lowSat', 'colorTest')
    lowVal = cv2.getTrackbarPos('lowVal', 'colorTest')
    highHue = cv2.getTrackbarPos('highHue', 'colorTest')
    highSat = cv2.getTrackbarPos('highSat', 'colorTest')
    highVal = cv2.getTrackbarPos('highVal', 'colorTest')

    icol= (lowHue,lowSat,lowVal,highHue,highSat,highVal)
    #cv2.imshow('frame', frame)
    mask=stackImages(frame,icol)
    # Show final output image
    cv2.imshow('main', mask)
    # con,hie = cv2.findContours(mask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    # area = []
    # for k in range(len(con)):
    #     area.append(cv2.contourArea(con[k]))
    # max=np.argmax(np,array(area))
    
    # x,y,h,w=cv2.boundingRect(max)
    # rect=frame[x,x+h,y,y+w]
     #  frame = frame[y:y + h, x:x + w]
        #cv2.rectangle(frame,(x,y),(x+w,y+h),(0,255,0),2)
   # cv2.imshow("main",rect)
    k = cv2.waitKey(5)
    if k == 27:
        break

cv2.destroyAllWindows()