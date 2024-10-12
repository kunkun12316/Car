import cv2
import onnxruntime as ort
import numpy as np
import Onnx as Onnx



def main():
    model_path = "sh.onnx"  # 替换成你的ONNX模型路径
    session = ort.InferenceSession(model_path)

    input_name = session.get_inputs()[0].name
    output_name = session.get_outputs()[0].name



    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Error: Cannot open the camera")
        return

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Error: Can't receive frame (stream end?). Exiting ...")
            break

        img = cv2.resize(frame, (640, 640))  # 模型输入的尺寸
        img = img.transpose(2, 0, 1)  # HWC -> CHW
        img = img[np.newaxis, :].astype(np.float32) / 255.0  # 扩展维度并归一化

        myonnx = Onnx.Onnx(model_path)

        myonnx.onnx_detect(img)

        # 输出输入图片维度调试信息
        print(f"Input image shape: {img.shape}")

        results = session.run([output_name], {input_name: img})

        cv2.imshow('Detection', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
