#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

// Função que retorna a hora atual no formato hh mm ss
struct tm* hora_atual(){
    // Pegando a hora atual e criando variável do tipo time_t que armazenará os segundos 
    struct tm *data_hora_atual;              
    time_t segundos;
    
    // Obtendo o tempo em segundos
    time(&segundos);   

    // Convertendo os segundos para o formato de hora hh mm ss
    data_hora_atual = localtime(&segundos);

    return data_hora_atual;
}

// Struct para armazenar a struct data de nascimento e o time_t dos segundos 
struct tupla_nascimento {
    struct tm *data_nascimento;
    time_t hora_nascimento;
};

// Função que retorna o nascimento de um familiar, indicando as horas, minutos e
// segundos do nascimento e uma variável time_t que representa a hora do nascimento
struct tupla_nascimento nascimento(pid_t familiar){
    struct tm *data_nascimento = hora_atual();
    
    // Salvando a hora do nascimento
    time_t hora_nascimento;
    hora_nascimento = time(NULL);
    struct tupla_nascimento tupla = {data_nascimento, hora_nascimento};

    return tupla;
}

// Função que imprime a morte de um familiar, indicando a horas, minutos e 
// segundos da morte e a idade que o familiar tinha quando morreu
void morte(pid_t familiar, char nome[20], int hora, int min, int dia, time_t hora_nascimento){
    struct tm *data_morte = hora_atual();
    
    // Calculando idade
    time_t hora_morte;
    hora_morte = time(NULL);
    int idade = difftime(hora_morte, hora_nascimento);

    // Imprimindo os dados da morte
    printf(">>> Morre o %s com PID=%d às %d:%d:%d com %d anos de idade. Nascido às %d:%d:%d. \n", nome, getpid(), data_morte->tm_hour, data_morte->tm_min, data_morte->tm_sec, idade, hora, min, dia);
}

int main(){
    // Incialização dos processos
    printf(">>> Início da árvore de processos com a passagem dos anos!\n");
    pid_t filho1, filho2, neto1, neto2;

    // Nascimento do pai
    struct tm* n_pai = nascimento(getpid()).data_nascimento;
    time_t hn_pai = nascimento(getpid()).hora_nascimento;
    printf(">>> Nasce o pai com PID=%d às %d:%d:%d. \n", getpid(), n_pai->tm_hour, n_pai->tm_min, n_pai->tm_sec);

    // Step 1: 14 anos depois, nasce o filho 1
    sleep(14);
    filho1 = fork();

    if(filho1 < 0){
        printf(">>> ERR: Nascimento do filho 1.");
        return 1;
    } else if(filho1 == 0){
        struct tm* n_filho1 = nascimento(filho1).data_nascimento;
        time_t hn_filho1 = nascimento(filho1).hora_nascimento;
        printf(">>> Nasce o filho 1 com PID=%d às %d:%d:%d. \n", getpid(), n_filho1->tm_hour, n_filho1->tm_min, n_filho1->tm_sec);

        // Step 3: 10 anos depois, nasce o neto 1
        sleep(12);
        neto1 = fork();

        if(neto1 < 0){
            printf(">>> ERR: Nascimento do neto 1.");
            return 1;
        } else if(neto1 == 0){
            struct tm* n_neto1 = nascimento(neto1).data_nascimento;
            time_t hn_neto1 = nascimento(neto1).hora_nascimento;
            printf(">>> Nasce o neto 1 com PID=%d às %d:%d:%d. \n", getpid(), n_neto1->tm_hour, n_neto1->tm_min, n_neto1->tm_sec);

            // Step 7: 12 anos depois, morre o neto 1
            sleep(12);
            morte(neto1, "neto 1", n_neto1->tm_hour, n_neto1->tm_min, n_neto1->tm_sec, hn_neto1);
            return 0;
        }

        // Step 5: 14 anos depois, morre o filho 1
        sleep(18);
        morte(filho1, "filho 1", n_filho1->tm_hour, n_filho1->tm_min, n_filho1->tm_sec, hn_filho1);
        return 0;
    } else {
        // Step 2: 2 anos depois, nasce o filho 2
        sleep(2);
        filho2 = fork();

        if(filho2 < 0){
            printf(">>> ERR: Nascimento do filho 2.");
            return 1;
        }
        else if(filho2 == 0){
            struct tm* n_filho2 = nascimento(filho2).data_nascimento;
            time_t hn_filho2 = nascimento(filho2).hora_nascimento;
            printf(">>> Nasce o filho 2 com PID=%d às %d:%d:%d. \n", getpid(), n_filho2->tm_hour, n_filho2->tm_min, n_filho2->tm_sec);
            
            // Step 4: 4 anos depois, nasce o neto 1
            sleep(14);
            neto2 = fork();

            if(neto2 < 0){
                printf(">>> ERR: Nascimento do neto 2.");
                return 1;
            } else if(neto2 == 0){
                struct tm* n_neto2 = nascimento(neto2).data_nascimento;
                time_t hn_neto2 = nascimento(neto2).hora_nascimento;
                printf(">>> Nasce o neto 2 com PID=%d às %d:%d:%d. \n", getpid(), n_neto2->tm_hour, n_neto2->tm_min, n_neto2->tm_sec);

                // Step 8: 18 anos depois, morre o neto 2
                sleep(18);
                morte(neto2, "neto 2", n_neto2->tm_hour, n_neto2->tm_min, n_neto2->tm_sec, hn_neto2);
                return 0;
            }

            // Step 6: 16 anos depois, morre o filho 2
            sleep(16);
            morte(filho2, "filho 2", n_filho2->tm_hour, n_filho2->tm_min, n_filho2->tm_sec, hn_filho2);
            return 0;
        }

        wait(NULL);

        // Step 9: 60 anos depois, morre o pai
        sleep(16);
        morte(getpid(), "pai", n_pai->tm_hour, n_pai->tm_min, n_pai->tm_sec, hn_pai);
        return 0;
    }

    // Finalização dos processos
    printf(">>> Fim da árvore de processos com a passagem dos anos!\n");
    return 0;
}