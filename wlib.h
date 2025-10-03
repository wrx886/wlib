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
        w_List_add(T)(this, element, 0);                               \
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
        w_List_add(T)(this, element, this->size);                     \
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
    w_List_removeLast_define_(T);

// ========================================================================================================================================================
//  Map
// ========================================================================================================================================================

// MapEntry 类型
#define w_Map_Entry(K, V) w_concat(w_concat(w_concat(w_Map_Entry_, K), _), V)

// MapEntry 定义
#define w_Map_Entry_type_define_(K, V)   \
    typedef struct w_Map_Entry(K, V)     \
    {                                   \
        K key;                          \
        V value;                        \
        struct w_Map_Entry(K, V) * next; \
    } w_Map_Entry(K, V);

// Map 类型
#define w_Map(K, V) w_concat(w_concat(w_concat(w_Map_, K), _), V)

// Map 类型定义
#define w_Map_type_define_(K, V)       \
    typedef struct w_Map(K, V)         \
    {                                  \
        w_Map_Entry(K, V) * *entryData; \
        int64_t entryDataSize;         \
        int64_t size;                  \
    } w_Map(K, V);

// Map 初始化
#define w_Map_init(K, V) w_concat(w_Map(K, V), _init)
#define w_Map_init_define_(K, V)                                                      \
    /**                                                                               \
     * Map 初始化                                                                  \
     * @param this Map                                                                \
     * @return void                                                                   \
     */                                                                               \
    static inline void w_Map_init(K, V)(w_Map(K, V) * this)                           \
    {                                                                                 \
        this->entryDataSize = 16;                                                     \
        this->entryData = w_malloc(sizeof(w_Map_Entry(K, V) *) * this->entryDataSize); \
        w_assert(this->entryData != NULL);                                            \
        memset(this->entryData, 0, sizeof(w_Map_Entry(K, V) *) * this->entryDataSize); \
        this->size = 0;                                                               \
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
            w_Map_Entry(K, V) *entry = this->entryData[i];     \
            while (entry != NULL)                             \
            {                                                 \
                /* 释放节点 */                                \
                w_Map_Entry(K, V) *next = entry->next;         \
                w_free(entry);                                \
                entry = next;                                 \
            }                                                 \
        }                                                     \
        memset(this, 0, sizeof(w_Map(K, V)));                 \
    }

// Map 放置 键值对 到 键值对数组
#define w_Map_putToEntryData_(K, V) w_concat(w_Map(K, V), _putToEntryData_)
#define w_Map_putToEntryData_define_(K, V)                                                                               \
    /**                                                                                                                  \
     * 放置 键值对 到 键值对数组                                                                              \
     * @param entryData 键值对数组                                                                                  \
     * @param entryDataSize 键值对数组大小                                                                        \
     * @param key 键                                                                                                    \
     * @param value 值                                                                                                  \
     * @return bool 是否添加新的元素                                                                             \
     */                                                                                                                  \
    static inline bool w_Map_putToEntryData_(K, V)(w_Map_Entry(K, V) * *entryData, int64_t entryDataSize, K key, V value) \
    {                                                                                                                    \
        w_assert(entryData != NULL);                                                                                     \
        w_assert(entryDataSize > 0);                                                                                     \
        /* 计算索引 */                                                                                                   \
        int64_t index = w_hash(K)(&key) & (entryDataSize - 1);                                                           \
                                                                                                                         \
        /* 链表头 */                                                                                                     \
        w_Map_Entry(K, V) *entry = entryData[index];                                                                      \
        while (entry != NULL)                                                                                            \
        {                                                                                                                \
            /* 键相等 */                                                                                                 \
            if (w_equals(K)(&(entry->key), &key))                                                                        \
            {                                                                                                            \
                /* 覆盖值 */                                                                                             \
                entry->value = value;                                                                                    \
                return false;                                                                                            \
            }                                                                                                            \
            entry = entry->next;                                                                                         \
        }                                                                                                                \
                                                                                                                         \
        /* 创建节点 */                                                                                                   \
        entry = w_malloc(sizeof(w_Map_Entry(K, V)));                                                                      \
        w_assert(entry != NULL);                                                                                         \
        entry->key = key;                                                                                                \
        entry->value = value;                                                                                            \
        /* 头插法 */                                                                                                     \
        entry->next = entryData[index];                                                                                  \
        entryData[index] = entry;                                                                                        \
        return true;                                                                                                     \
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
        w_Map_Entry(K, V) **newEntryData = w_malloc(sizeof(w_Map_Entry(K, V) *) * this->entryDataSize * 2);     \
        w_assert(newEntryData != NULL);                                                                       \
        memset(newEntryData, 0, sizeof(w_Map_Entry(K, V) *) * this->entryDataSize * 2);                        \
                                                                                                              \
        /* 将旧键值对数组中的键值对放置到新键值对数组中 */                                                    \
        for (int64_t i = 0; i < this->entryDataSize; i++)                                                     \
        {                                                                                                     \
            w_Map_Entry(K, V) *entry = this->entryData[i];                                                     \
            while (entry != NULL)                                                                             \
            {                                                                                                 \
                w_Map_putToEntryData_(K, V)(newEntryData, this->entryDataSize * 2, entry->key, entry->value); \
                entry = entry->next;                                                                          \
                w_free(entry);                                                                                \
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
        w_Map_Entry(K, V) *entry = this->entryData[index];            \
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
        w_Map_Entry(K, V) *entry = this->entryData[index];            \
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
                w_Map_Entry(K, V) *next = entry->next;                \
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
        w_Map_Entry(K, V) *entry = this->entryData[index];                 \
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
        w_Map_Entry(K, V) * entry;         \
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
     * @return w_Map_Entry 键值对，如果为 NULL 则迭代结束                                      \
     */                                                                                                   \
    static inline w_Map_Entry(K, V) * w_Map_Iterator_next(K, V)(w_Map_Iterator(K, V) * this)               \
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
        w_Map_Entry(K, V) *entry = this->entry;                                                            \
        this->entry = this->entry->next;                                                                  \
        return entry;                                                                                     \
    }

// Map 定义
// 定义 Map 需要定义 K 的 w_hash 和 w_equals 函数
#define w_Map_define(K, V)              \
    w_Map_Entry_type_define_(K, V);      \
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

#endif