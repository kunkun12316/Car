import Uart

if __name__ == '__main__':
    uart = Uart.Uart()

    uart.car_ready()
    uart.arm_move_angle(angle="out")
    uart.car_move_xy_cm(10,50)

    uart.closs()
    print("end off")
