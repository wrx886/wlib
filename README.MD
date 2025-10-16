# wlib

一个简单、通用、头文件化的 C 语言数据结构库。

## 快速开始

1.  将 `wlib.h` 复制到您的项目中
2.  在代码中包含头文件：
    ```c
    #include "wlib.h"
    ```
3.  使用前为需要的类型实例化模板（如 `w_Array_define(int);`）
4.  确保编译器支持 C99（如使用 `-std=c99` 编译选项）

## 示例用法

```c
#include "wlib.h"
#include <stdio.h>

// 为 int 类型定义数组
w_Array_define(int);

int main() {
    w_Array(int) arr;
    w_Array_init(int)(&arr, 5);
    
    // 使用数组...
    w_Array_set(int)(&arr, 0, 42);
    printf("%d\n", w_Array_get(int)(&arr, 0));
    
    w_Array_deinit(int)(&arr);
    return 0;
}
```

## 数据结构

当前支持的数据结构包括：
- **Array**: 固定大小数组
- **NDArray**: 多维数组  
- **List**: 动态数组
- **Map**: 哈希映射
- **Set**: 哈希集合
- **StringBuilder**: 字符串构建器

## 内存管理

默认使用标准 `malloc/free`，可通过定义 `w_malloc` 和 `w_free` 宏来自定义内存分配器。

**注意**: 所有通过 `w_*_init` 初始化的结构都必须使用对应的 `w_*_deinit` 释放。

## 许可证

MIT License - 详见文件头部版权声明。

## 贡献

欢迎提交 Issue 和 Pull Request！

---

*详细 API 文档请直接查看 `wlib.h` 头文件中的注释说明。*