

#Canny 阈值
import cv2

def nothing(*arg):
    pass

cv2.namedWindow('CannyTest')
frame=cv2.imread("3.jpg")

#frame=cv2.GaussianBlur(frame,(3,3),10)
frame=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)

cv2.createTrackbar('threshold1', 'CannyTest', 0, 511,nothing)
cv2.createTrackbar('threshold2', 'CannyTest', 0, 511,nothing)

while True:
    threshold1 = cv2.getTrackbarPos('threshold1', 'CannyTest')
    threshold2 = cv2.getTrackbarPos('threshold2', 'CannyTest')
   
    res=cv2.Canny(frame,threshold1,threshold2)
    kernel= cv2.getStructuringElement(cv2.MORPH_RECT,ksize=(2,2))
    imgDila= cv2.dilate(res,kernel)
    cv2.imshow("CannyTest",imgDila)

    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break
cv2.destroyAllWindows()