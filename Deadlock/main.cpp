#include "mainwindow.h"
#include <QApplication>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t rc1;
pthread_mutex_t rc2;
pthread_mutex_t rc3;
pthread_mutex_t rc4;
pthread_mutex_t rc5;
pthread_mutex_t rc6;
pthread_mutex_t rc7;

int main(int argc, char *argv[])
{
    pthread_mutex_init(&rc1, NULL);
    pthread_mutex_init(&rc2, NULL);
    pthread_mutex_init(&rc3, NULL);
    pthread_mutex_init(&rc4, NULL);
    pthread_mutex_init(&rc5, NULL);
    pthread_mutex_init(&rc6, NULL);
    pthread_mutex_init(&rc7, NULL);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
