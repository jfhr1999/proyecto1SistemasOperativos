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
#define PORT 8080 

int sock;
int valread;
int burstUpper, burstLower;

//funciones para crear procesos
int createBurst(int limiteIn, int limiteSup){
    int burst = rand() % (limiteSup - limiteIn + 1) + limiteIn; //crear un numero aleatorio entre 2 limites
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

void* sendToServer(void * arg);
void sendThread(char mode, int burst, int priority);

//Crea un thread, lo envia al server e imprime el pid que recibió como resultado

//Para compilar -> gcc cliente.c -o cliente -lpthread
//para ejecutar -> ./cliente   
int main(int argc, char const *argv[]) 
{ 
    //Bandera para ejecutar modo automatico
    bool run_auto = false;

    srand(time(NULL));

    //int sock = 0, valread; //para establecer el socket
    struct sockaddr_in serv_addr; 
    
    //variables para leer el archivo:
    FILE *f;
    char * line = NULL;
    size_t len = 0;
    ssize_t readLineTxt;

    //variables para establecer el tipo de ejecucion, manual o automatica
    char exe[50] = {0};

    //variables para el rango de valores del burst
    int tasa;

    
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    
    while(1){
        
        printf("Digite la forma de ejecucion: manual(m) o automatica(a) \n");
        printf("\n");
        scanf("%s", exe);
        if(!strcmp(exe,"fin")){//condicion de parada
            break;
        } else

        if(!strcmp(exe, "m")){
            send(sock , exe , strlen(exe) , 0 );
            //printf("Digite el nombre del archivo a leer: \n");//no se si hacerlo de forma constante arriba
            //scanf("%s",fileToRead);

            
            /*f=fopen(fileToRead,"rt");
            while ((readLineTxt = getline(&line, &len, f)) != -1) {
                printf("%s\n", line);
            }

                //tambien iria un read(sock, pId, 1024); para recibir el pid e imprimirlo
            }
            fclose(f);*/

            /*
            lineas = (array con cada linea del archivo???) o leer cada linea y hacer esto al leerla

            //Entonces un for aqui{

                sendThread("m", burst[i], prior[i], sock, valread);
                sleep(createRandSleep());
            }
            
            */


            valread = read( sock , buffer, 1024); //snipet para recibir algo por el socket
            printf("Process ID %s\n",buffer );
            printf("\n");
            printf("\n");
            
        } else

        if(!strcmp(exe, "a")){
            
            printf("Ingrese el minimo valor del rango del burst\n");
            scanf("%d",&burstLower);
            printf("Ingrese el maximo valor del rango del burst\n");
            scanf("%d",&burstUpper);

            printf("Ingrese la tasa de creacion\n"); //no se para que tho
            scanf("%d",&tasa);

            printf("Recibio valores");
            //se manda...
            int cont = 0;
            run_auto = true;
            while(run_auto){ //Como hago run_auto = false como usuario ?????????
                printf("%d", cont);
                int burst = 0;
                int prior = 0;
                
                sendThread('a', burst, prior); //a por modo automatico
                cont = cont + 1;
                //Si quiere enviar mas de un thread por segundo, usa la tasa
                //Si no, envia un thread por minuto
                if(tasa > 0){
                    sleep(60/tasa);
                } else {
                    sleep(60);
                }
            }
            

            //printf("Burst %i\n",burst ); 
            //printf("Priority %i\n",prior );

            //se recibe le pId
            //Necesito hacer esto en otra funcion, como hago??
            //valread = read( sock , buffer, 1024); //snipet para recibir algo por el socket
            //printf("Process ID %s\n",buffer );
            //printf("\n");
            //printf("\n");
            
        
        } else
        {
            printf("Comando no reconocido\n");
        }
        
        //send(sock , msg , strlen(msg) , 0 ); snippet para mandar algo por el socket 
        //valread = read( sock , buffer, 1024); //snipet para recibir algo por el socket
        //printf("Process ID %s\n",buffer );
         
    }
    return 0; 
} 

void* sendToServer(void * arg){
    //Guardo los valores que recibo como parametro
    int (*values)[3] = (int [3]) arg;
    int sleepNumber = *values[0];
    int burst = *values[1];
    int priority = *values[2];


    char buffer[1024] = {0};


    //Si es manual tiene un sleep antes de enviar la info, si es auto sleep es 0
    sleep(sleepNumber);


    char burstTxt[50];
    char priorityTxt[50];

    sprintf(burstTxt, "%d", burst);
    sprintf(priorityTxt, "%d", priority);

    strcat(burstTxt, ",");
    strcat(burstTxt, priorityTxt);

    printf("Burst en hilo: %d\n", burst);
    printf("Priority en hilo: %d\n", priority);

    //Enviar a servidor y recibir el numero del pid como respuesta
    send(sock , burstTxt , strlen(burstTxt) , 0 );
    valread = read( sock , buffer, 1024);

    //Retorna con un valor temporal para las pruebas, piensen que es el int que recibi
    printf("Respuesta de servidor, pid: %s\n", buffer);

    return NULL;

}

void sendThread(char mode, int burst, int priority){
    
    //Nuevo thread
    pthread_t newThread;
    //Para guardar el pid que recibe el thread como resultado
    //int *pid_result;

    
    //Meto los valores en un array para enviarlos al thread
    int * values[3];
    //Si es modo manual el sleep es 2, si es auto es 0
    if(mode == 'm'){
        values[0] = 2;
        values[1] = burst;
        values[2] = priority;
    } else {
        values[0] = 0;
        values[1] = createBurst(burstLower,burstUpper);
        values[2] = createPriority();
    }

    printf("Burst: %d\n", values[1]);
    printf("Priority: %d\n", values[2]);

    int valores = (values[1] * 10) + values[2];

    pthread_create(&newThread, NULL, sendToServer, &values);

    //Espera a que termine de correr el thread y recibe el resultado
    pthread_join(newThread, NULL);
    
}


