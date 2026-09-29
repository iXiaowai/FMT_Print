# Print_FMT

基于 [{fmt}](https://github.com/fmtlib/fmt) 整理的 C/C++ 格式化库，面向核心
格式化功能学习、资源受限配置和 ARM、瑞芯微、海思等嵌入式平台移植参考。

本仓库精简了上游项目的辅助构建内容和专项测试，但保留公共格式化 API。测试
集聚焦核心格式化回归和嵌入式配置；不包含 Android、Windows 专用构建支持，
也不覆盖颜色、OS/POSIX、C API、C++ module、CUDA、模糊测试等独立专项。

## 项目特点

- CMake 构建，可按需构建静态库、仅头文件库及可选功能目标
- 通过 `FMT_OS=OFF` 排除 OS 专项实现，保留其他格式化 API
- 保留核心格式化、printf 兼容和资源受限配置测试
- 提供中文 API、语法和快速入门文档
- 核心库无第三方依赖；测试框架随仓库提供，文档生成需要独立工具链

## 目录结构

```text
.
├── CMakeLists.txt
├── include/fmt/       公共头文件
├── src/               编译库及可选 API 的实现
├── test/
│   ├── core/          核心格式化和 API 回归测试
│   ├── embedded/      资源受限配置及性能冒烟检查
│   └── support/       Google Test 和测试辅助代码
├── doc/               英文和中文 API、语法及快速入门文档
├── support/           CMake 安装模板、文档工具配置和源码维护脚本
└── LICENSE
```

主要实现文件：

| 文件 | 用途 |
| --- | --- |
| `src/format.cc` | 编译版核心格式化库实现 |
| `src/os.cc` | `fmt/os.h` 的 OS 专项实现；由 `FMT_OS` 控制 |
| `src/fmt-c.cc` | 可选 C API 实现，对应 `fmt/fmt-c.h` |
| `src/fmt.cc` | 可选 C++ module 实现，仅在启用 `FMT_MODULE` 时使用 |

`include/fmt/` 中的公共头文件均予保留，包括 chrono、color、ranges、OS、
printf、C API 等可选功能头文件。裁剪专项测试或关闭某个构建目标不代表删除
对应的公共 API。

## 构建核心库

以下示例构建面向尺寸优化的静态核心库，并关闭文档、测试、安装、C++ module
和 OS 专项 API：

```sh
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=MinSizeRel \
  -DBUILD_SHARED_LIBS=OFF \
  -DFMT_TEST=OFF \
  -DFMT_DOC=OFF \
  -DFMT_INSTALL=OFF \
  -DFMT_MODULE=OFF \
  -DFMT_OS=OFF
cmake --build build
```

交叉编译时，添加目标平台或 SDK 提供的 toolchain file：

```sh
cmake -S . -B build-arm \
  -DCMAKE_TOOLCHAIN_FILE=/path/to/arm-toolchain.cmake \
  -DCMAKE_BUILD_TYPE=MinSizeRel \
  -DBUILD_SHARED_LIBS=OFF \
  -DFMT_TEST=OFF -DFMT_DOC=OFF -DFMT_INSTALL=OFF \
  -DFMT_MODULE=OFF -DFMT_OS=OFF
cmake --build build-arm
```

应用使用 CMake 时链接 `fmt::fmt`。`FMT_OS=OFF` 会省略 `src/os.cc`，此时不能
调用 `fmt/os.h` 中需要 OS 实现的 API；其他格式化 API 不受影响。更多配置和
工具链说明见[中文快速入门](doc/get-started_zh.md)。

## 构建和运行测试

在本机或有可用模拟器/运行器的目标环境中，可构建并运行保留的核心测试：

```sh
cmake -S . -B build-test \
  -DCMAKE_BUILD_TYPE=Debug \
  -DFMT_TEST=ON \
  -DFMT_DOC=OFF \
  -DFMT_INSTALL=OFF \
  -DFMT_MODULE=OFF \
  -DFMT_OS=OFF
cmake --build build-test
ctest --test-dir build-test --output-on-failure
```

交叉编译的测试程序需要在目标设备或模拟器上运行，才能执行 CTest。测试目录和
覆盖范围详见[测试说明](test/FMT_Test.md)。启用 `FMT_PEDANTIC=ON` 还会
增加警告检查，以及无异常、无 locale 配置的编译检查。

## 基本使用

格式化为字符串：

```cpp
#include <fmt/format.h>

int main() {
  auto text = fmt::format("value = {}", 123);
}
```

格式化并输出：

```cpp
#include <fmt/base.h>

int main() {
  fmt::print("Hello, {}!\n", "world");
}
```

若需控制输出缓冲区或避免使用返回 `std::string` 的接口，可使用
`fmt::format_to_n` 等输出迭代器 API，并传入调用方提供的缓冲区。

## 文档

- [中文快速入门](doc/get-started_zh.md)
- [中文 API 参考](doc/api_zh.md)
- [中文格式字符串语法](doc/syntax_zh.md)
- [英文快速入门](doc/get-started.md)
- [英文 API 参考](doc/api.md)
- [英文格式字符串语法](doc/syntax.md)

本仓库通过 CMake 的 `doc` target 生成 HTML 文档；构建需要安装 Doxygen、
MkDocs 和 `support/doc-requirements.txt` 中锁定的 Python 依赖。
`support/mkdocs.yml` 是站点构建配置。文档生成配置和依赖不是嵌入式库的构建
或运行依赖。

## 上游项目与许可证

本仓库是经过整理的移植参考版本，不等同于上游 `{fmt}` 的完整测试、构建和
发布配置。需要完整功能说明或上游最新版本时，请参阅
[上游 `{fmt}` 项目](https://github.com/fmtlib/fmt)。

许可证信息见 [LICENSE](LICENSE)。
