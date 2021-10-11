# GAMES101 - Assignment6

**作业完成的得分点**：

*   [5 points] 提交格式正确，包含所有需要的文件；代码可以在虚拟机下正确 编译运行。
*   [20 points] 包围盒求交：正确实现光线与包围盒求交函数。
*   [15 points] BVH 查找：正确实现 BVH 加速的光线与场景求交。



**函数说明**：

* Bounds3::IntersectP() : 判断包围盒 BoundingBox 与光线是否相交。
*  BVHAccel::getIntersection() : 递归调用IntersectP()，使用BVH加速求交过程。



**结果展示**：

![](images/result.png)

