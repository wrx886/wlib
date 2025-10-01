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

// 标识符拼接
#define w_concat_(a, b) a##b
#define w_concat(a, b) w_concat_(a, b)

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
//  数组
// ========================================================================================================================================================

// 数组类型
#define w_Array(T) w_concat(w_Array_, T)

// 数组类型定义
#define w_Array_type_define_(T)        \
    typedef struct                     \
    {                                  \
        T *elementData; /* 元素数据 */ \
        int size;       /* 元素数量 */ \
    } w_Array(T);

// 数组初始化
#define w_Array_init(T) w_concat(w_Array(T), _init)
#define w_Array_init_define_(T)                                         \
    /**                                                                 \
     * 数组初始化                                                  \
     * @param this 数组                                               \
     * @param size 数组大小                                         \
     * @return void                                                     \
     */                                                                 \
    static inline void w_Array_init(T)(w_Array(T) * this, int64_t size) \
    {                                                                   \
        w_assert(this != NULL);                                         \
        w_assert(size >= 0);                                            \
        this->elementData = malloc(sizeof(T) * size);                   \
        this->size = size;                                              \
    }

// 数组反初始化
#define w_Array_deinit(T) w_concat(w_Array(T), _deinit)
#define w_Array_deinit_define_(T)                           \
    /**                                                     \
     * 数组反初始化                                   \
     * @param this 数组                                   \
     * @return void                                         \
     */                                                     \
    static inline void w_Array_deinit(T)(w_Array(T) * this) \
    {                                                       \
        w_assert(this != NULL);                             \
        w_assert(this->elementData != NULL);                \
        free(this->elementData);                            \
        memset(this, 0, sizeof(w_Array(T)));                \
    }

// 获取数组元素
#define w_Array_get(T) w_concat(w_Array(T), _get)
#define w_Array_get_define_(T)                                       \
    /**                                                              \
     * 获取数组元素                                            \
     * @param this 数组                                            \
     * @param index 索引                                           \
     * @return T 数组元素                                        \
     */                                                              \
    static inline T w_Array_get(T)(w_Array(T) * this, int64_t index) \
    {                                                                \
        w_assert(this != NULL);                                      \
        w_assert(this->elementData != NULL);                         \
        w_assert(index >= 0 && index < this->size);                  \
        return this->elementData[index];                             \
    }

// 设置数组元素
#define w_Array_set(T) w_concat(w_Array(T), _set)
#define w_Array_set_define_(T)                                                 \
    /**                                                                        \
     * 设置数组元素                                                      \
     * @param this 数组                                                      \
     * @param index 索引                                                     \
     * @param val 值                                                          \
     * @return void                                                            \
     */                                                                        \
    static inline void w_Array_set(T)(w_Array(T) * this, int64_t index, T val) \
    {                                                                          \
        w_assert(this != NULL);                                                \
        w_assert(this->elementData != NULL);                                   \
        w_assert(index >= 0 && index < this->size);                            \
        this->elementData[index] = val;                                        \
    }

// 数组大小
#define w_Array_size(T) w_concat(w_Array(T), _size)
#define w_Array_size_define_(T)                              \
    /**                                                      \
     * 获取数组大小                                    \
     * @param this 数组                                    \
     * @return int64_t 数组大小                          \
     */                                                      \
    static inline int64_t w_Array_size(T)(w_Array(T) * this) \
    {                                                        \
        w_assert(this != NULL);                              \
        return this->size;                                   \
    }

// 数组定义
#define w_Array_define(T)      \
    w_Array_type_define_(T);   \
    w_Array_init_define_(T);   \
    w_Array_deinit_define_(T); \
    w_Array_get_define_(T);    \
    w_Array_set_define_(T);    \
    w_Array_size_define_(T)

#endif