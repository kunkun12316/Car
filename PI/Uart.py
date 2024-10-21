import time
import serial
from sympy import false


class Uart:
	def __init__(self, com="/dev/ttyS0"):
		# 初始化串口对象
		self.uart = serial.Serial(com, 115200, 8, 'N', 1, timeout=0.01)
		if self.uart.is_open:
			print("uart is open")

	def closs(self):
		self.uart.close()

	def uart_car_ready(self, wait=True):
		print("car is ready")
		#self.uart_send_command(task_id=99, param1=0, param2=0, wait=wait, timeout=99999)
		self.wait_for_32_ack(99,99999)

	def uart_send_command(self, task_id, param1, param2, wait=True, timeout=10):
		print(f"@{task_id:02d}!{param1:+04d}|{param2:+04d}#")
		self.uart.write(f"@{task_id:02d}!{param1:+04d}|{param2:+04d}#\n".encode("utf-8"))
		time.sleep(0.1)
		print(f"Send OK")
		if wait is True:
			print(f"wait task_id")
			if task_id == 9:
				print(f"timeout task_id 9 , timeout 1")
				self.wait_for_32_ack(task_id, timeout=1)
			if task_id == 7:
				print(f"timeout task_id 7 , timeout 1")
				self.wait_for_32_ack(task_id, timeout=3)
			else:
				self.wait_for_32_ack(task_id, timeout=timeout)


	def wait_for_32_ack(self, task_id, timeout=10):
		"""
		等待通过 UART（通用异步收发传输器）接口接收到特定的确认信号（ACK）。
		:param task_id: 任务代号
		:param timeout: 默认超时时间为 10 秒
		:return:
		"""
		time_begin = time.time()  # 记录当前时间
		time_end = time.time() + timeout  # 计算超时时间
		task = [task_id]  # 将预期确认信号转换为列表(task_id)

		while task:  # 进入循环，直到所有确认信号接收到或超时
			if time.time() > time_end:  # 检查是否超时
				print("time out")
				break  # 超时跳出循环

			if self.uart.in_waiting >= 2:
				rx_buf = self.uart.read(2)
				try:
					rx_buf_str = rx_buf.decode("utf-8")
					ack = int(rx_buf_str)
					print(f"get ack:{ack}")
				except UnicodeDecodeError:
					print(f"Failed to decode: {rx_buf}")
					continue
				except ValueError:
					print(f"Failed to convert to integer: {rx_buf_str}")
					continue

				if ack in task:  # 如果接收到的确认信号在任务列表中
					print(f"uart get {ack} ok")
					task.remove(ack)  # 从任务列表中移除该确认信号
			else:
				time.sleep(0.02)  # 没有数据可读时暂停 20 毫秒

		print(f"get ack {task_id}")  # 打印所有预期的确认信号

	def uart_raed_test(self):
		while True:
			if self.uart.in_waiting > 0:
				rx_buf = self.uart.read(1)
				print(f"Raw data received: {rx_buf}")
			else:
				time.sleep(0.5)

	def uart_read_data(self, wait=True):
		while True:
			if self.uart.in_waiting >= 9:
				data = self.uart.read(9)  # 读取9字节的数据
				print(f"get data: {data}")

				start_data = data[0:2]  # 取前两个字节
				valid_data_1 = data[2:5]  # 取有效数据1（3字节）
				valid_data_2 = data[5:8]  # 取有效数据2（3字节）
				end_data = data[8]  # 取结束字节

				# 检查起始位和结束位
				if start_data != b'\x3B\xB3' or end_data != 0x6B:
					print("Invalid QR data!")
					continue

				# 返回处理后的数据
				task_data = [[valid_data_1[i] for i in range(3)],
							[valid_data_2[i] for i in range(3)]]

				print(f"Qr Data {task_data}")

				return task_data  # 返回解析后的任务数据

	def uart_read_data_alone(self):
		while True:
			# 检查是否有数据可读，并且大于2字节
			if self.uart.in_waiting > 2:
				# 读取所有可用数据
				data = self.uart.read(self.uart.in_waiting)
				print(f"Received data: {data}")

			# 等待一小段时间，避免过度占用CPU资源
			time.sleep(0.1)

	def car_move_xy_cm(self, x, y, wait=True):
		print(f"car X{x} Y{y}")
		self.uart_send_command(task_id=1, param1=x, param2=y, wait=wait)

	def car_move_xy_mm(self, x, y, wait=False):
		print(f"car X_mm {x} Y_mm {y}")
		self.uart_send_command(task_id=9, param1=x, param2=y, wait=wait)

	def car_qr_read(self, wait=True):
		print("car qr read")
		self.uart_send_command(task_id=11, param1=0, param2=0, wait=false)

	def car_move_calibration(self, wait=True):
		"""
		车身角度回正
		:param wait:
		:return:
		"""
		print(f"car_move_calibration")
		self.uart_send_command(task_id=7, param1=0, param2=0, wait=wait)

	def car_move_interval(self, length, wait=False):
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

	# 高位0 低位1
	# 内位0 外位1
	def arm_move_height(self, height, wait=True):
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
		:param height: 转盘位 高1 暂存区 低0
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
