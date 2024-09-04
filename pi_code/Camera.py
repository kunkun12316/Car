import time
import cv2
import threading
import onnxruntime as ort
import numpy as np
from PyQt5.QtCore import center
from networkx.algorithms.bipartite import color
from sympy.physics.units.definitions.dimension_definitions import angle

import Onnx
import Vision


class Camera:
    #
    def __init__(self,src=0, center_sz=(320,240),center_wk=(320,240),model=False):
        self.src = src
        self.stream = cv2.VideoCapture(src)
        self.model = model
        if self.stream.isOpened():
            print("Camera is ready")
        self.stopped = False
        self.center_sz = center_sz
        self.center_wk = center_wk
        if self.model:
            self.onnx_wk = Onnx.Onnx("model.onnx",obj_class=("red", "green", "blue"))
            self.onnx_sz = Onnx.Onnx("model.onnx",obj_class=("red","green","blue","center_red","center_green","center_blue"))
        #使用threading的Thread方法创建一个新的线程，函数是self.update，args=()表明函数没有传入参数
        self.thread = threading.Thread(target=self.update,args=())
        for _ in range(10):
            (self.grabbed, self.frame) = self.stream.read()
        self.start()

    # 关闭视频流并销毁所有 OpenCV 创建的窗口
    def close(self):
        self.release() # 调用 release 方法，释放视频流资源并停止线程。
        cv2.destroyAllWindows() # 销毁所有由 OpenCV 创建的窗口，确保程序退出时不会留下残留的窗口。

    # 启动处理视频流的线程。
    def start(self):
        self.thread.setDaemon(True) # 将线程设置为守护线程，这意味着当主线程退出时，这个线程也会自动退出。
        self.thread.start() # 启动线程，线程将执行 self.update() 方法中的代码来不断读取视频流。

    # 在后台线程中不断读取视频流中的帧。
    def update(self):
        while True:
            if self.stopped: # 如果 self.stopped 为 True，退出循环并终止线程。
                return
            (self.grabbed, self.frame) = self.stream.read()

    # 返回当前捕捉到的图像帧和是否成功读取的标志。
    def read(self):
        # self.grabbed：布尔值，指示是否成功读取到一帧图像。
        # self.frame：捕捉到的图像帧。
        return self.grabbed, self.frame

    # 停止后台线程中的循环。
    def stop(self):
        self.stopped = True

    # 释放资源并停止视频流捕捉。
    def release(self):
        self.stop() # self.stop()：调用 stop 方法，设置 self.stopped 为 True，停止后台线程。
        self.stream.release() # self.stream.release()：释放视频流资源，关闭摄像头。

    ## 用于识别二维码，需要双摄像头，或者机械旋转摄像头
    # def qr_scan(self):
    #     while True:
    #         task1, task2 = Vision.qr_scan(frame=self.frame)
    #         if task1 is not None and task2 is not None:
    #             return task1, task2

    def target_scan(self, mod):
        """
        查找最接近中心的目标颜色及坐标偏移值
        :param mod: mod: “wk” 扫描物块 “sz”扫描十字
        :return: 最接近中心的目标颜色及坐标偏移值
        """

        center=self.center_wk if mod == "wk" else self.center_sz
        print(mod)
        while True:
            boxes,classes = self.onnx_wk.onnx_detect(frame=self.frame) if mod == "wk" else self.onnx_sz.onnx_detect(frame=self.frame)
            color, pos = Vision.find_closest_rectangle(boxes, classes, center, center)
            print(f"target_scan{color} {pos}")
            if color and pos is not None:
                return color,pos

    def line_scan(self):
        while True:
            angle, dis = Vision.line_scan(frame=self.frame)
            if angle is not None and dis is not None:
                print(f"line_scan {angle} {dis}")
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
            boxes, classes = self.onnx_wk.onnx_detect(frame=self.frame) if mod == "wk" else self.onnx_sz.onnx_detect(frame=self.frame)
            pos = Vision.find_target_color(boxes, classes, center,target_color)
            if pos is not None:
                print(f"target_scan_color {pos}")
                return pos

if __name__ == '__main__':
    camera0 = Camera(0,(320,240),(320,240),model=True)
    t = time.time()
    while True:
        print(f"fps: {1/(time.time()-t)}")
        t = time.time()

        print(camera0.target_scan("sz"))
        # print(camera0.target_scan_by_color(2,"sz"),"1")