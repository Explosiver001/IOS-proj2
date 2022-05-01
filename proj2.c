/**
 * @file 	proj2.c
 * @author 	Michal Novak  (xnovak3g@stud.fit.vutbr.cz)
 * @brief 	Projekt c. 2 do predmetu IOS - tvorba molekul vody
 * @date 	2022-04-18
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#include "args.h"
#include "error.h"
#include "shared_mem.h"
#include <semaphore.h>


void hydrogen_process(shared_mem_t mem){
	unsigned h_id; /*ID aktualniho atomu vodiku*/
	h_id = mem->h_counter++;

	/*Vytvareni atomu vodiku*/
	sem_wait(mem->writing);
		fprintf(mem->output, "%u: H %u: started\n", mem->p_counter++, h_id);
		fflush(mem->output);
	sem_post(mem->writing);

	/*Cekani na zarazeni do fronty*/
	usleep((rand()%mem->TI + 1)*1000); 

	/*Zarazeni atomu vodiku do fronty*/
	sem_wait(mem->writing);
		fprintf(mem->output, "%u: H %u: going to queue\n", mem->p_counter++, h_id);
		fflush(mem->output);
	sem_post(mem->writing);

	mem->h_in_queue++;

	/*Signal pro kyslik, ze vodiky jsou pripravene pro tvorbu molekuly, nebo ze je nedostatek atomu vodiku*/
	if(mem->h_in_queue > 1 || mem->h_total == mem->h_counter)
		sem_post(mem->h_mutex);
	
	
	sem_wait(mem->h_bond);
		/*Vytvareni molekuly vody*/
		if(mem->missing_atoms == false){
			int mol_id; /*ID aktualne vytvarene molekuly*/
			mol_id = mem->mol_counter;
			sem_wait(mem->writing);
				fprintf(mem->output, "%u: H %u: creating molecule %u\n", mem->p_counter++, h_id, mol_id);
				fflush(mem->output);
			sem_post(mem->writing);
			sem_wait(mem->o_mutex);
				sem_wait(mem->writing);
					fprintf(mem->output, "%u: H %u: molecule %u created\n", mem->p_counter++, h_id, mol_id);
					fflush(mem->output);
				sem_post(mem->writing);
		}
		/*Nedostatek kysliku nebo vodiku pro vytvoreni nove molekuly */
		else{
			sem_wait(mem->writing);
				fprintf(mem->output, "%u: H %u: not enough O or H\n", mem->p_counter++, h_id);
				fflush(mem->output);
			sem_post(mem->writing);

			/*Propusteni dalsich atomu vodiku*/
			sem_post(mem->h_bond);
		}
}

void oxygen_process(shared_mem_t mem){
	unsigned o_id; /*ID aktualniho atomu kysliku*/
	o_id = mem->o_counter++;

	/*Vytvareni atomu kysliku*/
	sem_wait(mem->writing);
		fprintf(mem->output, "%u: O %u: started\n", mem->p_counter++, o_id);
		fflush(mem->output);
	sem_post(mem->writing);

	/*Cekani na zarazeni do fronty*/
	usleep((rand()%mem->TI + 1)*1000);

	/*Zarazeni atomu vodiku do fronty*/
	sem_wait(mem->writing);
		fprintf(mem->output, "%u: O %u: going to queue\n", mem->p_counter++, o_id);
		fflush(mem->output);
	sem_post(mem->writing);

	

	sem_wait(mem->mutex);
		sem_wait(mem->h_mutex);
		/*Vytvareni molekuly vody*/
		if(mem->h_in_queue > 1 && mem->missing_atoms == false){
			sem_wait(mem->writing);
				fprintf(mem->output, "%u: O %u: creating molecule %u\n", mem->p_counter++, o_id,  ++mem->mol_counter);
				fflush(mem->output);
			sem_post(mem->writing);

			sem_post(mem->h_bond);
			mem->h_in_queue--;
			sem_post(mem->h_bond);
			mem->h_in_queue--;

			/*Cekani na vytvoreni molekuly vody*/
			usleep((rand()%mem->TB + 1)*1000); 

			/*Vyzvednuti 2 atomu vodiku z fronty*/
			sem_post(mem->o_mutex);
			sem_post(mem->o_mutex);

			sem_wait(mem->writing);
				fprintf(mem->output, "%u: O %u: molecule %u created\n", mem->p_counter++, o_id,  mem->mol_counter);
				fflush(mem->output);
			sem_post(mem->writing);
		}
		/*Nedostatek vodiku pro vytvoreni nove molekuly */
		else{
			mem->missing_atoms = true;
			sem_wait(mem->writing);
				fprintf(mem->output, "%u: O %u: not enough H\n", mem->p_counter++, o_id);
				fflush(mem->output);
			sem_post(mem->writing);
			while(mem->h_in_queue != 0){
				mem->h_in_queue--;
				sem_post(mem->h_bond);
			}
			sem_post(mem->h_mutex);
		}
		if(mem->mol_counter == mem->h_total/2){
			mem->missing_atoms = true;
			sem_post(mem->h_bond);
			sem_post(mem->h_mutex);
		}

	sem_post(mem->mutex);
}

int main (int argc, char **argv) {

	/* ziskani a ulozeni vstupnich argumentu funkce */
	input_t input; /*Promenna vstupnich hodnot*/
	getInputValues(&input, argc, argv);

	//printf("NO: %ld / NH: %ld / TI: %ld / TB: %ld\n\n", input.NO, input.NH, input.TI, input.TB); //! debug 

	/* vytvoreni sdileneho pametoveho bloku */
	shared_mem_t mem; /*Promenna sdilene pameti*/
	mem = create_shared_mem(sizeof(struct shared_mem)); 
	init_values(mem, input);
	
	/* inicializace generatoru pseudo-nahodnych cisel */
	srand(time(0));	

	for(unsigned i = 0; i<mem->h_total; i++){
		pid_t h_p = fork();
		
		if(h_p == 0){
			hydrogen_process(mem);
			exit(0);
		}
		else if(h_p == -1){
			error_exit_free(mem, "Chyba vytvareni procesu!\n");
		}
		else{
			mem->h_processes[i] = h_p;
			//printf("H pid: %d\n", h_p); //!debug
		}
	}

	for(unsigned i = 0; i<mem->o_total; i++){
		pid_t o_p = fork();
		
		if(o_p == 0){
			oxygen_process(mem);
			exit(0);
		}
		else if(o_p == -1){
			error_exit_free(mem, "Chyba vytvareni procesu!\n");
		}
		else{	
			mem->o_processes[i] = o_p;
			//printf("O pid: %d\n", o_p); //!debug
		}	
	}
	

	/*cekani na vsechny procesy*/
	for(unsigned i = 0; i<mem->o_total; i++){
		waitpid(mem->o_processes[i], NULL, 0);
	}
	for(unsigned i = 0; i<mem->h_total; i++){
		waitpid(mem->h_processes[i], NULL, 0);
	}

	/*uklid pameti*/
	destroy_shared_mem(mem);

	return 0;
}