// >>> Jantar dos filósofos utilizando hierarquia de recursos <<<

// A solução utilizada para resolver o problema do jantar dos filósofos,
// tratando as regiões críticas que cada trem atravessa e evitando deadlocks foi
// a solução de hierarquia de recursos que, atribui um pedido parcial aos recursos (garfos)
// e estabelece uma ordem/convenção onde todos os garfos serão solicitados em ordem.
// Os garfos são enumerados de 1 a 5 (no nosso caso, as regiões são enumeradas de 1 a 7)
// e cada filósofo sempre deverá pegar o primeiro garfo da ordem estabelicida e o próximo garfo da
// ordem estabelecida (no nosso caso, cada trem pega a primeira região crítica que ele
// irá passar, em seguida pega a próxima rergião e assim sucessivamente,
// e devolve a região crítica assim que passar por ela para que outro trem possa pegá-la).
// Essa solução não leva em consideração a ordem da devolução dos garfos.

// Ou seja, assim que o trem estiver prestes a entrar na primeira região crítica,
// bloqueamos todos os mutex das regiões críticas em ordem de passagem. E assim
// que o trem sair de uma região crítica, liberamos o mutex daquela região. Além disso,
// caso algum dos mutexes da região crítica de um determinado trem não consiga ser bloqueado no
// momento, se qualquer um destes mutex já tiver sido bloqueado por outro trem, desbloqueamos eles,
// e tentamos bloquear cada um na ordem novamente. Dessa forma, não existirão deadlocks.

// https://en.wikipedia.org/wiki/Dining_philosophers_problem

#include "trem.h"
#include <QtCore>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

// Importando os mutex de cada região
extern pthread_mutex_t rc1;
extern pthread_mutex_t rc2;
extern pthread_mutex_t rc3;
extern pthread_mutex_t rc4;
extern pthread_mutex_t rc5;
extern pthread_mutex_t rc6;
extern pthread_mutex_t rc7;

//Construtor parametrizdo
Trem::Trem(int ID, int x, int y) {
    this->ID = ID;
    this->x = x;
    this->y = y;
    velocidade = 100;
}

//Função a ser executada após executar trem->START
void Trem::run() {
    // Variáveis para controlar quais mutex foram pegos e devolvidos
    bool gotMutex[8] = {false, false, false, false, false, false, false, false};
    while(true){
        switch(ID){
        case 1:     //Trem 1
            if (y == 10 && x <300)
            {
                // Entrando no ponto critico 1
                if (gotMutex[1] == false && gotMutex[3] == false && x >= 274) {
                    // Pegando todos os mutex das regiões que o Trem 1 irá passar (1 e 3)
                    while(true){
                        std::cout << "Trem 1 - Tentando entrar mutex 1" << std::endl;
                        int res1 = pthread_mutex_trylock(&rc1);
                        std::cout << "Trem 1 - Tentando entrar mutex 3" << std::endl;
                        int res3 = pthread_mutex_trylock(&rc3);
                        if(res1 == 0 && res3 == 0){
                            std::cout << "Trem 1 - Entrou mutex 1" << std::endl;
                            std::cout << "Trem 1 - Entrou mutex 3" << std::endl;
                            break;
                        }
                        if(res1 == 0)
                            pthread_mutex_unlock(&rc1);
                        if(res3 == 0)
                            pthread_mutex_unlock(&rc3);
                    }
                    gotMutex[1] = true;
                    gotMutex[3] = true;
                }
                x+=10;
            }
            else if (x == 300 && y < 126) {
                y+=10;
            }
            else if (x > 50 && y == 130) {
                // Saindo do ponto critico 1 e liberando o mutex
                if (gotMutex[1] && x <= 274) {
                    pthread_mutex_unlock(&rc1);
                    std::cout << "Trem 1 - Saiu mutex 1" << std::endl;
                    gotMutex[1] = false;
                }

                // Saindo do pronto crítico 3 e liberando o mutex
                if (gotMutex[3] == true && x <= 150) {
                    std::cout << "Trem 1 - Saiu mutex 3" << std::endl;
                    pthread_mutex_unlock(&rc3);
                    gotMutex[3] = false;
                }
                x-=10;
            }
            else {
                y-=10;
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 2: //Trem 2
            // Entrando no ponto crítico 2
            if (y == 10 && x <550)
            {
                if (gotMutex[2] == false &&
                    gotMutex[5] == false &&
                    gotMutex[4] == false &&
                    gotMutex[1] == false &&
                    x >= 524)
                {
                    // Pegando todos os mutex das regiões que o Trem 2 irá passar (2, 5, 4 e 1)
                    while(true){
                        std::cout << "Trem 2 - Tentando entrar mutex 2" << std::endl;
                        int res2 = pthread_mutex_trylock(&rc2);
                        std::cout << "Trem 2 - Tentando entrar mutex 5" << std::endl;
                        int res5 = pthread_mutex_trylock(&rc5);
                        std::cout << "Trem 2 - Tentando entrar mutex 4" << std::endl;
                        int res4 = pthread_mutex_trylock(&rc4);
                        std::cout << "Trem 2 - Tentando entrar mutex 1" << std::endl;
                        int res1 = pthread_mutex_trylock(&rc1);
                        if(res2 == 0 && res5 == 0 && res4 == 0 && res1 == 0){
                            std::cout << "Trem 2 - Entrou mutex 2" << std::endl;
                            std::cout << "Trem 2 - Entrou mutex 5" << std::endl;
                            std::cout << "Trem 2 - Entrou mutex 4" << std::endl;
                            std::cout << "Trem 2 - Entrou mutex 1" << std::endl;
                            break;
                        }
                        if(res2 == 0)
                            pthread_mutex_unlock(&rc2);
                        if(res5 == 0)
                            pthread_mutex_unlock(&rc5);
                        if(res4 == 0)
                            pthread_mutex_unlock(&rc4);
                        if(res1 == 0)
                            pthread_mutex_unlock(&rc1);
                    }
                    gotMutex[2] = true;
                    gotMutex[5] = true;
                    gotMutex[4] = true;
                    gotMutex[1] = true;
                }
                // Saindo do ponto critico 1 e liberando o mutex
                if (gotMutex[1] && x > 321 && x < 350 ) {
                    pthread_mutex_unlock(&rc1);
                    std::cout << "Trem 2 - Saiu mutex 1" << std::endl;
                    gotMutex[1] = false;
                }
                x+=10;
            }
            else if (x == 550 && y < 126) {
                y+=10;
            }
            else if (x > 300 && y == 130) {
                // Saindo do ponto critico 2 e liberando o mutex
                if (gotMutex[2] && x <= 524) {
                    pthread_mutex_unlock(&rc2);
                    std::cout << "Trem 2 - Saiu mutex 2" << std::endl;
                    gotMutex[2] = false;
                }

                // Saindo do ponto critico 5 e liberando o mutex
                if (gotMutex[5] == true && x <= 400) {
                    pthread_mutex_unlock(&rc5);
                    std::cout << "Trem 2 - Saiu mutex 5" << std::endl;
                    gotMutex[5] = false;
                }
                x-=10;
            }
            else {
                // Saindo do ponto critico 4 e liberando o mutex
                if (gotMutex[4]) {
                    pthread_mutex_unlock(&rc4);
                    std::cout << "Trem 2 - Saiu mutex 4" << std::endl;
                    gotMutex[4] = false;
                }
                y-=10;
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 3: //Trem 3
            if (y == 10 && x <800) {
                // Saindo do ponto critico 2 e liberando o mutex
                if(gotMutex[2] && x > 575) {
                    pthread_mutex_unlock(&rc2);
                    std::cout << "Trem 3 - Saiu mutex 2" << std::endl;
                    gotMutex[2] = false;
                }
                x+=10;
            }
            else if (x == 800 && y < 126) {
                y+=10;
            }

            // Entrando no ponto crítico 6
            else if (x > 550 && y == 130)
            {
                if(gotMutex[6] == false && gotMutex[2] == false && x < 700) {
                    // Pegando todos os mutex das regiões que o Trem 3 irá passar (6 e 2)
                    while(true){
                        std::cout << "Trem 3 - Tentando entrar mutex 6" << std::endl;
                        int res6 = pthread_mutex_trylock(&rc6);
                        std::cout << "Trem 3 - Tentando entrar mutex 2" << std::endl;
                        int res2 = pthread_mutex_trylock(&rc2);
                        if(res6 == 0 && res2 == 0){
                            std::cout << "Trem 3 - Entrou mutex 6" << std::endl;
                            std::cout << "Trem 3 - Entrou mutex 2" << std::endl;
                            break;
                        }
                        if(res6 == 0)
                            pthread_mutex_unlock(&rc6);
                        if(res2 == 0)
                            pthread_mutex_unlock(&rc2);
                    }
                    gotMutex[6] = true;
                    gotMutex[2] = true;
                }
                x-=10;
            }
            else {
                // Saindo do ponto critico 6 e liberando o mutex
                if(gotMutex[6] && y < 125) {
                    pthread_mutex_unlock(&rc6);
                    std::cout << "Trem 3 - Saiu mutex 6" << std::endl;
                    gotMutex[6] = false;
                }
                y-=10;
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 4: //Trem 4
            if (y == 130 && x <420) {
                // Saindo do ponto critico 3 e liberando o mutex
                if(gotMutex[3] && x > 320) {
                    pthread_mutex_unlock(&rc3);
                    std::cout << "Trem 4 - Saiu mutex 3" << std::endl;
                    gotMutex[3] = false;
                }
                x+=10;
            }
            else if (x == 420 && y < 246) {
                // Saindo do ponto critico 4 e liberando o mutex
                if(gotMutex[4] && y > 150) {
                    pthread_mutex_unlock(&rc4);
                    std::cout << "Trem 4 - Saiu mutex 4" << std::endl;
                    gotMutex[4] = false;
                }
                y+=10;
            }
            else if (x > 170 && y == 250) {
                // Saindo do ponto critico 7 e liberando o mutex
                if(gotMutex[7] && x < 400) {
                    pthread_mutex_unlock(&rc7);
                    std::cout << "Trem 4 - Saiu mutex 7" << std::endl;
                    gotMutex[7] = false;
                }
                x-=10;
            }

            else
            {
                // Entrando no ponto crítico 3
                if(gotMutex[3] == false && gotMutex[4] == false && gotMutex[7] == false && y < 160) {
                    // Pegando todos os mutex das regiões que o Trem 4 irá passar (3, 4 e 7)
                    while(true){
                        std::cout << "Trem 4 - Tentando entrar mutex 3" << std::endl;
                        int res3 = pthread_mutex_trylock(&rc3);
                        std::cout << "Trem 4 - Tentando entrar mutex 4" << std::endl;
                        int res4 = pthread_mutex_trylock(&rc4);
                        std::cout << "Trem 4 - Tentando entrar mutex 7" << std::endl;
                        int res7 = pthread_mutex_trylock(&rc7);
                        if(res3 == 0 && res4 == 0 && res7 == 0){
                            std::cout << "Trem 4 - Entrou mutex 3" << std::endl;
                            std::cout << "Trem 4 - Entrou mutex 4" << std::endl;
                            std::cout << "Trem 4 - Entrou mutex 7" << std::endl;
                            break;
                        }
                        if(res3 == 0)
                            pthread_mutex_unlock(&rc3);
                        if(res4 == 0)
                            pthread_mutex_unlock(&rc4);
                        if(res7 == 0)
                            pthread_mutex_unlock(&rc7);
                    }
                    gotMutex[3] = true;
                    gotMutex[4] = true;
                    gotMutex[7] = true;
                }
                y-=10;
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 5: //Trem 5
            if (y == 130 && x <670) {
                // Saindo do ponto critico 7 e liberando o mutex
                if(gotMutex[7]) {
                    pthread_mutex_unlock(&rc7);
                    std::cout << "Trem 5 - Saiu mutex 7" << std::endl;
                    gotMutex[7] = false;
                }
                // Saindo do ponto critico 5 e liberando o mutex
                if(gotMutex[5] && x > 570) {
                    pthread_mutex_unlock(&rc5);
                    std::cout << "Trem 5 - Saiu mutex 5" << std::endl;
                    gotMutex[5] = false;
                }
                x+=10;
            }
            else if (x == 670 && y < 246) {
                // Saindo do ponto critico 6 e liberando o mutex
                if(gotMutex[6]) {
                    pthread_mutex_unlock(&rc6);
                    std::cout << "Trem 5 - Saiu mutex 6" << std::endl;
                    gotMutex[6] = false;
                }
                y+=10;
            }

            else if (x > 420 && y == 250)
            {
                // Entrando no ponto crítico 7
                if(gotMutex[7] == false && gotMutex[5] == false && gotMutex[6] == false && x < 460) {
                    // Pegando todos os mutex das regiões que o Trem 5 irá passar (7, 5 e 6)
                    while(true){
                        std::cout << "Trem 5 - Tentando entrar mutex 7" << std::endl;
                        int res7 = pthread_mutex_trylock(&rc7);
                        std::cout << "Trem 5 - Tentando entrar mutex 5" << std::endl;
                        int res5 = pthread_mutex_trylock(&rc5);
                        std::cout << "Trem 5 - Tentando entrar mutex 6" << std::endl;
                        int res6 = pthread_mutex_trylock(&rc6);
                        if(res7 == 0 && res5 == 0 && res6 == 0){
                            std::cout << "Trem 5 - Entrou mutex 7" << std::endl;
                            std::cout << "Trem 5 - Entrou mutex 5" << std::endl;
                            std::cout << "Trem 5 - Entrou mutex 6" << std::endl;
                            break;
                        }
                        if(res7 == 0)
                            pthread_mutex_unlock(&rc7);
                        if(res5 == 0)
                            pthread_mutex_unlock(&rc5);
                        if(res6 == 0)
                            pthread_mutex_unlock(&rc6);
                    }
                    gotMutex[7] = true;
                    gotMutex[5] = true;
                    gotMutex[6] = true;
                }
                x-=10;
            }
            else {
                y-=10;
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        default:
            break;
        }
        msleep(velocidade);

        // Controle de velocidade para quando a velocidade estiver no mínimo o trem parar sem perder os estados dos mutex
        while(velocidade == 99){}
    }
}
