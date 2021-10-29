#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

// Função para testar se o argumento é inteiro
int inteiro_positivo(char *str) {
    while (*str)
        if (!isdigit(*str++))
            return 0;
    return 1;
}

// Código da thread filha
void *threadBody (void *i)
{
  long n = *((int *) i);

  // Executa o algoritmo de collatz e imprime o resultado
  while(n != 1) {
    printf("%ld, ", n);

    if(n % 2 == 0) { n = n / 2; }
    else { n = 3 * n + 1; }
  }
  printf("%ld\n", n);

  pthread_exit (NULL);
}


int main(int argc, char *argvs[])
{
  // Testa se foi fornecido argumento na linha de comando
  if(argc == 1) {
    printf("ERRO - Faltando argumento (inteiro positivo).\n");
    exit(1);
  }

  int numero;

  // Testa se o argumento é um inteiro positivo
  if(inteiro_positivo(argvs[1])) {
    numero = atoi(argvs[1]);
  }
  else {
    printf("ERRO - O argumento deve ser um inteiro positivo (Máx 2147483647).\n");
    exit(1);
  }

  pthread_t thread;
  long status;

  // Cria a thread
  status = pthread_create (&thread, NULL, threadBody, (void *)&numero);
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
}
