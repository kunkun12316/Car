import onnxruntime as ort
import numpy as np
import cv2
import sys
import time

class Onnx:
    def __init__(self, model_path='model.onnx', obj=0.25, nms=0.45, obj_class=("red", "green", "blue")):
        """
        :param model_path:模型路径
        :param obj:目标检测的置信度阈值（OBJ_THRESH），默认值为 0.25
        :param nms:非最大抑制的阈值（NMS_THRESH），默认值为 0.45
        :param obj_class:目标检测的类别名称
        """
        self.model_path = model_path
        self.OBJ_THRESH = obj
        self.NMS_THRESH = nms
        self.IMG_SIZE = 640
        self.CLASSES = obj_class

        # Initialize ONNX model
        self.session = ort.InferenceSession(self.model_path, providers=['CPUExecutionProvider']) # 使用 ONNX Runtime 加载模型，并选择 CUDA 和 CPU 作为推理的执行提供者。
        self.input_name = self.session.get_inputs()[0].name # 获取模型的输入名，用于后续推理


    def onnx_detect(self, frame):

        img = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        img = cv2.resize(img, (self.IMG_SIZE, self.IMG_SIZE))
        img = img.astype('float32')
        img /= 255.0
        img = np.transpose(img, (2, 0, 1))  # HWC to CHW
        img = np.expand_dims(img, axis=0)

        start_time = time.time()
        # Existing code for preprocessing and inference
        outputs = self.session.run(None, {self.input_name: img})
        print(f"output : {outputs}")
        end_time = time.time()
        print(f"Inference time: {end_time - start_time:.4f} seconds")

