import time
import serial

class Uart:
    def __init__(self,com="/dev/serial0"):
        #初始化串口对象
        self.uart = serial.Serial(com, 115200, timeout=0.01)
        if self.uart.is_open:
            print("uart is open")

    def closs(self):
        self.uart.close()

    def car_ready(self,wart=True):
        print("car is ready")


    def uart_send_command(self,task_id,param1,param2,wait=True,timeout=10):
        print(f"@{task_id:02d}!{param1:+04d}|{param2:+04d}#")
        self.uart.write(f"@{task_id:02d}!{param1:+04d}|{param2:+04d}#".encode("utf-8"))
        time.sleep(0.1)
        if wait is True:
            self.wait_for_32_ack(timeout=timeout,task_id=task_id)

    def wait_for_32_ack(self,timeout=10,*args,**kwargs):
        """
        等待通过 UART（通用异步收发传输器）接口接收到特定的确认信号（ACK）。
        :param timeout:默认超时时间为 10 秒
        :param args:代表传入的所有位置参数，这些参数通常是预期接收到的确认信号（ACK）的 ID。
        :param kwargs:代表传入的所有关键字参数，虽然在这个函数中未使用，但可以用于扩展。
        :return:
        """
        time_begin = time.time() #记录当前时间
        time_end = time.time() + timeout #计算超时时间
        task = list(args) #将预期确认信号转换为列表(task_id)

        while task: # 进入循环，直到所有确认信号接收到或超时
            if time.time() > time_end: # 检查是否超时
                print("time out")
                break # 超时跳出循环


            if self.uart.in_waiting >= 2: # 检查UART串口是否有数据
                rx_buf = self.uart.read(2).decode("utf-8") # 读取两个字节并解码
                ack = int(rx_buf) # 将字符串转换为整数
                if ack in task: # 如果接收到的确认信号在任务列表中
                    print(f"get {ack}")
                    task.remove(ack) ## 从任务列表中移除该确认信号
            else:
                time.sleep(0.02) # 没有数据可读时暂停 20 毫秒

        print(f"get ack {args}") # 打印所有预期的确认信号


    def car_move_xy_cm(self,x,y,wait=True):
        print(f"car X{x} Y{y}")
        self.uart_send_command(task_id=1,param1=x,param2=y,wait=wait)

    def car_move_xy_mm(self, x, y, wait=True):
        print(f"car X{x} Y{y}")
        self.uart_send_command(task_id=9, param1=x, param2=y, wait=wait)


    def car_move_calibration(self, wait=True):
        """
        车身角度回正
        :param wait:
        :return:
        """
        print(f"car_move_calibration")
        self.uart_send_command(task_id=7, param1=0, param2=0, wait=wait)

    def car_move_interval(self, length, wait=True):
        """
        Y轴 150*mm移动
        :param length: 移动长度
        :param wait:
        :return:
        """
        print(f"car length {length}")
        self.car_move_xy_mm(0, 150 * length, wait=wait)

    # 车身旋转
    def car_move_angle(self, angle, wait=True):
        print(f"car angle {angle}")
        self.uart_send_command(task_id=2, param1=angle, param2=0, wait=wait)

    #  高位0 低位1
    #  内位0 外位1

    def arm_move_height(self,height,wait=True):
        """
        滑轨上下移动
        :param height: 高位0 低位1
        :param wait: 是否延时
        :return:
        """
        print(f"arm height {height}")
        height = 0 if height == "high" else 1
        self.uart_send_command(task_id=3, param1=height, param2=0, wait=wait)

    def arm_move_angle(self, angle, wait=True):
        """
        车身旋转
        :param angle: 旋转角度
        :param wait: 是否延时
        :return:
        """
        print(f"car angle {angle}")
        angle = 1 if angle == "out" else 0
        self.uart_send_command(task_id=4, param1=angle, param2=0, wait=wait)

    def arm_upload(self, disc_id, height, wait=True):
        """
        取物料
        :param disc_id: 目标颜色代号
        :param height: 高1 低0
        :param wait:
        :return:
        """
        print(f"arm upload {disc_id}")
        height = 1 if height == "high" else 0
        self.uart_send_command(task_id=5, param1=disc_id, param2=height, wait=wait)

    def arm_download(self, disc_id, height, wait=True):
        """
        放置物块
        :param disc_id:
        :param height:
        :param wait:
        :return:
        """
        print(f"arm download {height}")
        height = 1 if height == "high" else 0
        self.uart_send_command(task_id=6, param1=disc_id, param2=height, wait=wait)

    def send_lcd_tasks(self, task_list0, task_list1, wait=True):
        task0 = task_list0[0] * 100 + task_list0[1] * 10 + task_list0[2]
        task1 = task_list1[0] * 100 + task_list1[1] * 10 + task_list1[2]
        print(f"arm send_lcd_tasks {task0}+{task1}")
        self.uart_send_command(task_id=8, param1=task0, param2=task1, wait=wait)