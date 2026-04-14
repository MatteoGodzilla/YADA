#include <stdio.h>
#include "../yada.h"

typedef struct {
    int x;
    int y;
    int z;
} myBeautifulStruct;

YADA_DEF_TYPE(myBeautifulStruct);
YADA_DEF_TYPE(int), MyIntArray;
YADA_DEF_FUNCTIONS(int)

int comp(int* a, int* b){
    printf("COMPARISON: %p %p -> %d %d\n", a, b, *a, *b);
    return (*a) - (*b);
}
void printArr(MyIntArray* arr){
    printf("%d %d %p\n", arr->count, arr->capacity, arr->items);
    for(int i = 0; i < arr->count; i++){
        printf("%d\n", arr->items[i]);
    }
}

int main(){
    MyIntArray arr = {};
    puts("BEFORE");
    YADA_PUSH(int)(&arr, 2);
    YADA_PUSH(int)(&arr, 3);
    YADA_PUSH(int)(&arr, 1);
    YADA_PUSH(int)(&arr, 4);

    for(int i = 0; i < 10; i++){
        YADA_PUSH(int)(&arr, i*i);
    }
    YADA_INSERT_IDX(int)(&arr, 4, 100);
    printArr(&arr);

    puts("AFTER");
    int index = YADA_FIND_IDX(int)(&arr, 81, comp);
    printf("Index: %d\n", index);

    int last = YADA_POP(int)(&arr,0);
    printf("Last: %d\n", last);
    int second = YADA_REMOVE_IDX(int)(&arr, 2, -1);
    printf("Second: %d\n", second);
    
    printArr(&arr);
    puts("SORTING");

    YADA_SORT(int)(&arr, comp);
    printArr(&arr);

    printf("Does it contain 3? %s\n", (YADA_CONTAINS(int)(&arr, 3, comp) ? "true" : "false"));

    YADA_FREE(int)(&arr);
}
