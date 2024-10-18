from sympy import false

import Uart

if __name__ == '__main__':
    uart = Uart.Uart()

    # uart.car_qr_read()
    # task = uart.uart_read_data()

    uart.uart_send_command(task_id=6, param1=1, param2=0, wait=false)

    # uart.car_ready()
    # uart.arm_move_angle(angle="out")
    # uart.car_move_xy_cm(10,50)

    uart.closs()
    print("end off")
