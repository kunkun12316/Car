
import time
import Car
#center 物块偏左上需要XY减小，物块偏右下需要XY增大
if __name__ == "__main__":
    # 实例化car对象
    t = time.time()
    car = Car.Car(com='/dev/ttyUSB0', camG=3, camF=1, correct_times=5,center_sz=(244, 248) ,center_wk=(238, 230))
    # 前往二维码区
    car.car_ready()

    car.move_to_qr()

    car.qr_task()
    # 前往原料区

    car.move_to_material()
    car.material_first_times_task()
    # 前往粗加工区

    car.move_to_temp()
    car.temp_first_times_task()
    # 前往暂存区

    car.move_to_store()
    # car.line_correct()
    car.store_first_times_task()
    print(car.color_temp)
    # 返回原料区
    car.back_to_material()

    car.material_second_times_task()
    # 前往粗加工区

    car.move_to_temp()
    # car.line_correct()
    car.temp_second_times_task()
    # 前往暂存区
    car.move_to_store()
    car.store_second_times_task()

    # 返回起点

    car.back_to_begin()
    print(f"spent times :{time.time() - t}")
    del car
