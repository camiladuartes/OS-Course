#include "trem.h"
#include <QtCore>
#include <pthread.h>
#include <unistd.h>

extern pthread_mutex_t rc1;
extern pthread_mutex_t rc2;
extern pthread_mutex_t rc3;
extern pthread_mutex_t rc4;
extern pthread_mutex_t rc5;
extern pthread_mutex_t rc6;
extern pthread_mutex_t rc7;

//Construtor
Trem::Trem(int ID, int x, int y){
    this->ID = ID;
    this->x = x;
    this->y = y;
    velocidade = 100;
}

//Função a ser executada após executar trem->START
void Trem::run(){
    bool gotMutex = false;
    while(true){
        switch(ID){
        case 1:     //Trem 1
            // anda direita x
            if (y == 10 && x <300)
            {
                // entrando no ponto critico 1
                if (gotMutex == false && x >= 274) {
                    pthread_mutex_lock(&rc1);
                    pthread_mutex_lock(&rc3);
                    gotMutex = true;
                }
                x+=10;
            }
            // desce y
            else if (x == 300 && y < 126)
            {
                y+=10;
            }
            // anda esquerda x
            else if (x > 50 && y == 130)
            {
                //saindo do ponto critico 1
                if (gotMutex == true && x <= 274) {
                    pthread_mutex_unlock(&rc1);
                }
                if (gotMutex == true && x <= 150) {
                    pthread_mutex_unlock(&rc3);
                    gotMutex = false;
                }
                x-=10;
            }
            // sobe y
            else
            {
                y-=10;
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 2: //Trem 2
            if (y == 10 && x <550)
            {
                if (gotMutex == false && x >= 524) {
                    pthread_mutex_lock(&rc2);
                    pthread_mutex_lock(&rc5);
                    pthread_mutex_lock(&rc4);
                    pthread_mutex_lock(&rc1);
                    gotMutex = true;
                }
                if (gotMutex == true && x > 321 && x < 350 ) {
                    pthread_mutex_unlock(&rc1);
                    gotMutex = false;
                }
                x+=10;
            }
            else if (x == 550 && y < 126)
            {
                y+=10;
            }
            else if (x > 300 && y == 130)
            {
                //saindo do ponto critico 1
                if (gotMutex == true && x <= 524) {
                    pthread_mutex_unlock(&rc2);
                }
                if (gotMutex == true && x <= 648) {
                    pthread_mutex_unlock(&rc5);
                }
                x-=10;
            }
            else
            {
                if (gotMutex == true) {
                    pthread_mutex_unlock(&rc4);
                }
                y-=10;
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 3: //Trem 3
            if (y == 10 && x <800)
                x+=10;
            else if (x == 800 && y < 126)
                y+=10;
            else if (x > 550 && y == 130)
                x-=10;
            else
                y-=10;
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 4: //Trem 4
            if (y == 130 && x <420)
                x+=10;
            else if (x == 420 && y < 246)
                y+=10;
            else if (x > 170 && y == 250)
                x-=10;
            else
              y-=10;
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 5: //Trem 5
            if (y == 130 && x <670)
                x+=10;
            else if (x == 670 && y < 246)
                y+=10;
            else if (x > 420 && y == 250)
                x-=10;
            else
                y-=10;
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        default:
            break;
        }
        msleep(velocidade);
    }
}




