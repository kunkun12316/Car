import cv2
import numpy as np
import onnxruntime as ort
import time

# 模型和摄像头设置
MODEL_PATH = 'sh.onnx'
INPUT_WIDTH = 640  # 改回原来的尺寸
INPUT_HEIGHT = 640
SCORE_THRESHOLD = 0.25
NMS_THRESHOLD = 0.45
CONFIDENCE_THRESHOLD = 0.25

# 加载类别名称
CLASS_NAMES = ['Red', 'Red_Center', 'Green', 'Green_Center', 'Blue', 'Blue_Center']

# 初始化ONNX运行时会话
session = ort.InferenceSession(MODEL_PATH, providers=['CPUExecutionProvider'])

# 获取模型输入名称
input_name = session.get_inputs()[0].name


def preprocess_image(image):
    img = cv2.resize(image, (INPUT_WIDTH, INPUT_HEIGHT))
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    img = img.astype(np.float32) / 255.0
    img = img.transpose(2, 0, 1)
    img = np.expand_dims(img, axis=0)
    return img


def postprocess(output, original_image):
    predictions = np.squeeze(output[0])

    # 获取图像尺寸
    image_height, image_width = original_image.shape[:2]

    # 缩放因子
    x_factor = image_width / INPUT_WIDTH
    y_factor = image_height / INPUT_HEIGHT

    # 仅保留置信度高于阈值的检测结果
    mask = predictions[:, 4] > CONFIDENCE_THRESHOLD
    predictions = predictions[mask]

    if len(predictions) == 0:
        return []

    # 获取类别ID和置信度
    class_ids = np.argmax(predictions[:, 5:], axis=1)
    confidences = np.max(predictions[:, 5:], axis=1)

    # 获取边界框坐标
    boxes = predictions[:, :4]
    boxes[:, 0] = (boxes[:, 0] - boxes[:, 2] / 2) * x_factor
    boxes[:, 1] = (boxes[:, 1] - boxes[:, 3] / 2) * y_factor
    boxes[:, 2] = boxes[:, 2] * x_factor
    boxes[:, 3] = boxes[:, 3] * y_factor

    # 转换为整数
    boxes = boxes.astype(np.int32)

    # 非极大值抑制
    indices = cv2.dnn.NMSBoxes(boxes.tolist(), confidences.tolist(), CONFIDENCE_THRESHOLD, NMS_THRESHOLD)

    results = []
    for i in indices:
        results.append({
            'class_id': int(class_ids[i]),
            'confidence': float(confidences[i]),
            'box': boxes[i].tolist()
        })

    return results


def draw_detections(image, results):
    for result in results:
        box = result['box']
        class_id = result['class_id']
        confidence = result['confidence']
        color = (0, 255, 0)  # 绿色边界框

        if len(box) != 4:
            print(f"Warning: Unexpected box format: {box}")
            continue

        x, y, w, h = box
        cv2.rectangle(image, (int(x), int(y)), (int(x + w), int(y + h)), color, 2)
        label = f"{CLASS_NAMES[class_id]}: {confidence:.2f}"
        cv2.putText(image, label, (int(x), int(y) - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

    return image


# 主循环
cap = cv2.VideoCapture(0)  # 使用默认摄像头

# 设置较低的摄像头分辨率以提高性能
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    start_time = time.time()

    blob = preprocess_image(frame)
    outputs = session.run(None, {input_name: blob})
    results = postprocess(outputs, frame)

    print(f"Number of detections: {len(results)}")
    for result in results:
        print(f"Detection: class_id={result['class_id']}, confidence={result['confidence']:.2f}, box={result['box']}")

    frame_with_detections = draw_detections(frame, results)

    end_time = time.time()
    fps = 1 / (end_time - start_time)
    cv2.putText(frame_with_detections, f"FPS: {fps:.2f}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

    cv2.imshow("Detections", frame_with_detections)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()