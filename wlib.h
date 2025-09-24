#ifndef WLIB_H
#define WLIB_H

/**
 * 标识符命名规则：
 *  1. 函数名：w_函数名（首字母小写 + 驼峰）
 *  2. 类型定义：w_类型名（首字母大写 + 驼峰）
 *  3. 公开和隐藏：
 *      3.1 公开：w_标识符
 *      3.2 隐藏：w_标识符_
 *  4. 常量：w_常量名（全大写 + 下划线）
 *  5. 结构体成员访问：不应该直接访问，而是应该通过结构体提供的方法进行访问
 */

// ========================================================================================================================================================
//  标准库引入
// ========================================================================================================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// 统一内存的申请和释放
#define w_malloc(size) malloc(size)
#define w_free(ptr) free(ptr)

// ========================================================================================================================================================
//  断言定义
// ========================================================================================================================================================

/**
 * 断言，如果断言失败则会输出错误信息到错误输出流并退出程序
 * @param expr 断言条件
 */
#define w_assert(expr)                                                            \
    ({                                                                            \
        if (!(expr))                                                              \
        {                                                                         \
            fprintf(stderr, "[w_assert] %s:%d: %s\n", __FILE__, __LINE__, #expr); \
            exit(1);                                                              \
        }                                                                         \
    })

// ========================================================================================================================================================
//  数据类型需要支持的操作
// ========================================================================================================================================================

/**
 * 获取数据类型的哈希值计算函数
 * 函数原型：int64_t w_hash_##T(T this)
 * @param T 数据类型
 */
#define w_hash(T) w_hash_##T

/**
 * 获取数据类型的比较函数
 * 函数原型：bool w_equals_##T(T this, T other)
 * @param T 数据类型
 */
#define w_equals(T) w_equals_##T

// ========================================================================================================================================================
//  数组
// ========================================================================================================================================================

// 数组类型定义
#define w_Array_type_define_(T)                \
    struct w_Array_##T                         \
    {                                          \
        T *data;      /* 存储数据的数组指针 */ \
        int64_t size; /* 数组大小 */           \
    };

// 类型数组初始化函数定义
#define w_Array_init_define_(T)                  \
    /* 类型数组初始化函数 */                     \
    static inline void w_Array_init_##T(         \
        struct w_Array_##T *this,                \
        int64_t size /* 数组大小 */)             \
    {                                            \
        w_assert(size >= 0);                     \
        w_assert(this != NULL);                  \
        this->data = w_malloc(size * sizeof(T)); \
        memset(this->data, 0, size * sizeof(T)); \
        this->size = size;                       \
    }

// 类型数组反初始化函数定义
#define w_Array_deinit_define_(T)                                   \
    /* 类型数组反初始化函数 */                                      \
    static inline void w_Array_deinit_##T(struct w_Array_##T *this) \
    {                                                               \
        w_assert(this != NULL);                                     \
        w_assert(this->data != NULL);                               \
        w_free(this->data);                                         \
        this->data = NULL;                                          \
        this->size = -1;                                            \
    }

// 类型数组访问函数定义
#define w_Array_get_define_(T)                                \
    /*  类型数组访问函数 */                                   \
    static inline T w_Array_get_##T(struct w_Array_##T *this, \
                                    int64_t index /* 索引 */) \
    {                                                         \
        w_assert(this != NULL);                               \
        w_assert(this->data != NULL);                         \
        w_assert(index >= 0 && index < this->size);           \
        return this->data[index];                             \
    }

// 类型数组修改函数定义
#define w_Array_set_define_(T)                                   \
    /*  类型数组修改函数 */                                      \
    static inline void w_Array_set_##T(struct w_Array_##T *this, \
                                       int64_t index, /* 索引 */ \
                                       T value /* 新值 */)       \
    {                                                            \
        w_assert(this != NULL);                                  \
        w_assert(this->data != NULL);                            \
        w_assert(index >= 0 && index < this->size);              \
        this->data[index] = value;                               \
    }

/**
 * 定义数组类型，在全局作用域（所有 {} 外）中使用，使用后才定义这个类型的数组
 * @param T 数组元素类型
 */
#define w_Array_define(T)      \
    w_Array_type_define_(T);   \
    w_Array_init_define_(T);   \
    w_Array_deinit_define_(T); \
    w_Array_get_define_(T);    \
    w_Array_set_define_(T);

/**
 * 数组类型
 * @param T 数组元素类型
 */
#define w_Array(T) struct w_Array_##T

/**
 * 初始化数组，跳转到真实的初始化函数
 * 被调用函数原型：static inline void w_Array_init_##T(struct w_Array_##T *array, int64_t size)
 * @param T 数组类型
 */
#define w_Array_init(T) w_Array_init_##T

/**
 * 反初始化数组，跳转到真实的反初始化函数
 * 被调用函数原型：static inline void w_Array_deinit_##T(struct w_Array_##T *array)
 * @param T 数组类型
 */
#define w_Array_deinit(T) w_Array_deinit_##T

/**
 * 访问数组元素，跳转到真实的访问函数
 * 被调用函数原型：static inline T w_Array_get_##T(struct w_Array_##T *array, int64_t index)
 * @param T 数组类型
 */
#define w_Array_get(T) w_Array_get_##T

/**
 * 修改数组元素，跳转到真实的修改函数
 * 被调用函数原型：static inline void w_Array_set_##T(struct w_Array_##T *array, int64_t index, T value)
 * @param T 数组类型
 */
#define w_Array_set(T) w_Array_set_##T

#endif