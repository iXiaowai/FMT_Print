# Print_FMT Test Layout

测试面向 ARM、瑞芯微、海思等嵌入式交叉编译环境，优先验证核心格式化能力
及资源受限配置。常规测试由 `CMakeLists.txt` 注册并通过 CTest 运行：

- `core/`：基础格式化、参数、编译期检查及 printf 兼容测试
- `embedded/`：仅头文件、无内置格式化类型、无异常/区域设置编译检查及性能冒烟程序
- `support/`：Google Test 和核心测试共用的断言、mock 与辅助工具

默认测试不覆盖颜色、OS/POSIX、C API、C++ module、CUDA、模糊测试以及安装/
导出集成等非嵌入式目标专项。使用交叉工具链时可构建测试程序；若要运行
CTest，目标设备或模拟器必须可用。

可关闭库的 OS 专项 API，并运行保留的核心测试：

```sh
cmake -S . -B build -DFMT_TEST=ON -DFMT_OS=OFF
cmake --build build
ctest --test-dir build --output-on-failure
```

启用 `FMT_PEDANTIC=ON` 还会构建无异常和无区域设置的编译检查目标。
