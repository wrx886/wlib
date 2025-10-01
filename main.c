#include "wlib.h"

w_NDArray_define(int);

int main()
{
    w_NDArray(int) ndarray;
    w_NDArray_init(int)(&ndarray, 5, 1LL, 2LL, 3LL, 4ll, 5ll);

    int64_t *shape = w_NDArray_shape(int)(&ndarray);
    for (int64_t i = 0; i < w_NDArray_shapeSize(int)(&ndarray); i++)
    {
        printf("%d\n", shape[i]);
    }

    w_NDArray_set(int)(&ndarray, 1, 0, 0, 0, 0, 0);
    printf("%d\n", w_NDArray_get(int)(&ndarray, 0, 0, 0, 0, 0));

    w_NDArray_deinit(int)(&ndarray);
}
