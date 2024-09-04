import numpy as np
import cv2

# 形态学操作的核矩阵，用于图像腐蚀和膨胀操作。
kernel = np.ones((5,5),np.uint8)
# 用于定义图像截取区域的起点坐标和尺寸。
x = 0
y = 0
width = 600
height = 600
# 相机内参矩阵，用于图像去畸变。
P = [[458.654, 0, 367.215],
     [0, 457.296, 248.375],
     [0, 0, 1]]
# 畸变系数，用于图像去畸变。
K = [-0.28340811, 0.07395907, 0.00019359, 1.76187114e-05]

# 用于检测图像中的线条，并计算这些线条的平均极坐标参数（ρ 和 θ）。
def line_scan(frame):
    # 裁剪图像
    frame = frame[y: y + height, x: x + width]

    # 去畸变
    frame = cv2.undistort(frame, np.array(P), np.array(K))

    # 将图像转为灰度图
    img_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # 使用形态学操作进行图像处理
    open_img = cv2.morphologyEx(img_gray, cv2.MORPH_OPEN, kernel)
    close_img = cv2.morphologyEx(open_img, cv2.MORPH_CLOSE, kernel)

    # 进一步腐蚀和膨胀图像
    img_gray = cv2.erode(img_gray, kernel, iterations=4)
    img_gray = cv2.dilate(img_gray, kernel, iterations=4)

    # 边缘检测
    frame = cv2.Canny(close_img, 100, 145)

    # 霍夫变换检测线条
    lines = cv2.HoughLines(frame, 1, np.pi / 180, 110)

    # 计算平均的rho和theta值
    line_num = 0
    rho_ave = 0
    theta_ave = 0
    if lines is not None:
        for line in lines:
            rho_ave += line[0][0]
            theta_ave += line[0][1]
            line_num += 1

        k = int((np.cos(theta_ave / line_num) + 1) * 100)
        return k - 100, int(rho_ave / line_num)
    else:
        return None, None

# 用于识别二维码
def qr_scan(frame):
    qrcode = cv2.QRCodeDetector()  # 载入二维码检测器
    qr, points, code = qrcode.detectAndDecode(frame)  # 检测并解码二维码
    if code is None:  # 如果没有识别到二维码
        return None, None
    if code is not None:  # 识别到二维码
        return [int(qr[0]), int(qr[1]), int(qr[2])], [int(qr[4]), int(qr[5]), int(qr[6])]

# 用于找到距离中心点最近的矩形，并返回其颜色类别和与中心点的偏移。
def find_closest_rectangle(boxes, classes, center):
# boxes 是每个目标的边界框，classes 是目标的类别，center 是目标中心点的坐标。
    """
    寻找最接近中心的色块
    :param boxes: rknn 返回值 目标位置 左上X,Y右下X,Y
    :param center: 目标中心点
    :param classes: rknn返回值 目标序列号
    :return: 颜色,偏移坐标X,Y
    """
    if boxes is None:
        return None, None

    # 计算每个矩形的中心
    centers = np.zeros((len(boxes), 2))
    for i, rect in enumerate(boxes):
        x1, y1, x2, y2 = rect
        center_x = (x1 + x2) / 2
        center_y = (y1 + y2) / 2
        centers[i] = [center_x, center_y]

    # 计算距离并找到最接近中心的矩形
    distances = np.linalg.norm(centers - center, axis=1)
    closest_index = np.argmin(distances)

    dx = int(center[0] - centers[closest_index][0])
    dy = int(center[1] - centers[closest_index][1])

    color = classes[closest_index]
    return color + 1, [dx, dy]

# 用于找到指定颜色的目标，并返回该目标相对于中心点的偏移量。
def find_target_color(boxes, classes, center, target_color):
# target_color 是要寻找的目标颜色，通过对目标的颜色类别进行筛选找到对应的矩形框，并返回相对于中心点的偏移量。
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

# 用于通过两个点拟合一条直线，并返回该直线的角度和中点坐标。
def find_line(boxes):
    if boxes is None or len(boxes) != 2:
        return None, None

    centers = np.zeros((len(boxes), 2))
    for i, rect in enumerate(boxes):
        x1, y1, x2, y2 = rect
        center_x = (x1 + x2) / 2
        center_y = (y1 + y2) / 2
        centers[i] = [center_x, center_y]

    # 计算两点连线的角度
    angle_rad = np.arctan2(centers[0][1] - centers[1][1], centers[0][0] - centers[1][0])
    angle_deg = np.degrees(angle_rad)

    # 计算中点坐标
    x = int((centers[0][0] + centers[1][0]) / 2)
    y = int((centers[0][1] + centers[1][1]) / 2)

    return -angle_deg, [x, y]
