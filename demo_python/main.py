import Car
#center 物块偏左上需要XY减小，物块偏右下需要XY增大
if __name__ == "__main__":
    # 实例化car对象
    car = Car.Car(com='/dev/ttyUSB0', camG=3, camF=1, correct_times=3, center_sz=(238, 246) ,center_wk=(238, 230))

    car.car_ready()

    # 前往二维码区
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
