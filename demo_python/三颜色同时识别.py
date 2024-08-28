# 识别色块、色环,并通过串口发送坐标

import sensor, image, time,pyb#包含模块
from pyb import UART

ring_thresholds = [(7, 59, 22, 110, -128, 127),  # 红阈值
                (5, 19, -95, -12, 3, 52),  # 绿阈值
                (6, 40, -21, 0, -38, -3)] # 蓝阈值
graythreshold=[(50,100)]                      #黑白阈值

#thresholds=[(0, 100, 20, 127, -128, 127),      # 红阈值
            #(0, 38, -128, -11, -128, 127),     # 绿阈值
            #(0, 100, -128, 7, -128, -18)]   # 蓝阈值

#物料阈值数组
thresholds=[(16, 64, 20, 68, -67, 60),     # 红阈值
            (27, 63, -54, -21, -128, 127),     # 绿阈值
            (6, 70, -128, 103, -95, -24)]   # 蓝阈值

uart = pyb.UART(3, 9600, timeout_char = 100)#创建串口对象
uart.init(9600, bits=8, parity=None, stop=1, timeout_char=1000) # 使用给定参数初始化

led_red=pyb.LED(1)#创建一个LED对象
led_green=pyb.LED(2)#创建一个LED对象
led_blue=pyb.LED(3)#创建一个LED对象
led_red.on()
led_green.on()
led_blue.on()

sensor.reset()# 初始化传感器
sensor.set_hmirror(True)#水平翻转
sensor.set_vflip(True)#垂直翻转
sensor.set_pixformat(sensor.RGB565)#图像格式
sensor.set_framesize(sensor.QVGA)#图像像素320x240
sensor.skip_frames(time = 2000)#跳过2000帧
#sensor.set_auto_gain(False) #关闭自动增益
#sensor.set_auto_whitebal(False) #关闭白平衡

def wedges():#识别物块颜色,处理后串口发送
    location=[[0,0],[0,0],[0,0]]
    img = sensor.snapshot()#使用相机拍摄一张照片，并返回 image 对象
    i=0
    for blob in img.find_blobs([thresholds[0]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        if blob.area()<10000 and blob.cy()<240 and blob.cy()>100:#面积判断
            location[0]=[blob.cx(),blob.cy()]
            img.draw_rectangle(blob.rect())#红
            img.draw_cross(blob.cx(), blob.cy())
            #print("面积:",blob.area())
            print("red:(",blob.cx(),",",blob.cy(),")")#打印坐标
            #uart.write('red x:'+str(blob.cx())+' y:'+str(blob.cy())+'\n')
            i+=1

    for blob in img.find_blobs([thresholds[1]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        if blob.area()<10000 and blob.cy()<240 and blob.cy()>100:#面积判断
            location[1]=[blob.cx(),blob.cy()]
            img.draw_rectangle(blob.rect())#绿
            img.draw_cross(blob.cx(), blob.cy())
            #print("面积:",blob.area())
            print("green:(",blob.cx(),",",blob.cy(),")")#打印坐标
            #uart.write('green x:'+str(blob.cx())+' y:'+str(blob.cy())+'\n')
            i+=1

    for blob in img.find_blobs([thresholds[2]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        if blob.area()<10000 and blob.cy()<240 and blob.cy()>100:#面积判断
            location[2]=[blob.cx(),blob.cy()]
            img.draw_rectangle(blob.rect())#蓝
            img.draw_cross(blob.cx(), blob.cy())
            #print("面积:",blob.area())
            print("blue:(",blob.cx(),",",blob.cy(),")")#打印坐标
            #uart.write('blue x:'+str(blob.cx())+' y:'+str(blob.cy())+'\n')
            i+=1
    #print(i)
    if i==3:
        add=(location[0][1]+location[1][1]+location[2][1])/3#计算出颜色,串口发送
        if((add-location[0][1]<15 or add-location[0][1]>-15)and\
            (add-location[1][1]<15 or add-location[1][1]>-15)and\
            (add-location[2][1]<15 or add-location[2][1]>-15)):
            if location[0][0]>location[1][0]>location[2][0]:#123
                return 123
            if location[1][0]>location[0][0]>location[2][0]:#213
                return 213
            if location[2][0]>location[1][0]>location[0][0]:#321
                return 321
            if location[0][0]>location[2][0]>location[1][0]:#132
                return 132
            if location[1][0]>location[2][0]>location[0][0]:#231
                return 231
            if location[2][0]>location[0][0]>location[1][0]:#312
                return 312
    return 0

def wedges_1():#识别物块颜色,处理后串口发送
    location=[[0,0],[0,0],[0,0]]
    img = sensor.snapshot()#使用相机拍摄一张照片，并返回 image 对象
    i=0
    for blob in img.find_blobs([thresholds[0]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        if blob.area()<10000 and blob.cy()<100 and blob.cy()>0:#面积判断
            location[0]=[blob.cx(),blob.cy()]
            img.draw_rectangle(blob.rect())#红
            img.draw_cross(blob.cx(), blob.cy())
            #print("面积:",blob.area())
            print("red:(",blob.cx(),",",blob.cy(),")")#打印坐标
            #uart.write('red x:'+str(blob.cx())+' y:'+str(blob.cy())+'\n')
            i+=1

    for blob in img.find_blobs([thresholds[1]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        if blob.area()<10000 and blob.cy()<100 and blob.cy()>0:#面积判断
            location[1]=[blob.cx(),blob.cy()]
            img.draw_rectangle(blob.rect())#绿
            img.draw_cross(blob.cx(), blob.cy())
            #print("面积:",blob.area())
            print("green:(",blob.cx(),",",blob.cy(),")")#打印坐标
            #uart.write('green x:'+str(blob.cx())+' y:'+str(blob.cy())+'\n')
            i+=1

    for blob in img.find_blobs([thresholds[2]],area_threshold=300,pixels_threshold=200, area_threshold=200, merge=True):
        if blob.area()<10000 and blob.cy()<100 and blob.cy()>0:#面积判断
            location[2]=[blob.cx(),blob.cy()]
            img.draw_rectangle(blob.rect())#蓝
            img.draw_cross(blob.cx(), blob.cy())
            #print("面积:",blob.area())
            print("blue:(",blob.cx(),",",blob.cy(),")")#打印坐标
            #uart.write('blue x:'+str(blob.cx())+' y:'+str(blob.cy())+'\n')
            i+=1
    #print(i)
    if i==3:
        add=(location[0][1]+location[1][1]+location[2][1])/3#计算出颜色,串口发送
        if((add-location[0][1]<15 or add-location[0][1]>-15)and\
            (add-location[1][1]<15 or add-location[1][1]>-15)and\
            (add-location[2][1]<15 or add-location[2][1]>-15)):
            if location[0][0]>location[1][0]>location[2][0]:#123
                return 123
            if location[1][0]>location[0][0]>location[2][0]:#213
                return 213
            if location[2][0]>location[1][0]>location[0][0]:#321
                return 321
            if location[0][0]>location[2][0]>location[1][0]:#132
                return 132
            if location[1][0]>location[2][0]>location[0][0]:#231
                return 231
            if location[2][0]>location[0][0]>location[1][0]:#312
                return 312
    return 0

def ring(a):#识别色环
    if a=="r":
        img = sensor.snapshot()#使用相机拍摄一张照片，并返回 image 对象
        img.binary([ring_thresholds[0]])#根据阈值将像素设置为黑白
        img.dilate(2)#腐蚀像素
        for blob in img.find_blobs(graythreshold,area_threshold=15000, pixels_threshold=200, area_threshold=200, merge=True):
            img.draw_rectangle(blob.rect())#红
            img.draw_cross(blob.cx(), blob.cy())
            #print("面积:",blob.area())
            print("red:(",blob.cx(),",",blob.cy(),")")#打印坐标
            return 'r'

    if a=="g":
        img = sensor.snapshot()#使用相机拍摄一张照片，并返回 image 对象
        img.binary([ring_thresholds[1]])#根据阈值将像素设置为黑白
        img.dilate(2)#腐蚀像素
        for blob in img.find_blobs(graythreshold,area_threshold=15000, pixels_threshold=200, area_threshold=200, merge=True):
            img.draw_rectangle(blob.rect())#绿
            img.draw_cross(blob.cx(), blob.cy())
            #print("面积:",blob.area())
            print("green:(",blob.cx(),",",blob.cy(),")")#打印坐标
            return 'g'

    if a=="b":
        img = sensor.snapshot()#使用相机拍摄一张照片，并返回 image 对象
        img.binary([ring_thresholds[2]])#根据阈值将像素设置为黑白
        img.dilate(2)#腐蚀像素
        for blob in img.find_blobs(graythreshold,area_threshold=15000, pixels_threshold=200, area_threshold=200, merge=True):
            img.draw_rectangle(blob.rect())#蓝
            img.draw_cross(blob.cx(), blob.cy())
            #print("面积:",blob.area())
            print("blue:(",blob.cx(),",",blob.cy(),")")#打印坐标
            return 'b'

while(True):
    img = sensor.snapshot()#使用相机拍摄一张照片，并返回 image 对象
    time.sleep(50)
    while(1):
        string=str(wedges())
        string_1=str(wedges_1())
        if string!='0'and string_1!='0':
            print(string_1,string,"\n")
            #print(string,"\n")
            uart.write(string_1+"\n")
            uart.write(string+"\r\n")
            break
    time.sleep(100)
