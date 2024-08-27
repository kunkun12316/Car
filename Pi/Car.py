import time
import Uart
import cv2
import numpy as np

class Car:
    def __init__(self, com: str, cam: int, correct_times: int, center_sz=(238, 246)):
        self.task = [[0, 0, 0], [0, 0, 0]]
        self.color_temp = [[0, 0, 0], [0, 0, 0]]
        self.pos_now = 0
        self.correct_times = correct_times
        self.uart = Uart.Uart(com)