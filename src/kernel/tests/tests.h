#ifndef _TESTS_H_
#define _TESTS_H_

#include "stdint.h"

#define DUMMY_VAL 0xDEADCAFE

/*
 * Start a process with a parameter
 * End normally
 * Father wait for his child
 */
int64_t test1(void* arg);

/*
 * Test kill and exit
 */
int64_t test2(void* arg);

#endif /* _TESTS_H_ */
