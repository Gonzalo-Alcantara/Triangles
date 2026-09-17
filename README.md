# Triangles
Graphics triangle

## OpenGL
Build on:
- OS: Windows 10
- C: MinGW

Build with:
- OpenGL: 4.1 core
- glad
- IDE: VIM

Following:
- [OpenGL tutorial](https://learnopengl.com/Getting-started/Hello-Triangle)
- [Anton's OpenGL 4 Tutorials](https://antongerdelan.net/opengl/hellotriangle.html)


### Installation and use (look if works for mutiple platforms)
Build with cmake:
If u dont have a build/ subdirectory created, create it with your prefered method.
Then use this command with cmake installed:
`
cmake . -B build/
cmake --build build/
`
The executable will be one the build/ directory

If u dont have a GLFW already installed, download appropiate library of [GLFW](https://www.glfw.org/download.html) for your operating system,then place it in build/, or ut it in a PATH the computer will find when executing.

To run it execute it with your prefered method.
To run it on linux you will have to give the permission to execute.
