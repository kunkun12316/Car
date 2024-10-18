import cv2
import os

# 创建保存帧的目录
output_dir = "saved_frames_sh"
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# 初始化摄像头
cap = cv2.VideoCapture(0)

# 初始化帧计数器
frame_count = 1

while True:
    # 捕获视频帧
    ret, frame = cap.read()
    if not ret:
        break

    # 显示视频帧
    cv2.imshow("Video Feed", frame)

    # 检查键盘输入
    key = cv2.waitKey(1) & 0xFF
    if key == ord('p'):  # 如果按下 'P' 键
        # 保存当前帧
        frame_filename = os.path.join(output_dir, f"frame_{frame_count}.png")
        cv2.imwrite(frame_filename, frame)
        print(f"Saved: {frame_filename}")
        frame_count += 1
    elif key == ord('q'):  # 按 'Q' 键退出
        break

# 释放摄像头并关闭所有窗口
cap.release()
cv2.destroyAllWindows()
