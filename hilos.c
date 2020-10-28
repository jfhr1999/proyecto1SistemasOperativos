#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

int createBurst(int limiteIn, int limiteSup){
    srand(time(0));
    int burst = (rand() % (limiteSup - limiteIn + 1)) + limiteIn; //crear un numero aleatorio entre 2 limites
    return burst;
}

int createPriority(){
    int prioridad = (rand() % (5)) + 1;
    return prioridad;

}

void sendThread(int burst, int prior);
void* sendToServer(void * arg);


int main(int argc, char const *argv[]){
    bool bandera = false;
    bandera = true;
    while(bandera){
        int burst = createBurst(1, 5);
        int prior = createPriority();
        int tasa = 10;

        sendThread(burst, prior);

        if(tasa > 0){
            sleep(60/tasa);
        } else {
            sleep(60);
        }
    }
    printf("All done!\n");
    return 0;
}

void sendThread(int burst, int prior){
    //Nuevo thread
    pthread_t newThread;
    //Para guardar el pid que recibe el thread como resultado
    int *pid_result;

    int values[2];

    values[0] = burst;
    values[1] = prior;


    pthread_create(&newThread, NULL, sendToServer, &values);
    //Espera a que termine de correr el thread y recibe el resultado
    pthread_join(newThread, NULL);

}

void* sendToServer(void * arg){
    int (*values)[2] = (int (*) [2]) arg;
    int burstNum = *values[0];
    int priority = *values[1];

    int resPid = 0;

    sleep(2);

    printf("Respuesta de servidor, pid: %d\n", resPid);

    return NULL;

}