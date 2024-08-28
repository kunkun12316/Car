import time

import cv2
import threading
import rknn
import Vision


class Camera:
    def __init__(self, src=0, center_sz=(238, 246) ,center_wk=(238, 260), model=False):
        self.src = src
        self.stream = cv2.VideoCapture(src)
        self.model = model
        if self.stream.isOpened():
            print("cam is ready")
        self.stopped = False
        self.center_sz = center_sz
        self.center_wk = center_wk
        if self.model:
            self.rknn_wk = rknn.rknn(r"/home/radxa/Desktop/demo_python/wk.rknn", 0.5, 0.5)
            self.rknn_sz = rknn.rknn(r"/home/radxa/Desktop/demo_python/sz.rknn", 0.5, 0.5)
        self.thread = threading.Thread(target=self.update, args=())
        for _ in range(10):  # warm up the camera
            (self.grabbed, self.frame) = self.stream.read()
        self.start()

    def close(self):
        self.release()
        cv2.destroyAllWindows()

    def start(self):
        self.thread.setDaemon(True)
        self.thread.start()

    def update(self):
        while True:
            if self.stopped:
                return
            (self.grabbed, self.frame) = self.stream.read()

    def read(self):
        return self.grabbed, self.frame

    def stop(self):
        self.stopped = True

    def release(self):
        self.stop()
        self.stream.release()

    def qr_scan(self):
        while True:
            task1, task2 = Vision.qr_scan(frame=self.frame)
            if task1 is not None and task2 is not None:
                return task1, task2

    def target_scan(self, mod):
        """
        查找最接近中心的目标颜色及坐标偏移值
        :param mod: mod: “wk” 扫描物块 “sz”扫描十字
        :return: 最接近中心的目标颜色及坐标偏移值
        """

        center=self.center_wk if mod == "wk" else self.center_sz
        print(mod)
        while True:
            boxes, classes = self.rknn_wk.rknn_detect(frame=self.frame) if mod == "wk" else self.rknn_sz.rknn_detect(
                frame=self.frame)
            color, pos = Vision.find_closest_rectangle(boxes, classes, center)
            print(f"target_scan{color}{pos}")
            if color and pos is not None:
                return color, pos

    def line_scan(self):
        while True:
            angle, dis = Vision.line_scan(frame=self.frame)
            if angle is not None and dis is not None:
                print(f"line_scan{angle}+{dis}")
                return angle, dis

    def target_scan_by_color(self, target_color, mod):
        """
        查找目标颜色的坐标偏移值
        :param target_color: 目标颜色 同信息码
        :param mod: “wk” 扫描物块 “sz”扫描十字
        :return: 目标颜色的坐标偏移值
        """
        center = self.center_wk if mod == "wk" else self.center_sz
        while True:
            boxes, classes = self.rknn_wk.rknn_detect(frame=self.frame) if mod == "wk" else self.rknn_sz.rknn_detect(
                frame=self.frame)
            pos = Vision.find_target_color(boxes, classes, center, target_color)
            if pos is not None:
                print(f"target_color_scan{pos}")
                # return angle, dis[1]
                return pos


if __name__ == '__main__':
    camera0 = Camera(3, (242, 256),(242, 243), model=True)
    t = time.time()
    while True:
        print(f"fps:{1 / (time.time() - t)}")
        t = time.time()

        print(camera0.target_scan("sz"))
        # print(camera0.target_scan_by_color(2,"sz"),"1")
