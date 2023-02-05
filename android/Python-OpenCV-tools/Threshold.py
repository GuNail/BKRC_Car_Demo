
#二值化阈值
import cv2

def nothing(*arg):
    pass

cv2.namedWindow('ThresholdTest')
frame=cv2.imread("1.jpg")

frame=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)

cv2.createTrackbar('thresh', 'ThresholdTest', 128, 255,nothing)
cv2.createTrackbar('maxval', 'ThresholdTest', 255, 255,nothing)

while True:
    thresh = cv2.getTrackbarPos('thresh', 'ThresholdTest')
    maxval = cv2.getTrackbarPos('maxval', 'ThresholdTest')
    _,res=cv2.threshold(frame,thresh=thresh,maxval=maxval,type=1)

    cv2.imshow("ThresholdTest",res)

    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break
cv2.destroyAllWindows()