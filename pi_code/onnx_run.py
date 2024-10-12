import onnxruntime
import numpy as np

device_name =  'cpu' # 'cuda:0' # or 'cpu'
print(onnxruntime.get_available)

if device_name == 'cpu':
    providers = ['CPUExecutionProvider']
elif device_name == 'cuda:0':
    providers = ['CUDAExecutionProvider', 'CPUExecutionProvider']
# Create inference session
onnx_model = onnxruntime.InferenceSession('sh.onnx', providers=providers)
# Create the input（这里的输入对应slowfast的输入）
data = np.random.rand(1, 1, 3, 32, 256, 256).astype(np.float32)
# Inference
onnx_input = {onnx_model.get_inputs()[0].name: data}
outputs = onnx_model.run(None, onnx_input)
