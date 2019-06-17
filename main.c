
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "allfun.h"
#include <inttypes.h>
#include <string.h>

void print_num(int *num) {
    printf("%c", (num[1] == 0? '-' : ' '));
    for(int i = num[0]; i > 1; i--){
        printf("%0.9d",num[i]);
    }
}

void minus_big(int *a, int *b, int *result, int base){

    if (a[1] != b[1]) {
        sum_big_int(a, b, result, base);
        result[1] = a[1];
    }
    else {
        if(more_module(a, b)) minus_big_int(a, b, result, base);
        else minus_big_int(b, a, result, base);
        if (b[1]) result[1] = 0;
        else result[1] = 1;
    }

}

void sum_big(int *a, int *b, int *result, int base){

    if (a[1] == b[1]) {
        sum_big_int(a, b, result, base);
        result[1] = a[1];
    }
    else {
        if(more_module(a, b)){
            minus_big_int(a, b, result, base);
            result[1] = a[1];
        }
        else {
            minus_big_int(b, a, result, base);
            result[1] = b[1];
        }
    }

}


int more_module(int *a, int *b){

    if(a[0] > b[0]) return 1;
    else if(b[0] > a[0]) return 0;

    for (int i = a[0]; i > 1; --i) {
        if(a[i] > b[i]) return 1;
        else if (b[i] > a[i]) return 0;
    }
}

void sum_big_int_const(int *a, int const con, int base){

    int m = a[0];
    int first_sum = 1;
    __int64 c = 0;
    for (int i = 2; i <= m; i++){
        if(first_sum) {
            c += (__int64)a[i] + con;
            first_sum = 0;
        }
        else c += (__int64)a[i];
        a[i] = c % base;
        c /= base;
    }
    if(c > 0){
        m++;
        a = realloc(a, (m + 1) * sizeof(int));
        a[m] = c;
    }
    a[0] = m;
}

void sum_big_int(int *a, int *b, int *result, int base){

    int m = max(a[0],b[0]);
    result[0] = m;
    __int64 c = 0;

    for (int i = 2; i <= m; i++){
        c += (__int64)a[i] + (b[0] >= i? b[i] : 0);
        result[i] = c % base;
        c /= base;
    }

    if(c > 0){
        m++;
        result[0] = m;
        result = realloc(result, (m + 1) * sizeof(int));
        result[m] = c;
    }

}

void minus_big_int(int *a, int *b, int *result, int base){
    __int64 c = 0;
    result[0] = max(a[0],b[0]);

    for (int i = 2; i <= a[0]; i++){
        c += a[i] - (b[0] >= i? b[i] : 0) + base;
        result[i] = c % base;
        if(c < base) c = -1; else c = 0;
    }

    while(result[result[0]] == 0 && result[0] > 2) {
        result[0]-= 2;
        result = realloc(result, result[0] * sizeof(int));
    }
}

void multiply_big_int(int *a, int *b, int *result, int base){
    result[0] = max(a[0], b[0]);

    for(int i = 2; i <= a[0]; i++) {
        for(int j = 2; j <= b[0]; j++) {
            __int64 cr = (__int64)a[i] * b[j];
            int k = i + j - 2;
            while(cr > 0){
                if (k > result[0]) {
                    result = realloc(result, (k + 1) * sizeof(int));
                    result[0] = k;
                }
                cr += result[k];
                result[k] = cr % base;
                cr /= base;
                k++;
            }
        }
    }
    if (a[1] == b[1]) result[1] = 1;
    else result[1] = 0;

}

int more(int *a, int *b){
    if (a[1] > b[1]) return 1;
    else if (b[1] > a[1]) return 0;
    else if(a[0] > b[0]){
        if (a[1] > b[1]) return 1;
        else return 0;
    }
    else if(b[0] > a[0]){
        if(b[1] > a[1]) return 0;
        else return 1;
    }

    for (int i = a[0]; i > 1; --i) {
        if(a[i] > b[i]) return 1;
        else if (b[i] > a[i]) return 0;
    }

}

void div_big_int(int *a, int *b, int *result, int base){

    int* count;
    count = (int*) malloc(2 * sizeof(int));
    memset(count, 0, 3 * sizeof(result));

    count[0] = 2;
    count[1] = 1;

    div_recursive(a, b, result, base, count, 1);

    realloc(result, (count[0] + 1) * sizeof(int));
    memcpy(result, count, (count[0] + 1) * sizeof(int));

    if (a[1] == b[1]) result[1] = 1;
    else result[1] = 0;

}

void div_recursive(int *a, int *b, int *result, int base, int *count, int div_count){

    if(more_module(a, b)) {
        minus_big_int(a, b, result, base);
        sum_big_int_const(count, 1, base);
        div_recursive(result, b, result, base, count, 1);
    }

}

void mod_recursive(int *a, int *b, int *result, int base)
{
    if(more_module(a, b)) {
        minus_big_int(a, b, result, base);
        mod_recursive(result, b, result, base);
    }
}

void mod_big_int(int *a, int *b, int *result, int base){

    if (a[1] == b[1]) result[1] = 1;
    else result[1] = 0;

    if (more(b, a)) return;
    mod_recursive(a, b, result, base);

}



