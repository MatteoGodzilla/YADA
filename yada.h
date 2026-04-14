#ifndef YADA_H
#define YADA_H

#include <stdlib.h>
#include <stdbool.h>

#define YADA_TYPE(T) YADA_arr_##T
#define YADA_COMP_FUN(T) YADA_comp_##T

#define YADA_DEF_TYPE(T) \
    typedef int (*YADA_COMP_FUN(T))(T* a, T* b);\
    typedef struct { \
        int count; \
        int capacity; \
        T* items; \
    } YADA_TYPE(T)

#define YADA_MAX(a,b) ((a) > (b) ? (a) : (b))

//declaration of functions

#define YADA_RESERVE(T) yada_reserve_##T
#define _YADA_DEC_RESERVE(T) void YADA_RESERVE(T) (YADA_TYPE(T)* arr, unsigned int size)

#define YADA_PUSH(T) yada_push_##T
#define _YADA_DEC_PUSH(T) void YADA_PUSH(T) (YADA_TYPE(T)* arr, T obj)

#define YADA_POP(T) yada_pop_##T
#define _YADA_DEC_POP(T) T YADA_POP(T) (YADA_TYPE(T)* arr, T notFound)

#define YADA_CONTAINS(T) yada_contains_##T
#define _YADA_DEC_CONTAINS(T) bool YADA_CONTAINS(T) (YADA_TYPE(T)* arr, T elm, YADA_COMP_FUN(T) compare)

#define YADA_FIND_IDX(T) yada_find_idx_##T
#define _YADA_DEC_FIND_IDX(T) int YADA_FIND_IDX(T) (YADA_TYPE(T)* arr, T elm, YADA_COMP_FUN(T) compare)

#define YADA_INSERT_IDX(T) yada_insert_idx_##T
#define _YADA_DEC_INSERT_IDX(T) void YADA_INSERT_IDX(T) (YADA_TYPE(T)* arr, unsigned idx, T elm)

#define YADA_REMOVE_IDX(T) yada_remove_idx_##T
#define _YADA_DEC_REMOVE_IDX(T) T YADA_REMOVE_IDX(T) (YADA_TYPE(T)* arr, unsigned int idx, T notFound)

#define YADA_SORT(T) yada_sort_##T
#define _YADA_DEC_SORT(T) void YADA_SORT(T) (YADA_TYPE(T)* arr, YADA_COMP_FUN(T) compare)

#define YADA_FREE(T) yada_free_##T
#define _YADA_DEC_FREE(T) void YADA_FREE(T) (YADA_TYPE(T)* arr)

#define YADA_CLEAR(T) yada_clear##T
#define _YADA_DEC_CLEAR(T) void YADA_CLEAR(T) (YADA_TYPE(T)* arr)

//utility, should not be used directly
#define _YADA_INIT(T) _yada_init_##T
#define _YADA_DEC_INIT(T) void _YADA_INIT(T) (YADA_TYPE(T)* arr, unsigned int initialSize)

#define _YADA_GROW(T) _yada_grow_##T
#define _YADA_DEC_GROW(T) void _YADA_GROW(T) (YADA_TYPE(T)* arr)

//definition of functions
#define _YADA_DEF_RESERVE(T) _YADA_DEC_RESERVE(T) {\
    if(arr->items == NULL){\
        _YADA_INIT(T)(arr,size);\
    } else if(arr->capacity < size){\
        arr->capacity = size;\
        arr->items = realloc(arr->items, sizeof(T) * arr->capacity); \
    }\
}

#define _YADA_DEF_PUSH(T)  _YADA_DEC_PUSH(T) {\
    if(arr->items == NULL){ \
        _YADA_INIT(T)(arr,8);\
    } else if(arr->count + 1 >= arr->capacity){ \
        _YADA_GROW(T)(arr);\
    } \
    arr->items[arr->count] = obj; \
    arr->count++; \
}

#define _YADA_DEF_POP(T)  _YADA_DEC_POP(T) {\
    if(arr->items != NULL && arr->count > 0){ \
        arr->count--; \
        return arr->items[arr->count]; \
    } else { \
        return notFound; \
    }\
}

#define _YADA_DEF_CONTAINS(T) _YADA_DEC_CONTAINS(T) {\
    return YADA_FIND_IDX(T)(arr, elm, compare) != -1;\
}

#define _YADA_DEF_FIND_IDX(T) _YADA_DEC_FIND_IDX(T) {\
    if(arr->items != NULL){\
        for(int i = 0; i < arr->count; i++){\
            if(compare(&(arr->items[i]), &elm) == 0){\
                return i;\
            }\
        }\
    }\
    return -1;\
}

#define _YADA_DEF_INSERT_IDX(T)  _YADA_DEC_INSERT_IDX(T) {\
    if(arr->items != NULL && 0 <= idx && idx < arr->capacity){\
        if(arr->count + 1 > arr->capacity){\
            _YADA_GROW(T)(arr);\
        }\
        /* Move elements forward */ \
        for(int i = arr->count; i > idx; i--){\
            arr->items[i] = arr->items[i-1];\
        }\
        arr->items[idx] = elm;\
        arr->count++;\
    }\
}

#define _YADA_DEF_REMOVE_IDX(T)  _YADA_DEC_REMOVE_IDX(T) {\
    if(arr->items == NULL || idx < 0 || idx >= arr->count) {\
        return notFound;\
    }\
    if(idx == arr->count - 1){ \
        return YADA_POP(T)(arr, notFound); \
    } else { \
        T result = arr->items[idx];\
        int remaining = arr->count - idx; \
        /* can this be substituted with a memcpy? */ \
        for(int i = idx; i < arr->count - 1; i++){ \
            arr->items[idx] = arr->items[idx+1]; \
        } \
        arr->count--; \
        return result; \
    } \
}

#define _YADA_DEF_SORT(T) _YADA_DEC_SORT(T) {\
    if(arr->items != NULL) {\
        return qsort(arr->items, arr->count, sizeof(T), (__compar_fn_t)compare);\
    }\
}\

#define _YADA_DEF_FREE(T) _YADA_DEC_FREE(T) {\
    free(arr->items); \
    arr->items = NULL; \
}

#define _YADA_DEF_CLEAR(T) _YADA_DEC_CLEAR(T) {\
    arr->count = 0;\
}

//private
#define _YADA_DEF_INIT(T) _YADA_DEC_INIT(T) {\
    arr->count = 0;\
    arr->capacity = initialSize;\
    arr->items = malloc(sizeof(T) * arr->capacity);\
}

#define _YADA_DEF_GROW(T) _YADA_DEC_GROW(T) {\
    if(arr->items != NULL){\
        arr->capacity *= 2; \
        arr->items = realloc(arr->items, sizeof(T) * arr->capacity); \
    }\
}


//meta functions

#define YADA_DEC_FUNCTIONS(T)\
    _YADA_DEC_INIT(T);\
    _YADA_DEC_GROW(T);\
    _YADA_DEC_RESERVE(T);\
    _YADA_DEC_PUSH(T);\
    _YADA_DEC_POP(T);\
    _YADA_DEC_FIND_IDX(T); \
    _YADA_DEC_CONTAINS(T); \
    _YADA_DEC_INSERT_IDX(T);\
    _YADA_DEC_REMOVE_IDX(T);\
    _YADA_DEC_SORT(T);\
    _YADA_DEC_FREE(T);\
    _YADA_DEC_CLEAR(T);\

#define YADA_DEF_FUNCTIONS(T) \
    _YADA_DEF_INIT(T) \
    _YADA_DEF_GROW(T) \
    _YADA_DEF_RESERVE(T) \
    _YADA_DEF_PUSH(T) \
    _YADA_DEF_POP(T) \
    _YADA_DEF_FIND_IDX(T) \
    _YADA_DEF_CONTAINS(T) \
    _YADA_DEF_INSERT_IDX(T) \
    _YADA_DEF_REMOVE_IDX(T) \
    _YADA_DEF_SORT(T) \
    _YADA_DEF_FREE(T)\
    _YADA_DEF_CLEAR(T)\

#endif //YADA_H
