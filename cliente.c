#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#include <time.h>
#define PORT 8080 

char* createProcess(int limiteIn, int limiteSup){
    srand(time(0));
    char* numStr;
    numStr = malloc(sizeof(char)*50);
    int num = (rand() % (limiteSup - limiteIn + 1)) + limiteIn; 
    sprintf(numStr, "%i", num);

    return numStr;
}

//Para compilar -> gcc cliente.c -o cliente
//para ejecutar -> ./cliente   
int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; //para establecer el socket
    struct sockaddr_in serv_addr; 
    
    char * msg;//mensaje que se le ve le va a enviar al servidor
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
    char* randomN = createProcess(1,6);
    while(1){
        char* randomN = createProcess(1,6);
        printf("Digite el mensaje: \n");
        scanf("%s", msg);
        if(!strcmp(msg,"fin")){//condicion de parada
            break;
        }
        send(sock , msg , strlen(msg) , 0 ); 
        printf("Process sent\n"); 
        valread = read( sock , buffer, 1024); 
        printf("Process ID %s\n",buffer );
        printf("Rand number %s\n",randomN ); 
    }
    return 0; 
} 

