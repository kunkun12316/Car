import Car
import time

if __name__ == '__main__':
    car = Car.Car(com="/dev/ttyS0", cam=0, correct_times=5)

    car.car_ready()

    time.sleep(1)
    
    # 前往二维码区
    car.move_to_qr()
    car.qr_task()

    time.sleep(0.5)

    # 前往原料区
    car.move_to_material()
    time.sleep(0.5)
    car.material_first_times_task()

    time.sleep(1)

    # 前往粗加工区
    car.move_to_temp()
    car.temp_first_times_task()

    # 前往暂存区
    car.move_to_store()
    car.store_first_times_task()

    # 返回原料区
    car.back_to_material()
    car.material_second_times_task()

    # 前往粗加工区
    car.move_to_temp()
    car.temp_second_times_task()

    # 前往暂存区
    car.move_to_store()
    car.store_second_times_task()

    # 返回起点
    car.back_to_begin()
    del car

