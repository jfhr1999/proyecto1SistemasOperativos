#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

struct Process {
    int pId;
    int burst;
    int priority;
    int remaining;
};

typedef struct {
    struct Process *array;
    size_t used;
    size_t size;
} Array;

void initArray(Array *a, size_t initialSize) {
    a->array = malloc(initialSize * sizeof(struct Process));
    a->used = 0;
    a->size = initialSize;
}

void insertProcess(Array *a, int id, int burst, int priority) {
    struct Process p;
    p.pId = id;
    p.burst = burst;
    p.priority = priority;
    p.remaining = p.burst;
    if (a->used == a->size) {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(struct Process));
    }
    a->array[a->used++] = p;
} 

void freeArray(Array *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void FIFO(Array *a) {
    for (int i = 0; i < a->used; i++) {
        printf("Proceso %d", a->array[i].pId);
        printf(" con burst %d", a->array[i].burst);
        printf(" y prioridad %d", a->array[i].priority);
        printf(" entra en ejecucion\n\n");
        //sleep del thread por a->array[i].burst segundos
        a->array[i].remaining = 0;
        printf("Proceso terminado:\n");
        printf("PID: %d\n", a->array[i].pId);
        printf("Burst: %d\n", a->array[i].burst);
        printf("Prioridad: %d\n\n", a->array[i].priority);
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
            printf(" entra en ejecucion\n\n");
            //sleep por a->array[index].remaining segundos
            printf("Proceso terminado:\n");
            printf("PID: %d\n", a->array[index].pId);
            printf("Burst: %d\n", a->array[index].burst);
            printf("Prioridad: %d\n\n", a->array[index].priority);
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
        }else{
            printf("Proceso %d", a->array[index].pId);
            printf(" con burst %d", a->array[index].burst);
            printf(" y prioridad %d", a->array[index].priority);
            printf(" entra en ejecucion\n\n");
            //sleep por quantum segundos
            a->array[index].remaining = a->array[index].remaining - quantum;
            index++;
            index = index % a->used; 
        }
    }
}

//[1,2,5,4,3]

int main(){
    Array readyQueue;
    
    initArray(&readyQueue, 5);

    //-------------------------------------------------------Thread job scheduler
    insertProcess(&readyQueue, 0, 6, 2); //0-5
    insertProcess(&readyQueue, 1, 2, 3); //1-2
    insertProcess(&readyQueue, 2, 1, 2); //2-1
    insertProcess(&readyQueue, 3, 3, 3); //3-3
    insertProcess(&readyQueue, 4, 4, 2); //4-4
    //-------------------------------------------------------------------------

    
    //-------------------------------------------------------------Thread CPU scheduler

    //FIFO(&readyQueue);
    //SJF(&readyQueue);
    //HPF(&readyQueue);
    RR(&readyQueue, 2);

    //----------------------------------------------------------------------------------

    //---------------------------------------------------------------Thread de comandos
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

    freeArray(&readyQueue);

    return 0;
}