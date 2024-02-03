# AZH_Network_Program

将此仓库作为依赖库

```bash
mkdir 3rdparty
git submodule add https://github.com/azh-1415926/AZH_Network_Program.git 3rdparty/AZH_Network_Program
```

在 cmake 中配置

```cmake
add_subdirectory(3rdparty/AZH_Network_Program)
include_directories(${CMAKE_CURRENT_LIST_DIR}/3rdparty/AZH_Qt_Component/include)
# link with c
link_directories(${CMAKE_BINARY_DIR}/3rdparty/AZH_Network_Program)
# link with cxx
link_directories(${CMAKE_BINARY_DIR}/3rdparty/AZH_Network_Program_CXX)
```