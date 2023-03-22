#include <stddef.h>
#include <stdio.h>
#include "tests.h"

#define NUMBEROFTESTS 1

void generate_test_report(test_apps_t* test_table){
	print_test_no_arg("--------------TESTS REPORT START--------------\n");	
	for (int test_iter = 0 ; test_iter<NUMBEROFTESTS ; test_iter ++ ){
		debug_print_tests("Ran test id = %d // Test name = %s // Test status =",
					 test_table[test_iter].test_id,
					 test_table[test_iter].test_name);
		if (test_table[test_iter].test_return_value)
		{
			//Test failed
			debug_print_tests("\x1B[1\x1B[31m FAILED \x1b[0m// Return value = %d\n",
								test_table[test_iter].test_return_value );
		}
		else{
			debug_print_tests("\x1B[1m\x1B[32m PASSED \x1b[0m// Return value = %d\n",
								test_table[test_iter].test_return_value );	
		}
	}
	print_test_no_arg("--------------TESTS REPORT END--------------\n");	
}

int kernel_tests(void *arg) {
	/**
	 * Rc is used to indicate if the tests failed
	*/
	print_test_no_arg("\n---------------------Inside kernel tests---------------------\n");
	int rc = 0;

	// unsigned long int *addess_user_mode = (unsigned long int *) 0x40000000;
	// unsigned long int x = *addess_user_mode;
	// debug_print_tests("hello world %ld", x);
	
	print_test_no_arg("\n---------------------kernel_tests executing---------------------\n");


	test_apps_t test_table[NUMBEROFTESTS] = {
		// {test0, "test0", 0, 0},
		// {test1, "test1", 1, 0},
		// {test2, "test2", 2, 0},
		// {test3, "test3", 3, 0},
		// // {test4, "test4", 4, 0}, Takes a lot of time
		// {test5, "test5", 5, 0},
		// // {test6, "test6", 6, 0},//Test is so slow
		// // {test7, "test7", 7, 0},//Test is slow too
        // {test_memory, "test_memory", 8, 0},
	    // {test_sem, "test_sem", 9, 0},
	    // {test12_sem, "test12_sem", 10, 0},
		// {test13_sem, "test13_sem", 11, 0},
		// {test15_sem, "test15_sem", 12, 0},
		// //{test16_sem, "test16_sem", 12, 0},//TO MUCH MEMORY USAGE (fix memory leaks before runningthis test again)
		{test17_sem, "test17_sem", 12, 0},

	};

	print_test_no_arg("\n---------------------kernel_tests executing---------------------\n");
	/*
	 * Dans un second temps (quand vous aurez la création de task/processus), les tests devront
	 * être exécutés dans un processus dédié.
	 * Comme par exemple:
	 * int test_rc;
	 * int pid = sched_kstart(test0, 10, "Test 0", 0);
	 * sched_waitpid(pid, &test_rc);
	 * if (test_rc)
	 * 	rc = 1;
	 */
	
	int test_rc;
	int pid;
	for (int test_iter = 0 ; test_iter<NUMBEROFTESTS ; test_iter ++ ){
		debug_print_tests("\n-------------------%s START-------------------\n", test_table[test_iter].test_name);
		pid = start(test_table[test_iter].test_func, 4000, 128,test_table[test_iter].test_name, 0);
		waitpid(pid, &test_rc);
		test_table[test_iter].test_return_value = test_rc;		
		debug_print_tests("\n-------------------%s END-------------------\n", test_table[test_iter].test_name);
	}
	print_test_no_arg("\n---------------------kernel_tests Have been executed---------------------\n");

	generate_test_report(test_table);
	return rc;
}
