/**
 * @file 	shared_mem.c
 * @author 	Michal Novak  (xnovak3g@stud.fit.vutbr.cz)
 * @brief 	Funkce pro sdilenou pamet
 * @date 	2022-04-18
 * 
 */

#include "shared_mem.h"
#include "args.h"
#include "error.h"
#include <signal.h>


void* create_shared_mem(size_t size){
	void * mem; /*ukazatel na sdilenou pamet*/
	if((mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == NULL)
		error_exit("Chyba pri allokaci sdilene pameti!\n");
	return mem;
}

void init_values(shared_mem_t mem, input_t values){

	if((mem->mutex=create_shared_mem(sizeof(sem_t))) == NULL)
		error_exit_free(mem, "Chyba pri allokaci pameti a inicializaci hodnot!\n"); 
	if((mem->h_mutex=create_shared_mem(sizeof(sem_t))) == NULL)
		error_exit_free(mem, "Chyba pri allokaci pameti a inicializaci hodnot!\n");
	if((mem->o_mutex=create_shared_mem(sizeof(sem_t))) == NULL)
		error_exit_free(mem, "Chyba pri allokaci pameti a inicializaci hodnot!\n");
	if((mem->writing=create_shared_mem(sizeof(sem_t))) == NULL)
		error_exit_free(mem, "Chyba pri allokaci pameti a inicializaci hodnot!\n");
	if((mem->h_bond=create_shared_mem(sizeof(sem_t))) == NULL)
		error_exit_free(mem, "Chyba pri allokaci pameti a inicializaci hodnot!\n");

	if(sem_init(mem->mutex, 1, 1) == -1)
		error_exit_free(mem, "Chyba pri allokaci pameti a inicializaci hodnot!\n");
	if(sem_init(mem->h_mutex, 1, 0) == -1) /*Poc. hodnota 0 - otevre se az po vytvoreni druheho vodiku*/
		error_exit_free(mem, "Chyba pri allokaci pameti a inicializaci hodnot!\n");
	if(sem_init(mem->o_mutex, 1, 0) == -1)
		error_exit_free(mem, "Chyba pri allokaci pameti a inicializaci hodnot!\n");
	if(sem_init(mem->writing, 1, 1) == -1)
		error_exit_free(mem, "Chyba pri allokaci pameti a inicializaci hodnot!\n");
	if(sem_init(mem->h_bond, 1, 0) == -1)
		error_exit_free(mem, "Chyba pri allokaci pameti a inicializaci hodnot!\n");

	if((mem->output = fopen(FILENAME, "w")) == NULL)
		error_exit_free(mem, "Chyba pri allokaci pameti a inicializaci hodnot!\n");

	mem->p_counter = 1;
	mem->mol_counter = 0;
	mem->h_counter = 1;
	mem->h_total = values.NH;
	mem->o_counter = 1;
	mem->o_total = values.NO;
	mem->TB = values.TB;
	mem->TI = values.TI;

	mem->h_in_queue = 0;

	mem->missing_atoms = false;

	if((mem->o_processes = create_shared_mem(mem->o_total * sizeof(pid_t))) == NULL)
		error_exit_free(mem, "Chyba pri allokaci pameti a inicializaci hodnot!\n");

	for(unsigned i = 0; i<mem->o_total; i++){
		mem->o_processes[i] = 0;
	}

	if((mem->h_processes = create_shared_mem(mem->h_total * sizeof(pid_t))) == NULL)
		error_exit_free(mem, "Chyba pri allokaci pameti a inicializaci hodnot!\n");

	for(unsigned i = 0; i<mem->h_total; i++){
		mem->h_processes[i] = 0;
	}
}

void destroy_shared_mem(shared_mem_t mem){
	if(mem != NULL){
		if(mem->h_processes != NULL){
			for(unsigned i = 0; i< mem->h_total; i++){
				if(mem->h_processes[i]!=0 && kill(mem->o_processes[i], 0) == 0)
					kill(mem->h_processes[i], SIGKILL);
			}
			munmap(mem->h_processes, (mem->h_total * sizeof(pid_t)));
		}
		if(mem->o_processes != NULL){
			for(unsigned i = 0; i< mem->o_total; i++){
				if(mem->o_processes[i]!=0 && kill(mem->o_processes[i], 0) == 0)
					kill(mem->o_processes[i], SIGKILL);
			}
			munmap(mem->o_processes, (mem->o_total * sizeof(pid_t)));
		}
		if(mem->mutex != NULL){
			sem_destroy(mem->mutex);
			munmap(mem->mutex, sizeof(sem_t));
		}
		if(mem->h_mutex != NULL){
			sem_destroy(mem->h_mutex);
			munmap(mem->h_mutex, sizeof(sem_t));
		}
		if(mem->o_mutex != NULL){
			sem_destroy(mem->o_mutex);
			munmap(mem->o_mutex, sizeof(sem_t));
		}
		if(mem->writing != NULL){
			sem_destroy(mem->writing);
			munmap(mem->writing, sizeof(sem_t));
		}
		if(mem->h_bond != NULL){
			sem_destroy(mem->h_bond);
			munmap(mem->h_bond, sizeof(sem_t));
		}
		
		if(mem->output != NULL)
			fclose(mem->output);
	
		munmap(mem, sizeof(struct shared_mem));
	}
}

