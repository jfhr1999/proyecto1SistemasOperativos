#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

#define PORT 8080 

//Para compilar -> gcc servidor.c -o servidor
//para ejecutar -> ./servidor   

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};

    //para enviar los pId... creo
    int pNumber = 0; 
    char numStr[50];

    //para elegir el algoritmo
    char * alg;
    int quantum;    
     
       
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

    printf("Ingrese el tipo de algoritmo que se va a utilizar:  FIFO(f), SJF(s), HPF(h), Round Robin(r)\n");
    scanf("%s", alg);
    if(!strcmp(alg,"r")){
        printf("Ingrese el quantum para el Round Robin\n");
        scanf("%d",&quantum);
        printf("El quantum es de %d\n",quantum);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }

    else{
        printf("Servidor se ha conextado\n");
    }
    printf("EL servidor esta ejecutando y recibio un cliente\n");
    while(1){
        
         
        // el servidor escucha lo que le manda el usuario
        
        memset(buffer, '\0', sizeof (buffer));
        valread = read( new_socket , buffer, 1024);
        if(!strcmp(buffer,"fin")){
            break;
        }
        printf("El cliente envio: %s\n",buffer ); 
        sprintf(numStr, "%i", pNumber);
        send(new_socket , numStr , strlen(numStr) , 0 ); 
        
    }
    return 0; 
} 