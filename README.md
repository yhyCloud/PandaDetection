# PandaDetection
基于QT&amp;Python的熊猫检测系统

本项目前端界面实现采用qt编写，后端熊猫检测算法在Tensorflow ObjectDetection的API上进行改进.实现了对图像以及视频中的大熊猫检测的功能

系统界面1是图像选择界面，可以从计算机本地选择图片进行检测
![image](https://github.com/yhyCloud/PandaDetection/blob/master/%E7%B3%BB%E7%BB%9F%E7%95%8C%E9%9D%A2%E5%B1%95%E7%A4%BA/%E7%B3%BB%E7%BB%9F%E7%95%8C%E9%9D%A21.png)

系统界面2为图像实时检测结果界面，实时显示检测进度以及显示已检测图像的结果
![image](https://github.com/yhyCloud/PandaDetection/blob/master/%E7%B3%BB%E7%BB%9F%E7%95%8C%E9%9D%A2%E5%B1%95%E7%A4%BA/%E7%B3%BB%E7%BB%9F%E7%95%8C%E9%9D%A22.png)

系统界面3为视频检测结果结果界面，右上角可以播放检测列表中的视频，左下角为从视频中截取出的包含熊猫的视频片段，右下角可以播放检测结果视频
![image](https://github.com/yhyCloud/PandaDetection/blob/master/%E7%B3%BB%E7%BB%9F%E7%95%8C%E9%9D%A2%E5%B1%95%E7%A4%BA/%E7%B3%BB%E7%BB%9F%E7%95%8C%E9%9D%A23.png)

系统界面4为qt中嵌入百度地图的显示，通过读取图像的EXIF信息，来获取图像的地理位置，再显示在地图上（待完善）
![image](https://github.com/yhyCloud/PandaDetection/blob/master/%E7%B3%BB%E7%BB%9F%E7%95%8C%E9%9D%A2%E5%B1%95%E7%A4%BA/%E7%B3%BB%E7%BB%9F%E7%95%8C%E9%9D%A24.png)
