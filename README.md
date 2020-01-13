# OpenGL Model Viewer

## Dependencies

- OpenGL
- GLFW3
- GLEW
- Eigen3
- PCL

## Build

```
mkdir build
cd build
cmake ..
make
make install
```

## Run

```
cd output
./opengl_model_viewer <model_file_path>
```

## Screenshots

<img src="https://github.com/insaneyilin/opengl_model_viewer/blob/master/screenshots/example.png" width="960" />

## TODOs

- [ ] support .obj, .ply formats
- [ ] support config.
- [ ] support frame buffer mode(Render To Texture)

## Credits

The rainbow shaders and some codes are adapted from [SMRT-AIST/interactive_slam](https://github.com/SMRT-AIST/interactive_slam) .
