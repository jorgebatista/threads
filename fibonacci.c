#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

// Argumentos para a thread filha. n = tamanho de sequêcia de Fibonacci, p = ponteiro para o array que armazena a sequêcia
typedef struct _array_and_size {
  int n;
  long *p;
} array_and_size;

// Função para testar se o argumento é inteiro positivo
int inteiro_positivo(char *str) {
    while (*str)
        if (!isdigit(*str++))
            return 0;
    return 1;
}

// Código da thread filha
void *threadBody (void* arg)
{
  array_and_size *dados = arg;

  // Gera a sequência de Fibonacci
  if(dados->n > 0) {
    dados->p[0] = 0;
  }
  if(dados->n > 1) {
    dados->p[1] = 1;
  }
  if(dados->n > 2) {
    for(int i = 2; i < dados->n; i++) {
      dados->p[i] = dados->p[i-1] + dados->p[i-2];
    }
  }

  pthread_exit (NULL);
}

int main (int argc, char *argv[]) {

  // Testa se foi fornecido argumento na linha de comando
  if(argc == 1) {
    printf("ERRO - Faltando argumento (inteiro positivo).\n");
    exit(1);
  }

  array_and_size dados;

  // Testa se o argumento é um inteiro positivo
  if(inteiro_positivo(argv[1])) {
    dados.n = atoi(argv[1]);
  }
  else {
    printf("ERRO - O argumento deve ser um inteiro positivo (Máx 2147483647).\n");
    exit(1);
  }

  // Cria o array para armazenar a sequência de Fibonacci
  dados.p = (long *) malloc(dados.n * sizeof(long));

  pthread_t thread;
  long status;

  // Cria a thread
  status = pthread_create(&thread, NULL, threadBody, &dados);
  if (status)
  {
     perror ("pthread_join") ;
     exit (1) ;
  }

  // Aguarda o encerramento da thread
  status = pthread_join(thread, NULL);
  if (status)
  {
     perror ("pthread_join") ;
     exit (1) ;
  }

  // Imprime a sequêcia de Fibonacci
  for(int i = 0; i < dados.n - 1; i++) {
    printf("%ld, ", dados.p[i]);
  }
  printf("%ld\n", dados.p[dados.n-1]);
}
