//
// Created by kaval on 28.05.2019.
//

#ifndef UNTITLED_ALLFUN_H
#define UNTITLED_ALLFUN_H

#endif //UNTITLED_ALLFUN_H
#define max(x, y) ( (x) > (y) ? (x) : (y) )
#define min(x, y) ( (x) < (y) ? (x) : (y) )


void sum_big_int_const(int *a, int const con, int base);
void print_num(int *num);
void sum_big_int(int *a, int *b, int *result, int base);
void minus_big_int(int *a, int *b, int *result, int base);
void multiply_big_int(int *a, int *b, int *result, int base);
void div_big_int(int *a, int *b, int *result, int base);
void test_minus_big_int(int *a, int *b, int *result, int *test_result, int base);
void test_sum_big_int(int *a, int *b, int *result, int *test_result, int base);
void test_multiply_big_int(int *a, int *b, int *result, int *test_result, int base);
int multiply_on_ten(int a_size, int num);
int more(int *a, int *b);
void mod_big_int(int *a, int *b, int *result, int base);
void test_div_big_int(int *a, int *b, int *result, int *test_result, int base);
void test_mod_big_int(int *a, int *b, int *result, int *test_result, int base);
void write_in_file(int *result, FILE *file);
int more_module(int *a, int *b);
void sum_big(int *a, int *b, int *result, int base);
void minus_big(int *a, int *b, int *result, int base);
void div_recursive(int *a, int *b, int *result, int base, int *count, int div_count);

