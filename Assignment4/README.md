# GAMES101 - Assignment4

作业完成的得分点：

*   [5 分] 提交的格式正确，包含所有必须的文件。代码可以编译和运行。
*   [20 分] De Casteljau 算法：对于给定的控制点，代码能够产生正确的 Bézier 曲线。



函数说明：

* bezier() : 绘制 Bézier 曲线。使 t 在[0, 1] 的范围内进行迭代，并调用 recursive_bezier() 获取 t 处的点，将点绘制在 OpenCV::Mat 对象上。
*  recursive_bezier() : 实现 de Casteljau 算法，返回 Bézier 曲线上对应点的坐标。
