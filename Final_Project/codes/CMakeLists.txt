cmake_minimum_required(VERSION 3.10)
project(MeshSimplify)

set (CMAKE_CXX_STANDARD 17)

add_executable(MeshSimplify 
	main.cpp 
	global.hpp
	vector.hpp
	matrix.hpp
	model.hpp
)

target_link_libraries(MeshSimplify PUBLIC -fsanitize=undefined)

