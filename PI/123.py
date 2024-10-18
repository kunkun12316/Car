import time

import cv2
import numpy as np
import argparse
import onnxruntime as ort
import math


class yolov5_lite():
    def __init__(self, model_pb_path, label_path, confThreshold=0.5, nmsThreshold=0.5, objThreshold=0.5):
        so = ort.SessionOptions()
        so.log_severity_level = 3
        self.net = ort.InferenceSession(model_pb_path, so)
        self.classes = list(map(lambda x: x.strip(), open(label_path, 'r').readlines()))
        self.num_classes = len(self.classes)
        anchors = [[10, 13, 16, 30, 33, 23], [30, 61, 62, 45, 59, 119], [116, 90, 156, 198, 373, 326]]
        self.nl = len(anchors)
        self.na = len(anchors[0]) // 2
        self.no = self.num_classes + 5
        self.grid = [np.zeros(1)] * self.nl
        self.stride = np.array([8., 16., 32.])
        self.anchor_grid = np.asarray(anchors, dtype=np.float32).reshape(self.nl, -1, 2)

        self.confThreshold = confThreshold
        self.nmsThreshold = nmsThreshold
        self.objThreshold = objThreshold
        self.input_shape = (self.net.get_inputs()[0].shape[2], self.net.get_inputs()[0].shape[3])

    def resize_image(self, srcimg, keep_ratio=True):
        top, left, newh, neww = 0, 0, self.input_shape[0], self.input_shape[1]
        if keep_ratio and srcimg.shape[0] != srcimg.shape[1]:
            hw_scale = srcimg.shape[0] / srcimg.shape[1]
            if hw_scale > 1:
                newh, neww = self.input_shape[0], int(self.input_shape[1] / hw_scale)
                img = cv2.resize(srcimg, (neww, newh), interpolation=cv2.INTER_AREA)
                left = int((self.input_shape[1] - neww) * 0.5)
                img = cv2.copyMakeBorder(img, 0, 0, left, self.input_shape[1] - neww - left, cv2.BORDER_CONSTANT,
                                         value=0)  # add border
            else:
                newh, neww = int(self.input_shape[0] * hw_scale), self.input_shape[1]
                img = cv2.resize(srcimg, (neww, newh), interpolation=cv2.INTER_AREA)
                top = int((self.input_shape[0] - newh) * 0.5)
                img = cv2.copyMakeBorder(img, top, self.input_shape[0] - newh - top, 0, 0, cv2.BORDER_CONSTANT, value=0)
        else:
            img = cv2.resize(srcimg, self.input_shape, interpolation=cv2.INTER_AREA)
        return img, newh, neww, top, left

    def _make_grid(self, nx=20, ny=20):
        xv, yv = np.meshgrid(np.arange(ny), np.arange(nx))
        return np.stack((xv, yv), 2).reshape((-1, 2)).astype(np.float32)

    def postprocess(self, frame, outs, pad_hw):
        newh, neww, padh, padw = pad_hw
        frameHeight = frame.shape[0]
        frameWidth = frame.shape[1]
        ratioh, ratiow = frameHeight / newh, frameWidth / neww
        # Scan through all the bounding boxes output from the network and keep only the
        # ones with high confidence scores. Assign the box's class label as the class with the highest score.
        classIds = []
        confidences = []
        boxes = []
        center_points = []  # 保存检测到的框的中心点
        detected_objects = []  # 保存检测到的对象类别和中心点

        for detection in outs:
            scores = detection[5:]
            classId = np.argmax(scores)
            confidence = scores[classId]
            if confidence > self.confThreshold and detection[4] > self.objThreshold:
                center_x = int((detection[0] - padw) * ratiow)
                center_y = int((detection[1] - padh) * ratioh)
                width = int(detection[2] * ratiow)
                height = int(detection[3] * ratioh)
                left = int(center_x - width / 2)
                top = int(center_y - height / 2)
                classIds.append(classId)
                confidences.append(float(confidence))
                boxes.append([left, top, width, height])
                center_points.append((center_x, center_y))  # 记录中心点

        # Perform non maximum suppression to eliminate redundant overlapping boxes with
        # lower confidences.
        indices = cv2.dnn.NMSBoxes(boxes, confidences, self.confThreshold, self.nmsThreshold)

        # Check if indices are empty, if so, return the original frame
        if len(indices) > 0:
            # Iterate through the indices and process each bounding box
            for i in indices.flatten():  # Flatten the indices to ensure they are 1D
                box = boxes[i]
                left = box[0]
                top = box[1]
                width = box[2]
                height = box[3]
                frame = self.drawPred(frame, classIds[i], confidences[i], left, top, left + width, top + height)

                # 返回检测到的框的中心点坐标
                center_x, center_y = center_points[i]
                detected_objects.append((self.classes[classIds[i]], (center_x, center_y)))
                # cv2.circle(frame, (center_x, center_y), radius=5, color=(0, 0, 255), thickness=-1)  # 红色圆圈
                # print(f"Detected {self.classes[classIds[i]]} at center ({center_x}, {center_y})")
        else:
            print("No valid boxes after NMS")
        return frame, boxes, classIds

    def drawPred(self, frame, classId, conf, left, top, right, bottom):
        # Draw a bounding box.
        cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), thickness=4)

        label = '%.2f' % conf
        label = '%s:%s' % (self.classes[classId], label)

        # Display the label at the top of the bounding box
        labelSize, baseLine = cv2.getTextSize(label, cv2.FONT_HERSHEY_SIMPLEX, 0.5, 1)
        top = max(top, labelSize[1])
        # cv.rectangle(frame, (left, top - round(1.5 * labelSize[1])), (left + round(1.5 * labelSize[0]), top + baseLine), (255,255,255), cv.FILLED)
        cv2.putText(frame, label, (left, top - 10), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), thickness=2)
        return frame

    def detect(self, srcimg):
        img, newh, neww, top, left = self.resize_image(srcimg)
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        img = img.astype(np.float32) / 255.0
        blob = np.expand_dims(np.transpose(img, (2, 0, 1)), axis=0)

        outs = self.net.run(None, {self.net.get_inputs()[0].name: blob})[0].squeeze(axis=0)
        row_ind = 0
        for i in range(self.nl):
            h, w = int(self.input_shape[0] / self.stride[i]), int(self.input_shape[1] / self.stride[i])
            length = int(self.na * h * w)
            if self.grid[i].shape[2:4] != (h, w):
                self.grid[i] = self._make_grid(w, h)

            print(f"row_ind: {row_ind}, length: {length}, outs shape: {outs.shape}")
            outs[row_ind:row_ind + length, 0:2] = (outs[row_ind:row_ind + length, 0:2] * 2. - 0.5 + np.tile(
                self.grid[i], (self.na, 1))) * int(self.stride[i])
            outs[row_ind:row_ind + length, 2:4] = (outs[row_ind:row_ind + length, 2:4] * 2) ** 2 * np.repeat(
                self.anchor_grid[i], h * w, axis=0)
            row_ind += length

        # 调用修改后的 postprocess 返回检测结果和中心点
        frame, boxes, classIds = self.postprocess(srcimg, outs, (newh, neww, top, left))

        cv2.imwrite("frame.jpg", frame)  # 保存图像

        return boxes, classIds


# def detect_objects_from_camera(modelpath, classfile, confThreshold=0.5, nmsThreshold=0.6):
#     parser = argparse.ArgumentParser()
#     parser.add_argument('--modelpath', type=str, default=modelpath, help="onnx filepath")
#     parser.add_argument('--classfile', type=str, default=classfile, help="classname filepath")
#     parser.add_argument('--confThreshold', default=0.5, type=float, help='class confidence')
#     parser.add_argument('--nmsThreshold', default=0.6, type=float, help='nms iou thresh')
#     args = parser.parse_args()
#
#     # 初始化模型
#     net = yolov5_lite(args.modelpath, args.classfile, confThreshold=args.confThreshold,
#                       nmsThreshold=args.nmsThreshold)
#
#     cap = cv2.VideoCapture(0)  # 打开摄像头
#
#     ret, frame = cap.read()
#
#     print("Capturing image...")
#     boxes, classIds = net.detect(frame)
#
#     cap.release()
#     cv2.destroyAllWindows()
#
#     return boxes, classIds


# 使用示例
if __name__ == "__main__":
    model_path = "sh.onnx"  # 模型路径
    class_file = "color.names"  # 类别文件路径

    parser = argparse.ArgumentParser()
    parser.add_argument('--modelpath', type=str, default=model_path, help="onnx filepath")
    parser.add_argument('--classfile', type=str, default=class_file, help="classname filepath")
    parser.add_argument('--confThreshold', default=0.5, type=float, help='class confidence')
    parser.add_argument('--nmsThreshold', default=0.6, type=float, help='nms iou thresh')
    args = parser.parse_args()

    # 初始化模型
    net = yolov5_lite(args.modelpath, args.classfile, confThreshold=args.confThreshold,
                      nmsThreshold=args.nmsThreshold)

    cap = cv2.VideoCapture(0)  # 打开摄像头

    print("Init over!")
    prev_time = 0  # 用于计算帧率的变量

    while True:
        ret, frame = cap.read()  # 捕获当前帧
        if not ret:
            print("Failed to grab frame")
            break

        key = cv2.waitKey(1) & 0xFF  # 获取键盘输入

        start_time = time.time()

        # 进行推理
        boxes, classIds = net.detect(frame)

        end_time = time.time()
        inference_time = end_time - start_time
        print(f"Inference Time: {inference_time:.4f} seconds")


        # 计算当前帧率
        current_time = time.time()
        fps = 1 / (current_time - prev_time)
        prev_time = current_time

        # 在视频帧上显示推理时间和帧率
        cv2.putText(frame, f"Inference Time: {inference_time:.4f} s", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
        cv2.putText(frame, f"FPS: {fps:.2f}", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)

        # 显示推理结果
        cv2.imshow('Inference', frame)

        if key == ord('q'):  # 如果按下 'q' 键，退出循环
            break

    cap.release()  # 释放摄像头资源
    cv2.destroyAllWindows()  # 关闭所有窗口



    # boxes, classIds = detect_objects_from_camera(model_path, class_file)
    # print(f"classIds : {classIds}")
    # print(f"boxes : {boxes}")
