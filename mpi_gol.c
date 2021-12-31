#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <unistd.h>
#include <math.h>

#define N 10
#define itera_max 2000
#define cores 2

//int grid [N][N];
int **grid;
int new_grid[N][N];


int** alloc_2d_int(int rows, int cols) {
	int *data = (int *)malloc(rows*cols*sizeof(int));
	int **array= (int **)malloc(rows*sizeof(int*));
	for (int i=0; i<rows; i++)
		array[i] = &(data[cols*i]);

	return array;
}




void inicia_grids_zero(){
	int i, j;
	//iniciando com zero
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			grid[i][j] = 0;
			new_grid[i][j] = 0;
		}
	}
}

void geracao_inicial(){
	//GLIDER
	int lin = 1, col = 1;
	grid[lin  ][col+1] = 1;
	grid[lin+1][col+2] = 1;
	grid[lin+2][col  ] = 1;
	grid[lin+2][col+1] = 1;
	grid[lin+2][col+2] = 1;
	 
	//R-pentomino
	/*lin =10; col = 30;
	grid[lin  ][col+1] = 1;
	grid[lin  ][col+2] = 1;
	grid[lin+1][col  ] = 1;
	grid[lin+1][col+1] = 1;
	grid[lin+2][col+1] = 1;*/
	
}

int getNeighbors(int **table, int i, int j){
	int numAliveNeighbors = 0;

	// Up
	if(i != 0){
		if(table[i - 1][j] == 1){
			numAliveNeighbors++;
		}
	}else{
		if(table[N - 1][j] == 1){
			numAliveNeighbors++;
		}
	}

	// Down
	if(table[(i + 1)%N][j] == 1){
		numAliveNeighbors++;
	}

	// Left
	if(j != 0){
		if(table[i][j - 1] == 1){
			numAliveNeighbors++;
		}
	}else{
		if(table[i][N - 1] == 1){
			numAliveNeighbors++;
		}
	}

	// Right
	if(table[i][(j + 1)%N] == 1){
		numAliveNeighbors++;
	}

	// Upper-Right Corner
	if((i == 0) && (j == N - 1)){
		if(table[N - 1][0] == 1){
			numAliveNeighbors++;
		}
	}else{
		// i!=0 || j != n-1
		if(i == 0){
			// já sabemos que j != N - 1
			if(table[N - 1][j + 1] == 1){
				numAliveNeighbors++;
			}
		}else{// i != 0
			if(j == N - 1){
				if(table[i - 1][0] == 1){
					numAliveNeighbors++;
				}
			}else{
				if(table[i - 1][j + 1] == 1){
					numAliveNeighbors++;
				}
			}
		}
	}

	// Lower-Right Corner
	if(table[(i + 1)%N][(j + 1)%N] == 1){
		numAliveNeighbors++;
	}

	// Upper-Left Corner
	if((i == 0) && (j == 0)){
		 if(table[N - 1][N - 1] == 1){
			numAliveNeighbors++;
		}
	}else{
		// i!=0 || j != 0
		if(i == 0){
			// já sabemos que j != 0
			if(table[N - 1][j -1] == 1){
				numAliveNeighbors++;
			}
		}else{// i != 0
			if(j == 0){
				if(table[i - 1][N - 1] == 1){
					numAliveNeighbors++;
				}
			}else{
				if(table[i - 1][j - 1] == 1){
					numAliveNeighbors++;
				}
			}
		}
	}


	// Lower-Left Corner
	if((i == N - 1) && (j == 0)){
		 if(table[0][N - 1] == 1){
			numAliveNeighbors++;
		}
	}else{
		// i!=n-1 || j != 0
		if(i == N - 1){
			// já sabemos que j != 0
			if(table[0][j - 1] == 1){
				numAliveNeighbors++;
			}
		}else{// i != n-1
			if(j == 0){
				if(table[i + 1][N - 1] == 1){
					numAliveNeighbors++;
				}
			}else{
				if(table[i + 1][j - 1] == 1){
					numAliveNeighbors++;
				}
			}
		}
	}

	return numAliveNeighbors;
}

// Lógica do Game of Life
void game_of_life(int noProcesses, int processId){

	int i, j, upper, lower, chunk;

	/*
	
	========== Divisão de linhas =============

	p0     (0) -> (n/3)
	p1     (n/3) -> (2n/3)
	p2     (2n/3) -> (n)
	
	de 
		processId * N / noProcesses 
	até
		(processId+1) * N / noProcesses

	==========================================

	*/
	

	// ><><><><><><><><><><<>>><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><

	//if ((N % noProcesses) == 0){ // divisao inteira

		//nova abordagem usando chunks

		//objetivo desse for e calcular o chunk

		chunk = ceil(((float) N) / noProcesses);

		lower = chunk * processId;

		upper = lower + chunk;

		if (upper > N){ 
			upper = N;
		}
		

		printf("LOWER: %d\n", lower);
		printf("UPPER: %d\n", upper);


		for (i = lower; i < upper; i++){
			for (j = 0; j < N; j++){
				//aplicar as regras do jogo da vida

				//celulas vivas com menos de 2 vizinhas vivas morrem
				if(grid[i][j] == 1 && getNeighbors(grid, i, j) < 2){
					new_grid[i][j] = 0;
				}

				//célula viva com 2 ou 3 vizinhos deve permanecer viva para a próxima geração
				else if (grid[i][j] == 1 && getNeighbors(grid, i, j) == 2 || getNeighbors(grid, i, j) == 3){
					new_grid[i][j] = 1;
				}

				//célula viva com 4 ou mais vizinhos morre por superpopulação
				else if (grid[i][j] == 1 && getNeighbors(grid, i, j) >= 4){
					new_grid[i][j] = 0;
				}

				//morta com exatamente 3 vizinhos deve se tornar viva
				else if (grid[i][j] == 0 && getNeighbors(grid, i, j) == 3){
					new_grid[i][j] = 1;
				}
			}
		}
	//}	



	//passar a nova geração para atual
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			grid[i][j] = new_grid[i][j];
		}
	}
}


int count_LiveCells(){
	int i;
	int j;
	int cont = 0;
		for (i = 0; i < N; i++){
				for (j = 0; j < N; j++){
						if (grid[i][j] == 1){
								cont++;
						}
				}
		}
	
	return cont;
}

int main (int argc, char** argv){
	int i, j;
	int var;
	int vida;
	int cont = 0;
	int processId; /* rank (ID) dos processos */
	int noProcesses; /* Número de processos */
	MPI_Status status;

	grid = alloc_2d_int(N,N);
	inicia_grids_zero();

	geracao_inicial();

	time_t start, end;
	
	// Inicializa os processos de acordo com
	// os argumentos passados pelo mpirun
	MPI_Init(&argc, &argv);
	// A quantidade de processos que foram iniciados
	MPI_Comm_size(MPI_COMM_WORLD, &noProcesses);
	// O id do processo sendo executado
	MPI_Comm_rank(MPI_COMM_WORLD, &processId);

	printf("Processo %d/%d executando\n", processId, noProcesses);

	struct timespec tstart={0,0}, tend={0,0};
	clock_gettime(CLOCK_MONOTONIC, &tstart);

	int start_row = start_row = processId * (N / noProcesses);

	printf("indice[%d] = %d\n", processId, processId*(N / noProcesses));


	// Executa iterações do Game of Life
	for (vida = 0; vida < itera_max; vida++){
		printf("Geração %d | Processo %d\n", vida, processId);
		
		for (i = 0; i < N; i++){
			for (j = 0; j < N; j++){

				if (grid[i][j] == 1){
					printf("\033[1;31m");
					printf("%d", grid[i][j]);
					printf("\033[0m");
				}
				else{
					printf("%d", grid[i][j]);
				}
			}
			printf("\n");
		}
		//printf("VIVOS: %d\n", count_LiveCells());

		/*if(processId == 0){ //primeiro processo
			// preenche a tabela
			for (j = 0; j < N; j ++){
				if(j < N / noProcesses){
					// responsabilidade do Proc.0 mesmo
					
				}else{
					// Solicita a linha j (bloqueia até receber).
					int* row;
					//MPI_Recv(row, N, MPI_INT, MPI_ANY_SOURCE, j, MPI_COMM_WORLD, &status);
				}

			}
			// Transmite a tabela para os outros (noProcesses - 1) Processos
			//MPI_Send(buf, count, datatype, dest, tag, MPI_COMM_WORLD);

		}else{ //demais processos
		
			// int MPI_Send(void *data_to_send, int send_count, MPI_Datatype send_type, 
			//  int destination_ID, int tag, MPI_Comm comm); 

			// data_to_send: variable of a C type that corresponds to the send_type supplied below
			// send_count: number of data elements to be sent (nonnegative int)
			// send_type: datatype of the data to be sent (one of the MPI datatype handles)
			// destination_ID: process ID of destination (int)
			// tag: message tag (int)
			// comm: communicator (handle) 
			for (j = 0)
			MPI_Send(buf, count, datatype, dest, tag, MPI_COMM_WORLD);

			MPI_Recv(buf, count, datatype, src, tag, MPI_COMM_WORLD, status);
		}*/
	 

		game_of_life(noProcesses, processId);

		if ( N % noProcesses == 0 ){
			// divisão igualitária
			MPI_Allgather(&(grid[start_row][0]),
				N*(N / noProcesses),
				MPI_INT,
				&(grid[0][0]),
				N*(N / noProcesses),
				MPI_INT,
				MPI_COMM_WORLD
			);
			printf("\n");
		}
		else{
			int color;
			int key;
			if(processId != noProcesses - 1){
					color = 0;
					key = processId;
			}else{
					color = 1;
					key = processId;
			}
			 
			// Cria comunicador que exclui o último processo
			MPI_Comm Comm_separa_ultimo;
			MPI_Comm_split(MPI_COMM_WORLD, color, key, &Comm_separa_ultimo);

			if(color == 0){
				int N_i = (noProcesses - 1) * (N / noProcesses); 
				int noProc_i = noProcesses - 1;

				MPI_Allgather(&(grid[start_row][0]),
					N*(N_i / noProc_i),
					MPI_INT,
					&(grid[0][0]),
					N*(N_i / noProc_i),
					MPI_INT,
					Comm_separa_ultimo
				);
				printf("\n");
			}

			MPI_Barrier(MPI_COMM_WORLD);
			printf("[proc. %d] Fim do Allgather \n", processId);            
			
			// =============  Debug =====================
			int upper, lower, chunk;
			
			chunk = ceil(((float) N) / noProcesses);

			lower = chunk * processId;

			upper = lower + chunk;

			if (upper > N){ 
				upper = N;
			}


			sleep(2*processId);
			printf("Pedaço do processo %d: \n", processId);
			for (i = lower; i < upper; i++){
				for (j = 0; j < N; j++){
					if (grid[i][j] == 1){
						printf("\033[1;31m");
						printf("%d", grid[i][j]);
						printf("\033[0m");
					}
					else{
						printf("\e[0;32m");
						printf("%d", grid[i][j]);
						printf("\033[0m");
					}
				}
				printf("\n");
			}
			printf("\n");


			if(processId == 0 ){
				for (i = 0; i < N; i++){
					for (j = 0; j < N; j++){

						if (grid[i][j] == 1){
							printf("\033[1;31m");
							printf("%d", grid[i][j]);
							printf("\033[0m");
						}
						else{
							printf("\e[0;34m");
							printf("%d", grid[i][j]);
							printf("\033[0m");
						}
					}
					printf("\n");
				}
			}

			// =============  Debug =====================

			MPI_Bcast(
				&(grid[(noProcesses - 1) * (N / noProcesses)][0]),
				N * (N - (noProcesses - 1) * (N / noProcesses)),
				MPI_INT,
				noProcesses - 1,
				MPI_COMM_WORLD
			);

			printf("[proc. %d] Fim do broadcast \n", processId);            

			if(processId == 0){
				MPI_Send(&(grid[0][0]), N*N, MPI_INT, noProcesses - 1, 0, MPI_COMM_WORLD);
			}else if(processId == (noProcesses - 1)){
				// ultimo processo (pode ter numero diferente de linhas)
				MPI_Recv(&(grid[0][0]), N*N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
			}
			printf("Processo %d passou ! viva \n", processId);
			MPI_Barrier(MPI_COMM_WORLD);
			printf("[proc. %d] Fim do send / recv \n", processId);            
		}
		sleep(50);
		//getchar(); //para fazer o for esperar por um enter
	}


	cont = count_LiveCells ();

	clock_gettime(CLOCK_MONOTONIC, &tend);

	free(grid[0]);
	free(grid);


	MPI_Finalize();

	printf("VIVOS: %d\n", cont);
	printf("CORES: %d\n", cores);
	printf("TEMPO: %.5f segundos\n\n",
		   ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
		   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

	

/*
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			printf("%d", grid[i][j]);
		}
		printf("\n");
	}
*/
	// Termina os processos em execução

	return 0;
}
