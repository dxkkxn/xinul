#ifndef _TESTS_H_
#define _TESTS_H_

#include "stdint.h"
#include "../process/process.h"
#include "../process/helperfunc.h"
#include "../process/timer_api.h"
#include "../process/memory_api.h"
#include "../process/semaphore_api.h"
#include "assert.h"
#include "drivers/clint.h"


/*
 * Run kernel test suite
 */
int kernel_tests(void *arg);

/*
 * Start a simple process
 */
int test0(void* arg);

/**
 * IT test
 */
extern void test_it();


/*
 * Start a process with a parameter
 * End normally
 * Father wait for his child
 */
int test1(void* arg);

/*
 * Test kill and exit
 */
int test2(void* arg);

/*
 * Prioities and chprio
 */
int test3(void* arg);

/*
 * shared time and active waiting
 */
int test4(void* arg);

/*
 * Robustness of the scheduler
 */
int test5(void* arg);

/*
 * timer tests
 */
int test6(void* arg);

/*
 * Timer and shared memory
 */
int test7(void* arg);

/*
 * Test the process memory api
 */
int test_memory(void* arg);

/**
 * Test les semaphores 
 */
int test_sem(void *arg);

/*
 *  Message queues test
 */
int test10(void* arg);
int test13_msg(void* arg);
int test12(void *arg);
int test13(void *arg);
int test14(void *arg);
int test15(void *arg);
int test17(void *arg);

/**
 * Semaphores test 
 */
int test12_sem(void *arg);
int test13_sem(void *arg);
int test15_sem(void *arg);
int test16_sem(void *arg);
int test17_sem(void *arg);


/*
 *  
 */
int test11(void *arg);


/*
 *  
 */
int test20(void *arg);

/*
 *  
 */
int test21(void *arg);

/*
 * 
 */
int test22(void *arg);


typedef struct test_apps {
    process_function_t test_func;
    const char *test_name;
    int test_return_value;
} test_apps_t ;

/**
 * @brief Generates a test report showing all of the executed tests and their status
 * @param test_table a table taht contains test_apps indicating every test ran and 
 * the retutn value of every test from whichwe ill get a return value
*/
void generate_test_report(test_apps_t* test_table);


// /**
//  * A table that holds the data structs for the tests
//  */
// extern  test_apps_t test_table[];

// /**
//  * This function will help located the test using their name
// */
// extern  test_apps_t *find_app(uint16_t test_id);

#endif /* _TESTS_H_ */
