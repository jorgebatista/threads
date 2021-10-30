#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para passar os índices como argumento das threads
typedef struct {
    int i;
    int j;
} indices;

int *lista_original, *lista_ordenada;

// Algoritmo de ordenação
void bubbleSort(int i, int j) {
  int aux;

  if(j < i) {
    return;
  }

  for(int x = i; x < j - 1; x++) {
    if(lista_original[x] > lista_original[x+1]) {
      aux = lista_original[x];
      lista_original[x] = lista_original[x+1];
      lista_original[x+1] = aux;
    }
  }

  bubbleSort(i, j-1);
}


// Thread para ordenar uma metade da lista
void *threadOrdenacao(void *arg) {
  indices *indices_t  = (indices *) arg;

  bubbleSort(indices_t->i, indices_t->j);

  pthread_exit (NULL);
}



// Thread para mesclar as duas metades ordenadas da lista
void *threadMesclagem (void *arg) {
  indices *indices_t = (indices *) arg;
  int i = 0, j = indices_t->i, k = 0;

  // Compara os primeiros elementos ainda não selecionados de cada metade ordenada, seleciona o menor e adiciona na lista ordenada final e assim sucessivamente
  while(i < indices_t->i && j < indices_t->j) {
    if(lista_original[i] <= lista_original[j]) {
      lista_ordenada[k] = lista_original[i];
      i++;
    }
    else {
      lista_ordenada[k] = lista_original[j];
      j++;
    }
    k++;
  }
  // Adiciona os elementos restantes da primeira metade ordena
  while(i < indices_t->i) {
    lista_ordenada[k] = lista_original[i];
    i++;
    k++;
  }
  // Adiciona os elementos restantes da segunda metade ordena
  while(j < indices_t->j) {
    lista_ordenada[k] = lista_original[j];
    j++;
    k++;
  }

  pthread_exit (NULL);
}



int main(int argc, char *argv[]) {
  int size;

  // Teste se foi fornecida a lista de argumentos
  if(argc == 1) {
    printf("Forneça os números a serem ordenados como argumentos. (Apenas números inteiros)\n");
    exit(1);
  }
  // Tamanho da lista de argumentos
  size = argc - 1;
  // Aloca o array para armazenar a lista de entrada e a lista ordenada do resultado
  lista_original = (int*) malloc(size * sizeof(int));
  lista_ordenada = (int*) malloc(size * sizeof(int));
  int n;
  char* end;
  // Testa se todos os argumentos são inteiros e os adicionam no array, caso contrário aborta o programa
  for(int i = 1; i < argc; i++) {
    n = strtol(argv[i], &end, 10);
    if(*end != 0) {
      printf("Argumento inválido. Encerrando...\n");
      exit(1);
    }
    else {
      lista_original[i-1] = n;
    }
  }

  pthread_t thread[3];
  long status;

  indices indices_t1, indices_t2, indices_t3;

  // Cria a thread para ordenar a primeira metade da lista
  indices_t1.i = 0;
  indices_t1.j = size/2;
  status = pthread_create (&thread[0], NULL, threadOrdenacao, (void *) &indices_t1);
  if (status)
  {
     perror ("pthread_create") ;
     exit (1) ;
  }

  // Cria a thread para ordenar a segunda metade da lista
  indices_t2.i = size/2;
  indices_t2.j = size;
  status = pthread_create (&thread[1], NULL, threadOrdenacao, (void *) &indices_t2);
  if (status)
  {
     perror ("pthread_create") ;
     exit (1) ;
  }

  // Aguarda a execução das threads de ordenação
  status = pthread_join(thread[0], NULL);
  if (status)
  {
     perror ("pthread_create") ;
     exit (1) ;
  }
  status = pthread_join(thread[1], NULL);
  if (status)
  {
     perror ("pthread_create") ;
     exit (1) ;
  }

  // Cria a thread para mesclar as duas metades ordenadas da lista
  indices_t3.i = size/2;
  indices_t3.j = size;
  status = pthread_create (&thread[2], NULL, threadMesclagem, (void *) &indices_t3);
  if (status)
  {
     perror ("pthread_create") ;
     exit (1) ;
  }

  // Aguarda a execução da thread de mesclagem
  status = pthread_join(thread[2], NULL);
  if (status)
  {
     perror ("pthread_create") ;
     exit (1) ;
  }

  // Imprime a lista ordenada
  for(int i = 0; i < size; i++) {
    printf("%d ", lista_ordenada[i]);
  }
  printf("\n");
}
