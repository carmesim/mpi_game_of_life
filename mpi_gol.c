#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define N 50
#define itera_max 2000
#define cores 2

int grid [N][N];
int new_grid[N][N];

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
    lin =10; col = 30;
    grid[lin  ][col+1] = 1;
    grid[lin  ][col+2] = 1;
    grid[lin+1][col  ] = 1;
    grid[lin+1][col+1] = 1;
    grid[lin+2][col+1] = 1;
    
}

int getNeighbors(int table[N][N], int i, int j){
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

    int i, j, upper;
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
    
    if(processId == noProcesses){
        upper = N;
    }else{    
        upper = ((processId + 1) * N / noProcesses);
    }

    for (i = processId * N / noProcesses; i < upper; i++){

        //nesse momento somente 1 th

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

    // Executa iterações do Game of Life
    for (vida = 0; vida < itera_max; vida++){
        /*
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
        }*/
        //printf("VIVOS: %d\n", count_LiveCells());

        if(processId == 0){ //primeiro processo
            // preenche a tabela
            for (j = 0; j < N; j ++){
                if(j < N / noProcesses){
                    // responsabilidade do Proc.0 mesmo
                    
                }else{
                    // Solicita a linha j (bloqueia até receber).
                    int* row;
                    MPI_Recv(row, N, MPI_INT, MPI_ANY_SOURCE, j, MPI_COMM_WORLD, &status);
                }

            }
            // Transmite a tabela para os outros (noProcesses - 1) Processos
            MPI_Send(buf, count, datatype, dest, tag, MPI_COMM_WORLD);

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
        }


        game_of_life(noProcesses, processId);
        //getchar(); //para fazer o for esperar por um enter
    }


    cont = count_LiveCells ();

    clock_gettime(CLOCK_MONOTONIC, &tend);


    if (processId == 0) {
        MPI_Finalize();
    }

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