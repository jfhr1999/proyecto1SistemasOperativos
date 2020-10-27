#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#include <time.h>
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

//Para compilar -> gcc cliente.c -o cliente
//para ejecutar -> ./cliente   
int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; //para establecer el socket
    struct sockaddr_in serv_addr; 
    
    //variables para leer el archivo:
    FILE *f;
    char * line = NULL;
    size_t len = 0;
    ssize_t readLineTxt;

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
            while ((readLineTxt = getline(&line, &len, f)) != -1) {
                printf("%s\n", line);
            }

                //tambien iria un read(sock, pId, 1024); para recibir el pid e imprimirlo
            }
            fclose(f);*/
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

            printf("Burst %i\n",burst ); 
            printf("Priority %i\n",prior );

            //se recibe le pId
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

