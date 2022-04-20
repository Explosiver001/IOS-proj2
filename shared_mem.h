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
 * @brief 	Struktura prvku ve fronte
 * 
 */
typedef struct element * element_t;
struct element{
	int id;				/*ID cekajiciho prvku*/
	element_t next;		/*Nasledujici prvek ve fronte*/
};

/**
 * @brief 	Struktura fronty 
 * 
 */
typedef struct {
	element_t head;		/*Prvni prvek ve fronte*/
} * queue_t;




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

	bool missing_atoms;

	FILE * output; /*vystupni soubor*/
	
	sem_t * o_mutex; 
	sem_t * h_mutex;
	sem_t * mutex; 
	sem_t * h_bond;
	sem_t * appending;
	sem_t * writing; /*Semafor pro spravny zapis do souboru*/
} * shared_mem_t;




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