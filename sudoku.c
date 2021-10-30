#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// Matriz para armazenar a solução de entrada
int sudoku[9][9];
// Array para armazenar os resultados de cada thread
int resultados[27];

// Função das threads de conferência da solução. Baseado no seu id, a thread determina qual subgrade, linha ou coluna ela deve conferir.
void *threadBody(void *arg) {
  long id = (long) arg;
  // [row_1][col_1] célula de início da conferência, [row_2][col_2] célula de término da conferência
  int row_1 = 0, row_2 = 8, col_1 = 0, col_2 = 8;

  // Id de 0 a 8 confere subgrades
  if(id < 9) {
    row_1 = (id / 3) * 3;
    row_2 = row_1 + 2;
    col_1 = (id % 3) * 3;
    col_2 = col_1 + 2;
  }
  // Id de 9 a 17 confere linhas
  else if (id < 18) {
    row_1 = id - 9;
    row_2 = row_1;
  }
  // Id de 18 a 26 confere colunas
  else {
    col_1 = id - 18;
    col_2 = col_1;
  }

  // Array para sinalizar a ocorrência de cada número de 1 a 9. O índice que representa um número 'n' é n-1
  int numeros[9] = {0};

  // Confere desde a célula inicial até a final, registrando quais números de 1 a 9 aparecem
  for(int i = row_1; i <= row_2; i++) {
    for(int j = col_1; j <= col_2; j++) {
      numeros[sudoku[i][j]-1] = 1;
    }
  }

  // Testa se todos os números de 1 a 9 estão presentes, se estiverem o resultado é positivo(1), caso contrário é negativo (0).
  int resultado = 1;
  for(int i = 0; i < 9; i++) {
    if(numeros[i] == 0) {
      resultado = 0;
    }
  }
  // Armaneza o resultado desta thread no array de resultados das threads
  resultados[(int) id] = resultado;

  pthread_exit (NULL) ;
}



int main () {
  int i = 0, j, n;
  char buffer[64];
  char* token;

  printf("\nDigite as linhas da matriz de solução para o Sudoku (cada linha deve conter 9 números de 1 a 9 separados por espaços):\n\n");

  // Loop para ler as linhas
  while(i < 9) {
    printf("Linha %d > ", i + 1);
    fgets(buffer, 64, stdin);
    // Divide as linhas em tokens
    token = strtok(buffer, " ");
    j = 0;
    // Testa se cada token da linha é um número de 1 a 9
    while(token != NULL && j < 9) {
      n = atoi(token);
      if(n > 0 && n <= 9) {
        sudoku[i][j] = n;
        j++;
        token = strtok(NULL, " ");
      }
      else {
        token = NULL;
      }
    }
    // Testa se tem 9 números válidos (de 1 a 9) na linha
    if(j == 9) {
      i++;
    }
    else {
      printf("Entrada inválida, tente novamente.\n");
    }
  }

  pthread_t thread[27];
  long id, status;

  // Cria 27 threads, 9 para avaliar as soluções de cada subgrade, 9 para avaliar as soluções de cada linha e 9 para avaliar as soluções de cada coluna
  for(id = 0; id < 27; id++) {
    status = pthread_create(&thread[id], NULL, threadBody, (void *) id);
    if (status)
    {
       perror ("pthread_create") ;
       exit (1) ;
    }
  }

  // Espera a execução de todas as threads criadas
  for(int i = 0; i < 27; i++) {
    status = pthread_join(thread[i], NULL);
  }
  if (status)
  {
     perror ("pthread_create") ;
     exit (1) ;
  }

  // Confere os resultados de cada thread para determinar se a solução é válida
  int solucao = 1;
  for(int i = 0; i < 27; i++) {
    if(resultados[i] == 0) {
      solucao = 0;
    }
  }
  if(solucao) {
    printf("\nSolução correta!\n");
  }
  else {
    printf("Solução incorreta!\n");
  }
}
