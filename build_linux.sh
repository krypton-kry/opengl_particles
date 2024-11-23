#!/usr/bin/bash

GLFW_LIB="thirdparty/glfw/build/src/libglfw3.a"

build_particles()
{
	if [ $? -eq 0 ]; then
		echo "GLFW Found!"
		echo "Building Particles"

		cp  "$GLFW_LIB" .
		gcc src/*.c -o particles libglfw3.a -lXm -lX11 -lpthread -lm -ldl -Iinclude -Ithirdparty -O3 -s
		rm libglfw3.a
	else
		echo "Error"
	fi
}

if [ -f  "$GLFW_LIB" ]
then
	build_particles
else
	# build glfw
	echo "Building GLFW ..."
	mkdir thirdparty/glfw/build

	pushd thirdparty/glfw/build/
	cmake .. 
	make
	popd
fi