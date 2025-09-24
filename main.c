#include "wlib.h"

// 定义 int64 类型的数组
w_Array_define(int64_t);

int main()
{
    w_Array(int64_t) arr;
    w_Array_init(int64_t)(&arr, 16);

    w_Array_set(int64_t)(&arr, 0, 123456);
    printf("%lld\n", w_Array_get(int64_t)(&arr, 0));

    w_Array_deinit(int64_t)(&arr);
}
