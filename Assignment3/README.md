# GAMES101 - Assignment3

作业完成的得分点：

*   [5 分] 提交格式正确，包括所有需要的文件。代码可以正常编译、执行。
*   [10 分] 参数插值: 正确插值颜色、法向量、纹理坐标、位置 (Shading Position) 并将它们传递给 fragment_shader_payload. 
*   [20 分]Blinn-phong 反射模型: 正确实现 phong_fragment_shader 对应的 反射模型。
*   [5 分] Texture mapping: 将 phong_fragment_shader 的代码拷贝到 texture_fragment_shader, 在此基础上正确实现 Texture Mapping. 
*   [10 分] Bump mapping 与 Displacement mapping: 正确实现 Bump mapping 与 Displacement mapping. 
*   [Bonus 3 分] 尝试更多模型: 找到其他可用的.obj 文件，提交渲染结果并 把模型保存在 /models 目录下。这些模型也应该包含 Vertex Normal 信息。



函数说明：

* rasterize_triangle() ：实现法向量、颜色、纹理颜色的插值，方法类似Assignment2.
* get_projection_matrix() ：实现投影矩阵。
* phong_fragment_shader() ：实现 Blinn-Phong 模型，计算 Fragment Color.
* texture_fragment_shader() ：在实现 Blinn-Phong 的基础上，将纹理颜色视为公式中的 kd，实现Texture mapping.
* bump_fragment_shader() ：在实现 Blinn-Phong 的 基础上，实现 Bump mapping.
* displacement_fragment_shader() ：在实现 Bump mapping 的基础上，实现 Displacement mapping.



Bonus说明：

* 修改main.cpp中的.obj文件路径，指定为../model/cube/cube.obj，对立方体模型进行渲染，运行示例保存至images文件夹下的cube_normal.png及cube_bump.png.

* 修改texture_path，可对模型渲染出不同的纹理，对比示例见bump.png与bump2.png，及displacement.png与displacement2.png.

  

  
