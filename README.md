# *Conway's Game of Life* com MPI*
Programação Concorrente e Distribuída - 2021.2

## Divisão unidimensional do tabuleiro
Seja um tabuleiro NxN:
![](https://i.imgur.com/NTlFKrv.png)
Todos os processos, exceto o último trabalham com o mesmo número de linhas.

## Principais funções do MPI utilizadas
* **MPI_Allgather** ([doc.](https://docs.microsoft.com/en-us/message-passing-interface/mpi-allgather-function))
![](https://i.imgur.com/dYH8FEy.png) Fonte: mpitutorial.com
* **MPI_Bcast** ([doc.](https://docs.microsoft.com/en-us/message-passing-interface/mpi-bcast-function))
 ![](https://i.imgur.com/G7XNuYy.png) Fonte: mpitutorial.com

* **MPI_Send** ([doc.](https://docs.microsoft.com/en-us/message-passing-interface/mpi-send-function))
* **MPI_Recv** ([doc.](https://docs.microsoft.com/en-us/message-passing-interface/mpi-recv-function))

## Lógica para o caso em que N não é multiplo do número de processos
![](https://i.imgur.com/VMReEF6.png)



## Autores:
- Frederico J. R. Pelogia ([fpelogia](https://github.com/fpelogia/))
- Gustavo B. Oliveira ([GustavoBarbaro](https://github.com/GustavoBarbaro/))
- Vinícius R. Miguel ([vrmiguel](https://github.com/vrmiguel))


