from sympy import false

import Uart

if __name__ == '__main__':
    uart = Uart.Uart()

    uart.car_ready()
    
    #uart.uart_read_data_alone()
    #uart.uart_raed_test()

    #uart.car_qr_read()
    #task = uart.uart_read_data()

    #uart.uart_send_command(task_id=1, param1=10, param2=83, wait=True)
    #uart.uart_send_command(task_id=1, param1=0, param2=65, wait=False)
    
    #uart.uart_send_command(task_id=5, param1=1, param2=1, wait=false)

    #uart.car_ready()
    uart.arm_move_angle(angle="out")
    #uart.car_move_xy_cm(10,50)

    uart.closs()
    print("end off")
