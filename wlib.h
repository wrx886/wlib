/*
 * wlib.h - A simple C library containing common data structures
 *
 * MIT License
 *
 * Copyright (c) 2025 wrx886
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef WLIB_H
#define WLIB_H

/**
 * C 标准：C99
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
#include <pthread.h> // 引入 pthread 库

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
// 函数原型：int64_t w_hash(T)(T* this);
#define w_hash(T) w_concat(w_hash_, T)

// 比较函数
// 函数原型：int64_t w_compare(T)(T* this, T* other);
#define w_compare(T) w_concat(w_compare_, T)

// 判断是否相等
// 函数原型：bool w_equals(T)(T* this, T* other);
#define w_equals(T) w_concat(w_equals_, T)

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
        w_assert(this->elementData != NULL);                            \
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
        memset(this, 0, sizeof(w_NDArray(T)));                  \
    }

// 多维数组下标映射
#define w_NDArray_map_(T) w_concat(w_NDArray(T), _map_)
#define w_NDArray_map_define_(T)                                                  \
    /**                                                                           \
     * 多维数组下标映射                                                   \
     * @param this 多维数组                                                   \
     * @param indices 索引（必须传入 int64 类型，否则会报错）     \
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
#define w_NDArray_get_define_(T)                                          \
    /**                                                                   \
     * 多维数组获取元素                                           \
     * @param this 多维数组                                           \
     * @param ... 索引（必须传入 int64 类型，否则会报错） \
     * @return T 元素                                                   \
     */                                                                   \
    static inline T w_NDArray_get(T)(w_NDArray(T) * this, ...)            \
    { /* 返回 */                                                          \
        va_list indices;                                                  \
        va_start(indices, this);                                          \
        int64_t index = w_NDArray_map_(T)(this, indices);                 \
        va_end(indices);                                                  \
        return this->elementData[index];                                  \
    }

// 多维数组设置元素
#define w_NDArray_set(T) w_concat(w_NDArray(T), _set)
#define w_NDArray_set_define_(T)                                           \
    /**                                                                    \
     * 多维数组设置元素                                            \
     * @param this 多维数组                                            \
     * @param value 值                                                    \
     * @param ... 索引（必须传入 int64 类型，否则会报错）  \
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

// ========================================================================================================================================================
//  列表
// ========================================================================================================================================================

// 列表类型
#define w_List(T) w_concat(w_List_, T)

// 列表类型定义
#define w_List_type_define_(T)       \
    typedef struct                   \
    {                                \
        T *elementData;              \
        int64_t size;     /* 大小 */ \
        int64_t capacity; /* 容量 */ \
    } w_List(T);

// 列表初始化
#define w_List_initWithCapacity(T) w_concat(w_List(T), _initWithCapacity)
#define w_List_initWithCapacity_define_(T)                                                \
    /**                                                                                   \
     * 列表初始化                                                                    \
     * @param this 列表                                                                 \
     * @param initCapacity 初始容量                                                   \
     * @return void                                                                       \
     */                                                                                   \
    static inline void w_List_initWithCapacity(T)(w_List(T) * this, int64_t initCapacity) \
    {                                                                                     \
        w_assert(this != NULL);                                                           \
        this->elementData = w_malloc(sizeof(T) * initCapacity);                           \
        w_assert(this->elementData != NULL);                                              \
        this->size = 0;                                                                   \
        this->capacity = initCapacity;                                                    \
    }

// 列表初始化
#define w_List_init(T) w_concat(w_List(T), _init)
#define w_List_init_define_(T)                          \
    /**                                                 \
     * 列表初始化                                  \
     * @param this 列表                               \
     */                                                 \
    static inline void w_List_init(T)(w_List(T) * this) \
    {                                                   \
        w_List_initWithCapacity(T)(this, 16);           \
    }

// 列表销毁
#define w_List_deinit(T) w_concat(w_List(T), _deinit)
#define w_List_deinit_define_(T)                          \
    /**                                                   \
     * 列表销毁                                       \
     * @param this 列表                                 \
     * @return void                                       \
     */                                                   \
    static inline void w_List_deinit(T)(w_List(T) * this) \
    {                                                     \
        w_assert(this != NULL);                           \
        w_assert(this->elementData != NULL);              \
        w_free(this->elementData);                        \
        memset(this, 0, sizeof(w_List(T)));               \
    }

// 列表获取大小
#define w_List_size(T) w_concat(w_List(T), _size)
#define w_List_size_define_(T)                             \
    /**                                                    \
     * 列表获取大小                                  \
     * @param this 列表                                  \
     * @return int64_t 大小                              \
     */                                                    \
    static inline int64_t w_List_size(T)(w_List(T) * this) \
    {                                                      \
        w_assert(this != NULL);                            \
        w_assert(this->elementData != NULL);               \
        return this->size;                                 \
    }

// 列表获取容量
#define w_List_capacity(T) w_concat(w_List(T), _capacity)
#define w_List_capacity_define_(T)                             \
    /**                                                        \
     * 列表获取容量                                      \
     * @param this 列表                                      \
     * @return int64_t 容量                                  \
     */                                                        \
    static inline int64_t w_List_capacity(T)(w_List(T) * this) \
    {                                                          \
        w_assert(this != NULL);                                \
        w_assert(this->elementData != NULL);                   \
        return this->capacity;                                 \
    }

// 列表获取元素
#define w_List_get(T) w_concat(w_List(T), _get)
#define w_List_get_define_(T)                                      \
    /**                                                            \
     * 列表获取元素                                          \
     * @param this 列表                                          \
     * @param index 索引                                         \
     * @return T 元素                                            \
     */                                                            \
    static inline T w_List_get(T)(w_List(T) * this, int64_t index) \
    {                                                              \
        w_assert(this != NULL);                                    \
        w_assert(this->elementData != NULL);                       \
        w_assert(index >= 0 && index < this->size);                \
        return this->elementData[index];                           \
    }

// 列表设置元素
#define w_List_set(T) w_concat(w_List(T), _set)
#define w_List_set_define_(T)                                                    \
    /**                                                                          \
     * 列表设置元素                                                        \
     * @param this 列表                                                        \
     * @param index 索引                                                       \
     * @param element 元素                                                     \
     * @return void                                                              \
     */                                                                          \
    static inline void w_List_set(T)(w_List(T) * this, int64_t index, T element) \
    {                                                                            \
        w_assert(this != NULL);                                                  \
        w_assert(this->elementData != NULL);                                     \
        w_assert(index >= 0 && index < this->size);                              \
        this->elementData[index] = element;                                      \
    }

// 列表添加元素
#define w_List_add(T) w_concat(w_List(T), _add)
#define w_List_add_define_(T)                                                    \
    /**                                                                          \
     * 列表添加元素                                                        \
     * @param this 列表                                                        \
     * @param index 索引（插入到这个位置）                            \
     * @param element 元素                                                     \
     */                                                                          \
    static inline void w_List_add(T)(w_List(T) * this, int64_t index, T element) \
    {                                                                            \
        /* 断言 */                                                               \
        w_assert(this != NULL);                                                  \
        w_assert(this->elementData != NULL);                                     \
        w_assert(index >= 0 && index <= this->size);                             \
                                                                                 \
        /* 扩容 */                                                               \
        if (this->size >= this->capacity)                                        \
        {                                                                        \
            this->capacity *= 2;                                                 \
            T *newElementData = w_malloc(this->capacity * sizeof(T));            \
            w_assert(newElementData != NULL);                                    \
            memcpy(newElementData, this->elementData, this->size * sizeof(T));   \
            w_free(this->elementData);                                           \
            this->elementData = newElementData;                                  \
        }                                                                        \
                                                                                 \
        /* 添加元素 */                                                           \
        for (int64_t i = this->size; i > index; i--)                             \
        {                                                                        \
            this->elementData[i] = this->elementData[i - 1];                     \
        }                                                                        \
        this->elementData[index] = element;                                      \
        this->size++;                                                            \
    }

// 列表删除元素
#define w_List_remove(T) w_concat(w_List(T), _remove)
#define w_List_remove_define_(T)                                      \
    /**                                                               \
     * 列表删除元素                                             \
     * @param this 列表                                             \
     * @param index 索引                                            \
     * @return T 删除的元素                                      \
     */                                                               \
    static inline T w_List_remove(T)(w_List(T) * this, int64_t index) \
    {                                                                 \
        /* 断言 */                                                    \
        w_assert(this != NULL);                                       \
        w_assert(this->elementData != NULL);                          \
        w_assert(index >= 0 && index < this->size);                   \
                                                                      \
        /* 删除元素 */                                                \
        T element = this->elementData[index];                         \
        for (int64_t i = index; i < this->size - 1; i++)              \
        {                                                             \
            this->elementData[i] = this->elementData[i + 1];          \
        }                                                             \
        this->size--;                                                 \
        return element;                                               \
    }

// 列表是否为空
#define w_List_isEmpty(T) w_concat(w_List(T), _isEmpty)
#define w_List_isEmpty_define_(T)                          \
    /**                                                    \
     * 列表是否为空                                  \
     * @param this 列表                                  \
     * @return bool true:为空 false:不为空            \
     */                                                    \
    static inline bool w_List_isEmpty(T)(w_List(T) * this) \
    {                                                      \
        w_assert(this != NULL);                            \
        w_assert(this->elementData != NULL);               \
        return this->size == 0;                            \
    }

// 列表插入元素到头部
#define w_List_addFirst(T) w_concat(w_List(T), _addFirst)
#define w_List_addFirst_define_(T)                                     \
    /**                                                                \
     * 列表插入元素到头部                                     \
     * @param this 列表                                              \
     * @param element 元素                                           \
     */                                                                \
    static inline void w_List_addFirst(T)(w_List(T) * this, T element) \
    {                                                                  \
        w_List_add(T)(this, 0, element);                               \
    }

// 列表插入元素到尾部
#define w_List_addLast(T) w_concat(w_List(T), _addLast)
#define w_List_addLast_define_(T)                                     \
    /**                                                               \
     * 列表插入元素到尾部                                    \
     * @param this 列表                                             \
     * @param element 元素                                          \
     */                                                               \
    static inline void w_List_addLast(T)(w_List(T) * this, T element) \
    {                                                                 \
        w_List_add(T)(this, this->size, element);                     \
    }

// 列表删除头部元素
#define w_List_removeFirst(T) w_concat(w_List(T), _removeFirst)
#define w_List_removeFirst_define_(T)                       \
    /**                                                     \
     * 列表删除头部元素                             \
     * @param this 列表                                   \
     * @return T 删除的元素                            \
     */                                                     \
    static inline T w_List_removeFirst(T)(w_List(T) * this) \
    {                                                       \
        return w_List_remove(T)(this, 0);                   \
    }

// 列表删除尾部元素
#define w_List_removeLast(T) w_concat(w_List(T), _removeLast)
#define w_List_removeLast_define_(T)                       \
    /**                                                    \
     * 列表删除尾部元素                            \
     * @param this 列表                                  \
     * @return T 删除的元素                           \
     */                                                    \
    static inline T w_List_removeLast(T)(w_List(T) * this) \
    {                                                      \
        return w_List_remove(T)(this, this->size - 1);     \
    }

// 列表获取数据指针
#define w_List_data(T) w_concat(w_List(T), _data)
#define w_List_data_define_(T)                                                        \
    /**                                                                               \
     * 列表获取数据指针                                                       \
     * @param this 列表                                                             \
     * @return T * 数据指针（慎用，如果扩容会导致该指针被释放） \
     */                                                                               \
    static inline T *w_List_data(T)(w_List(T) * this)                                 \
    {                                                                                 \
        w_assert(this != NULL);                                                       \
        w_assert(this->elementData != NULL);                                          \
        return this->elementData;                                                     \
    }

// 清空列表
#define w_List_clear(T) w_concat(w_List(T), _clear)
#define w_List_clear_define_(T)                          \
    /**                                                  \
     * 清空列表                                      \
     * @param this 列表                                \
     * @return void                                      \
     */                                                  \
    static inline void w_List_clear(T)(w_List(T) * this) \
    {                                                    \
        w_assert(this != NULL);                          \
        w_assert(this->elementData != NULL);             \
        this->size = 0; /*  列表大小设置为 0 即可 */     \
    }

// 列表定义
#define w_List_define(T)                \
    w_List_type_define_(T);             \
    w_List_initWithCapacity_define_(T); \
    w_List_init_define_(T);             \
    w_List_deinit_define_(T);           \
    w_List_size_define_(T);             \
    w_List_capacity_define_(T);         \
    w_List_get_define_(T);              \
    w_List_set_define_(T);              \
    w_List_add_define_(T);              \
    w_List_remove_define_(T);           \
    w_List_isEmpty_define_(T);          \
    w_List_addFirst_define_(T);         \
    w_List_addLast_define_(T);          \
    w_List_removeFirst_define_(T);      \
    w_List_removeLast_define_(T);       \
    w_List_data_define_(T);             \
    w_List_clear_define_(T)

// ========================================================================================================================================================
//  Map
// ========================================================================================================================================================

// MapEntry 类型
#define w_Map_Entry(K, V) w_concat(w_concat(w_concat(w_Map_Entry_, K), _), V)

// MapEntry 定义
#define w_Map_Entry_type_define_(K, V)   \
    typedef struct w_Map_Entry(K, V)     \
    {                                    \
        K key;                           \
        V value;                         \
        struct w_Map_Entry(K, V) * next; \
    } w_Map_Entry(K, V);

// Map 类型
#define w_Map(K, V) w_concat(w_concat(w_concat(w_Map_, K), _), V)

// Map 类型定义
#define w_Map_type_define_(K, V)        \
    typedef struct w_Map(K, V)          \
    {                                   \
        w_Map_Entry(K, V) * *entryData; \
        int64_t entryDataSize;          \
        int64_t size;                   \
    } w_Map(K, V);

// Map 初始化
#define w_Map_init(K, V) w_concat(w_Map(K, V), _init)
#define w_Map_init_define_(K, V)                                                       \
    /**                                                                                \
     * Map 初始化                                                                   \
     * @param this Map                                                                 \
     * @return void                                                                    \
     */                                                                                \
    static inline void w_Map_init(K, V)(w_Map(K, V) * this)                            \
    {                                                                                  \
        this->entryDataSize = 16;                                                      \
        this->entryData = w_malloc(sizeof(w_Map_Entry(K, V) *) * this->entryDataSize); \
        w_assert(this->entryData != NULL);                                             \
        memset(this->entryData, 0, sizeof(w_Map_Entry(K, V) *) * this->entryDataSize); \
        this->size = 0;                                                                \
    }

// Map 释放
#define w_Map_deinit(K, V) w_concat(w_Map(K, V), _deinit)
#define w_Map_deinit_define_(K, V)                            \
    /**                                                       \
     * Map 释放                                             \
     * @param this Map                                        \
     * @return void                                           \
     */                                                       \
    static inline void w_Map_deinit(K, V)(w_Map(K, V) * this) \
    {                                                         \
        /* 遍历链表 */                                        \
        for (int64_t i = 0; i < this->entryDataSize; i++)     \
        {                                                     \
            /* 链表头 */                                      \
            w_Map_Entry(K, V) *entry = this->entryData[i];    \
            while (entry != NULL)                             \
            {                                                 \
                /* 释放节点 */                                \
                w_Map_Entry(K, V) *next = entry->next;        \
                w_free(entry);                                \
                entry = next;                                 \
            }                                                 \
        }                                                     \
        memset(this, 0, sizeof(w_Map(K, V)));                 \
    }

// Map 放置 键值对 到 键值对数组
#define w_Map_putToEntryData_(K, V) w_concat(w_Map(K, V), _putToEntryData_)
#define w_Map_putToEntryData_define_(K, V)                                                                                \
    /**                                                                                                                   \
     * 放置 键值对 到 键值对数组                                                                               \
     * @param entryData 键值对数组                                                                                   \
     * @param entryDataSize 键值对数组大小                                                                         \
     * @param key 键                                                                                                     \
     * @param value 值                                                                                                   \
     * @return bool 是否添加新的元素                                                                              \
     */                                                                                                                   \
    static inline bool w_Map_putToEntryData_(K, V)(w_Map_Entry(K, V) * *entryData, int64_t entryDataSize, K key, V value) \
    {                                                                                                                     \
        w_assert(entryData != NULL);                                                                                      \
        w_assert(entryDataSize > 0);                                                                                      \
        /* 计算索引 */                                                                                                    \
        int64_t index = w_hash(K)(&key) & (entryDataSize - 1);                                                            \
                                                                                                                          \
        /* 链表头 */                                                                                                      \
        w_Map_Entry(K, V) *entry = entryData[index];                                                                      \
        while (entry != NULL)                                                                                             \
        {                                                                                                                 \
            /* 键相等 */                                                                                                  \
            if (w_equals(K)(&(entry->key), &key))                                                                         \
            {                                                                                                             \
                /* 覆盖值 */                                                                                              \
                entry->value = value;                                                                                     \
                return false;                                                                                             \
            }                                                                                                             \
            entry = entry->next;                                                                                          \
        }                                                                                                                 \
                                                                                                                          \
        /* 创建节点 */                                                                                                    \
        entry = w_malloc(sizeof(w_Map_Entry(K, V)));                                                                      \
        w_assert(entry != NULL);                                                                                          \
        entry->key = key;                                                                                                 \
        entry->value = value;                                                                                             \
        /* 头插法 */                                                                                                      \
        entry->next = entryData[index];                                                                                   \
        entryData[index] = entry;                                                                                         \
        return true;                                                                                                      \
    }

// Map 扩容
#define w_Map_realloc_(K, V) w_concat(w_Map(K, V), _realloc_)
#define w_Map_realloc_define_(K, V)                                                                           \
    /**                                                                                                       \
     * Map 扩容                                                                                             \
     * @param this Map                                                                                        \
     * @return void                                                                                           \
     */                                                                                                       \
    static inline void w_Map_realloc_(K, V)(w_Map(K, V) * this)                                               \
    {                                                                                                         \
        /* 申请新的键值对数组 */                                                                              \
        w_Map_Entry(K, V) **newEntryData = w_malloc(sizeof(w_Map_Entry(K, V) *) * this->entryDataSize * 2);   \
        w_assert(newEntryData != NULL);                                                                       \
        memset(newEntryData, 0, sizeof(w_Map_Entry(K, V) *) * this->entryDataSize * 2);                       \
                                                                                                              \
        /* 将旧键值对数组中的键值对放置到新键值对数组中 */                                                    \
        for (int64_t i = 0; i < this->entryDataSize; i++)                                                     \
        {                                                                                                     \
            w_Map_Entry(K, V) *entry = this->entryData[i];                                                    \
            while (entry != NULL)                                                                             \
            {                                                                                                 \
                w_Map_putToEntryData_(K, V)(newEntryData, this->entryDataSize * 2, entry->key, entry->value); \
                w_Map_Entry(K, V) *next = entry->next;                                                        \
                w_free(entry);                                                                                \
                entry = next;                                                                                 \
            }                                                                                                 \
        }                                                                                                     \
                                                                                                              \
        /*  更新 */                                                                                           \
        w_free(this->entryData);                                                                              \
        this->entryData = newEntryData;                                                                       \
        this->entryDataSize *= 2;                                                                             \
    }

// Map 放置键值对
#define w_Map_put(K, V) w_concat(w_Map(K, V), _put)
#define w_Map_put_define_(K, V)                                                            \
    /**                                                                                    \
     * Map 放置键值对                                                                 \
     * @param this Map                                                                     \
     * @param key 键                                                                      \
     * @param value 值                                                                    \
     * @return void                                                                        \
     */                                                                                    \
    static inline void w_Map_put(K, V)(w_Map(K, V) * this, K key, V value)                 \
    {                                                                                      \
        w_assert(this != NULL);                                                            \
        w_assert(this->entryData != NULL);                                                 \
        w_assert(this->entryDataSize > 0);                                                 \
        w_assert(this->size >= 0);                                                         \
                                                                                           \
        /* 扩容 */                                                                         \
        if ((double)this->size / this->entryDataSize >= 0.75)                              \
        {                                                                                  \
            w_Map_realloc_(K, V)(this);                                                    \
        }                                                                                  \
                                                                                           \
        /* 添加键值对 */                                                                   \
        if (w_Map_putToEntryData_(K, V)(this->entryData, this->entryDataSize, key, value)) \
        {                                                                                  \
            this->size++;                                                                  \
        }                                                                                  \
    }

// Map 获取值
#define w_Map_get(K, V) w_concat(w_Map(K, V), _get)
#define w_Map_get_define_(K, V)                                      \
    /**                                                              \
     * Map 获取值                                                 \
     * 如果元素不存在，则报错                             \
     * @param this Map                                               \
     * @param key 键                                                \
     * @return 值                                                   \
     */                                                              \
    static inline V w_Map_get(K, V)(w_Map(K, V) * this, K key)       \
    {                                                                \
        w_assert(this != NULL);                                      \
        w_assert(this->entryData != NULL);                           \
        w_assert(this->entryDataSize > 0);                           \
        w_assert(this->size >= 0);                                   \
                                                                     \
        /* 计算索引 */                                               \
        int64_t index = w_hash(K)(&key) & (this->entryDataSize - 1); \
                                                                     \
        /* 查找 */                                                   \
        w_Map_Entry(K, V) *entry = this->entryData[index];           \
        while (entry != NULL)                                        \
        {                                                            \
            if (w_equals(K)(&(entry->key), &key))                    \
            {                                                        \
                return entry->value;                                 \
            }                                                        \
            entry = entry->next;                                     \
        }                                                            \
                                                                     \
        /* 未找到 */                                                 \
        w_assert(false);                                             \
    }

// Map 删除键值对
#define w_Map_remove(K, V) w_concat(w_Map(K, V), _remove)
#define w_Map_remove_define_(K, V)                                   \
    /**                                                              \
     * Map 删除键值对                                           \
     * @param this Map                                               \
     * @param key 键                                                \
     * @return void                                                  \
     */                                                              \
    static inline void w_Map_remove(K, V)(w_Map(K, V) * this, K key) \
    {                                                                \
        w_assert(this != NULL);                                      \
        w_assert(this->entryData != NULL);                           \
        w_assert(this->entryDataSize > 0);                           \
        w_assert(this->size >= 0);                                   \
                                                                     \
        /* 删除 */                                                   \
        int64_t index = w_hash(K)(&key) & (this->entryDataSize - 1); \
        w_Map_Entry(K, V) *entry = this->entryData[index];           \
        if (entry == NULL)                                           \
        {                                                            \
            return;                                                  \
        }                                                            \
        /* 头 */                                                     \
        if (w_equals(K)(&(entry->key), &key))                        \
        {                                                            \
            this->entryData[index] = entry->next;                    \
            w_free(entry);                                           \
            this->size--;                                            \
            return;                                                  \
        }                                                            \
        /* 中间 */                                                   \
        while (entry->next != NULL)                                  \
        {                                                            \
            if (w_equals(K)(&(entry->next->key), &key))              \
            {                                                        \
                w_Map_Entry(K, V) *next = entry->next;               \
                entry->next = next->next;                            \
                w_free(next);                                        \
                this->size--;                                        \
                return;                                              \
            }                                                        \
            entry = entry->next;                                     \
        }                                                            \
    }

// Map 大小
#define w_Map_size(K, V) w_concat(w_Map(K, V), _size)
#define w_Map_size_define_(K, V)                               \
    /**                                                        \
     * Map 大小                                              \
     * @param this Map                                         \
     * @return int64_t 键值对数量                         \
     */                                                        \
    static inline int64_t w_Map_size(K, V)(w_Map(K, V) * this) \
    {                                                          \
        w_assert(this != NULL);                                \
        w_assert(this->entryData != NULL);                     \
        w_assert(this->entryDataSize > 0);                     \
        w_assert(this->size >= 0);                             \
        return this->size;                                     \
    }

// Map 是否包含键
#define w_Map_containsKey(K, V) w_concat(w_Map(K, V), _containsKey)
#define w_Map_containsKey_define_(K, V)                                   \
    /**                                                                   \
     * Map 是否包含键                                                \
     * @param this Map                                                    \
     * @param key 键                                                     \
     * @return bool                                                       \
     */                                                                   \
    static inline bool w_Map_containsKey(K, V)(w_Map(K, V) * this, K key) \
    {                                                                     \
        w_assert(this != NULL);                                           \
        w_assert(this->entryData != NULL);                                \
        w_assert(this->entryDataSize > 0);                                \
        w_assert(this->size >= 0);                                        \
                                                                          \
        /* 索引 */                                                        \
        int64_t index = w_hash(K)(&key) & (this->entryDataSize - 1);      \
        w_Map_Entry(K, V) *entry = this->entryData[index];                \
        while (entry != NULL)                                             \
        {                                                                 \
            if (w_equals(K)(&(entry->key), &key))                         \
            {                                                             \
                return true;                                              \
            }                                                             \
            entry = entry->next;                                          \
        }                                                                 \
        return false;                                                     \
    }

// Map 迭代器
#define w_Map_Iterator(K, V) w_concat(w_Map(K, V), _Iterator)
#define w_Map_Iterator_type_define_(K, V) \
    typedef struct                        \
    {                                     \
        w_Map(K, V) * map;                \
        int64_t index;                    \
        w_Map_Entry(K, V) * entry;        \
    } w_Map_Iterator(K, V);

// Map 获取迭代器
#define w_Map_iterator(K, V) w_concat(w_Map(K, V), _iterator)
#define w_Map_iterator_define_(K, V)                                                                               \
    /**                                                                                                            \
     * Map 获取迭代器                                                                                         \
     * 使用完毕后不需要释放，使用期间不允许修改 Map，Map 修改后需要重新获取迭代器 \
     * @param this Map                                                                                             \
     * @return w_Map_Iterator 返回一个新的迭代器                                                          \
     */                                                                                                            \
    static inline w_Map_Iterator(K, V) w_Map_iterator(K, V)(w_Map(K, V) * this)                                    \
    {                                                                                                              \
        w_assert(this != NULL);                                                                                    \
        w_assert(this->entryData != NULL);                                                                         \
        w_assert(this->entryDataSize > 0);                                                                         \
        return (w_Map_Iterator(K, V)){this, 0, this->entryData[0]};                                                \
    }

// Map 迭代器获取下一个
#define w_Map_Iterator_next(K, V) w_concat(w_Map(K, V), _Iterator_next)
#define w_Map_Iterator_next_define_(K, V)                                                                 \
    /**                                                                                                   \
     * 迭代器获取下一个键值对（可以修改值，但不能修改键，会同步到 Map 中） \
     * @param this 迭代器                                                                              \
     * @return w_Map_Entry 键值对，如果为 NULL 则迭代结束                                     \
     */                                                                                                   \
    static inline w_Map_Entry(K, V) * w_Map_Iterator_next(K, V)(w_Map_Iterator(K, V) * this)              \
    {                                                                                                     \
        w_assert(this != NULL);                                                                           \
        w_assert(this->map != NULL);                                                                      \
        w_assert(this->map->entryData != NULL);                                                           \
        w_assert(this->map->entryDataSize > 0);                                                           \
                                                                                                          \
        /* 移动到下一个索引 */                                                                            \
        while (this->entry == NULL)                                                                       \
        {                                                                                                 \
            this->index++;                                                                                \
            this->entry = this->map->entryData[this->index];                                              \
            if (this->index >= this->map->entryDataSize)                                                  \
            {                                                                                             \
                return NULL;                                                                              \
            }                                                                                             \
        }                                                                                                 \
                                                                                                          \
        /* 返回并移动到下一个位置 */                                                                      \
        w_Map_Entry(K, V) *entry = this->entry;                                                           \
        this->entry = this->entry->next;                                                                  \
        return entry;                                                                                     \
    }

// Map 定义
// 定义 Map 需要定义 K 的 w_hash 和 w_equals 函数
#define w_Map_define(K, V)              \
    w_Map_Entry_type_define_(K, V);     \
    w_Map_type_define_(K, V);           \
    w_Map_init_define_(K, V);           \
    w_Map_deinit_define_(K, V);         \
    w_Map_putToEntryData_define_(K, V); \
    w_Map_realloc_define_(K, V);        \
    w_Map_put_define_(K, V);            \
    w_Map_get_define_(K, V);            \
    w_Map_remove_define_(K, V);         \
    w_Map_size_define_(K, V);           \
    w_Map_containsKey_define_(K, V);    \
    w_Map_Iterator_type_define_(K, V);  \
    w_Map_iterator_define_(K, V);       \
    w_Map_Iterator_next_define_(K, V);

// ========================================================================================================================================================
//  Set
// ========================================================================================================================================================

// Set 类型
#define w_Set(T) w_concat(w_Set_, T)

// Set Map 值类型
typedef struct
{
} w_Set_MapValueType_;

// Set 类型定义
#define w_Set_type_define_(T)              \
    w_Map_define(T, w_Set_MapValueType_);  \
    typedef struct                         \
    {                                      \
        w_Map(T, w_Set_MapValueType_) map; \
    } w_Set(T);

// Set 初始化
#define w_Set_init(T) w_concat(w_Set(T), _init)
#define w_Set_init_define_(T)                           \
    /**                                                 \
     * Set 初始化                                    \
     * @param this Set                                  \
     * @return void                                     \
     */                                                 \
    static inline void w_Set_init(T)(w_Set(T) * this)   \
    {                                                   \
        w_assert(this != NULL);                         \
        w_Map_init(T, w_Set_MapValueType_)(&this->map); \
    }

// Set 销毁
#define w_Set_deinit(T) w_concat(w_Set(T), _deinit)
#define w_Set_deinit_define_(T)                           \
    /**                                                   \
     * Set 销毁                                         \
     * @param this Set                                    \
     * @return void                                       \
     */                                                   \
    static inline void w_Set_deinit(T)(w_Set(T) * this)   \
    {                                                     \
        w_assert(this != NULL);                           \
        w_Map_deinit(T, w_Set_MapValueType_)(&this->map); \
    }

// Set 添加
#define w_Set_add(T) w_concat(w_Set(T), _add)
#define w_Set_add_define_(T)                                                           \
    /**                                                                                \
     * Set 添加                                                                      \
     * @param this Set                                                                 \
     * @param value 值                                                                \
     * @return void                                                                    \
     */                                                                                \
    static inline void w_Set_add(T)(w_Set(T) * this, T value)                          \
    {                                                                                  \
        w_assert(this != NULL);                                                        \
        w_Map_put(T, w_Set_MapValueType_)(&this->map, value, (w_Set_MapValueType_){}); \
    }

// Set 移除
#define w_Set_remove(T) w_concat(w_Set(T), _remove)
#define w_Set_remove_define_(T)                                  \
    /**                                                          \
     * Set 移除                                                \
     * @param this Set                                           \
     * @param value 值                                          \
     * @return void                                              \
     */                                                          \
    static inline void w_Set_remove(T)(w_Set(T) * this, T value) \
    {                                                            \
        w_assert(this != NULL);                                  \
        w_Map_remove(T, w_Set_MapValueType_)(&this->map, value); \
    }

// Set 包含
#define w_Set_contains(T) w_concat(w_Set(T), _contains)
#define w_Set_contains_define_(T)                                            \
    /**                                                                      \
     * Set 包含                                                            \
     * @param this Set                                                       \
     * @param value 值                                                      \
     * @return bool 是否包含                                             \
     */                                                                      \
    static inline bool w_Set_contains(T)(w_Set(T) * this, T value)           \
    {                                                                        \
        w_assert(this != NULL);                                              \
        return w_Map_containsKey(T, w_Set_MapValueType_)(&this->map, value); \
    }

// Set 大小
#define w_Set_size(T) w_concat(w_Set(T), _size)
#define w_Set_size_define_(T)                                  \
    /**                                                        \
     * Set 大小                                              \
     * @param this Set                                         \
     * @return int 大小                                      \
     */                                                        \
    static inline int w_Set_size(T)(w_Set(T) * this)           \
    {                                                          \
        w_assert(this != NULL);                                \
        return w_Map_size(T, w_Set_MapValueType_)(&this->map); \
    }

// Set 迭代器
#define w_Set_Iterator(T) w_concat(w_Set(T), _Iterator)
#define w_Set_Iterator_type_define_(T)                      \
    typedef struct                                          \
    {                                                       \
        w_Map_Iterator(T, w_Set_MapValueType_) mapIterator; \
    } w_Set_Iterator(T);

// Set 获取迭代器
#define w_Set_iterator(T) w_concat(w_Set(T), _iterator)
#define w_Set_iterator_define_(T)                                                                                  \
    /**                                                                                                            \
     * Set 获取迭代器                                                                                         \
     * 使用完毕后不需要释放，使用期间不允许修改 Set，Set 修改后需要重新获取迭代器 \
     * @param this Set                                                                                             \
     * @return w_Set_Iterator(T) 迭代器                                                                         \
     */                                                                                                            \
    static inline w_Set_Iterator(T) w_Set_iterator(T)(w_Set(T) * this)                                             \
    {                                                                                                              \
        w_assert(this != NULL);                                                                                    \
        return (w_Set_Iterator(T)){w_Map_iterator(T, w_Set_MapValueType_)(&this->map)};                            \
    }

// Set 迭代器获取下一个元素
#define w_Set_Iterator_next(T) w_concat(w_Set(T), _Iterator_next)
#define w_Set_Iterator_next_define_(T)                                                                                \
    /**                                                                                                               \
     * Set 迭代器获取下一个元素                                                                             \
     * @param this 迭代器                                                                                          \
     * @param value 将下一个元素放入所指向的地址                                                        \
     * @return bool 是否有下一个元素                                                                          \
     */                                                                                                               \
    static inline bool w_Set_Iterator_next(T)(w_Set_Iterator(T) * this, T * value)                                    \
    {                                                                                                                 \
        w_assert(this != NULL);                                                                                       \
        w_Map_Entry(T, w_Set_MapValueType_) *entry = w_Map_Iterator_next(T, w_Set_MapValueType_)(&this->mapIterator); \
        if (entry)                                                                                                    \
        {                                                                                                             \
            *value = entry->key;                                                                                      \
            return true;                                                                                              \
        }                                                                                                             \
        return false;                                                                                                 \
    }

// Set 定义
// 定义 Set 需要定义 T 的 w_hash 和 w_equals 函数
#define w_Set_define(T)             \
    w_Set_type_define_(T);          \
    w_Set_init_define_(T);          \
    w_Set_deinit_define_(T);        \
    w_Set_add_define_(T);           \
    w_Set_remove_define_(T);        \
    w_Set_contains_define_(T);      \
    w_Set_size_define_(T);          \
    w_Set_Iterator_type_define_(T); \
    w_Set_iterator_define_(T);      \
    w_Set_Iterator_next_define_(T);

// ========================================================================================================================================================
//  数字类型的哈希和比较操作定义
// ========================================================================================================================================================

// 数字类型哈希函数
#define w_number_hash_define_(T)               \
    static inline int64_t w_hash(T)(T * value) \
    {                                          \
        w_assert(value != NULL);               \
        return (int64_t)*value;                \
    }

// 数字类型比较函数
#define w_number_equals_define_(T)               \
    static inline bool w_equals(T)(T * a, T * b) \
    {                                            \
        w_assert(a != NULL && b != NULL);        \
        return *a == *b;                         \
    }

// 数字类型比较函数
#define w_number_compare_define_(T)                  \
    static inline int64_t w_compare(T)(T * a, T * b) \
    {                                                \
        w_assert(a != NULL && b != NULL);            \
        T sub = *a - *b;                             \
        if (sub < 0)                                 \
        {                                            \
            return -1;                               \
        }                                            \
        else if (sub > 0)                            \
        {                                            \
            return 1;                                \
        }                                            \
        else                                         \
        {                                            \
            return 0;                                \
        }                                            \
    }

// 数字比较和哈希函数类型定义
#define w_number_type_hash_and_compare_define_(T) \
    w_number_hash_define_(T);                     \
    w_number_equals_define_(T);                   \
    w_number_compare_define_(T);

// 使用宏定义定义所有数字类型的比较和哈希函数
w_number_type_hash_and_compare_define_(int8_t);
w_number_type_hash_and_compare_define_(int16_t);
w_number_type_hash_and_compare_define_(int32_t);
w_number_type_hash_and_compare_define_(int64_t);
w_number_type_hash_and_compare_define_(uint8_t);
w_number_type_hash_and_compare_define_(uint16_t);
w_number_type_hash_and_compare_define_(uint32_t);
w_number_type_hash_and_compare_define_(uint64_t);
w_number_type_hash_and_compare_define_(bool);
w_number_type_hash_and_compare_define_(char);
w_number_type_hash_and_compare_define_(short);
w_number_type_hash_and_compare_define_(int);
w_number_type_hash_and_compare_define_(long);
w_number_type_hash_and_compare_define_(float);
w_number_type_hash_and_compare_define_(double);

// ========================================================================================================================================================
//  指针类型
// ========================================================================================================================================================

// 指针类型
// 简单封装，为了传入 w_concat 后能输出有效标识符
#define w_Ptr(T) w_concat(w_Ptr_, T)

// 指针类型定义
#define w_Ptr_define(T) typedef T *w_Ptr(T)

// ========================================================================================================================================================
//  字符串构造器
// ========================================================================================================================================================

// StringBuilder 类型定义
typedef char w_StringBuilder_ValueType_;
w_List_define(w_StringBuilder_ValueType_);
typedef struct
{
    w_List(w_StringBuilder_ValueType_) list;
} w_StringBuilder;

/**
 * StringBuilder 初始化
 * @param this
 * @return void
 */
static inline void w_StringBuilder_init(w_StringBuilder *this)
{
    w_assert(this != NULL);
    w_List_init(w_StringBuilder_ValueType_)(&(this->list));
    w_List_addLast(w_StringBuilder_ValueType_)(&(this->list), '\0');
}

/**
 * StringBuilder 释放
 * @param this
 * @return void
 */
static inline void w_StringBuilder_deinit(w_StringBuilder *this)
{
    w_assert(this != NULL);
    w_List_deinit(w_StringBuilder_ValueType_)(&(this->list));
}

/**
 * 获取大小
 * @param this
 * @return int64_t 这个大小是不包含 '\0' 字符的
 */
static inline int64_t w_StringBuilder_size(w_StringBuilder *this)
{
    w_assert(this != NULL);
    return w_List_size(w_StringBuilder_ValueType_)(&(this->list)) - 1;
}

/**
 * 获取容量
 * @param this
 * @return int64_t
 */
static inline int64_t w_StringBuilder_capacity(w_StringBuilder *this)
{
    w_assert(this != NULL);
    return w_List_capacity(w_StringBuilder_ValueType_)(&(this->list));
}

/**
 * 写入到 C 字符串
 * @param this
 * @param buffer 要被写入的缓冲区（注意：buffer 的大小应该是 size + 1，因为要留一个位置给 '\0'）
 * @return void
 */
static inline void w_StringBuilder_toChars(w_StringBuilder *this, char *buffer)
{
    w_assert(this != NULL);
    w_assert(buffer != NULL);
    int64_t size = w_StringBuilder_size(this);
    memcpy(buffer, w_List_data(w_StringBuilder_ValueType_)(&(this->list)), size * sizeof(w_StringBuilder_ValueType_));
    buffer[size] = '\0';
}

/**
 * 获取数据
 * @param this
 * @return char * 返回 C 字符串（慎用，该位置在添加数据后可能被释放）
 */
static inline char *w_StringBuilder_data(w_StringBuilder *this)
{
    w_assert(this != NULL);
    return w_List_data(w_StringBuilder_ValueType_)(&(this->list));
}

/**
 * 翻转序列
 * @param this
 * @return void
 */
static inline void w_StringBuilder_reverse(w_StringBuilder *this)
{
    w_assert(this != NULL);
    int64_t i = 0, j = w_StringBuilder_size(this) - 1;
    while (i < j)
    {
        char temp = w_List_get(w_StringBuilder_ValueType_)(&(this->list), i);
        w_List_set(w_StringBuilder_ValueType_)(&(this->list), i, w_List_get(w_StringBuilder_ValueType_)(&(this->list), j));
        w_List_set(w_StringBuilder_ValueType_)(&(this->list), j, temp);
        i++;
        j--;
    }
}

/**
 * 获取某个索引的元素
 * @param this
 * @param index 索引
 * @return char
 */
static inline char w_StringBuilder_charAt(w_StringBuilder *this, int64_t index)
{
    w_assert(this != NULL);
    w_assert(index >= 0 && index < w_StringBuilder_size(this));
    return w_List_get(w_StringBuilder_ValueType_)(&(this->list), index);
}

/**
 * 设置某个索引的元素
 * @param this
 * @param index 索引
 * @param value 要设置的字符
 * @return void
 */
static inline void w_StringBuilder_setCharAt(w_StringBuilder *this, int64_t index, char value)
{
    w_assert(this != NULL);
    w_assert(index >= 0 && index < w_StringBuilder_size(this));
    w_List_set(w_StringBuilder_ValueType_)(&(this->list), index, value);
}

/**
 * 将 char 插入到此序列
 * @param this
 * @param index 索引
 * @param value 要插入的字符
 * @return void
 */
static inline void w_StringBuilder_insertChar(w_StringBuilder *this, int64_t index, char value)
{
    // 断言
    w_assert(this != NULL);
    w_assert(index >= 0 && index <= w_StringBuilder_size(this));
    // 插入
    w_List_add(w_StringBuilder_ValueType_)(&(this->list), index, value);
}

/**
 * 将 char[] 的子序列插入到此序列
 * @param this
 * @param index 索引
 * @param str 要插入的字符串
 * @param offset 子序列的起始索引
 * @param len 子序列的长度
 * @return void
 */
static inline void w_StringBuilder_insertSubChars(w_StringBuilder *this, int64_t index, const char *str, int64_t offset, int64_t len)
{
    // 断言
    w_assert(this != NULL);
    w_assert(str != NULL);
    int64_t strSize = strlen(str);
    w_assert(offset >= 0 && offset <= strSize);
    w_assert(len >= 0 && offset + len <= strSize);

    // 循环插入
    for (int64_t i = 0; i < len; i++)
    {
        w_StringBuilder_insertChar(this, index + i, str[offset + i]);
    }
}

/**
 * 将 char[] 添加到此序列
 * @param this
 * @param str 要添加的字符串
 * @return void
 */
static inline void w_StringBuilder_insertChars(w_StringBuilder *this, int64_t index, const char *str)
{
    w_StringBuilder_insertSubChars(this, index, str, 0, strlen(str));
}

/**
 * 将 bool 插入到此序列
 * @param this
 * @param index 索引
 * @param value 要插入的布尔值
 * @return void
 */
static inline void w_StringBuilder_insertBool(w_StringBuilder *this, int64_t index, bool value)
{
    w_StringBuilder_insertChars(this, index, value ? "true" : "false");
}

/**
 * 将 double 插入到此序列
 * @param this
 * @param index 索引
 * @param value 要插入的 double 值
 * @return void
 */
static inline void w_StringBuilder_insertDouble(w_StringBuilder *this, int64_t index, double value)
{
    w_assert(this != NULL);
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.15g", value);
    w_StringBuilder_insertChars(this, index, buffer);
}

/**
 * 将 float 插入到此序列
 * @param this
 * @param index 索引
 * @param value 要插入的 float 值
 * @return void
 */
static inline void w_StringBuilder_insertFloat(w_StringBuilder *this, int64_t index, float value)
{
    w_StringBuilder_insertDouble(this, index, value);
}

/**
 * 将 long long (int64_t) 插入到此序列
 * @param this
 * @param index 索引
 * @param value 要插入的 long long (int64_t) 值
 * @return void
 */
static inline void w_StringBuilder_insertLong(w_StringBuilder *this, int64_t index, int64_t value)
{
    w_assert(this != NULL);
    char buffer[32];
    sprintf(buffer, "%lld", value);
    w_StringBuilder_insertChars(this, index, buffer);
}

/**
 * 将 int 插入到此序列
 * @param this
 * @param index 索引
 * @param value 要插入的 int 值
 * @return void
 */
static inline void w_StringBuilder_insertInt(w_StringBuilder *this, int64_t index, int value)
{
    w_StringBuilder_insertLong(this, index, value);
}

/**
 * 将 StringBuilder 的子序列插入到此序列
 * @param this
 * @param index 索引
 * @param value 要插入的字符串
 * @param offset 子序列的起始索引
 * @param len 子序列的长度
 * @return void
 */
static inline void w_StringBuilder_insertSubStringBuilder(w_StringBuilder *this, int64_t index, w_StringBuilder *value, int64_t offset, int64_t len)
{
    w_StringBuilder_insertSubChars(this, index, w_StringBuilder_data(value), offset, len);
}

/**
 * 将 StringBuilder 插入到此序列
 * @param this
 * @param value 要追加的字符串
 * @return void
 */
static inline void w_StringBuilder_insertStringBuilder(w_StringBuilder *this, int64_t index, w_StringBuilder *value)
{
    w_StringBuilder_insertSubStringBuilder(this, index, value, 0, w_StringBuilder_size(value));
}

/**
 * 将 char 追加到此序列
 * @param this
 * @param value 要追加的 char 值
 * @return void
 */
static inline void w_StringBuilder_appendChar(w_StringBuilder *this, char value)
{
    w_StringBuilder_insertChar(this, w_StringBuilder_size(this), value);
}

/**
 * 将 char[] 的子序列追加到此序列
 * @param this
 * @param value 要追加的字符串
 * @param offset 子序列的起始索引
 * @param len 子序列的长度
 * @return void
 */
static inline void w_StringBuilder_appendSubChars(w_StringBuilder *this, char *value, int64_t offset, int64_t len)
{
    w_StringBuilder_insertSubChars(this, w_StringBuilder_size(this), value, offset, len);
}

/**
 * 将 char[] 追加到此序列
 * @param this
 * @param value 要追加的字符串
 * @return void
 */
static inline void w_StringBuilder_appendChars(w_StringBuilder *this, char *value)
{
    w_StringBuilder_insertChars(this, w_StringBuilder_size(this), value);
}

/**
 * 将 bool 追加到序列中
 * @param this
 * @param value 要追加的布尔值
 * @return void
 */
static inline void w_StringBuilder_appendBool(w_StringBuilder *this, bool value)
{
    w_StringBuilder_insertBool(this, w_StringBuilder_size(this), value);
}

/**
 * 将 double 加到此序列
 * @param this
 * @param value 要追加的 double 值
 * @return void
 */
static inline void w_StringBuilder_appendDouble(w_StringBuilder *this, double value)
{
    w_StringBuilder_insertDouble(this, w_StringBuilder_size(this), value);
}

/**
 * 将 float 追加到此序列
 * @param this
 * @param value 要追加的 float 值
 * @return void
 */
static inline void w_StringBuilder_appendFloat(w_StringBuilder *this, float value)
{
    w_StringBuilder_insertFloat(this, w_StringBuilder_size(this), value);
}

/**
 * 将 long 追加到此序列
 * @param this
 * @param value 要追加的 long 值
 * @return void
 */
static inline void w_StringBuilder_appendLong(w_StringBuilder *this, int64_t value)
{
    w_StringBuilder_insertLong(this, w_StringBuilder_size(this), value);
}

/**
 * 将 int 追加到此序列
 * @param this
 * @param value 要追加的 int 值
 * @return void
 */
static inline void w_StringBuilder_appendInt(w_StringBuilder *this, int value)
{
    w_StringBuilder_insertInt(this, w_StringBuilder_size(this), value);
}

/**
 * 将 StringBuilder 的子序列追加到此序列
 * @param this
 * @param value 要追加的字符串
 * @param offset 子序列的起始索引
 * @param len 子序列的长度
 * @return void
 */
static inline void w_StringBuilder_appendSubStringBuilder(w_StringBuilder *this, w_StringBuilder *value, int64_t offset, int64_t len)
{
    w_StringBuilder_insertSubStringBuilder(this, w_StringBuilder_size(this), value, offset, len);
}

/**
 * 追加 StringBuilder 到此序列
 * @param this
 * @param value 要追加的 StringBuilder
 * @return void
 */
static inline void w_StringBuilder_appendStringBuilder(w_StringBuilder *this, w_StringBuilder *value)
{
    w_StringBuilder_insertStringBuilder(this, w_StringBuilder_size(this), value);
}

/**
 * 删除字符
 * @param this
 * @param index 要删除的字符的索引
 * @return void
 */
static inline void w_StringBuilder_removeCharAt(w_StringBuilder *this, int64_t index)
{
    w_assert(this != NULL);
    w_List_remove(w_StringBuilder_ValueType_)(&(this->list), index);
}

/**
 * 删除序列
 * @param this
 * @param offset 要删除的子序列的起始索引
 * @param len 要删除的子序列的长度
 * @return void
 */
static inline void w_StringBuilder_remove(w_StringBuilder *this, int64_t offset, int64_t len)
{
    w_assert(this != NULL);
    for (int64_t i = 0; i < len; i++)
    {
        w_StringBuilder_removeCharAt(this, offset);
    }
}

/**
 * 哈希函数
 * @param this
 * @return int64_t
 */
static inline int64_t w_hash(w_StringBuilder)(w_StringBuilder *this)
{
    w_assert(this != NULL);
    int64_t hash = 0;
    int64_t size = w_StringBuilder_size(this);
    for (int64_t i = 0; i < size; i++)
    {
        hash = hash * 31 + w_StringBuilder_charAt(this, i);
    }
    return hash;
}

/**
 * 比较函数
 * @param this
 * @param other
 * @return int64_t
 */
static inline int64_t w_compare(w_StringBuilder)(w_StringBuilder *this, w_StringBuilder *other)
{
    w_assert(this != NULL);
    w_assert(other != NULL);
    int64_t i = 0;
    while (true)
    {
        int64_t c1 = i < w_StringBuilder_size(this) ? w_StringBuilder_charAt(this, i) : '\0';
        int64_t c2 = i < w_StringBuilder_size(other) ? w_StringBuilder_charAt(other, i) : '\0';
        if (c1 > c2)
        {
            return 1;
        }
        else if (c1 < c2)
        {
            return -1;
        }
        i++;
    }
    return 0;
}

/**
 * 比较函数
 * @param this
 * @param other
 * @return bool
 */
static inline bool w_equals(w_StringBuilder)(w_StringBuilder *this, w_StringBuilder *other)
{
    w_assert(this != NULL);
    w_assert(other != NULL);
    if (w_StringBuilder_size(this) != w_StringBuilder_size(other))
    {
        // 长度不同
        return false;
    }
    return w_compare(w_StringBuilder)(this, other) == 0;
}

/**
 * 查找子串
 * @param this
 * @param fromIndex 起始索引
 * @param value 要查找的字符串
 * @return int64_t 起始索引，未找到返回 -1
 */
static inline int64_t w_StringBuilder_indexOfWithFromIndex(w_StringBuilder *this, int64_t fromIndex, const char *value)
{
    // 字符串长度
    int64_t thisLen = w_StringBuilder_size(this);
    int64_t valueLen = strlen(value);
    // 构建 next 数组
    int64_t *next = w_malloc(sizeof(int64_t) * valueLen);
    w_assert(next != NULL);
    next[0] = -1;
    int i = -1, j = 1;
    while (j < valueLen - 1)
    {
        if (i == -1 || value[i] == value[j])
        {
            i++;
            j++;
            next[j] = i;
        }
        else
        {
            i = next[i];
        }
    }
    // KMP 算法核心
    i = fromIndex;
    j = 0;
    while (i < thisLen && j < valueLen)
    {
        if (j == -1 || w_StringBuilder_charAt(this, i) == value[j])
        {
            i++;
            j++;
        }
        else
        {
            j = next[j];
        }
    }
    // 返回
    int64_t ret;
    if (j >= valueLen)
    {
        ret = i - valueLen;
    }
    else
    {
        ret = -1;
    }
    w_free(next);
    return ret;
}

/**
 * 查找子串
 * @param this
 * @param value 要查找的子串
 * @return int64_t 起始索引，未找到返回 -1
 */
static inline int64_t w_StringBuilder_indexOf(w_StringBuilder *this, const char *value)
{
    return w_StringBuilder_indexOfWithFromIndex(this, 0, value);
}

/**
 * 清空序列
 * @param this
 * @return void
 */
static inline void w_StringBuilder_clear(w_StringBuilder *this)
{
    w_assert(this != NULL);
    w_List_clear(w_StringBuilder_ValueType_)(&(this->list));
}

// ========================================================================================================================================================
//  大整型
// ========================================================================================================================================================

// 大整型定义
typedef uint8_t w_BigInt_BitType_;
w_List_define(w_BigInt_BitType_);
typedef struct
{
    w_List(w_BigInt_BitType_) nums;
    int8_t signum; // 符号位：-1 0 +1
} w_BigInt;

/**
 * 大整型初始化
 * @param this
 * @param number 数字字符串
 * @return void
 */
static inline void w_BigInt_init(w_BigInt *this, char *number)
{
    w_assert(this != NULL);
    w_List_init(w_BigInt_BitType_)(&(this->nums));
    // 使用 StringBuilder 存储数字
    w_StringBuilder numberBuilder;
    w_StringBuilder_init(&numberBuilder);
    w_StringBuilder_appendSubChars(&numberBuilder,
                                   number,
                                   number[0] == '-' || number[0] == '+' ? 1 : 0,
                                   number[0] == '-' || number[0] == '+' ? strlen(number) - 1 : strlen(number));
    // 使用模二除法转为二进制
    int64_t bitIndex = 0;
    while (w_StringBuilder_size(&numberBuilder) > 0)
    {
        // 去除多余的 0
        while (w_StringBuilder_size(&numberBuilder) > 0 &&
               w_StringBuilder_charAt(&numberBuilder, 0) == '0')
        {
            w_StringBuilder_removeCharAt(&numberBuilder, 0);
        }

        // 余数
        int64_t remainder = 0;

        // 除以二运算
        for (int64_t i = 0; i < w_StringBuilder_size(&numberBuilder); i++)
        {
            int64_t digit = w_StringBuilder_charAt(&numberBuilder, i) - '0';
            w_assert(digit >= 0 && digit <= 9);
            int64_t temp = remainder * 10 + digit;
            w_StringBuilder_setCharAt(&numberBuilder, i, temp / 2 + '0');
            remainder = temp % 2;
        }

        // 写入余数到数组
        if (bitIndex % 8 == 0)
        {
            w_List_addLast(w_BigInt_BitType_)(&(this->nums), 0);
        }
        w_List_set(w_BigInt_BitType_)(&(this->nums),
                                      bitIndex / 8,
                                      w_List_get(w_BigInt_BitType_)(&(this->nums), bitIndex / 8) | (remainder << (bitIndex % 8)));
        bitIndex++;
    }

    // 去除多余的 0
    while (w_List_size(w_BigInt_BitType_)(&(this->nums)) > 0 && w_List_get(w_BigInt_BitType_)(&(this->nums), w_List_size(w_BigInt_BitType_)(&(this->nums)) - 1) == 0)
    {
        w_List_removeLast(w_BigInt_BitType_)(&(this->nums));
    }

    // 符号位
    this->signum = number[0] == '-' ? -1 : 1;
    // 表示 0 时候，nums 为空且 signum 为 0
    if ((w_List_size(w_BigInt_BitType_)(&(this->nums)) == 1 && w_List_get(w_BigInt_BitType_)(&(this->nums), 0) == 0) || w_List_size(w_BigInt_BitType_)(&(this->nums)) == 0)
    {
        this->signum = 0;
        w_List_clear(w_BigInt_BitType_)(&(this->nums));
    }

    // 释放
    w_StringBuilder_deinit(&numberBuilder);
}

/**
 * 大整型析构
 * @param this
 * @return void
 */
static inline void w_BigInt_deinit(w_BigInt *this)
{
    w_assert(this != NULL);
    w_List_deinit(w_BigInt_BitType_)(&(this->nums));
}

/**
 * 转为字符串构造器
 * @param this
 * @param builder 字符串构造器
 * @return void
 */
static inline void w_BigInt_toStringBuilder(w_BigInt *this, w_StringBuilder *builder)
{
    w_assert(this != NULL);
    w_assert(builder != NULL);
    // 清空 builder
    w_StringBuilder_clear(builder);
    w_StringBuilder_appendChar(builder, '0');
    // 0
    if (this->signum == 0)
    {
        return;
    }
    // 乘二加下一位
    for (int64_t i = w_List_size(w_BigInt_BitType_)(&(this->nums)) - 1; i >= 0; i--)
    {
        w_BigInt_BitType_ num = w_List_get(w_BigInt_BitType_)(&(this->nums), i);
        for (int64_t j = 7; j >= 0; j--)
        {
            // 进位
            int64_t carry = 0;
            // 乘二
            for (int64_t k = w_StringBuilder_size(builder) - 1; k >= 0; k--)
            {
                carry = (w_StringBuilder_charAt(builder, k) - '0') * 2 + carry;
                w_StringBuilder_setCharAt(builder, k, carry % 10 + '0');
                carry = carry / 10;
            }
            if (carry > 0)
            {
                w_StringBuilder_insertChar(builder, 0, carry + '0');
            }
            // 取位
            int64_t digit = (num >> j) & 1;
            // 加上这个位
            carry = w_StringBuilder_charAt(builder, w_StringBuilder_size(builder) - 1) - '0' + digit;
            w_StringBuilder_setCharAt(builder, w_StringBuilder_size(builder) - 1, carry % 10 + '0');
            carry = carry / 10;
            for (int64_t k = w_StringBuilder_size(builder) - 2; k >= 0; k--)
            {
                carry = (w_StringBuilder_charAt(builder, k) - '0') + carry;
                w_StringBuilder_setCharAt(builder, k, carry % 10 + '0');
                carry = carry / 10;
            }
            if (carry > 0)
            {
                w_StringBuilder_insertChar(builder, 0, carry + '0');
            }
        }
    }
    // 符号位
    if (this->signum < 0)
    {
        w_StringBuilder_insertChar(builder, 0, '-');
    }
}

/**
 * 比较函数
 * @param this
 * @param other
 * @return int64_t 返回 this - other 的符号（-1 0 +1）
 */
static inline int64_t w_compare(w_BigInt)(w_BigInt *this, w_BigInt *other)
{
    w_assert(this != NULL && other != NULL);
    // 符号位不同
    if (this->signum > other->signum)
    {
        return 1;
    }
    else if (this->signum < other->signum)
    {
        return -1;
    }
    else if (this->signum == 0 && other->signum == 0)
    {
        // 0
        return 0;
    }
    // 符号位相同
    int64_t thisSize = w_List_size(w_BigInt_BitType_)(&(this->nums));
    int64_t otherSize = w_List_size(w_BigInt_BitType_)(&(other->nums));
    int64_t maxSize = thisSize > otherSize ? thisSize : otherSize;
    for (int64_t i = maxSize - 1; i >= 0; i--)
    {
        // 按位比较
        w_BigInt_BitType_ thisDigit = i < thisSize ? w_List_get(w_BigInt_BitType_)(&(this->nums), i) : 0;
        w_BigInt_BitType_ otherDigit = i < otherSize ? w_List_get(w_BigInt_BitType_)(&(other->nums), i) : 0;
        if (thisDigit > otherDigit)
        {
            return 1;
        }
        else if (thisDigit < otherDigit)
        {
            return -1;
        }
    }
    // 相同
    return 0;
}

/**
 * 比较函数
 * @param this
 * @param other
 * @return bool
 */
static inline bool w_equals(w_BigInt)(w_BigInt *this, w_BigInt *other)
{
    return w_compare(w_BigInt)(this, other) == 0;
}

/**
 * 哈希函数
 * @param this
 * @return int64_t
 */
static inline int64_t w_hash(w_BigInt)(w_BigInt *this)
{
    w_assert(this != NULL);
    // 符号位
    int64_t hash = this->signum;
    // 数字位
    for (int64_t i = w_List_size(w_BigInt_BitType_)(&(this->nums)) - 1; i >= 0; i--)
    {
        hash += w_List_get(w_BigInt_BitType_)(&(this->nums), i);
    }
    // 返回
    return hash;
}

/**
 * 复制函数，将 this 复制到 result 中
 * @param this
 * @param result
 * @return void
 */
static inline void w_BigInt_copyTo(w_BigInt *this, w_BigInt *result)
{
    w_assert(this != NULL && result != NULL);
    w_assert(this != result);
    // 符号位
    result->signum = this->signum;
    // 数字位
    w_List_clear(w_BigInt_BitType_)(&(result->nums));
    for (int64_t i = 0; i < w_List_size(w_BigInt_BitType_)(&(this->nums)); i++)
    {
        w_List_addLast(w_BigInt_BitType_)(&(result->nums), w_List_get(w_BigInt_BitType_)(&(this->nums), i));
    }
}

/**
 * 常量 0
 * @return w_BigInt * 禁止修改内部的任何值，否则会导致 bug
 */
static inline w_BigInt *w_BigInt_ZERO()
{
    static w_BigInt ZERO;
    static bool init = false;
    if (!init)
    {
        w_BigInt_init(&ZERO, "0");
        init = true;
    }
    return &ZERO;
}

/**
 * 常量 1
 * @return w_BigInt * 禁止修改内部任何值，否则会导致 bug
 */
static inline w_BigInt *w_BigInt_ONE()
{
    static w_BigInt ONE;
    static bool init = false;
    if (!init)
    {
        w_BigInt_init(&ONE, "1");
        init = true;
    }
    return &ONE;
}

/**
 * 常量 2
 * @return w_BigInt * 禁止修改内部任何值，否则会导致 bug
 */
static inline w_BigInt *w_BigInt_TWO()
{
    static w_BigInt TWO;
    static bool init = false;
    if (!init)
    {
        w_BigInt_init(&TWO, "2");
        init = true;
    }
    return &TWO;
}

/**
 * 常量 -1
 * @return w_BigInt * 禁止修改内部任何值，否则会导致 bug
 */
static inline w_BigInt *w_BigInt_NEGATIVE_ONE()
{
    static w_BigInt NEGATIVE_ONE;
    static bool init = false;
    if (!init)
    {
        w_BigInt_init(&NEGATIVE_ONE, "-1");
        init = true;
    }
    return &NEGATIVE_ONE;
}

/**
 * 常量 10
 * @return w_BigInt * 禁止修改内部任何值，否则会导致 bug
 */
static inline w_BigInt *w_BigInt_TEN()
{
    static w_BigInt TEN;
    static bool init = false;
    if (!init)
    {
        w_BigInt_init(&TEN, "10");
        init = true;
    }
    return &TEN;
}

/**
 * 同符号加法，需要手动确保 this 和 other 符号位相同
 * @param this
 * @param other
 * @param result
 * @return void
 */
static inline void w_BigInt_addSameSign_(w_BigInt *this, w_BigInt *other, w_BigInt *result)
{
    w_assert(this != NULL && other != NULL && result != NULL);
    w_assert(this != result && other != result);
    // 清空 result
    w_List_clear(w_BigInt_BitType_)(&(result->nums));
    // 符号位
    result->signum = this->signum != 0 ? this->signum : other->signum;
    // 计算数字位的和
    int64_t thisSize = w_List_size(w_BigInt_BitType_)(&(this->nums));
    int64_t otherSize = w_List_size(w_BigInt_BitType_)(&(other->nums));
    int64_t maxSize = thisSize > otherSize ? thisSize : otherSize;
    int64_t carry = 0; // 进位
    for (int64_t i = 0; i < maxSize; i++)
    {
        // 取出数字
        int64_t thisDigit = i < thisSize ? w_List_get(w_BigInt_BitType_)(&(this->nums), i) : 0;
        int64_t otherDigit = i < otherSize ? w_List_get(w_BigInt_BitType_)(&(other->nums), i) : 0;
        // 计算和
        int64_t sum = thisDigit + otherDigit + carry;
        w_List_addLast(w_BigInt_BitType_)(&(result->nums), sum % 256);
        carry = sum / 256;
    }
    if (carry > 0)
    {
        w_List_addLast(w_BigInt_BitType_)(&(result->nums), carry);
    }
}

/**
 * 同符号减法，需要手动确保 this 和 other 符号位相同
 * @param this
 * @param other
 * @param result
 * @return void
 */
static inline void w_BigInt_subSameSign_(w_BigInt *this, w_BigInt *other, w_BigInt *result)
{
    w_assert(this != NULL && other != NULL && result != NULL);
    w_assert(this != result && other != result);
    // 清空 result
    w_List_clear(w_BigInt_BitType_)(&(result->nums));
    // 判断数字大小并处理符号位
    result->signum = this->signum != 0 ? this->signum : other->signum;
    if (w_compare(w_BigInt)(this, other) < 0)
    {
        result->signum = -result->signum; // 符号位取反
        // 交换，使得 this >= other
        w_BigInt *temp = this;
        this = other;
        other = temp;
    }
    // 数字位减法
    int64_t thisSize = w_List_size(w_BigInt_BitType_)(&(this->nums));
    int64_t otherSize = w_List_size(w_BigInt_BitType_)(&(other->nums));
    int64_t maxSize = thisSize > otherSize ? thisSize : otherSize;
    int64_t borrow = 0; // 借位
    for (int64_t i = 0; i < maxSize; i++)
    {
        // 获取数字
        int64_t thisDigit = i < thisSize ? w_List_get(w_BigInt_BitType_)(&(this->nums), i) : 0;
        int64_t otherDigit = i < otherSize ? w_List_get(w_BigInt_BitType_)(&(other->nums), i) : 0;

        // 减法
        int64_t diff = thisDigit - otherDigit - borrow;
        if (diff < 0)
        {
            diff += 256;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }

        w_List_addLast(w_BigInt_BitType_)(&(result->nums), diff);
    }
    // 清空多余的 0
    while (w_List_size(w_BigInt_BitType_)(&(result->nums)) > 0 &&
           w_List_get(w_BigInt_BitType_)(&(result->nums), w_List_size(w_BigInt_BitType_)(&(result->nums)) - 1) == 0)
    {
        w_List_removeLast(w_BigInt_BitType_)(&(result->nums));
    }
    // 是否为 0
    if (w_List_size(w_BigInt_BitType_)(&(result->nums)) == 0)
    {
        result->signum = 0;
    }
}

/**
 * 加法
 * @param this
 * @param other
 * @param result
 * @return void
 */
static inline void w_BigInt_add(w_BigInt *this, w_BigInt *other, w_BigInt *result)
{
    w_assert(this != NULL && other != NULL && result != NULL);
    w_assert(this != result && other != result);
    // 符号位不同
    if (this->signum * other->signum < 0)
    {
        // 减法，即对 other 取反后 进行加法
        w_BigInt_subSameSign_(this, other, result);
    }
    else
    {
        // 加法，即直接进行加法
        w_BigInt_addSameSign_(this, other, result);
    }
}

/**
 * 减法
 * @param this
 * @param other
 * @param result
 * @return void
 */
static inline void w_BigInt_sub(w_BigInt *this, w_BigInt *other, w_BigInt *result)
{
    w_assert(this != NULL && other != NULL && result != NULL);
    w_assert(this != result && other != result);
    // 符号位不同
    if (this->signum * other->signum < 0)
    {
        // 加法，即对 other 取反后 进行加法
        w_BigInt_addSameSign_(this, other, result);
    }
    else
    {
        // 减法，即直接进行减法
        w_BigInt_subSameSign_(this, other, result);
    }
}

/**
 * 乘法
 * @param this
 * @param other
 * @param result
 * @return void
 */
static inline void w_BigInt_mul(w_BigInt *this, w_BigInt *other, w_BigInt *result)
{
    w_assert(this != NULL && other != NULL && result != NULL);
    w_assert(this != result && other != result);
    // 清空 result
    w_List_clear(w_BigInt_BitType_)(&(result->nums));
    result->signum = 0;
    // 0
    if (this->signum * other->signum == 0)
    {
        return;
    }
    // 数字位乘法
    w_BigInt mulTemp, resultTemp;
    w_BigInt_init(&mulTemp, "0");
    w_BigInt_init(&resultTemp, "0");
    // 从高位往低位乘
    for (int64_t i = w_List_size(w_BigInt_BitType_)(&(this->nums)) - 1; i >= 0; i--)
    {
        // 清零
        mulTemp.signum = 1; // 符号位为正
        w_List_clear(w_BigInt_BitType_)(&(mulTemp.nums));
        // 进位
        int64_t carry = 0;
        // 从低位往高位乘
        for (int64_t j = 0; j < w_List_size(w_BigInt_BitType_)(&(other->nums)); j++)
        {
            // 获取数字
            int64_t thisDigit = w_List_get(w_BigInt_BitType_)(&(this->nums), i);
            int64_t otherDigit = w_List_get(w_BigInt_BitType_)(&(other->nums), j);
            // 乘法
            int64_t product = thisDigit * otherDigit + carry;
            w_List_addLast(w_BigInt_BitType_)(&(mulTemp.nums), product % 256);
            carry = product / 256;
        }
        if (carry > 0)
        {
            w_List_addLast(w_BigInt_BitType_)(&(mulTemp.nums), carry);
        }
        // result 乘以 256
        if (result->signum != 0)
        {
            w_List_addFirst(w_BigInt_BitType_)(&(result->nums), 0);
        }
        // result 加上 temp
        w_BigInt_add(result, &mulTemp, &resultTemp);
        w_BigInt_copyTo(&resultTemp, result); // 赋值给 result
    }
    // 释放临时变量
    w_BigInt_deinit(&mulTemp);
    w_BigInt_deinit(&resultTemp);
    // 符号位设置
    result->signum = this->signum * other->signum;
}

/**
 * 除法
 * @param this 被除数
 * @param other 除数
 * @param result 商
 * @return void
 */
static inline void w_BigInt_div(w_BigInt *this, w_BigInt *other, w_BigInt *result)
{
    w_assert(this != NULL && other != NULL && result != NULL);
    w_assert(this != result && other != result);
    w_assert(other->signum != 0); // 除数不能为 0
    // 清空 result
    result->signum = 0;
    w_List_clear(w_BigInt_BitType_)(&(result->nums));
    // 当前被除数
    w_BigInt divTemp, quotient, temp, tmp;
    w_BigInt_init(&divTemp, "0");
    w_BigInt_init(&quotient, "0");
    w_BigInt_init(&temp, "0");
    w_BigInt_init(&tmp, "0");
    for (int64_t i = w_List_size(w_BigInt_BitType_)(&(this->nums)) - 1; i >= 0; i--)
    {
        // 放入一位
        divTemp.signum = 1;
        w_List_addFirst(w_BigInt_BitType_)(&(divTemp.nums), w_List_get(w_BigInt_BitType_)(&(this->nums), i));
        // 商重置为 0
        quotient.signum = 0;
        w_List_clear(w_BigInt_BitType_)(&(quotient.nums));
        // 商不断递增，直到 商乘以除数大于 被除数
        while (true)
        {
            // 计算 商 + 1 的值
            w_BigInt_add(&quotient, w_BigInt_ONE(), &tmp);
            // 计算 (商 + 1) * 除数
            w_BigInt_mul(&tmp, other, &temp);
            temp.signum = temp.signum != 0 ? 1 : 0; // 确保积非负
            // 判断是否位合适的商
            if (w_compare(w_BigInt)(&temp, &divTemp) > 0)
            {
                // 减去 商 * 除数
                w_BigInt_mul(&quotient, other, &temp);
                temp.signum = temp.signum != 0 ? 1 : 0;
                w_BigInt_sub(&divTemp, &temp, &tmp);
                w_BigInt_copyTo(&tmp, &divTemp);
                // 当前商 * 256 + 当前商位
                if (result->signum != 0)
                {
                    w_List_addFirst(w_BigInt_BitType_)(&(result->nums), 0);
                }
                w_BigInt_add(result, &quotient, &tmp);
                w_BigInt_copyTo(&tmp, result);
                break;
            }
            else
            {
                // 商 + 1
                w_BigInt_add(&quotient, w_BigInt_ONE(), &tmp);
                w_BigInt_copyTo(&tmp, &quotient);
            }
        }
    }
    // 释放
    w_BigInt_deinit(&divTemp);
    w_BigInt_deinit(&quotient);
    w_BigInt_deinit(&temp);
    w_BigInt_deinit(&tmp);
    // 符号位
    if (result->signum != 0)
    {
        // 商不为 0 时设置符号
        result->signum = this->signum * other->signum;
    }
}

/**
 * 取模
 * @param this
 * @param other
 * @param result result = this % other
 * @return void
 */
static inline void w_BigInt_mod(w_BigInt *this, w_BigInt *other, w_BigInt *result)
{
    w_assert(this != NULL && other != NULL && result != NULL);
    w_assert(this != result && other != result);
    w_assert(other->signum != 0); // 除数不能为 0

    w_BigInt quotient, temp;
    w_BigInt_init(&quotient, "0");
    w_BigInt_init(&temp, "0");

    // 计算商
    w_BigInt_div(this, other, &quotient);
    // 减去商乘以除数
    w_BigInt_mul(&quotient, other, &temp);
    w_BigInt_sub(this, &temp, result);

    w_BigInt_deinit(&quotient);
    w_BigInt_deinit(&temp);
}

/**
 * 快速幂
 * @param this 底数
 * @param other 幂数
 * @param result 结果
 * @return void
 */
static inline void w_BigInt_pow(w_BigInt *this, int64_t other, w_BigInt *result)
{
    w_assert(this != NULL && result != NULL);
    w_assert(this != result);
    w_assert(this->signum != 0); // 底数不能为 0
    w_assert(other >= 0);        // 幂数不能小于 0

    // 初始化临时变量
    w_BigInt temp, thisTemp;
    w_BigInt_init(&temp, "0");
    w_BigInt_init(&thisTemp, "0");

    // 初始化 result 为 1
    w_BigInt_copyTo(w_BigInt_ONE(), result);

    // 初始化 thisTemp
    w_BigInt_copyTo(this, &thisTemp);

    // 开始循环
    while (other > 0)
    {
        if ((other & 1) != 0)
        {
            // 奇数处理方案 result = result * this
            w_BigInt_mul(result, &thisTemp, &temp);
            w_BigInt_copyTo(&temp, result);
        }
        // 偶数处理方案 this = this * this
        w_BigInt_mul(&thisTemp, &thisTemp, &temp);
        w_BigInt_copyTo(&temp, &thisTemp);
        // 幂数右移一位
        other >>= 1;
    }

    // 释放
    w_BigInt_deinit(&temp);
    w_BigInt_deinit(&thisTemp);
}

/**
 * 最大公约数
 * @param this
 * @param other
 * @param result result = gcd(this, other)
 * @return void
 */
static inline void w_BigInt_gcd(w_BigInt *this, w_BigInt *other, w_BigInt *result)
{
    w_assert(this != NULL && other != NULL && result != NULL);
    w_assert(this != result && other != result);

    // 临时变量
    w_BigInt temp, thisTemp, otherTemp;
    w_BigInt_init(&temp, "0");
    w_BigInt_init(&thisTemp, "0");
    w_BigInt_init(&otherTemp, "0");
    w_BigInt_copyTo(this, &thisTemp);
    w_BigInt_copyTo(other, &otherTemp);
    thisTemp.signum = thisTemp.signum != 0 ? 1 : 0;
    otherTemp.signum = otherTemp.signum != 0 ? 1 : 0;

    while (otherTemp.signum != 0)
    {
        // other = this % other, this <=> other
        w_BigInt_mod(&thisTemp, &otherTemp, &temp);
        w_BigInt_copyTo(&otherTemp, &thisTemp);
        w_BigInt_copyTo(&temp, &otherTemp);
    }

    // 返回
    w_BigInt_copyTo(&thisTemp, result);

    // 释放
    w_BigInt_deinit(&temp);
    w_BigInt_deinit(&thisTemp);
    w_BigInt_deinit(&otherTemp);
}

/**
 * 绝对值
 * @param this
 * @param result result = |this|
 * @return void
 */
static inline void w_BigInt_abs(w_BigInt *this, w_BigInt *result)
{
    w_assert(this != NULL && result != NULL);
    w_assert(this != result);
    w_BigInt_copyTo(this, result);
    result->signum = result->signum != 0 ? 1 : 0;
}

/**
 * 取相反数
 * @param this
 * @param result result = -this
 * @return void
 */
static inline void w_BigInt_negate(w_BigInt *this, w_BigInt *result)
{
    w_assert(this != NULL && result != NULL);
    w_assert(this != result);
    w_BigInt_copyTo(this, result);
    result->signum = -result->signum;
}

/**
 * 转为补码表示
 * @param this
 * @param result 数字位使用补码表示
 * @return void
 */
static inline void w_BigInt_toComplement_(w_BigInt *this, w_BigInt *result)
{
    w_assert(this != NULL && result != NULL);
    w_assert(this != result);
    w_BigInt_copyTo(this, result);
    if (result->signum < 0)
    {
        // 负数
        // 符号位设置为正数
        result->signum = 1;
        // 按位取反，符号位不变（反码）
        int64_t len = w_List_size(w_BigInt_BitType_)(&(result->nums));
        for (int64_t i = 0; i < len; i++)
        {
            w_List_set(w_BigInt_BitType_)(&(result->nums), i, ~w_List_get(w_BigInt_BitType_)(&(result->nums), i));
        }
        // 数字部分加 1
        w_BigInt temp;
        w_BigInt_init(&temp, "0");
        w_BigInt_add(result, w_BigInt_ONE(), &temp);
        w_BigInt_copyTo(&temp, result);
        w_BigInt_deinit(&temp);
        // 符号位处理
        if (w_List_size(w_BigInt_BitType_)(&(result->nums)) > len)
        {
            result->signum = this->signum ^ w_List_get(w_BigInt_BitType_)(&(result->nums), len);
            w_List_removeLast(w_BigInt_BitType_)(&(result->nums));
            if (result->signum < 0)
            {
                result->signum = -1;
            }
            else if (result->signum > 0)
            {
                result->signum = 1;
            }
        }
        else
        {
            result->signum = this->signum;
        }
        // 去除多余的 0
        while (w_List_size(w_BigInt_BitType_)(&(result->nums)) > 0 && w_List_get(w_BigInt_BitType_)(&(result->nums), w_List_size(w_BigInt_BitType_)(&(result->nums)) - 1) == 0)
        {
            w_List_removeLast(w_BigInt_BitType_)(&(result->nums));
        }
        // 表示 0 时候，nums 为空且 signum 为 0
        if (w_List_size(w_BigInt_BitType_)(&(result->nums)) == 0)
        {
            result->signum = 0;
        }
    }
}

/**
 * 按位与
 * @param this
 * @param other
 * @param result result = this & other
 * @return void
 */
static inline void w_BigInt_and(w_BigInt *this, w_BigInt *other, w_BigInt *result)
{
    w_assert(this != NULL && other != NULL && result != NULL);
    w_assert(this != result && other != result);

    // 清空 result
    result->signum = 0;
    w_List_clear(w_BigInt_BitType_)(&(result->nums));

    // 临时变量
    w_BigInt temp, thisTemp, otherTemp;
    w_BigInt_init(&temp, "0");
    w_BigInt_init(&thisTemp, "0");
    w_BigInt_init(&otherTemp, "0");

    // 取补码
    w_BigInt_toComplement_(this, &thisTemp);
    w_BigInt_toComplement_(other, &otherTemp);

    // 按位与
    int64_t thisLen = w_List_size(w_BigInt_BitType_)(&(thisTemp.nums));
    int64_t otherLen = w_List_size(w_BigInt_BitType_)(&(otherTemp.nums));
    int64_t maxLen = thisLen > otherLen ? thisLen : otherLen;
    for (int64_t i = 0; i < maxLen; i++)
    {
        int64_t thisNum = i < thisLen ? w_List_get(w_BigInt_BitType_)(&(thisTemp.nums), i) : 0;
        int64_t otherNum = i < otherLen ? w_List_get(w_BigInt_BitType_)(&(otherTemp.nums), i) : 0;
        w_List_addLast(w_BigInt_BitType_)(&(result->nums), thisNum & otherNum);
    }

    // 符号位
    result->signum = thisTemp.signum & otherTemp.signum;
    if (result->signum < 0)
    {
        result->signum = -1;
    }
    else if (result->signum > 0)
    {
        result->signum = 1;
    }

    // 去除多余的 0
    while (w_List_size(w_BigInt_BitType_)(&(result->nums)) > 0 && w_List_get(w_BigInt_BitType_)(&(result->nums), w_List_size(w_BigInt_BitType_)(&(result->nums)) - 1) == 0)
    {
        w_List_removeLast(w_BigInt_BitType_)(&(result->nums));
    }
    // 表示 0 时候，nums 为空且 signum 为 0
    if (w_List_size(w_BigInt_BitType_)(&(result->nums)) == 0)
    {
        result->signum = 0;
    }

    // 取补码，得到原码
    w_BigInt_toComplement_(result, &temp);
    w_BigInt_copyTo(&temp, result);

    // 释放
    w_BigInt_deinit(&temp);
    w_BigInt_deinit(&thisTemp);
    w_BigInt_deinit(&otherTemp);
}

/**
 * 按位或
 * @param this
 * @param other
 * @param result result = this | other
 * @return void
 */
static inline void w_BigInt_or(w_BigInt *this, w_BigInt *other, w_BigInt *result)
{
    w_assert(this != NULL && other != NULL && result != NULL);
    w_assert(this != result && other != result);

    // 清空 result
    result->signum = 0;
    w_List_clear(w_BigInt_BitType_)(&(result->nums));

    // 临时变量
    w_BigInt temp, thisTemp, otherTemp;
    w_BigInt_init(&temp, "0");
    w_BigInt_init(&thisTemp, "0");
    w_BigInt_init(&otherTemp, "0");

    // 取补码
    w_BigInt_toComplement_(this, &thisTemp);
    w_BigInt_toComplement_(other, &otherTemp);

    // 按位与
    int64_t thisLen = w_List_size(w_BigInt_BitType_)(&(thisTemp.nums));
    int64_t otherLen = w_List_size(w_BigInt_BitType_)(&(otherTemp.nums));
    int64_t maxLen = thisLen > otherLen ? thisLen : otherLen;
    for (int64_t i = 0; i < maxLen; i++)
    {
        int64_t thisNum = i < thisLen ? w_List_get(w_BigInt_BitType_)(&(thisTemp.nums), i) : 0;
        int64_t otherNum = i < otherLen ? w_List_get(w_BigInt_BitType_)(&(otherTemp.nums), i) : 0;
        w_List_addLast(w_BigInt_BitType_)(&(result->nums), thisNum | otherNum);
    }

    // 符号位
    result->signum = thisTemp.signum | otherTemp.signum;
    if (result->signum < 0)
    {
        result->signum = -1;
    }
    else if (result->signum > 0)
    {
        result->signum = 1;
    }

    // 去除多余的 0
    while (w_List_size(w_BigInt_BitType_)(&(result->nums)) > 0 && w_List_get(w_BigInt_BitType_)(&(result->nums), w_List_size(w_BigInt_BitType_)(&(result->nums)) - 1) == 0)
    {
        w_List_removeLast(w_BigInt_BitType_)(&(result->nums));
    }
    // 表示 0 时候，nums 为空且 signum 为 0
    if (w_List_size(w_BigInt_BitType_)(&(result->nums)) == 0)
    {
        result->signum = 0;
    }

    // 取补码，得到原码
    w_BigInt_toComplement_(result, &temp);
    w_BigInt_copyTo(&temp, result);

    // 释放
    w_BigInt_deinit(&temp);
    w_BigInt_deinit(&thisTemp);
    w_BigInt_deinit(&otherTemp);
}

/**
 * 按位异或
 * @param this
 * @param other
 * @param result result = this ^ other
 * @return void
 */
static inline void w_BigInt_xor(w_BigInt *this, w_BigInt *other, w_BigInt *result)
{
    w_assert(this != NULL && other != NULL && result != NULL);
    w_assert(this != result && other != result);

    // 清空 result
    result->signum = 0;
    w_List_clear(w_BigInt_BitType_)(&(result->nums));

    // 临时变量
    w_BigInt temp, thisTemp, otherTemp;
    w_BigInt_init(&temp, "0");
    w_BigInt_init(&thisTemp, "0");
    w_BigInt_init(&otherTemp, "0");

    // 取补码
    w_BigInt_toComplement_(this, &thisTemp);
    w_BigInt_toComplement_(other, &otherTemp);

    // 按位与
    int64_t thisLen = w_List_size(w_BigInt_BitType_)(&(thisTemp.nums));
    int64_t otherLen = w_List_size(w_BigInt_BitType_)(&(otherTemp.nums));
    int64_t maxLen = thisLen > otherLen ? thisLen : otherLen;
    for (int64_t i = 0; i < maxLen; i++)
    {
        int64_t thisNum = i < thisLen ? w_List_get(w_BigInt_BitType_)(&(thisTemp.nums), i) : 0;
        int64_t otherNum = i < otherLen ? w_List_get(w_BigInt_BitType_)(&(otherTemp.nums), i) : 0;
        w_List_addLast(w_BigInt_BitType_)(&(result->nums), thisNum ^ otherNum);
    }

    // 符号位
    result->signum = thisTemp.signum ^ otherTemp.signum;
    if (result->signum < 0)
    {
        result->signum = -1;
    }
    else if (result->signum > 0)
    {
        result->signum = 1;
    }

    // 去除多余的 0
    while (w_List_size(w_BigInt_BitType_)(&(result->nums)) > 0 && w_List_get(w_BigInt_BitType_)(&(result->nums), w_List_size(w_BigInt_BitType_)(&(result->nums)) - 1) == 0)
    {
        w_List_removeLast(w_BigInt_BitType_)(&(result->nums));
    }
    // 表示 0 时候，nums 为空且 signum 为 0
    if (w_List_size(w_BigInt_BitType_)(&(result->nums)) == 0)
    {
        result->signum = 0;
    }

    // 取补码，得到原码
    w_BigInt_toComplement_(result, &temp);
    w_BigInt_copyTo(&temp, result);

    // 释放
    w_BigInt_deinit(&temp);
    w_BigInt_deinit(&thisTemp);
    w_BigInt_deinit(&otherTemp);
}

/**
 * 按位取反
 * @param this
 * @param result result = ~this
 * @return void
 */
static inline void w_BigInt_not(w_BigInt *this, w_BigInt *result)
{
    w_assert(this != NULL && result != NULL);
    w_assert(this != result);

    // 临时变量
    w_BigInt temp;
    w_BigInt_init(&temp, "0");

    // 取补码
    w_BigInt_toComplement_(this, result);

    // 按位与
    for (int64_t i = 0; i < w_List_size(w_BigInt_BitType_)(&(result->nums)); i++)
    {
        w_List_set(w_BigInt_BitType_)(&(result->nums), i, ~(w_List_get(w_BigInt_BitType_)(&(result->nums), i)));
    }

    // 符号位
    result->signum = -1 * result->signum;
    if (result->signum < 0)
    {
        result->signum = -1;
    }
    else if (result->signum > 0)
    {
        result->signum = 1;
    }

    // 去除多余的 0
    while (w_List_size(w_BigInt_BitType_)(&(result->nums)) > 0 && w_List_get(w_BigInt_BitType_)(&(result->nums), w_List_size(w_BigInt_BitType_)(&(result->nums)) - 1) == 0)
    {
        w_List_removeLast(w_BigInt_BitType_)(&(result->nums));
    }
    // 表示 0 时候，nums 为空且 signum 为 0
    if (w_List_size(w_BigInt_BitType_)(&(result->nums)) == 0)
    {
        result->signum = 0;
    }

    // 取补码，得到原码
    w_BigInt_toComplement_(result, &temp);
    w_BigInt_copyTo(&temp, result);

    // 释放
    w_BigInt_deinit(&temp);
}

/**
 * 左移位
 * @param this
 * @param n 移动的位数
 * @param result result = this << n
 * @return void
 */
static inline void w_BigInt_shiftLeft(w_BigInt *this, int64_t n, w_BigInt *result)
{
    w_assert(this != NULL && result != NULL);
    w_assert(this != result);
    // 赋值
    w_BigInt_copyTo(this, result);
    // 插入
    for (int64_t i = 0; i < n / 8; i++)
    {
        w_List_addFirst(w_BigInt_BitType_)(&(result->nums), 0);
    }
    // 移位
    int64_t bits = n % 8;
    if (bits > 0)
    {
        w_List_addLast(w_BigInt_BitType_)(&(result->nums), 0);
        for (int64_t i = w_List_size(w_BigInt_BitType_)(&(result->nums)) - 1; i > 0; i--)
        {
            w_List_set(w_BigInt_BitType_)(&(result->nums), i, (w_List_get(w_BigInt_BitType_)(&(result->nums), i) << bits) | (w_List_get(w_BigInt_BitType_)(&(result->nums), i - 1) >> (8 - bits)));
        }
        w_List_set(w_BigInt_BitType_)(&(result->nums), 0, w_List_get(w_BigInt_BitType_)(&(result->nums), 0) << bits);
    }
    // 去除多余的 0
    while (w_List_size(w_BigInt_BitType_)(&(result->nums)) > 0 && w_List_get(w_BigInt_BitType_)(&(result->nums), w_List_size(w_BigInt_BitType_)(&(result->nums)) - 1) == 0)
    {
        w_List_removeLast(w_BigInt_BitType_)(&(result->nums));
    }
    // 表示 0 时候，nums 为空且 signum 为 0
    if (w_List_size(w_BigInt_BitType_)(&(result->nums)) == 0)
    {
        result->signum = 0;
    }
}

/**
 * 右移位
 * @param this
 * @param n 移动的位数
 * @param result result = this >> n
 * @return void
 */
static inline void w_BigInt_shiftRight(w_BigInt *this, int64_t n, w_BigInt *result)
{
    w_assert(this != NULL && result != NULL);
    w_assert(this != result);
    // 赋值
    w_BigInt_copyTo(this, result);
    // 删除
    for (int64_t i = 0; i < n / 8 && w_List_size(w_BigInt_BitType_)(&(result->nums)) > 0; i++)
    {
        w_List_removeFirst(w_BigInt_BitType_)(&(result->nums));
    }
    // 移位
    int64_t bits = n % 8;
    if (w_List_size(w_BigInt_BitType_)(&(result->nums)) > 0 && bits > 0)
    {
        for (int64_t i = 0; i < w_List_size(w_BigInt_BitType_)(&(result->nums)) - 1; i++)
        {
            w_List_set(w_BigInt_BitType_)(&(result->nums), i, (w_List_get(w_BigInt_BitType_)(&(result->nums), i) >> bits) | (w_List_get(w_BigInt_BitType_)(&(result->nums), i + 1) << (8 - bits)));
        }
        w_List_set(w_BigInt_BitType_)(&(result->nums), w_List_size(w_BigInt_BitType_)(&(result->nums)) - 1, w_List_get(w_BigInt_BitType_)(&(result->nums), w_List_size(w_BigInt_BitType_)(&(result->nums)) - 1) >> bits);
    }
    // 去除多余的 0
    while (w_List_size(w_BigInt_BitType_)(&(result->nums)) > 0 && w_List_get(w_BigInt_BitType_)(&(result->nums), w_List_size(w_BigInt_BitType_)(&(result->nums)) - 1) == 0)
    {
        w_List_removeLast(w_BigInt_BitType_)(&(result->nums));
    }
    // 负数 >> n 时，结果为负数，当 n 足够大时，结果一定为 -1
    if (w_List_size(w_BigInt_BitType_)(&(result->nums)) == 0 && result->signum < 0)
    {
        w_List_addLast(w_BigInt_BitType_)(&(result->nums), 1);
    }
    // 表示 0 时候，nums 为空且 signum 为 0
    if (w_List_size(w_BigInt_BitType_)(&(result->nums)) == 0)
    {
        result->signum = 0;
    }
}

/**
 * 取 bit 位
 * @param this
 * @param bitIndex 位索引
 * @return int8_t 返回 0 或 1
 */
static inline int8_t w_BigInt_bitAt(w_BigInt *this, int64_t bitIndex)
{
    w_assert(this != NULL);
    w_assert(bitIndex >= 0);
    // 获取 bitIndex 位的索引
    int64_t index = bitIndex / 8;
    // 获取 bitIndex 位的索引
    int8_t bit = bitIndex % 8;
    // 获取 bitIndex 位的值
    return ((index < w_List_size(w_BigInt_BitType_)(&(this->nums)) ? w_List_get(w_BigInt_BitType_)(&(this->nums), index) : 0) >> bit) & 1;
}

// ========================================================================================================================================================
//  Deque
// ========================================================================================================================================================

// Deque 类型定义
#define w_Deque(T) w_concat(w_Deque_, T)
#define w_Deque_type_define_(T)                              \
    typedef struct                                           \
    {                                                        \
        T *elementData;   /* 元素数据 */                     \
        int64_t capacity; /* 容量，会少用一个以区分空和满 */ \
        int64_t begin;    /* 起始位置 */                     \
        int64_t end;      /* 终止位置的下一位 */             \
    } w_Deque(T);

// Deque 初始化
#define w_Deque_init(T) w_concat(w_Deque(T), _init)
#define w_Deque_init_define_(T)                           \
    /**                                                   \
     * 初始化                                          \
     * @param this                                        \
     * @return void                                       \
     */                                                   \
    static inline void w_Deque_init(T)(w_Deque(T) * this) \
    {                                                     \
        w_assert(this != NULL);                           \
        this->elementData = w_malloc(sizeof(T) * 16);     \
        w_assert(this->elementData != NULL);              \
        this->capacity = 16;                              \
        this->begin = 0;                                  \
        this->end = 0;                                    \
    }

// Deque 释放
#define w_Deque_deinit(T) w_concat(w_Deque(T), _deinit)
#define w_Deque_deinit_define_(T)                           \
    /**                                                     \
     * 释放                                               \
     * @param this                                          \
     * @return void                                         \
     */                                                     \
    static inline void w_Deque_deinit(T)(w_Deque(T) * this) \
    {                                                       \
        w_assert(this != NULL);                             \
        w_assert(this->elementData != NULL);                \
        w_free(this->elementData);                          \
        memset(this, 0, sizeof(w_Deque(T)));                \
    }

// Deque 扩容
#define w_Deque_expand_(T) w_concat(w_Deque(T), _expand_)
#define w_Deque_expand_define_(T)                                                                                  \
    /**                                                                                                            \
     * 扩容                                                                                                      \
     * @param this                                                                                                 \
     * @return void                                                                                                \
     */                                                                                                            \
    static inline void w_Deque_expand_(T)(w_Deque(T) * this)                                                       \
    {                                                                                                              \
        w_assert(this != NULL);                                                                                    \
        w_assert(this->elementData != NULL);                                                                       \
        /* 扩容 */                                                                                                 \
        T *newElementData = w_malloc(sizeof(T) * this->capacity * 2);                                              \
        w_assert(newElementData != NULL);                                                                          \
        /* 拷贝数据 */                                                                                             \
        if (this->begin <= this->end)                                                                              \
        {                                                                                                          \
            /* 不涉及跨越问题，直接拷贝即可 */                                                                     \
            memcpy(newElementData, this->elementData + this->begin, (this->end - this->begin) * sizeof(T));        \
        }                                                                                                          \
        else                                                                                                       \
        {                                                                                                          \
            /* 跨越问题 */                                                                                         \
            /* begin -> eof */                                                                                     \
            memcpy(newElementData, this->elementData + this->begin, (this->capacity - this->begin) * sizeof(T));   \
            /* 0 -> end */                                                                                         \
            memcpy(newElementData + this->capacity - this->begin, this->elementData, (this->end) * sizeof(T));     \
        }                                                                                                          \
        /* 释放旧数据 */                                                                                           \
        w_free(this->elementData);                                                                                 \
        this->elementData = newElementData;                                                                        \
        this->end = this->end >= this->begin ? this->end - this->begin : this->capacity - this->begin + this->end; \
        this->begin = 0;                                                                                           \
        this->capacity *= 2;                                                                                       \
    }

// Deque 元素个数
#define w_Deque_size(T) w_concat(w_Deque(T), _size)
#define w_Deque_size_define_(T)                                                                               \
    /**                                                                                                       \
     * 元素个数                                                                                           \
     * @param this                                                                                            \
     * @return int64_t Deque 中的元素个数                                                               \
     */                                                                                                       \
    static inline int64_t w_Deque_size(T)(w_Deque(T) * this)                                                  \
    {                                                                                                         \
        w_assert(this != NULL);                                                                               \
        w_assert(this->elementData != NULL);                                                                  \
        return this->end >= this->begin ? this->end - this->begin : this->capacity - this->begin + this->end; \
    }

// 入栈
#define w_Deque_push(T) w_concat(w_Deque(T), _push)
#define w_Deque_push_define_(T)                                      \
    /**                                                              \
     * 入栈                                                        \
     * @param this                                                   \
     * @param element 入栈元素                                   \
     * @return void                                                  \
     */                                                              \
    static inline void w_Deque_push(T)(w_Deque(T) * this, T element) \
    {                                                                \
        w_assert(this != NULL);                                      \
        w_assert(this->elementData != NULL);                         \
        /* 判断是否扩容 */                                           \
        if (w_Deque_size(T)(this) >= this->capacity - 1)             \
        {                                                            \
            w_Deque_expand_(T)(this);                                \
        }                                                            \
        /* 推入栈 */                                                 \
        this->elementData[this->end] = element;                      \
        this->end = (this->end + 1) % this->capacity;                \
    }

// 出栈
#define w_Deque_pop(T) w_concat(w_Deque(T), _pop)
#define w_Deque_pop_define_(T)                                       \
    /**                                                              \
     * 出栈                                                        \
     * @param this                                                   \
     * @param result 出栈元素                                    \
     * @return void                                                  \
     */                                                              \
    static inline void w_Deque_pop(T)(w_Deque(T) * this, T * result) \
    {                                                                \
        w_assert(this != NULL);                                      \
        w_assert(this->elementData != NULL);                         \
        w_assert(w_Deque_size(T)(this) > 0); /* 栈非空 */            \
        w_assert(result != NULL);                                    \
        /* 出栈 */                                                   \
        int64_t end = this->end - 1;                                 \
        if (end < 0)                                                 \
        {                                                            \
            end = this->capacity - 1;                                \
        }                                                            \
        *result = this->elementData[end];                            \
        this->end = end;                                             \
    }

// 入队
#define w_Deque_enqueue(T) w_concat(w_Deque(T), _enqueue)
#define w_Deque_enqueue_define_(T)                                      \
    /**                                                                 \
     * 入队                                                           \
     * @param this                                                      \
     * @param element 入队元素                                      \
     * @return void                                                     \
     */                                                                 \
    static inline void w_Deque_enqueue(T)(w_Deque(T) * this, T element) \
    {                                                                   \
        w_assert(this != NULL);                                         \
        w_assert(this->elementData != NULL);                            \
        /* 扩容 */                                                      \
        if (w_Deque_size(T)(this) >= this->capacity - 1)                \
        {                                                               \
            w_Deque_expand_(T)(this);                                   \
        }                                                               \
        /* 入队 */                                                      \
        this->elementData[this->end] = element;                         \
        this->end = (this->end + 1) % this->capacity;                   \
    }

// 出队
#define w_Deque_dequeue(T) w_concat(w_Deque(T), _dequeue)
#define w_Deque_dequeue_define_(T)                                       \
    /**                                                                  \
     * 出队                                                            \
     * @param this                                                       \
     * @param result 出队元素                                        \
     * @return void                                                      \
     */                                                                  \
    static inline void w_Deque_dequeue(T)(w_Deque(T) * this, T * result) \
    {                                                                    \
        w_assert(this != NULL);                                          \
        w_assert(this->elementData != NULL);                             \
        w_assert(w_Deque_size(T)(this) > 0);                             \
        w_assert(result != NULL);                                        \
        /* 出队 */                                                       \
        *result = this->elementData[this->begin];                        \
        this->begin = (this->begin + 1) % this->capacity;                \
    }

// 根据索引获取元素
#define w_Deque_get(T) w_concat(w_Deque(T), _get)
#define w_Deque_get_define_(T)                                                     \
    /**                                                                            \
     * 根据索引获取元素                                                    \
     * @param this                                                                 \
     * @param index 索引                                                         \
     * @param result 获取的元素                                               \
     */                                                                            \
    static inline void w_Deque_get(T)(w_Deque(T) * this, int64_t index, T *result) \
    {                                                                              \
        w_assert(this != NULL && this->elementData != NULL);                       \
        w_assert(index >= 0 && index < w_Deque_size(T)(this));                     \
        w_assert(result != NULL);                                                  \
        index = (this->begin + index) % this->capacity;                            \
        *result = this->elementData[index];                                        \
    }

// 根据索引设置元素
#define w_Deque_set(T) w_concat(w_Deque(T), _set)
#define w_Deque_set_define_(T)                                                     \
    /**                                                                            \
     * 根据索引设置元素                                                    \
     * @param this                                                                 \
     * @param index 索引                                                         \
     * @param element 设置的元素                                              \
     */                                                                            \
    static inline void w_Deque_set(T)(w_Deque(T) * this, int64_t index, T element) \
    {                                                                              \
        w_assert(this != NULL && this->elementData != NULL);                       \
        w_assert(index >= 0 && index < w_Deque_size(T)(this));                     \
        index = (this->begin + index) % this->capacity;                            \
        this->elementData[index] = element;                                        \
    }

// Deque 定义
#define w_Deque_define(T)       \
    w_Deque_type_define_(T);    \
    w_Deque_init_define_(T);    \
    w_Deque_deinit_define_(T);  \
    w_Deque_expand_define_(T);  \
    w_Deque_size_define_(T);    \
    w_Deque_push_define_(T);    \
    w_Deque_pop_define_(T);     \
    w_Deque_enqueue_define_(T); \
    w_Deque_dequeue_define_(T); \
    w_Deque_get_define_(T);     \
    w_Deque_set_define_(T);

// ========================================================================================================================================================
//  阻塞队列
// ========================================================================================================================================================

// 阻塞队列定义，需要先行定义 w_Deque
#define w_BlockingQueue(T) w_concat(w_BlockingQueue_, T)
#define w_BlockingQueue_type_define_(T)          \
    typedef struct                               \
    {                                            \
        w_Deque(T) deque;                        \
        pthread_mutex_t opMutex;    /* 操作锁 */ \
        pthread_mutex_t emptyMutex; /* 空锁 */   \
    } w_BlockingQueue(T);

// 初始化
#define w_BlockingQueue_init(T) w_concat(w_BlockingQueue(T), _init)
#define w_BlockingQueue_init_define_(T)                                   \
    /**                                                                   \
     * 阻塞队列初始化                                              \
     * @param this                                                        \
     * @return void                                                       \
     */                                                                   \
    static inline void w_BlockingQueue_init(T)(w_BlockingQueue(T) * this) \
    {                                                                     \
        w_assert(this != NULL);                                           \
        w_Deque_init(T)(&this->deque);                                    \
        pthread_mutex_init(&this->opMutex, NULL);                         \
        pthread_mutex_init(&this->emptyMutex, NULL);                      \
    }

// 销毁
#define w_BlockingQueue_deinit(T) w_concat(w_BlockingQueue(T), _deinit)
#define w_BlockingQueue_deinit_define_(T)                                   \
    /**                                                                     \
     * 销毁                                                               \
     * @param this                                                          \
     * @return void                                                         \
     */                                                                     \
    static inline void w_BlockingQueue_deinit(T)(w_BlockingQueue(T) * this) \
    {                                                                       \
        w_assert(this != NULL);                                             \
        w_Deque_deinit(T)(&this->deque);                                    \
        pthread_mutex_destroy(&this->opMutex);                              \
        pthread_mutex_destroy(&this->emptyMutex);                           \
        pthread_mutex_lock(&this->emptyMutex); /* 锁定 empty */             \
    }

// 取出元素
#define w_BlockingQueue_take(T) w_concat(w_BlockingQueue(T), _take)
#define w_BlockingQueue_take_define_(T)                                               \
    /**                                                                               \
     * 取出元素                                                                   \
     * @param this                                                                    \
     * @param result 取出的元素                                                  \
     * @return void                                                                   \
     */                                                                               \
    static inline void w_BlockingQueue_take(T)(w_BlockingQueue(T) * this, T * result) \
    {                                                                                 \
        w_assert(this != NULL);                                                       \
        w_assert(result != NULL);                                                     \
        /* 锁定 empty */                                                              \
        pthread_mutex_lock(&this->emptyMutex);                                        \
        /* 锁定 op */                                                                 \
        pthread_mutex_lock(&this->opMutex);                                           \
        /* 出队 */                                                                    \
        w_Deque_dequeue(T)(&this->deque, result);                                     \
        /* 判空 */                                                                    \
        if (w_Deque_size(T)(&this->deque) > 0)                                        \
        {                                                                             \
            /* 释放 empty */                                                          \
            pthread_mutex_unlock(&this->emptyMutex);                                  \
        }                                                                             \
        /* 释放 op */                                                                 \
        pthread_mutex_unlock(&this->opMutex);                                         \
    }

// 放入元素
#define w_BlockingQueue_put(T) w_concat(w_BlockingQueue(T), _put)
#define w_BlockingQueue_put_define_(T)                                              \
    /**                                                                             \
     * 放入元素                                                                 \
     * @param this                                                                  \
     * @param element 放入的元素                                               \
     * @return void                                                                 \
     */                                                                             \
    static inline void w_BlockingQueue_put(T)(w_BlockingQueue(T) * this, T element) \
    {                                                                               \
        w_assert(this != NULL);                                                     \
        /* 锁定 op */                                                               \
        pthread_mutex_lock(&this->opMutex);                                         \
        /* 入队 */                                                                  \
        w_Deque_enqueue(T)(&this->deque, element);                                  \
        /* 释放 empty */                                                            \
        if (w_Deque_size(T)(&this->deque) == 1)                                     \
        {                                                                           \
            pthread_mutex_unlock(&this->emptyMutex);                                \
        }                                                                           \
        /* 释放 op */                                                               \
        pthread_mutex_unlock(&this->opMutex);                                       \
    }

// 队列大小
#define w_BlockingQueue_size(T) w_concat(w_BlockingQueue(T), _size)
#define w_BlockingQueue_size_define_(T)                                      \
    /**                                                                      \
     * 获取队列大小                                                    \
     * @param this                                                           \
     * @return int64_t 队列大小                                          \
     */                                                                      \
    static inline int64_t w_BlockingQueue_size(T)(w_BlockingQueue(T) * this) \
    {                                                                        \
        w_assert(this != NULL);                                              \
        /* 锁定 op */                                                        \
        pthread_mutex_lock(&this->opMutex);                                  \
        int64_t size = w_Deque_size(T)(&this->deque);                        \
        /* 释放 op */                                                        \
        pthread_mutex_unlock(&this->opMutex);                                \
        return size;                                                         \
    }

// 阻塞队列定义
#define w_BlockingQueue_define(T)      \
    w_BlockingQueue_type_define_(T);   \
    w_BlockingQueue_init_define_(T);   \
    w_BlockingQueue_deinit_define_(T); \
    w_BlockingQueue_take_define_(T);   \
    w_BlockingQueue_put_define_(T);    \
    w_BlockingQueue_size_define_(T);

#endif