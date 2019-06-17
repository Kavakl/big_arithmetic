//
// Created by kaval on 04.06.2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include "allfun.h"

// redefine assert to set a boolean flag
#ifdef assert
#undef assert
#endif
#define assert(x) (rslt = rslt && (x))

// main result
static int rslt;

// variables controlling stub functions
static int expected_code;
static int should_exit;
static jmp_buf jump_env;

// test suite main variables
static int done;
static int num_tests;
static int tests_passed;



int main(int argc, char *argv[])
{
    const int base = 1000 * 1000 * 1000;
    FILE *file;
    int c;
    int countB = 0;
    int countA = 0;
    int beginA = 1;
    int signA = 1;
    int signB = 1;
    int *a, *b, *result, *test_result;
    int countString = 1;

    if ((file=fopen(argv[1], "r"))==NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }
    while((c = fgetc(file)) != EOF){
        if (c == '-' && countString == 1) {
            signA = 0;
            continue;
        }
        if (c == '-' && countString == 2) {
            signB = 0;
            continue;
        }

        if (c == '\n') {
            beginA = 0;
            countString++;
            if(countString == 3) break;
            continue;
        }
        if (beginA) countA++;
        else countB++;
    }
    fclose(file);

    if (countA % 9){
        a = (int*) malloc((countA / 9 + 3) * sizeof(int));
        memset(a, 0,(countA / 9 + 3) * sizeof(a));
        a[0] = countA / 9 + 2;
    }
    else {
        a = (int*) malloc((countA / 9 + 2) * sizeof(int));
        memset(a, 0,(countA / 9 + 2) * sizeof(a));
        a[0] = countA / 9 + 1;
    }
    if (countB % 9) {
        b = (int*) malloc((countB / 9 + 3) * sizeof(int));
        memset(b, 0,(countB / 9 + 3) * sizeof(b));
        b[0] = countB / 9 + 2;
    }
    else {
        b = (int*) malloc((countB / 9 + 2) * sizeof(int));
        memset(b, 0,(countB / 9 + 2) * sizeof(b));
        b[0] = countB / 9 + 2;
    }

    a[1] = signA;
    b[1] = signB;

    if ((file=fopen(argv[1], "r"))==NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }
    countString = 1;

    while((c = fgetc(file)) != EOF){

        if (c == '\n') {
            countString++;
            if(countString == 3) break;
            continue;
        }
        if (c == '-') continue;

        if (countA) {
            int num = multiply_on_ten(countA, c - '0');
            if (!(countA % 9)) a[countA / 9 + 1] += num;
            else a[countA / 9 + 2] += num;
            countA--;
        }
        else {
            int num = multiply_on_ten(countB, c - '0');
            if (!(countB % 9)) b[countB / 9 + 1] += num;
            else b[countB / 9 + 2] += num;
            countB--;
        }
    }
    fclose(file);

    if ((file=fopen(argv[2], "w"))==NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }


//    // for test 1
    int max = max(a[0], b[0]) + 1;

    test_result = (int*) malloc(max * sizeof(int));
    result = (int*) malloc(max * sizeof(int));

    memset(test_result, 0, max * sizeof(test_result));

    test_result[0] = max - 1;
    test_result[1] = 222222222;
    test_result[2] = 212122;

    num_tests = 0;
    tests_passed = 0;
    done = 0;

    test_minus_big_int(a, b, result, test_result, base);
    write_in_file(result, file);
    free(result);

    //for test 2
    result = (int*) malloc(max * sizeof(int));
    memset(test_result, 0, max * sizeof(test_result));

    test_result[0] = max - 1;
    test_result[1] = 444444444;
    test_result[2] = 434344;

    test_sum_big_int(a, b, result, test_result, base);
    write_in_file(result, file);
    free(result);

    //for test 3
    result = (int*) malloc(max * sizeof(int));
    memset(test_result, 0, max * sizeof(result));

    test_result[0] = max - 1;
    test_result[1] = 2;

    test_div_big_int(a, b, result, test_result, base);
    write_in_file(result, file);
    free(result);

    //for test 4
    result = (int*) malloc(max * sizeof(int));
    memset(test_result, 0, max * sizeof(result));

    test_result[0] = max - 1;
    test_result[1] = 111111111;
    test_result[2] = 101011;

    test_mod_big_int(a, b, result, test_result, base);
    write_in_file(result, file);
    free(result);

    //for test 5
    max *= 2;
    test_result = (int*)realloc(test_result, max * sizeof(int));
    result = (int*)malloc(max * sizeof(int));
    memset(result, 0, max * sizeof(result));
    memset(test_result, 0, max * sizeof(result));

    test_result[0] = max - 1;
    test_result[1] = 399027435;

    test_multiply_big_int(a, b, result, test_result, base);
    write_in_file(result, file);

    free(test_result);
    free(result);

    printf("Total tests passed: %d\n", tests_passed);
    done = 1;
    return !(tests_passed == num_tests);
}

void write_in_file(int *result, FILE *file)
{

    for(int i = result[0]; i > 0 ; --i){
        fprintf(file,"%d",result[i]);
    }
    fprintf(file,"\n");

}

int multiply_on_ten(int a_size, int num)
{   if (!(a_size % 9)) return num * 100000000;
    for (int i = a_size % 9; i > 1; i--) {
        num *= 10;
    }
    return num;
}

//  utility function
void TestStart(char *name)
{
    num_tests++;
    rslt = 1;
    printf("-- Testing %s ... ",name);
}

//  utility function
void TestEnd()
{
    if (rslt) tests_passed++;
    printf("%s\n", rslt ? "success" : "fail");
}

// stub function
void exit(int code)
{
    if (!done)
    {
        assert(should_exit==1);
        assert(expected_code==code);
        longjmp(jump_env, 1);
    }
    else
    {
        _exit(code);
    }
}

// test case
void test_minus_big_int(int *a, int *b, int *result, int *test_result, int base)
{
    int jmp_rval;
    int passed = 0;

    TestStart("test_minus");
    should_exit = 0;
    if (!(jmp_rval=setjmp(jump_env)))
    {
        minus_big(a, b, result, base);
    }

    for (int i = 1; i <= result[0]; i++) {
        if(result[i] != test_result[i]) {
            passed = 0;
            break;
        } else passed = 1;
    }

    printf(" result = ");
    print_num(result);
    printf(" ");

    assert(jmp_rval==0);
    assert(passed==1);
    TestEnd();
}

void test_sum_big_int(int *a, int *b, int *result, int *test_result, int base)
{
    int jmp_rval;
    int passed = 0;
    TestStart("test_sum");
    should_exit = 0;
    if (!(jmp_rval=setjmp(jump_env)))
    {
        sum_big(a, b, result, base);
    }

    for (int i = 1; i <= result[0]; i++) {
        if(result[i] != test_result[i]) {
            passed = 0;
            break;
        } else passed = 1;
    }

    printf(" result = ");
    print_num(result);
    printf(" ");

    assert(jmp_rval==0);
    assert(passed==1);
    TestEnd();
}

void test_multiply_big_int(int *a, int *b, int *result, int *test_result, int base)
{
    int jmp_rval;
    int passed = 0;

    TestStart("test_multiply");
    should_exit = 0;
    if (!(jmp_rval=setjmp(jump_env)))
    {
        multiply_big_int(a, b, result, base);
    }

    for (int i = 1; i <= result[0]; i++) {
        if(result[i] != test_result[i]) {
            passed = 0;
            break;
        } else passed = 1;
    }

    printf(" result = ");
    print_num(result);
    printf(" ");

    assert(jmp_rval==0);
    assert(passed==1);
    TestEnd();

}

void test_div_big_int(int *a, int *b, int *result, int *test_result, int base)
{
    int jmp_rval;
    int passed = 0;

    TestStart("test_div");
    should_exit = 0;

    if (!(jmp_rval=setjmp(jump_env)))
    {
        div_big_int(a, b, result, base);
    }

    for (int i = 1; i <= result[0]; ++i) {
        if(result[i] != test_result[i]) {
            passed = 0;
            break;
        } else passed = 1;
    }

    printf(" result = ");
    print_num(result);
    printf(" ");

    assert(jmp_rval==0);
    assert(passed==1);
    TestEnd();


}

void test_mod_big_int(int *a, int *b, int *result, int *test_result, int base)
{
    int jmp_rval;
    int passed = 0;

    TestStart("test_mod");
    should_exit = 0;
    if (!(jmp_rval=setjmp(jump_env)))
    {
        mod_big_int(a, b, result, base);
    }

    for (int i = 1; i <= result[0]; i++) {
        if(result[i] != (test_result[0] >= i? test_result[i] : 0)) {
            passed = 0;
            break;
        } else passed = 1;
    }

    printf(" result = ");
    print_num(result);
    printf(" ");

    assert(jmp_rval==0);
    assert(passed==1);
    TestEnd();
}

