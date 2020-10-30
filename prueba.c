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
#include <netinet/in.h>
#define PORT 8080 

char algorithm[50] = {0};
int quantum = 0;
bool run = true;
bool insertInProgress = false;
bool zona_critica = false;

struct Process {
    int pId;
    int burst;
    int priority;
    int remaining;
    int tat; //Turn-around time
    int wt; //Waiting time
};

typedef struct {
    struct Process *array;
    size_t used;
    size_t size;
} Array;

Array readyQueue;
Array doneQueue;

void initArray(Array *a, size_t initialSize) {
    a->array = malloc(initialSize * sizeof(struct Process));
    a->used = 0;
    a->size = initialSize;
}

void insertProcess(Array *a, int id, int burst, int priority, int tat, int wt) {
    struct Process p;
    p.pId = id;
    p.burst = burst;
    p.priority = priority;
    p.remaining = p.burst;
    p.tat = tat;
    p.wt = wt;
    if (a->used == a->size) {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(struct Process));
    }
    a->array[a->used++] = p;
    printf("Internal insert done. \n");
}


void freeArray(Array *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void FIFO(Array *a) {
    int size = a->used;
    for (int i = 0; i < size; i++) {
        printf("Proceso %d", a->array[0].pId);
        printf(" con burst %d", a->array[0].burst);
        printf(" y prioridad %d", a->array[0].priority);
        printf(" entra en ejecucion\n\n");
        //sleep del thread por a->array[0].burst segundos
        sleep(a->array[0].burst);
        a->array[0].remaining = 0;
        printf("Proceso terminado:\n");
        printf("PID: %d\n", a->array[0].pId);
        printf("Burst: %d\n", a->array[0].burst);
        printf("Prioridad: %d\n\n", a->array[0].priority);

        for (int j = 0; j < a->used; j++) {
            a->array[j].tat = a->array[j].tat + a->array[0].burst; 
        }

        a->array[0].wt = a->array[0].tat - a->array[0].burst;

        insertProcess(&doneQueue, a->array[0].pId, a->array[0].burst, a->array[0].priority, a->array[0].tat, a->array[0].wt);


        struct Process p;
        for (int j = 0; j < a->used - 1; j++) {
            p = a->array[j+1];
            a->array[j] = p;
        }

        printf("\nUsed: %d\n", a->used);
        printf("i: %d\n", i);
        a->used--;

    }
}

void SJF(Array *a) {
    int size = a->used;
    for (int i = 0; i < size; i++) {
        int shortestJobIndex = 0;
        for (int j = 0; j < a->used; j++) {
            if (a->array[j].burst < a->array[shortestJobIndex].burst){
                shortestJobIndex = j;
            }
        }
        printf("Proceso %d", a->array[shortestJobIndex].pId);
        printf(" con burst %d", a->array[shortestJobIndex].burst);
        printf(" y prioridad %d", a->array[shortestJobIndex].priority);
        printf(" entra en ejecucion\n\n");
        //sleep del thread por a->array[i].burst segundos
        sleep(a->array[i].burst);
        a->array[shortestJobIndex].remaining = 0;
        printf("Proceso terminado:\n");
        printf("PID: %d\n", a->array[shortestJobIndex].pId);
        printf("Burst: %d\n", a->array[shortestJobIndex].burst);
        printf("Prioridad: %d\n\n", a->array[shortestJobIndex].priority);
        struct Process p;
        for (int j = shortestJobIndex; j < a->used - 1; j++) {
            p = a->array[j+1];
            a->array[j] = p;
        }

        a->used--;
    }
}

void HPF(Array *a) {
    int size = a->used;
    for (int i = 0; i < size; i++) {
        int topPriorityIndex = 0;
        for (int j = 0; j < a->used; j++) {
            if (a->array[j].priority < a->array[topPriorityIndex].priority){
                topPriorityIndex = j;
            }
        }
        printf("Proceso %d", a->array[topPriorityIndex].pId);
        printf(" con burst %d", a->array[topPriorityIndex].burst);
        printf(" y prioridad %d", a->array[topPriorityIndex].priority);
        printf(" entra en ejecucion\n\n");
        //sleep del thread por a->array[i].burst segundos
        sleep(a->array[i].burst);
        a->array[topPriorityIndex].remaining = 0;
        printf("Proceso terminado:\n");
        printf("PID: %d\n", a->array[topPriorityIndex].pId);
        printf("Burst: %d\n", a->array[topPriorityIndex].burst);
        printf("Prioridad: %d\n\n", a->array[topPriorityIndex].priority);
        struct Process p;
        for (int j = topPriorityIndex; j < a->used - 1; j++) {
            p = a->array[j+1];
            a->array[j] = p;
        }
        a->used--;
    }
}

void RR(Array *a, int quantum) {
    int index = 0;
    while(a->used > 0){
        if(quantum >= a->array[index].remaining){
            printf("Proceso %d", a->array[index].pId);
            printf(" con burst %d", a->array[index].burst);
            printf(" y prioridad %d", a->array[index].priority);
            printf(" con remaining %d", a->array[index].remaining);
            printf(" entra en ejecucion\n\n");
            //sleep por a->array[index].remaining segundos
            sleep(a->array[index].remaining);
            printf("Proceso terminado:\n");
            printf("PID: %d\n", a->array[index].pId);
            printf("Burst: %d\n", a->array[index].burst);
            printf("Prioridad: %d\n\n", a->array[index].priority);
            zona_critica = true;
            if(index + 1 != a->used){
                struct Process p;
                for (int i = index; i < a->used - 1; i++) {
                    p = a->array[i+1];
                    a->array[i] = p;
                }
            }else{
                index = 0; 
            }
            a->used--;
            zona_critica = false;
        }else{
            printf("Proceso %d", a->array[index].pId);
            printf(" con burst %d", a->array[index].burst);
            printf(" y prioridad %d", a->array[index].priority);
            printf(" con remaining %d", a->array[index].remaining);
            printf(" entra en ejecucion\n\n");
            printf(" con quantum %d", quantum);
            //sleep por quantum segundos
            sleep(quantum);
            a->array[index].remaining = a->array[index].remaining - quantum;
            index++;
            index = index % a->used; 
        }
    }
}

void* runCpuScheduler(void *arg);
void* listenForCommands(void *arg);

//[1,2,5,4,3]

int main(){

    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};

    //para enviar los pId
    int pNumber = 0; 
    char numStr[50] = {0};

    //para elegir el algoritmo
    char alg[50] = {0};  


    //Thread CPU scheduler--------------------------------------
    initArray(&readyQueue, 5);
    //Despues de init creo el CPU scheduler
    pthread_t cpuScheduler;
    pthread_create(&cpuScheduler, NULL, runCpuScheduler, NULL); 
    //-----------------------------------------------------------

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    bool valid_algorithm = false;
    //Elegir algoritmo
    do{
        printf("Ingrese el tipo de algoritmo que se va a utilizar:  FIFO(f), SJF(s), HPF(h), Round Robin(r)\n");
        scanf("%s", algorithm);
        if(!strcmp(algorithm,"r")){
            printf("Ingrese el quantum para el Round Robin\n");
            scanf("%d",&quantum);
            printf("El quantum es de %d\n",quantum);
            valid_algorithm = true;
        } else
        if(!strcmp(algorithm, "f") || !strcmp(algorithm,"s") || !strcmp(algorithm, "h")){
            valid_algorithm = true;
        } else{
            printf("Comando no reconocido. Intente de nuevo. \n");
        }

    } while(!valid_algorithm);

    //Socket acepta conexion
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  (socklen_t*)&addrlen))<0) { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } else{
        printf("Servidor se ha conextado\n");
    }

    printf("EL servidor esta ejecutando y recibio un cliente\n");

    //Thread comandos----------------------------------------------
    //Inicializar consola para ver cola de ready o salir
    pthread_t commands;
    pthread_create(&commands, NULL, listenForCommands, NULL); 
    //-------------------------------------------------------------

    char separator[] = ",";
    
    while(run){
        
         
        // el servidor escucha lo que le manda el usuario
        
        //Paso la informacion recibida a un buffer
        memset(buffer, '\0', sizeof (buffer));
        valread = read( new_socket , buffer, 1024);
        if(!strcmp(buffer,"fin")){ //??????
            break;
        }
        printf("El cliente envio: %s\n",buffer ); 

        //Envio un pId como respuesta
        sprintf(numStr, "%i", pNumber);
        send(new_socket , numStr , strlen(numStr) , 0 );
        
        //Paso el string recibido a ints
        char* token;
        int var;
        int input[2];
        int i = 0;

        token = strtok(buffer, separator);
        while(token != NULL){
            sscanf(token, "%d", &var);
            input[i++] = var;

            token = strtok(NULL, separator);
        }

        //Job scheduler--------------------------------------------------------
        insertInProgress = true;
        while(zona_critica){}
        insertProcess(&readyQueue, pNumber, input[0], input[1], 0, 0);
        printf("Inserted into queue. \n");
        insertInProgress = false;
        //---------------------------------------------------------------------

        //Incremento el pId para el proximo proceso
        pNumber = pNumber + 1; 
        
    }


    //---------------------------------------------------------------Thread Llevar estadisticas
    
    //printf("%d\n", readyQueue.used);
    
    /*printf("Proceso %d\n", readyQueue.array[0].pId);
    printf("Prioridad: %d\n", readyQueue.array[0].priority);
    printf("Burst: %d\n", readyQueue.array[0].burst);
    printf("Burst restante: %d\n\n", readyQueue.array[0].remaining);

    printf("Proceso %d\n", readyQueue.array[1].pId);
    printf("Prioridad: %d\n", readyQueue.array[1].priority);
    printf("Burst: %d\n", readyQueue.array[1].burst);
    printf("Burst restante: %d\n\n", readyQueue.array[1].remaining);*/

    /*struct Process p1 = readyQueue.array[0];

    printf("Proceso %d\n", p1.pId);
    printf("Prioridad: %d\n", p1.priority);
    printf("Burst: %d\n", p1.burst);
    printf("Burst restante: %d\n\n", p1.remaining);*/

    pthread_join(cpuScheduler, NULL);
    pthread_join(commands, NULL);
    printf("Socket listen done. \n");

    freeArray(&readyQueue);
    printf("Array freed.\n");
    return 0;
}

void* runCpuScheduler(void *arg){
    while(run){
        if(!insertInProgress){ //Si esta haciendo un insert, try again later
        //printf("No insert in progress.\n");
            if(readyQueue.used > 0){ //Si esta vacio el queue, try again later
            //printf("Queue not empty.\n");
                if(!strcmp(algorithm, "f")){ //FIFO
                    //printf("FIFO.\n");
                    FIFO(&readyQueue);
                } else if(!strcmp(algorithm, "s")){ //SJF
                    //printf("SJF\n");
                    SJF(&readyQueue);
                } else if(!strcmp(algorithm, "h")){ //HPF
                    //printf("HPF.\n");
                    HPF(&readyQueue);
                } else if(!strcmp(algorithm, "r")){ //Round Robin
                    //printf("Round Robin.\n");
                    RR(&readyQueue, quantum);
                }
            }
        }
    }
    printf("CPU Scheduler done. \n");
    return NULL;
}

void* listenForCommands(void *arg){
    char command[50] = {0};
    while(run){
        printf("Digite 'c' para ver la cola o 'fin' para salir: \n");
        scanf("%s", command);
        if(!strcmp(command,"fin")){//condicion de parada
            run = false;
        } else if(!strcmp(command, "c")){
            printf("-----Cola:-------\n");
            for (int i = 0; i < readyQueue.used; i++) {
                printf("Proceso %d\n", readyQueue.array[i].pId);
                printf("Prioridad: %d\n", readyQueue.array[i].priority);
                printf("Burst: %d\n", readyQueue.array[i].burst);
                printf("Burst restante: %d\n\n", readyQueue.array[i].remaining);
            }
        } else{
            printf("Comando no reconocido \n");
        }
    }
    printf("Command listener done. \n");
    return NULL;
}