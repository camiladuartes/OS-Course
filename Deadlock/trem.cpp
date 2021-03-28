#include "trem.h"
#include <QtCore>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

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
    bool gotMutex[8] = {false, false, false, false, false, false, false, false};
    while(true){
        switch(ID){
        case 1:     //Trem 1
            // anda direita x
            if (y == 10 && x <300)
            {
                // entrando no ponto critico 1
                if (gotMutex[1] == false && gotMutex[3] == false && x >= 274) {
                    pthread_mutex_lock(&rc1);
                    pthread_mutex_lock(&rc3);
                    gotMutex[1] = true;
                    gotMutex[3] = true;
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
                if (gotMutex[1] == true && x <= 274) {
                    pthread_mutex_unlock(&rc1);
                    gotMutex[1] = false;
                }
                if (gotMutex[3] == true && x <= 150) {
                    pthread_mutex_unlock(&rc3);
                    gotMutex[3] = false;
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
                if (gotMutex[2] == false &&
                    gotMutex[5] == false &&
                    gotMutex[4] == false &&
                    gotMutex[1] == false &&
                    x >= 524)
                {
                    pthread_mutex_lock(&rc2);
                    pthread_mutex_lock(&rc5);
                    pthread_mutex_lock(&rc4);
                    pthread_mutex_lock(&rc1);
                    gotMutex[2] = true;
                    gotMutex[5] = true;
                    gotMutex[4] = true;
                    gotMutex[1] = true;
                }
                if (gotMutex[1] == true && x > 321 && x < 350 ) {
                    pthread_mutex_unlock(&rc1);
                    gotMutex[1] = false;
                }
                x+=10;
            }
            else if (x == 550 && y < 126)
            {
                y+=10;
            }
            else if (x > 300 && y == 130)
            {
                if (gotMutex[2] == true && x <= 524) {
                    pthread_mutex_unlock(&rc2);
                    gotMutex[2] = false;
                }
                if (gotMutex[5] == true && x <= 648) {
                    pthread_mutex_unlock(&rc5);
                    gotMutex[5] = false;
                }
                x-=10;
            }
            else
            {
                if (gotMutex[4] == true) {
                    pthread_mutex_unlock(&rc4);
                    gotMutex[4] = false;
                }
                y-=10;
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 3: //Trem 3
            if (y == 10 && x <800)
            {
                if(gotMutex[2] == true && x > 575) {
                    pthread_mutex_unlock(&rc2);
                    gotMutex[2] = false;
                }
                x+=10;
            }
            else if (x == 800 && y < 126)
            {
                y+=10;
            }
            else if (x > 550 && y == 130)
            {
                if(gotMutex[6] == false && gotMutex[2] == false && x < 700) {
                    pthread_mutex_lock(&rc6);
                    pthread_mutex_lock(&rc2);
                    gotMutex[6] = true;
                    gotMutex[2] = true;
                }
                x-=10;
            }
            else
            {
                if(gotMutex[6] == true && y < 125) {
                    pthread_mutex_unlock(&rc6);
                    gotMutex[6] = false;
                }
                y-=10;
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 4: //Trem 4
            if (y == 130 && x <420)
            {
                if(gotMutex[3] == true && x > 320) {
                    pthread_mutex_unlock(&rc3);
                    gotMutex[3] = false;
                }
                x+=10;
            }
            else if (x == 420 && y < 246)
            {
                if(gotMutex[4] == true && y > 150) {
                    pthread_mutex_unlock(&rc4);
                    gotMutex[4] = false;
                }
                y+=10;
            }
            else if (x > 170 && y == 250)
            {
                if(gotMutex[7] == true && x < 400) {
                    pthread_mutex_unlock(&rc7);
                    gotMutex[7] = false;
                }
                x-=10;
            }
            else
            {
                if(gotMutex[3] == false && gotMutex[4] == false && gotMutex[7] == false && y < 160) {
                    pthread_mutex_lock(&rc3);
                    pthread_mutex_lock(&rc4);
                    pthread_mutex_lock(&rc7);
                    gotMutex[3] = true;
                    gotMutex[4] = true;
                    gotMutex[7] = true;
                }
                y-=10;
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 5: //Trem 5
            if (y == 130 && x <670)
            {
                if(gotMutex[7] == true) {
                    pthread_mutex_unlock(&rc7);
                    gotMutex[7] = false;
                }
                if(gotMutex[5] == true && x > 570) {
                    pthread_mutex_unlock(&rc5);
                    gotMutex[5] = false;
                }
                x+=10;
            }
            else if (x == 670 && y < 246)
            {
                if(gotMutex[6] == true) {
                    pthread_mutex_unlock(&rc6);
                    gotMutex[6] = false;
                }
                y+=10;
            }
            else if (x > 420 && y == 250)
            {
                if(gotMutex[7] == false && gotMutex[5] == false && gotMutex[6] == false && x < 460) {
                    pthread_mutex_lock(&rc7);
                    pthread_mutex_lock(&rc5);
                    pthread_mutex_lock(&rc6);
                    gotMutex[7] = true;
                    gotMutex[5] = true;
                    gotMutex[6] = true;
                }
                x-=10;
            }
            else
            {
                y-=10;
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        default:
            break;
        }
        msleep(velocidade);
        while(velocidade == 99){}
    }
}




