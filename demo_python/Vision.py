import numpy as np
import cv2

kernel = np.ones((5, 5), np.uint8)
x = 0
y = 0
width = 600
height = 400
P = [[458.654, 0, 367.215],
     [0, 457.296, 248.375],
     [0, 0, 1]]
K = [-0.28340811, 0.07395907, 0.00019359, 1.76187114e-05]


def line_scan(frame):
    frame = frame[y: y + height, x: x + width]
    frame = cv2.undistort(frame, np.array(P), np.array(K))
    img = frame.copy()
    img_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    open_img = cv2.morphologyEx(img_gray, cv2.MORPH_OPEN, kernel)
    close_img = cv2.morphologyEx(open_img, cv2.MORPH_CLOSE, kernel)
    img_gray = cv2.erode(img_gray, kernel, iterations=4)  # 腐蚀
    img_gray = cv2.dilate(img_gray, kernel, iterations=4)  # 膨胀
    # cv2.imshow("frame", close_img)
    frame = cv2.Canny(close_img, 100, 145)
    lines = cv2.HoughLines(frame, 1, np.pi / 180, 110)

    line_num = 0
    rho_ave = 0
    theta_ave = 0
    if lines is not None:
        for line in lines:
            rho_ave += line[0][0]
            theta_ave += line[0][1]
            line_num += 1

        k = int((np.cos(theta_ave / line_num) + 1) * 100)
        print(k)
        return k - 100, int(rho_ave / line_num)
    else:
        return None, None
        # print("极坐标系中的直线斜率：",int( (np.cos(theta)+1)*100)  )
        # print("juli：", rho)
        # cv2.line(img, (x1, y1), (x2, y2), (0, 0, 255), 2)


def qr_scan(frame):
    """

    :param frame: 传入图像
    :return: 二维码信息
    """
    qrcode = cv2.QRCodeDetector()  # 载入文件库
    QR, points, code = qrcode.detectAndDecode(frame)  # 对二维码进行解码，返回二维码的信息
    if code is None:  # 如果code返回值是none说明没有识别到二维码
        return None, None
    if code is not None:  # 如果code有返回值说明识别到二维码
        return [int(QR[0]), int(QR[1]), int(QR[2])], [int(QR[4]), int(QR[5]), int(QR[6])]


def find_closest_rectangle(boxes, classes, center):
    """
    寻找最接近中心的色块
    :param boxes: rknn 返回值 目标位置 左上X,Y右下X,Y
    :param center: 目标中心点
    :param classes: rknn返回值 目标序列号
    :return: 颜色,偏移坐标X,Y
    """
    if boxes is None:
        return None, None
    centers = np.zeros((len(boxes), 2))
    for i, rect in enumerate(boxes):
        x1, y1, x2, y2 = rect
        center_x = (x1 + x2) / 2
        center_y = (y1 + y2) / 2
        centers[i] = [center_x, center_y]
    distances = np.linalg.norm(centers - center, axis=1)
    closest_index = np.argmin(distances)
    dx = int(center[0] - centers[closest_index][0])
    dy = int(center[1] - centers[closest_index][1])
    print(classes)
    print(closest_index)
    color = classes[closest_index]
    return color + 1, [dx, dy]


def find_target_color(boxes, classes, center, target_color):
    """
    
    :param boxes: rknn 返回值 目标位置 左上X,Y右下X,Y 
    :param center: 目标中心点
    :param classes: rknn返回值 目标序列号
    :param target_color:  寻找的目标颜色
    :return:  type: list 与center的偏差值X,Y
    """""

    if boxes is None:
        return None

    if np.sum(np.isin(classes, target_color - 1)) != 1:
        return None

    color_index = np.where(classes == target_color - 1)[0][0]
    x1, y1, x2, y2 = boxes[color_index]
    center_x = (x1 + x2) / 2
    center_y = (y1 + y2) / 2
    centers = [center_x, center_y]
    dx = int(center[0] - centers[0])
    dy = int(center[1] - centers[1])

    return [dx, dy]


def find_line(boxes):
    """

    :param boxes: rknn 返回值 目标位置 左上X,Y右下X,Y
    :return: 返回两个目标点位拟合出直线的倾角,两点连线中点坐标X,Y
    """
    if boxes is None or len(boxes) != 2:
        return None, None
    centers = np.zeros((len(boxes), 2))
    for i, rect in enumerate(boxes):
        x1, y1, x2, y2 = rect
        center_x = (x1 + x2) / 2
        center_y = (y1 + y2) / 2
        centers[i] = [center_x, center_y]

    angle_rad = np.arctan2(centers[0][1] - centers[1][1], centers[0][0] - centers[1][0])
    angle_deg = np.degrees(angle_rad)

    x = int((centers[0][0] + centers[1][0]) / 2)
    y = int((centers[0][1] + centers[1][1]) / 2)

    print(angle_deg)
    return -angle_deg, [x, y]
