/**
 * @file 	args.c
 * @author 	Michal Novak  (xnovak3g@stud.fit.vutbr.cz)
 * @brief	Zpracovani vstupnich argumentu programu
 * @date 	2022-04-18
 * 
 */

#ifndef _ARGS_H_
#define _ARGS_H_

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief 		Struktura pro uchovani vstupnich parametru programu
 */
typedef struct input{
	long NO;	/*pocet kysliku*/
	long NH;	/*pocet vodiku*/
	long TI;	/*maximalni cas v milisekundach, po ktery atom kysliku/vodiku po svem vytvoreni ceka, nez se zaradi do fronty na vytvareni molekul; 0<=TI<=100*/
	long TB;	/*maximalni cas v milisekundach nutny pro vytvoreni jedne molekuly; 0<=TB<=1000*/
} input_t;

/**
 * @brief 				Funkce pro ziskani 
 *  
 * @param inputValues	datova struktura pro ulozeni parametru, predava se odkazem (neni potreba dynamicky alokovat)
 * @param argc 			pocet vstupnich argumentu programu 
 * @param argv			pole vstupnich parametru
 * 
 * @return 				Vstupni hodnoty programu
 */
void getInputValues(input_t * inputValues, int argc, char** argv);

#endif
