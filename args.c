/**
 * @file 	args.c
 * @author 	Michal Novak  (xnovak3g@stud.fit.vutbr.cz)
 * @brief	Zpracovani vstupnich argumentu programu
 * @date 	2022-04-18
 * 
 */

#include "args.h"
#include "error.h"

void getInputValues(input_t * inputValues, int argc, char** argv){
	if(argc!=5){
		error_exit("Byl zadan nespravny pocet parametru (Spravne: 4, zadano: %d)!\n", argc);
	}
	for(int i = 1; i<argc; i++){
		char * endptr;
		long temp = strtol(argv[i], &endptr, 10);
		if((temp==0 && endptr[0]!='\0') || endptr[0]!='\0'){
			error_exit("Parametr cislo: %d neni cislo!\n", i);
		}
		switch(i){
			case 1:
				inputValues->NO=temp;
				break;
			case 2:
				inputValues->NH=temp;
				break;
			case 3:
				if(temp<0 || temp>1000){
					error_exit("Parametr TI je mimo povoleny rozsah <0,1000> (Zadano: %ld)!\n", temp);
				}
				inputValues->TI=temp;
				break;
			case 4:
				if(temp<0 || temp>1000){
					error_exit("Parametr TB je mimo povoleny rozsah <0,1000> (Zadano: %ld)!\n", temp);
				}
				inputValues->TB=temp;
				break;
		}
	}
}