import cv2
cap = cv2.VideoCapture(0)
if(cap.isOpened() == False):
    print("The camera is disconnected")
width = cap.get(cv2.CAP_PROP_FRAME_WIDTH)
height = cap.get(cv2.CAP_PROP_FRAME_HEIGHT)
print(width,height)
while (cap.isOpened()):
    ret,img = cap.read()
    cv2.imshow("output",img)
    k = cv2.waitKey(1)
    if(k == 27):
        break