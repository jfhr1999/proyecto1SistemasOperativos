#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

int createBurst(int limiteIn, int limiteSup){
    //srand(time(0));
    int burst = (rand() % (limiteSup - limiteIn + 1)) + limiteIn; //crear un numero aleatorio entre 2 limites
    return burst;
}

int createPriority(){
    int prioridad = (rand() % (5)) + 1;
    return prioridad;

}

int createRandSleep(){
    int sleep = (rand() % (8 - 3 + 1)) + 3;
    return sleep; 
}

int main(int argc, char const *argv[]) {
    for(int i = 0; i < 10; i++){
        int burst = createBurst(1, 10);
        printf("Num %d", burst);
        sleep(1);
    }
    return 0;
}