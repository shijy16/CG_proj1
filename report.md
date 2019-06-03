# 真实感图像渲染

石景宜

## 一、 完成功能

- [x] RT
  - [x] 折射、反射、漫反射等基本功能
  - [x] 模糊反射，软阴影
- [x] 贴图
- [x] 超采样抗锯齿



## 二、 贝塞尔曲线

贝塞尔旋转体曲面如下：

![1559486962858](D:\code\computer_graphics\CG_proj1\report_pic\1.png)

$ \left(\left(1-t\right)\left(\left(1-t\right)\left(\left(1-t\right)x_0+tx_1\right)+t\left(\left(1-t\right)x_1+tx_2\right)\right)+t\left(\left(1-t\right)\left(\left(1-t\right)x_1+tx_2\right)+t\left(\left(1-t\right)x_2+tx_3\right)\right),\left(1-t\right)\left(\left(1-t\right)\left(\left(1-t\right)y_0+ty_1\right)+t\left(\left(1-t\right)y_1+ty_2\right)\right)+t\left(\left(1-t\right)\left(\left(1-t\right)y_1+ty_2\right)+t\left(\left(1-t\right)y_2+ty_3\right)\right)\right) ​$

