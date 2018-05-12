/*
* EA871 - LAB - MULTITHREAD_COUNTER.
* AUTHOR: CAIO ZOWYE CAMACHO CABRAL
* RA: 135216
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>

// Declaração de variáveis globais
int TOTAL_PRIMOS = 0;					// Variavel global acumuladora
int P = 0;								// MARCA POSICAO DO VETOR QUE ESTA PROCESSADA.
uint64_t vet[64];						// Guarda os numeros inseridos pelo usuario
int VSIZE;								// Guarda numero de elementos do vetor 'vet'

// Declaração de variáveis globais relacionadas às THREADS
#define MAX_THREADS 4
pthread_mutex_t trava_varredura_vet;
pthread_mutex_t trava_acc;

// Prototipo das funcoes
int verificaPrimo(int64_t NUMBER);
void *thread_function(void *arg);

int main(){
	pthread_t threads[MAX_THREADS];   
    int i = 0, j = 0, n = 0;
  	char str[1000], *s = str, *t = NULL;
  	int num_threads;

/*
* ---------------------------------------------------------------------------------------
* ------------------------------- LEITURA DAS ENTRADAS ----------------------------------
* ---------------------------------------------------------------------------------------
*/

	fgets(s, 1000, stdin);

	// Remove '\n' do final da string
	while(str[i] != '\n') i++;
	str[i] = '\0';

	// Tokeniza e insere no vetor de unsigned long int - vet[]
	while ((t = strtok(s, " ")) != NULL) {
		s = NULL;
		vet[n++] = atoi(t);
	}

	VSIZE = n;	//VSIZE <- #elementos a verificar

/*
* ---------------------------------------------------------------------------------------
* --------------------------------- CRIA AS THREADS -------------------------------------
* ---------------------------------------------------------------------------------------
*/

	num_threads = MAX_THREADS;

	// Para os casos em que precisa de um numero de threads menor que MAX_THREADS 
	if(VSIZE < num_threads){
		num_threads = VSIZE;	// numero de threads passa a ser o tamanho do vet de entrada.
	}

    for(i = 0;(i<num_threads);i++){
        pthread_create(&(threads[i]), NULL, thread_function, NULL);
    }

	// Mata as TREADS
	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	// imprime na saida padrao a quantidade de PRIMOS:
	printf ("%d\n", TOTAL_PRIMOS);

	return 0;
}

/*
* ---------------------------------------------------------------------------------------
* ------------------------------------- FUNCOES -----------------------------------------
* ---------------------------------------------------------------------------------------
*/

int verificaPrimo(int64_t NUMBER){
	//VERIFICA SE EH PRIMO
	int64_t divisor;
	unsigned int count = 0;

		if(NUMBER == 0 || NUMBER == 1){
			// Para os casos especiais, retorna 0;
			return 0;
		}else{
			for(divisor=1;divisor<=NUMBER;divisor++){
				if(NUMBER % divisor == 0) count++;
			}	
				// SE FOR PRIMO RETORNA 1
				// RETORNA 0 C.C
				if(count == 2){
					return 1;
				}else{
					return 0;
			}
		}
	return 1;
}

// Cada THREAD invoca essa funcao.  
void* thread_function(void *arg) {
	int contador;
	int i = 0;
	int FIM;
	int64_t numero;

    for(;;){
    	FIM = 0;
        pthread_mutex_lock(&trava_varredura_vet);
        if(P < VSIZE){
        	numero = vet[P];
        	P++;
        }else FIM = 1;
        pthread_mutex_unlock(&trava_varredura_vet);
        
        if(FIM == 1) break;


        contador = verificaPrimo(numero);

        pthread_mutex_lock(&trava_acc);
        TOTAL_PRIMOS = TOTAL_PRIMOS + verificaPrimo(numero);
        pthread_mutex_unlock(&trava_acc);
    }

}