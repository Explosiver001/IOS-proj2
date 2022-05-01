/**
 * @file 	shared_mem.h
 * @author 	Michal Novak  (xnovak3g@stud.fit.vutbr.cz)
 * @brief 	Funkce a struktury pro sdilenou pamet
 * @date 	2022-04-18
 * 
 */

#ifndef _SHARED_MEM_H_
#define _SHARED_MEM_H_

#include <sys/mman.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>
#include "args.h"

#define FILENAME "proj2.out"

/**
 * @brief Struktura polezek ve sdilene pameti
 * 
 */
typedef struct shared_mem {
	unsigned p_counter; /*Pocitadlo akci*/
	unsigned h_counter; /*Pocitadlo pouzitych vodiku*/
	unsigned o_counter; /*Pocitadlo pouzitych kysliku*/
	unsigned mol_counter; /*Pocitadlo vytvorenych molekul*/

	int h_in_queue;	/*Pocet vodiku ve fronte*/

	pid_t * o_processes; /*Vsechny detske procesy kysliku*/
	pid_t * h_processes; /*Vsechny detske procesy vodiku*/

	unsigned h_total; /*Celkovy pocet zadanych vodiku*/
	unsigned o_total; /*Celkovy pocet zadanych kysliku*/

	unsigned TI; /*Cas, ktery kazdy atom ceka, nez je zarazen do fronty*/
	unsigned TB; /*Cas na vytvoreni molekuly*/

	bool missing_atoms; /*Informace, jestli chybi atomy k spracovani*/

	FILE * output; /*vystupni soubor*/
	
	sem_t * o_mutex; /*Semafor pro podmet od kysliku pro vodik, aby se vodik uvolnil z fronty*/
	sem_t * h_mutex; /*Semafor pro podmet od vodiku pro kyslik, ze jsou k dispozici 2 atomy*/
	sem_t * mutex;  /*Semafor pro tvorbu molekul*/
	sem_t * h_bond; /*Semafor, ktery informuje vodik, ze byla dokoncena tvorba molekuly*/
	sem_t * writing; /*Semafor pro spravny zapis do souboru*/
} * shared_mem_t; /*Struktura sdilene pameti*/




/**
 * @brief 			Zabrani pametoveho bloku pro sdilenou pamet
 * 
 * @param size		Velikost bloku v bytech 
 * @return 			Ukazatel na pamet / NULL pri selhani
 */
void* create_shared_mem(size_t size);

/**
 * @brief 			Inicializace hodnot sdilene pameti
 * 
 * @param mem 		Ukazatel na sdilenou pamet
 * @param values 	Vstupni hodnoty programu
 */
void init_values(shared_mem_t mem, input_t values);


/**
 * @brief 		Funkce pro zniceni sdilene pameti
 * 
 * @param mem 	Ukazatel na sdilenou pamet	
 */
void destroy_shared_mem(shared_mem_t mem);

#endif
