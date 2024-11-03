import time
import Uart
import Camera

class Car:
    def __init__(self, com: str, cam: int, correct_times: int,center_sz=(290, 220) ,center_wk=(270, 210)):
        """
        :param com: 串口号
        :param cam: 摄像头
        :param correct_times: 校准次数
        :param center_sz:
        :param center_wk:
        """
        self.task = [[1,2,3] , [1,2,3]]
        self.color_temp = [[2,2,2] , [1,2,3]]
        self.pos_now = 0
        self.correct_times = correct_times
        self.uart = Uart.Uart(com)
        self.camera = Camera.Camera(cam,center_sz,center_wk,True)

    def __del__(self):
        self.uart.closs()
        self.camera.close()

    def car_ready(self):
        self.uart.uart.flushOutput() # 确保缓冲区中的所有数据都被传送到接收端
        self.uart.uart_car_ready()
        #self.uart.arm_move_angle(angle="out", wait=True)

    def move_to_qr(self):
        """
        车辆行驶到二维码区域
        :return:
        """
        time.sleep(0.5)
        self.uart.car_move_xy_cm(-21, 0, True)
        self.uart.car_move_xy_cm(0, 65, True)
        self.uart.uart_read_move_data(2)
        
        # self.uart.arm_move_angle(angle="in", wait=True)

    def qr_task(self):
        """
        等待接收STM32返回二维码数据
        :return:
        """
        self.uart.car_qr_read()
        self.task = self.uart.uart_read_data()


    def move_to_material(self):
        """
        车辆行驶到原料区域
        :return:
        """
        self.camera.center_wk = (270, 235)

        self.uart.arm_move_angle(angle="out", wait=False)
        #self.uart.arm_move_height(height="high", wait=True)
        self.uart.car_move_xy_cm(12,0)
        self.uart.car_move_xy_cm(0,83)

    def material_skip(self, color_skip):
        """
        用于根据目标颜色和位置决定是否需要等待
        :param color_skip: 目标颜色
        :return:
        """
        color, pos = self.camera.target_scan("wk")
        if color_skip == color:
            if pos[0] >= -270 or pos[1] <= 180:
                print("waiting")
                time.sleep(10)

    def material_task(self, task_id: int):
        """
        原料区任务
        :param task_id: 任务号
        """
        print("material_task")
        # 进行校准，对准原料区。防止偏移过大
        self.material_correct()
        # 若第一次扫到为第一个目标颜色，延时跳过（防止漏抓）。
        self.material_skip(self.task[task_id][0])
        # 遍历任务，若扫到颜色为目标颜色，进行抓取。
        for dict_id, color in enumerate(self.task[task_id]):
            while True:
                if color == self.camera.target_scan("wk")[0]:
                    if dict_id != 2:
                        self.uart.arm_upload(dict_id,"high")
                    else: # 若是最后一个物块，在物块放置下去后直接进行下一个动作（无需等待升到最高）
                        self.uart.arm_upload(dict_id,"high",False)
                        self.uart.wait_for_32_ack(85,10)
                    break

    def material_first_times_task(self):
        """
        第一次到达原料区任务
        """
        print("material_first_times_task")
        self.material_task(0)

    def material_second_times_task(self):
        """
        第二次到达原料区任务 载物盘位置
        """
        print("material_second_times_task")
        self.material_task(1)

    def line_correct(self):
        """
        用于调整机器人根据扫描的线条来校正方向和位置
        :return:
        """
        for index in range(self.correct_times - 2):
            angle, dis = self.camera.line_scan()
            self.uart.car_move_angle(int(angle / 2))
            self.uart.car_move_xy_mm(int((240 - dis) / 10), 0)

    def material_correct(self):
        """
        用于调整物料的位置，等待物块不在移动
        :return:
        """
        last_pos = self.camera.target_scan("wk")[1]
        last_pos_x = last_pos[0]
        last_pos_y = last_pos[1]
        time.sleep(0.2)  # 等待一段时间后再次扫描，判断物块是否发生移动。
        while True:
            now_pos = self.camera.target_scan("wk")[1]
            now_pos_x = now_pos[0]
            now_pos_y = now_pos[1]
            if abs(last_pos_x - now_pos_x) >= 10 or abs(last_pos_y - now_pos_y) >= 10:  # 若物块移动，等待至物块静止
                last_pos_x = now_pos_x
                last_pos_y = now_pos_y
                time.sleep(0.2)
            else:
                for index in range(1):  # 对原料区进行校准
                    pos = self.camera.target_scan("wk")[1]
                    self.uart.car_move_xy_mm(int((pos[1]) / 5), int((pos[0]) / 5))
                    time.sleep(0.2)
                break

    def place_correct_temp(self, mod: str, color=1):
        """
        圆心校准粗加工区
        """
        print("place_correct start !")
        for index in range(self.correct_times):
            print(f"place_correct {index}")
            if color != 1:
                pos = self.camera.target_scan_by_color(color,
                                                        "wk") if mod == "wk" else self.camera.target_scan_by_color(
                    color, "sh")
            else:
                pos = self.camera.target_scan("wk")[1] if mod == "wk" else self.camera.target_scan("sh")[1]
            self.uart.car_move_xy_mm(int((pos[1]) / 5), int((pos[0]) / 5))
            time.sleep(0.2)

    def place_correct_store(self, mod: str, color=3):
        """
        圆心校准暂存区
        """
        print("place_correct start !")
        for index in range(self.correct_times):
            print(f"place_correct {index}")
            if color != 3:
                pos = self.camera.target_scan_by_color(color,
                                                        "wk") if mod == "wk" else self.camera.target_scan_by_color(
                    color, "sh")
            else:
                pos = self.camera.target_scan("wk")[1] if mod == "wk" else self.camera.target_scan("sh")[1]
            self.uart.car_move_xy_mm(int((pos[1]) / 5), int((pos[0]) / 5))
            time.sleep(0.2)

    def place_correct_move(self, mod: str, color : int):
        """
        圆心校准暂存区
        """
        print("place_correct start !")
        for index in range(self.correct_times):
            print(f"place_correct {index}")
            if color != 0:
                pos = self.camera.target_scan_by_color(color,
                                                        "wk") if mod == "wk" else self.camera.target_scan_by_color(
                    color, "sh")
            else:
                pos = self.camera.target_scan("wk")[1] if mod == "wk" else self.camera.target_scan("sh")[1]
            self.uart.car_move_xy_mm(int((pos[1]) / 5), int((pos[0]) / 5))
            time.sleep(0.3)
    
    def place_with_correct(self, mod: str, disc_id: int, height: str, color: int=0):
        """
        校准后放置
        :param color: 目标颜色
        :param mod: 模型扫描模式
        :param disc_id:载物台位置号
        :param height:放置高度
        """
        self.place_correct_temp(mod=mod, color=color)
        time.sleep(1)
        self.uart.arm_download(disc_id, height,False)
        self.uart.wait_for_32_ack(96,6) # 等待夹爪松开

    def move_to_temp(self):
        """
        车辆行驶到暂存区域
        """
        
        
        self.camera.center_sz = (290, 240)
        self.uart.car_move_xy_cm(-5, 0)
        self.uart.car_move_xy_cm(0, 42)
        

        self.uart.car_move_angle(90)
        time.sleep(2)
        self.uart.car_move_calibration()
        time.sleep(1)

        self.uart.car_move_xy_cm(5, 0)
        self.uart.car_move_xy_cm(0, 175)
        time.sleep(2)
        self.uart.car_move_angle(90)
        time.sleep(2)
        self.uart.car_move_calibration()
        time.sleep(1)

        self.uart.car_move_xy_cm(10, 0)
        self.uart.car_move_xy_cm(0, 70)
        self.uart.car_move_calibration()
        time.sleep(1)



    def download_function_first_times_to_place_temp(self,task_id: int, temp_id: int):
        """
        到达粗加工区位置放置物料
        :param task_id: 任务号
        :param temp_id: 区域号
        """
        self.pos_now = 0
        color_now = self.camera.target_scan("sh")[0]
        print(f"color_now {color_now}")
		
        self.color_temp[temp_id][0] = color_now
        if color_now == self.task[task_id][0]: # 若第一个点的颜色为任务的第一个颜色
            self.place_with_correct("sh",self.task[task_id][0] - 1,"low",self.task[task_id][0])
            self.uart.car_move_interval(1)
            self.uart.wait_for_32_ack(1,1) # 等待走完15cm和机械臂到达最高点
            color_now = self.camera.target_scan("sh")[0]
            self.color_temp[temp_id][1] = color_now # 这里可能需要修改，与自己训练的模型代号相关，如果是1，2，3则不需要修改，反之0，1，2则要改为5
            self.color_temp[temp_id][2] = (6 - self.task[task_id][0] - self.color_temp[temp_id][1])

            if color_now == self.task[task_id][1]:
                self.place_with_correct("sh", self.task[task_id][1] - 1, "low", self.task[task_id][1])
                self.uart.car_move_interval(1)
                self.uart.wait_for_32_ack(1, 1)
                self.place_with_correct("sh", self.task[task_id][2] - 1, "low", self.task[task_id][2])
                self.pos_now = 2
            else:

                self.uart.car_move_interval(1)
                self.place_with_correct("sh", self.task[task_id][1] - 1, "low", self.task[task_id][1])
                self.uart.car_move_interval(-1)
                self.uart.wait_for_32_ack(1, 1)
                self.place_with_correct("sh", self.task[task_id][2] - 1, "low", self.task[task_id][2])
                self.pos_now = 1
        else: # 第一个点的颜色不是任务的第一个颜色
            self.uart.car_move_interval(1)
            color_now = self.camera.target_scan("sh")[0]
            self.color_temp[temp_id][1] = color_now
            self.color_temp[temp_id][2] = (6 - self.color_temp[temp_id][0] - self.color_temp[temp_id][1])

            if color_now == self.task[task_id][0]:
                self.place_with_correct("sh", self.task[task_id][1] - 1, "low", self.task[task_id][0])
                if self.color_temp[temp_id][0] == self.task[task_id][1]:
                    self.uart.car_move_interval(-1)
                    self.place_with_correct("sh", self.task[task_id][1] - 1, "low", self.task[task_id][1])

                    self.uart.car_move_interval(2)
                    self.place_with_correct("sh", self.task[task_id][2] - 1, "low", self.task[task_id][2])

                    self.pos_now = 2
                else:
                    self.uart.car_move_interval(1)
                    self.place_with_correct("sh", self.task[task_id][1] - 1, "low", self.task[task_id][1])

                    self.uart.car_move_interval(-2)
                    self.place_with_correct("sh", self.task[task_id][2] - 1, "low", self.task[task_id][2])
                    self.pos_now = 0
            else:
                self.uart.car_move_interval(-1)

                self.place_with_correct("sh", self.task[task_id][0] - 1, "low", self.task[task_id][0])
                if self.task[task_id][1] == self.color_temp[temp_id][0]:
                    self.uart.car_move_interval(2)
                    self.place_with_correct("sh", self.task[task_id][1] - 1, "low", self.task[task_id][1])

                    self.uart.car_move_interval(-1)
                    self.place_with_correct("sh", self.task[task_id][2] - 1, "low", self.task[task_id][2])
                    self.pos_now = 1
                else:
                    self.uart.car_move_interval(1)
                    self.place_with_correct("sh", self.task[task_id][1] - 1, "low", self.task[task_id][1])

                    self.uart.car_move_interval(1)
                    self.place_with_correct("sh", self.task[task_id][2] - 1, "low", self.task[task_id][2])
                    self.pos_now = 0

    def download_function_first_times_to_place_store(self, task_id: int, temp_id: int):
        """
        到达暂存区位置放置物料
        :param task_id: 任务号
        :param temp_id: 区域号
        """
        self.pos_now = 0
        color_now = self.camera.target_scan("sh")[0]
        print(f"color_now {color_now}")

        self.color_temp[temp_id][0] = color_now
        if color_now == self.task[task_id][0]:  # 若第一个点的颜色为任务的第一个颜色
            time.sleep(1)
            self.place_with_correct("sh", self.task[task_id][0] - 1, "low", self.task[task_id][0])
            self.uart.car_move_interval(-1)
            self.uart.wait_for_32_ack(1, 2)  # 等待走完15cm和机械臂到达最高点
            color_now = self.camera.target_scan("sh")[0]
            self.color_temp[temp_id][1] = color_now  # 这里可能需要修改，与自己训练的模型代号相关，如果是1，2，3则不需要修改，反之0，1，2则要改为5
            self.color_temp[temp_id][2] = (6 - self.task[task_id][0] - self.color_temp[temp_id][1])

            if color_now == self.task[task_id][1]:
                time.sleep(1)
                self.place_with_correct("sh", self.task[task_id][1] - 1, "low", self.task[task_id][1])
                time.sleep(1)
                self.uart.car_move_interval(-1)
                self.uart.wait_for_32_ack(1, 1)
                self.place_with_correct("sh", self.task[task_id][2] - 1, "low", self.task[task_id][2])
                self.pos_now = 2
            else:
                time.sleep(1)
                self.uart.car_move_interval(-1)
                time.sleep(1)
                self.place_with_correct("sh", self.task[task_id][1] - 1, "low", self.task[task_id][1])
                self.uart.car_move_interval(1)
                self.uart.wait_for_32_ack(1, 1)
                self.place_with_correct("sh", self.task[task_id][2] - 1, "low", self.task[task_id][2])
                self.pos_now = 1
        else:  # 第一个点的颜色不是任务的第一个颜色
            time.sleep(1)
            self.uart.car_move_interval(-1)
            time.sleep(1)
            color_now = self.camera.target_scan("sh")[0]
            self.color_temp[temp_id][1] = color_now
            self.color_temp[temp_id][2] = (6 - self.color_temp[temp_id][0] - self.color_temp[temp_id][1])

            if color_now == self.task[task_id][0]:
                self.place_with_correct("sh", self.task[task_id][0] - 1, "low", self.task[task_id][0])
                if self.color_temp[temp_id][0] == self.task[task_id][1]:
                    time.sleep(0.5)
                    self.uart.car_move_interval(1, False)
                    time.sleep(0.5)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(0.5)
                    self.place_with_correct("sh", self.task[task_id][1] - 1, "low", self.task[task_id][1])

                    self.uart.car_move_interval(-2, False)
                    time.sleep(1)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(0.3)
                    self.place_with_correct("sh", self.task[task_id][2] - 1, "low", self.task[task_id][2])

                    self.pos_now = 1
                else:
                    time.sleep(0.5)
                    self.uart.car_move_interval(-1, False)
                    time.sleep(0.5)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(0.5)
                    self.place_with_correct("sh", self.task[task_id][1] - 1, "low", self.task[task_id][1])

                    time.sleep(0.5)
                    self.uart.car_move_interval(2, False)
                    time.sleep(1)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(1)
                    self.place_with_correct("sh", self.task[task_id][2] - 1, "low", self.task[task_id][2])
                    self.pos_now = 2
            else:
                time.sleep(1)
                self.uart.car_move_interval(-1,True)
                time.sleep(1)
                self.place_correct_store("sh",1)
                self.place_with_correct("sh", self.task[task_id][0] - 1, "low", self.task[task_id][0])
                if self.task[task_id][1] == self.color_temp[temp_id][0]:
                    time.sleep(1)
                    self.uart.car_move_interval(2, False)
                    time.sleep(1)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(1)
                    self.place_with_correct("sh", self.task[task_id][1] - 1, "low", self.task[task_id][1])

                    time.sleep(1)
                    self.uart.car_move_interval(-1, False)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(1)
                    self.place_with_correct("sh", self.task[task_id][2] - 1, "low", self.task[task_id][2])
                    self.pos_now = 1
                else:
                    time.sleep(1)
                    self.uart.car_move_interval(1, False)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(1)
                    self.place_with_correct("sh", self.task[task_id][1] - 1, "low", self.task[task_id][1])

                    time.sleep(1)
                    self.uart.car_move_interval(1, False)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(1)
                    self.place_with_correct("sh", self.task[task_id][2] - 1, "low", self.task[task_id][2])
                    self.pos_now = 2

    def download_function_second_times_to_place_temp(self,task_id: int, temp_id: int,height: str):
        """
        第二次到达某位置放置物料
        :param task_id: 任务号
        :param temp_id: 区域号
        :param height: 放置高度（主要为堆叠设置）
        """
        mod = "wk" if height == "high" else "sz"
        self.pos_now = 0
        # 遍历寻找对应颜色的区域并且放置
        for task_index, color_task in enumerate(self.task[task_id]):
            for list_index, color_temp in enumerate(self.color_temp[temp_id]):
                if color_temp == color_task:
                    # 前往寻找到的位置
                    time.sleep(1)
                    self.uart.car_move_interval( list_index - self.pos_now )
                    time.sleep(1)
                    self.place_with_correct(mod, task_index, height, color_temp)
                    time.sleep(2)
                    # 更新当前的位置
                    self.pos_now = list_index

    def download_function_second_times_to_place_store(self,task_id: int, temp_id: int,height: str):
        """
        第二次到达某位置放置物料
        :param task_id: 任务号
        :param temp_id: 区域号
        :param height: 放置高度（主要为堆叠设置）
        """
        mod = "wk" if height == "high" else "sz"

        self.pos_now = 0
        color_now = self.camera.target_scan(mod)[0]
        print(f"color_now {color_now}")

        self.color_temp[temp_id][0] = color_now
        if color_now == self.task[task_id][0]:  # 若第一个点的颜色为任务的第一个颜色
            time.sleep(1)
            self.place_with_correct(mod, self.task[task_id][0] - 1, height, self.task[task_id][0])
            self.uart.car_move_interval(-1)
            self.uart.wait_for_32_ack(1, 2)  # 等待走完15cm和机械臂到达最高点
            color_now = self.camera.target_scan(mod)[0]
            self.color_temp[temp_id][1] = color_now  # 这里可能需要修改，与自己训练的模型代号相关，如果是1，2，3则不需要修改，反之0，1，2则要改为5
            self.color_temp[temp_id][2] = (6 - self.task[task_id][0] - self.color_temp[temp_id][1])

            if color_now == self.task[task_id][1]:
                time.sleep(1)
                self.place_with_correct(mod, self.task[task_id][1] - 1, height, self.task[task_id][1])
                time.sleep(1)
                self.uart.car_move_interval(-1)
                self.uart.wait_for_32_ack(1, 1)
                self.place_with_correct(mod, self.task[task_id][2] - 1, height, self.task[task_id][2])
                self.pos_now = 2
            else:
                time.sleep(1)
                self.uart.car_move_interval(-1)
                time.sleep(1)
                self.place_with_correct(mod, self.task[task_id][1] - 1, height, self.task[task_id][1])
                self.uart.car_move_interval(1)
                self.uart.wait_for_32_ack(1, 1)
                self.place_with_correct(mod, self.task[task_id][2] - 1, height, self.task[task_id][2])
                self.pos_now = 1
        else:  # 第一个点的颜色不是任务的第一个颜色
            self.uart.car_move_interval(-1)
            time.sleep(1)
            color_now = self.camera.target_scan(mod)[0]
            self.color_temp[temp_id][1] = color_now
            self.color_temp[temp_id][2] = (6 - self.color_temp[temp_id][0] - self.color_temp[temp_id][1])

            if color_now == self.task[task_id][0]:
                self.place_with_correct(mod, self.task[task_id][0] - 1, height, self.task[task_id][0])
                if self.color_temp[temp_id][0] == self.task[task_id][1]:
                    time.sleep(0.5)
                    self.uart.car_move_interval(1, False)
                    time.sleep(0.5)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(0.5)
                    self.place_with_correct(mod, self.task[task_id][1] - 1, height, self.task[task_id][1])

                    self.uart.car_move_interval(-2, False)
                    time.sleep(1)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(0.3)
                    self.place_with_correct(mod, self.task[task_id][2] - 1, height, self.task[task_id][2])

                    self.pos_now = 1
                else:
                    time.sleep(0.5)
                    self.uart.car_move_interval(-1, False)
                    time.sleep(0.5)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(0.5)
                    self.place_with_correct(mod, self.task[task_id][1] - 1, height, self.task[task_id][1])

                    time.sleep(0.5)
                    self.uart.car_move_interval(2, False)
                    time.sleep(1)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(1)
                    self.place_with_correct(mod, self.task[task_id][2] - 1, height, self.task[task_id][2])
                    self.pos_now = 2
            else:
                time.sleep(0.5)
                self.uart.car_move_interval(-1)
                time.sleep(1)
                self.place_with_correct(mod, self.task[task_id][0] - 1, height, self.task[task_id][0])
                if self.task[task_id][1] == self.color_temp[temp_id][0]:
                    time.sleep(1)
                    self.uart.car_move_interval(2, False)
                    time.sleep(1)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(1)
                    self.place_with_correct(mod, self.task[task_id][1] - 1, height, self.task[task_id][1])

                    time.sleep(1)
                    self.uart.car_move_interval(-1, False)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(1)
                    self.place_with_correct(mod, self.task[task_id][2] - 1, height, self.task[task_id][2])
                    self.pos_now = 1
                else:
                    time.sleep(1)
                    self.uart.car_move_interval(1, False)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(1)
                    self.place_with_correct(mod, self.task[task_id][1] - 1, height, self.task[task_id][1])

                    time.sleep(1)
                    self.uart.car_move_interval(1, False)
                    self.uart.wait_for_32_ack(1, 1)
                    time.sleep(1)
                    self.place_with_correct(mod, self.task[task_id][2] - 1, height, self.task[task_id][2])
                    self.pos_now = 2


    def upload_function(self, task_id: int):
        """
        装载物块
        :param task_id: 任务号
        """
        for task_index, color_task in enumerate(self.task[task_id]):
            for list_index, color_temp in enumerate(self.color_temp[task_id]):
                if color_temp == color_task:
                    time.sleep(1)
                    self.uart.car_move_interval(list_index - self.pos_now) # # 每一次行走一段15cm过后
                    self.uart.wait_for_32_ack(10,3)
                    if task_index != 3: # 到达第一个目标，对第一个靶心进行校准，保证后续能直接抓取物块而节省了校准时间
                        self.place_correct_temp("wk", color_temp)
                        print("up load correct")
                    else:
                        time.sleep(1)

                    self.uart.arm_upload(task_index, "low", False) # 装载
                    if task_index != 2: # 若不是最后一个物块，等待95返回，即成功抓起后
                        self.uart.wait_for_32_ack(95,5)
                    else: # 若是最后一个物块，等待85返回，即成功装在托盘后，小车移动
                        self.uart.wait_for_32_ack(85,5)
                    self.pos_now = list_index  # 更新当前位置，用于遍历颜色序列

    def temp_first_times_task(self):
        """
        第一次到达暂存区任务
        """
        print("temp_first_times_download")
        # 放置

        self.place_correct_temp("sh")
        self.download_function_first_times_to_place_temp(0,0)
        print("temp_first_times_upload")
        self.uart.car_move_calibration()
        time.sleep(1)

        #装载
        self.upload_function(0)
        

    def temp_second_times_task(self):
        """
        第二次到达暂存区任务
        """

        self.camera.center_sz = (290, 235)
        self.place_correct_temp("sz")
        print("temp_second_times_download")
        # 放置

        print(f"Task_ID : {self.task}")
        print(f"Temp_ID : {self.color_temp}")
        
        self.download_function_first_times_to_place_temp(1,0)
        print("temp_second_times_upload")
        self.uart.car_move_calibration()
        time.sleep(1)

        # 装载
        self.upload_function(1)
        
    def move_to_store(self):
        """
        车辆行驶到存放区域
        """
        self.uart.car_move_calibration()
        time.sleep(1)

        self.uart.car_move_xy_cm(-10,0 )
        self.uart.car_move_xy_cm(0, -70 + -15 * self.pos_now )
        self.uart.car_move_angle(-90)
        time.sleep(2)

        self.uart.car_move_calibration()
        time.sleep(1)


        self.uart.car_move_xy_cm(10, 0)
        self.uart.car_move_xy_cm(0, -60)
        self.uart.car_move_calibration()
        time.sleep(1)


    def store_first_times_task(self):
        """
        第一次到达装载区任务
        """
        self.camera.center_sz = (290, 235)
        
        self.place_correct_move("sh",3)
        self.uart.car_move_interval(-2)
        time.sleep(1)
        self.place_correct_move("sh",1)
        self.uart.arm_download(0, 0,True)

        self.uart.car_move_interval(1)
        time.sleep(0.5)
        self.place_correct_move("sh",2)
        self.uart.arm_download(1, 0,True)

        self.uart.car_move_interval(1)
        time.sleep(0.5)
        self.place_correct_move("sh",3)
        self.uart.arm_download(2, 0,True)

        #self.download_function_first_times_to_place_store(0, 0)

    def store_second_times_task(self):
        """
        第二次到达装载区任务
        """
        
        self.camera.center_wk = (268, 250)

        self.uart.car_move_interval(-2)
        self.place_correct_move("wk",1)
        self.uart.arm_download(0, "high",True)

        self.uart.car_move_interval(1)
        self.place_correct_move("wk",2)
        self.uart.arm_download(1, "high",True)

        self.uart.car_move_interval(1)
        self.place_correct_move("wk",3)
        self.uart.arm_download(2, "high",True)
        
        
        #self.download_function_second_times_to_place_store(1, 1, "high")

    def back_to_material(self):
        """
        返回原料区
        """                
        self.camera.center_wk = (270, 235)

        self.uart.car_move_xy_cm(-10, 0)
        self.uart.car_move_xy_cm(0, -60 + (-15 * (self.pos_now + 1)))
        self.uart.car_move_angle(-90)
        time.sleep(2)

        self.uart.car_move_calibration()
        time.sleep(1)

        self.uart.car_move_xy_cm(10, 0)
        self.uart.car_move_xy_cm(0, -40)

    def back_to_begin(self):
        """
        返回起点
        """
        time.sleep(0.5)
        self.uart.car_move_xy_cm(-10, -90 + (-15 * (self.pos_now - 1)))
        
        time.sleep(0.5)
        self.uart.car_move_angle(-90)
    
        time.sleep(0.5)
        self.uart.car_move_calibration()
        time.sleep(0.5)
       
        time.sleep(0.5)
        self.uart.uart_send_command(task_id=1, param1=0, param2=-190, wait=True)
    
        time.sleep(0.5)
        self.uart.car_move_calibration()
        time.sleep(0.5)
    
        time.sleep(0.5)
        self.uart.uart_send_command(task_id=1, param1=18, param2=0, wait=True)

        print("ALL DONE")

if __name__ == "__main__":
    # 实例化car对象
    car = Car(com="/dev/serial0",cam=0,correct_times=5)

    car.move_to_qr()
    car.qr_task()
    car.move_to_material()
    car.material_first_times_task()
    car.move_to_temp()
    car.temp_first_times_task()
    car.move_to_store()
    car.store_first_times_task()
    car.back_to_material()
    car.material_second_times_task()
    car.move_to_temp()
    car.temp_second_times_task()
    car.move_to_store()
    car.store_second_times_task()
    car.back_to_begin()
