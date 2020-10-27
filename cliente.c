#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#define PORT 8080 


//funciones para crear procesos
int createBurst(int limiteIn, int limiteSup){
    srand(time(0));
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

void* sendToServer(void * arg){
    //Guardo los valores que recibo como parametro
    int (*values)[3] = (int (*) [3]) arg;
    int sleepNumber = *values[0];
    int burst = *values[1];
    int priority = *values[2];

    //Hago espacio para el int que voy a recibir
    int *resPid = (int *)malloc(sizeof(int));

    //Si es manual tiene un sleep antes de enviar la info, si es auto sleep es 0
    sleep(sleepNumber);

    //Enviar a servidor y recibir el numero del pid como respuesta
    //PENDING

    //Retorna con un valor temporal para las pruebas, piensen que es el int que recibi
    *resPid = 2;
    return resPid;

}

//Crea un thread, lo envia al server e imprime el pid que recibió como resultado
void sendThread(char mode, int burst, int priority){
    
    //Nuevo thread
    pthread_t newThread;
    //Para guardar el pid que recibe el thread como resultado
    int *pid_result;

    
    //Meto los valores en un array para enviarlos al thread
    int values[3];
    //Si es modo manual el sleep es 2, si es auto es 0
    if(strcmp(mode, "m") == 0){
        values[0] = 2;
    } else {
        values[0] = 0;
    }
    values[1] = burst;
    values[2] = priority;

    pthread_create(&newThread, NULL, sendToServer, &values);

    //Espera a que termine de correr el thread y recibe el resultado
    pthread_join(newThread, (void*)&pid_result);
    printf("Respuesta de servidor, pid: %d\n", *pid_result);
}

//Para compilar -> gcc cliente.c -o cliente
//para ejecutar -> ./cliente   
int main(int argc, char const *argv[]) 
{ 
    //Bandera para ejecutar modo automatico
    bool run_auto = false;

    int sock = 0, valread; //para establecer el socket
    struct sockaddr_in serv_addr; 
    
    //variables para leer el archivo:
    FILE *f;
    int c;
    unsigned char symb;
    char * fileToRead;

    //variables para establecer el tipo de ejecucion, manual o automatica
    char *exe;

    //variables para el rango de valores del burst
    int burstUpper, burstLower, tasa;

    
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
        }

        if(!strcmp(exe,"m")){
            send(sock , exe , strlen(exe) , 0 );
            //printf("Digite el nombre del archivo a leer: \n");//no se si hacerlo de forma constante arriba
            //scanf("%s",fileToRead);

            
            /*f=fopen(fileToRead,"rt");
            while((c=fgetc(f))!=EOF){
                //por aqui van los threads para enviar los datos pero igual
                //hay que hacer un brete para que ver como mandamos los valores
                symb= (unsigned char) c;  
                if(symb >= '0' && symb <='9')
                    printf("%c",symb);

                //tambien iria un read(sock, pId, 1024); para recibir el pid e imprimirlo
            }
            fclose(f);*/

            /*
            lineas = (array con cada linea del archivo???) o leer cada linea y hacer esto al leerla

            //Entonces un for aqui{

                sendThread("m", burst[i], prior[i]);
                sleep(createRandSleep());
            }
            
            */


            valread = read( sock , buffer, 1024); //snipet para recibir algo por el socket
            printf("Process ID %s\n",buffer );
            printf("\n");
            printf("\n");
            
        }

        if(!strcmp(exe,"a")){
            send(sock , exe , strlen(exe) , 0 );
            printf("Ingrese el minimo valor del rango del burst\n");
            scanf("%d",&burstLower);
            printf("Ingrese el maximo valor del rango del burst\n");
            scanf("%d",&burstUpper);

            printf("Ingrese la tasa de creacion\n"); //no se para que tho
            scanf("%d",&tasa);

            int burst = createBurst(burstLower,burstUpper);
            int prior = createPriority();

            //se manda...
            run_auto = true;
            while(run_auto){ //Como hago run_auto = false como usuario ?????????
                sendThread("a", burst, prior); //a por modo automatico
                
                //Si quiere enviar mas de un thread por segundo, usa la tasa
                //Si no, envia un thread por minuto
                if(tasa > 0){
                    sleep(60/tasa);
                } else {
                    sleep(60);
                }
            }
            

            printf("Burst %i\n",burst ); 
            printf("Priority %i\n",prior );

            //se recibe le pId
            //Necesito hacer esto en otra funcion, como hago??
            valread = read( sock , buffer, 1024); //snipet para recibir algo por el socket
            printf("Process ID %s\n",buffer );
            printf("\n");
            printf("\n");
            

        }
        //send(sock , msg , strlen(msg) , 0 ); snippet para mandar algo por el socket 
        //valread = read( sock , buffer, 1024); //snipet para recibir algo por el socket
        //printf("Process ID %s\n",buffer );
         
    }
    return 0; 
} 

