#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define PI 3.141592

// Vetor que vai armazenar cada área calculada pela thread
float *areas;

// Contador para exibir qual trapézio a thread está calculando no momento
int cont = 0;

// Função constante que retorna 5.0
float funcao1(float x){
    return 5.0;
}

// Função que calcula o sen(2x)+cos(5x) com os limites em radianos
float funcao2(float x){
    float seno = (PI/180) * sin(2.0*x);
    float coseno = (PI/180) * cos(5.0*x);
    return (seno + coseno);
}

// Função que calcula a área dos trapézios que cada thread é responsável
void* areaTrapezio(int* buffer){
    // Recebendo os valores passados pelo buffer
    int id = buffer[0];
    int t = buffer[1];
    int n = buffer[2];
    float a = buffer[3];
    float b = buffer[4];
    int funcao = buffer[5];

    // Quantidade de trapézios que cada thread vai calcular a área
    int divisao = floor(n/t);

    // Quantidade de trapézios que a última thread vai acrescentar para cálculo da área
    if(id == n-1){
        int resto = 0;
        if(n % t != 0){
            resto = n%t;
            divisao += resto;
        }
    }

    // Calculando localmente o valor para local_a, local_b e local_n que cada thread irá utilizar
    float local_a = id*(b/t);
    float local_b = local_a+(b/t);
    int local_n = divisao;

    // Calculando o h da solução para o cálculo da área de um trapézio
    float h = (local_b-local_a)/local_n;
    float area_total = 0.0;
        
    // Caso a função escolhida seja a função1
    if(funcao == 1){
        // Calculando a área total pela fórmula e adicionando no vetor que está na memória compartilhada
        int i=1;
        cont++;
        printf(">>> A thread %d está calculando a área do trapézio %d.\n", id, cont);

        area_total = (funcao1(local_a)+funcao1(local_a+h))*(h/2);
        areas[id*divisao] = area_total;
        
        for(i = 1; i < local_n; i++){
            float x_i = local_a+i*h;
            area_total += (funcao1(x_i) + funcao1(x_i+h))*(h/2);

            printf(">>> A thread %d está calculando a área do trapézio %d.\n", id, cont+1);
            cont++;

            // Atualizando o valor de cada área calculada no vetor que está na memória compartilhada
            areas[(id*divisao)+i] = (funcao1(x_i) + funcao1(x_i+h))*(h/2);
        }
    // Caso a função escolhida seja a função2
    } else {
        // Calculando a área total pela fórmula e adicionando no vetor que está na memória compartilhada
        int i=1;
        cont++;
        printf(">>> A thread %d está calculando a área do trapézio %d.\n", id, cont);

        area_total = (funcao2(local_a)+funcao2(local_a+h))*(h/2);
        areas[id*divisao] = area_total;

        for(i = 1; i < local_n; i++){
            float x_i = local_a+i*h;
            area_total += (funcao2(x_i) + funcao2(x_i+h))*(h/2);

            printf(">>> A thread %d está calculando a área do trapézio %d.\n", id, cont+1);
            cont++;

            // Atualizando o valor de cada área calculada no vetor que está na memória compartilhada
            areas[(id*divisao)+i] = (funcao2(x_i) + funcao2(x_i+h))*(h/2);
        }
    }

    // printf(">>> Soma das áreas dos trapézios calculada pela thread %d: %f\n", id, area_total);
    
    // Finalizando a thread que tem sau rotina principal realizada
    pthread_exit(NULL);
}

int main(){
    float t, n, a, b, f;
    t = -1;

    // Conferindo condição t>1 e fazendo leituras dos dados
    while(t < 1){
        printf(">>> Informe o número de threads: ");
        scanf("%f", &t);
    }
    printf(">>> Informe o número de trapézios: ");
    scanf("%f", &n);
    printf(">>> Informe o a: ");
    scanf("%f", &a);
    printf(">>> Informe o b: ");
    scanf("%f", &b);
    printf(">>> Informe a função [1 ou 2]: ");
    scanf("%f", &f);

    // Criando o vetor que guardará a área de cada um dos trapézios com alocação dinâmica
    areas = malloc(sizeof(float) * n);

    // Crianto o vetor de threads
    pthread_t threads[(int)t];
    int status;
    void *thread_return;

    // Criando o buffer que enviará os dados para cada thread utilizando alocação dinâmica
    int *buffer = malloc(sizeof(float) * 6);
    buffer[0] = 0;    // Id Thread
    buffer[1] = t;    // Número de threads
    buffer[2] = n;    // Número de trapézios
    buffer[3] = a;    // Valor escolhido para o ponto a
    buffer[4] = b;    // Valor escolhido para o ponto b
    buffer[5] = f;    // Função escolhida

    // Criando as threads
    for(int i = 0; i < t; i++){
        printf(">>> Criando thread %d...\n", i);
        buffer[0] = i;
        status = pthread_create(&threads[i], NULL, (void*)areaTrapezio, buffer);

        // Condição para verificar erro na criação da thread
        if(status!=0){
            printf(">>> Erro na criação da thread %d. Código de Erro: %d\n", i, status);
        }

        // Finalizando a thread
        pthread_join(threads[i], &thread_return);
        printf(">>> Thread %d finalizada.\n", i);
    }

    // Exibindo o valor da área calculada em cada trapézio 
    float area_total = 0;
    printf(">>> Áreas calculadas: \n");
    for(int i = 0; i < n; i++){
        printf("Valor da área %d: %f (%.2e)\n", i+1, areas[i], areas[i]);
        area_total += areas[i];
    }

    // Exibindo o resultado final da soma de todas as áreas calculadas em notação científica
    printf(">>> Soma da área total dos trapézios: %.2e\n", area_total);

    // Finalizando a execução com sucesso
    exit(EXIT_SUCCESS);
}