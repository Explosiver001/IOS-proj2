/**
 * @file 	error.h
 * @author 	Michal Novak  (xnovak3g@stud.fit.vutbr.cz)
 * @brief 	Chybove unkonceni souboru
 * @date 	2022-04-18
 * 
 */

#ifndef _ERROR_H_
#define _ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * @brief			Funkce pro chybove ukonceni programu a vypis chybove hlasky na chybovy vystup
 * 
 * @param fmt 		Format chyboveho vystupu
 * @param ... 		Parametry formatu chyboveho vystupu
 */
void error_exit(const char *fmt, ...);

/**
 * @brief 			Funkce pro chybove ukonceni programu a vypis chybove hlasky na chybovy vystup + zniceni sdilene pameti
 * 
 * @param mem 		Ukazatel na sdilenou pamet
 * @param fmt 		Format chyboveho vystupu
 * @param ... 		Parametry formatu chyboveho vystupu
 */
void error_exit_free(void * mem, const char *fmt, ...);

/**
 * @brief		Pomocna funkce pro vypis chybove hlasky na chybovy vystup
 * 
 * @param fmt 	Format chyboveho vystupu
 * @param args 	Seznam parametru formatu chyboveho vystupu
 */
void error_msg_print(const char *fmt, va_list args);

#endif