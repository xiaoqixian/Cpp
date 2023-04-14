### CMake 速查

#### 设置项目名字和版本号

```cmake
project(Tutorial VERSION 1.0)
```

#### 设置 C++ 标准

```cmake
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

确保放在 `add_executable` 之前。

#### 设置

