import os
import json
import random
import base64
import shutil
import argparse
from pathlib import Path
from glob import glob
from dic_lab import dic_labels


def generate_labels(dic_labs):
    path_input_json = dic_labels['path_json']
    ratio = dic_labs['ratio']

    for index, labelme_annotation_path in enumerate(glob(f'{path_input_json}/*.json')):
        # 读取文件名
        image_id = os.path.basename(labelme_annotation_path).rstrip('.json')

        # 计算是train 还是 valid
        train_or_valid = 'train' if random.random() < ratio else 'valid'

        # 读取labelme格式的json文件
        with open(labelme_annotation_path, 'r') as labelme_annotation_file:
            labelme_annotation = json.load(labelme_annotation_file)

        # yolo 格式的 lables
        yolo_annotation_path = os.path.join(train_or_valid, 'labels', image_id + '.txt')
        with open(yolo_annotation_path, 'w') as yolo_annotation_file:

            # yolo 格式的图像保存
            yolo_image = base64.decodebytes(labelme_annotation['imageData'].encode())
            yolo_image_path = os.path.join(train_or_valid, 'images', image_id + '.jpg')

            with open(yolo_image_path, 'wb') as yolo_image_file:
                yolo_image_file.write(yolo_image)

            # 获取位置信息
            for shape in labelme_annotation['shapes']:
                if shape['shape_type'] == 'rectangle':
                    points = shape['points']
                    scale_width = 1.0 / labelme_annotation['imageWidth']
                    scale_height = 1.0 / labelme_annotation['imageHeight']
                    width = (points[1][0] - points[0][0]) * scale_width
                    height = (points[1][1] - points[0][1]) * scale_height
                    x = ((points[1][0] + points[0][0]) / 2) * scale_width
                    y = ((points[1][1] + points[0][1]) / 2) * scale_height
                    object_class = dic_labels[shape['label']]
                    yolo_annotation_file.write(f'{object_class} {x} {y} {width} {height}\n')

                elif shape['shape_type'] == 'circle':
                    center = shape['points'][0]
                    radius = shape['points'][1][0] - center[0]  # Assuming the second point gives radius
                    scale_width = 1.0 / labelme_annotation['imageWidth']
                    scale_height = 1.0 / labelme_annotation['imageHeight']
                    x = center[0] * scale_width
                    y = center[1] * scale_height
                    width = height = 2 * radius * scale_width
                    object_class = dic_labels[shape['label']]
                    yolo_annotation_file.write(f'{object_class} {x} {y} {width} {height}\n')

                elif shape['shape_type'] == 'point':
                    point = shape['points'][0]
                    scale_width = 1.0 / labelme_annotation['imageWidth']
                    scale_height = 1.0 / labelme_annotation['imageHeight']
                    x = point[0] * scale_width
                    y = point[1] * scale_height
                    width = height = 0.01  # Small bounding box for point
                    object_class = dic_labels[shape['label']]
                    yolo_annotation_file.write(f'{object_class} {x} {y} {width} {height}\n')

                else:
                    print(f'Invalid type {shape["shape_type"]} in annotation {labelme_annotation_path}')

        print(f"Created lab {index} : {image_id}")


if __name__ == "__main__":
    os.makedirs(os.path.join("train",'images'),exist_ok=True)
    os.makedirs(os.path.join("train",'labels'),exist_ok=True)
    os.makedirs(os.path.join("valid",'images'),exist_ok=True)
    os.makedirs(os.path.join("valid",'labels'),exist_ok=True)
    generate_labels(dic_labels)
 
