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
#include <stdarg.h>

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
//  哈希和比较操作
// ========================================================================================================================================================

// 哈希函数
#define w_hash(T) w_concat(w_hash_, T)

// 比较函数
#define w_compare(T) w_concat(w_compare_, T)

// 判断是否相等
#define w_equal(T) w_concat(w_equal_, T)

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
        this->elementData = w_malloc(sizeof(T) * size);                 \
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
        w_free(this->elementData);                          \
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

// ========================================================================================================================================================
//  多维数组
// ========================================================================================================================================================

// 多维数组类型
#define w_NDArray(T) w_concat(w_NDArray_, T)

// 多维数组类型定义
#define w_NDArray_type_define_(T)      \
    typedef struct                     \
    {                                  \
        T *elementData; /* 元素数据 */ \
        int64_t *shape; /* 形状*/      \
        int64_t shapeSize;             \
    } w_NDArray(T);

// 多维数组初始化
#define w_NDArray_init(T) w_concat(w_NDArray(T), _init)
#define w_NDArray_init_define_(T)                                                     \
    /**                                                                               \
     * 多维数组初始化                                                          \
     * @param this 多维数组                                                       \
     * @param shapeSize 形状大小                                                  \
     * @param ... 形状（必须传入 int64 类型，否则会报错）             \
     * @return void                                                                   \
     */                                                                               \
    static inline void w_NDArray_init(T)(w_NDArray(T) * this, int64_t shapeSize, ...) \
    {                                                                                 \
        /* 处理可变参数 */                                                            \
        va_list shape;                                                                \
        va_start(shape, shapeSize);                                                   \
        w_assert(shapeSize > 0);                                                      \
        w_assert(this != NULL);                                                       \
                                                                                      \
        /* 创建形状 */                                                                \
        this->shape = w_malloc(sizeof(int64_t) * shapeSize);                          \
        w_assert(this->shape != NULL);                                                \
        w_assert(shapeSize > 0);                                                      \
        int64_t elementSize = 1;                                                      \
        for (int64_t i = 0; i < shapeSize; i++)                                       \
        {                                                                             \
            this->shape[i] = va_arg(shape, int64_t);                                  \
            elementSize *= this->shape[i];                                            \
        }                                                                             \
                                                                                      \
        /* 创建元素数据 */                                                            \
        this->elementData = w_malloc(sizeof(T) * elementSize);                        \
        w_assert(this->elementData != NULL);                                          \
        this->shapeSize = shapeSize;                                                  \
                                                                                      \
        /* 清理可变参数 */                                                            \
        va_end(shape);                                                                \
    }

// 多维数组销毁
#define w_NDArray_deinit(T) w_concat(w_NDArray(T), _deinit)
#define w_NDArray_deinit_define_(T)                             \
    /**                                                         \
     * 多维数组销毁                                       \
     * @param this 多维数组                                 \
     * @return void                                             \
     */                                                         \
    static inline void w_NDArray_deinit(T)(w_NDArray(T) * this) \
    {                                                           \
        w_assert(this != NULL);                                 \
        w_assert(this->elementData != NULL);                    \
        w_assert(this->shape != NULL);                          \
        w_free(this->elementData);                              \
        w_free(this->shape);                                    \
    }

// 多维数组下标映射
#define w_NDArray_map_(T) w_concat(w_NDArray(T), _map_)
#define w_NDArray_map_define_(T)                                                  \
    /**                                                                           \
     * 多维数组下标映射                                                   \
     * @param this 多维数组                                                   \
     * @param indices 索引                                                      \
     * @return int64_t 索引                                                     \
     */                                                                           \
    static inline int64_t w_NDArray_map_(T)(w_NDArray(T) * this, va_list indices) \
    {                                                                             \
        w_assert(this != NULL);                                                   \
        w_assert(this->shape != NULL);                                            \
        w_assert(this->shapeSize > 0);                                            \
        w_assert(this->elementData != NULL);                                      \
                                                                                  \
        /* 计算索引 */                                                            \
        int64_t index = 0;                                                        \
        for (int64_t i = 0; i < this->shapeSize - 1; i++)                         \
        {                                                                         \
            int64_t index_ = va_arg(indices, int64_t);                            \
            w_assert(index_ >= 0 && index_ < this->shape[i]);                     \
            /*  计算索引 (i1 - i(n-1)) */                                         \
            index += index_;                                                      \
            index *= this->shape[i + 1];                                          \
        }                                                                         \
        /* i(n) */                                                                \
        int64_t index_ = va_arg(indices, int64_t);                                \
        w_assert(index_ >= 0 && index_ < this->shape[this->shapeSize - 1]);       \
        index += index_;                                                          \
                                                                                  \
        /* 返回索引 */                                                            \
        return index;                                                             \
    }

// 多维数组获取元素
#define w_NDArray_get(T) w_concat(w_NDArray(T), _get)
#define w_NDArray_get_define_(T)                               \
    /**                                                        \
     * 多维数组获取元素                                \
     * @param this 多维数组                                \
     * @param ... 索引                                       \
     * @return T 元素                                        \
     */                                                        \
    static inline T w_NDArray_get(T)(w_NDArray(T) * this, ...) \
    { /* 返回 */                                               \
        va_list indices;                                       \
        va_start(indices, this);                               \
        int64_t index = w_NDArray_map_(T)(this, indices);      \
        va_end(indices);                                       \
        return this->elementData[index];                       \
    }

// 多维数组设置元素
#define w_NDArray_set(T) w_concat(w_NDArray(T), _set)
#define w_NDArray_set_define_(T)                                           \
    /**                                                                    \
     * 多维数组设置元素                                            \
     * @param this 多维数组                                            \
     * @param value 值                                                    \
     * @param ... 索引                                                   \
     * @return void                                                        \
     */                                                                    \
    static inline void w_NDArray_set(T)(w_NDArray(T) * this, T value, ...) \
    { /* 设置 */                                                           \
        va_list indices;                                                   \
        va_start(indices, value);                                          \
        int64_t index = w_NDArray_map_(T)(this, indices);                  \
        va_end(indices);                                                   \
        this->elementData[index] = value;                                  \
    }

// 多维数组获取形状
#define w_NDArray_shape(T) w_concat(w_NDArray(T), _shape)
#define w_NDArray_shape_define_(T)                                 \
    /**                                                            \
     * 多维数组获取形状                                    \
     * @param this 多维数组                                    \
     * @return int64_t * 形状                                    \
     */                                                            \
    static inline int64_t *w_NDArray_shape(T)(w_NDArray(T) * this) \
    {                                                              \
        w_assert(this != NULL);                                    \
        w_assert(this->shape != NULL);                             \
        w_assert(this->shapeSize > 0);                             \
        w_assert(this->elementData != NULL);                       \
        return this->shape;                                        \
    }

// 多维数组获取形状大小
#define w_NDArray_shapeSize(T) w_concat(w_NDArray(T), _shapeSize)
#define w_NDArray_shapeSize_define_(T)                                \
    /**                                                               \
     * 多维数组获取形状大小                                 \
     * @param this 多维数组                                       \
     * @return int64_t 形状大小                                   \
     */                                                               \
    static inline int64_t w_NDArray_shapeSize(T)(w_NDArray(T) * this) \
    {                                                                 \
        w_assert(this != NULL);                                       \
        w_assert(this->shape != NULL);                                \
        w_assert(this->shapeSize > 0);                                \
        w_assert(this->elementData != NULL);                          \
        return this->shapeSize;                                       \
    }

// 多维数组定义
#define w_NDArray_define(T)      \
    w_NDArray_type_define_(T);   \
    w_NDArray_init_define_(T);   \
    w_NDArray_deinit_define_(T); \
    w_NDArray_map_define_(T);    \
    w_NDArray_get_define_(T);    \
    w_NDArray_set_define_(T);    \
    w_NDArray_shape_define_(T);  \
    w_NDArray_shapeSize_define_(T);

#endif