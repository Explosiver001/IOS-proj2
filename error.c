/**
 * @file 	error.h
 * @author 	Michal Novak  (xnovak3g@stud.fit.vutbr.cz)
 * @brief 	Chybove unkonceni souboru
 * @date 	2022-04-18
 * 
 */

#include "error.h"
#include "shared_mem.h"

void error_exit_free(void * mem, const char *fmt, ...){
	va_list args;
	va_start(args, fmt);

	error_msg_print(fmt, args);

	destroy_shared_mem(mem);

	va_end(args);

	exit(1);
}

void error_exit(const char *fmt, ...){
	va_list args;
	va_start(args, fmt);

	error_msg_print(fmt, args);

	va_end(args);

	exit(1);
}

void error_msg_print(const char *fmt, va_list args){
	vfprintf(stderr, fmt, args);
}


