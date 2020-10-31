#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

int main(int argc, char const *argv[]) 
{
    //variables para leer el archivo:
    FILE *f;
    char * line = NULL;
    size_t len = 0;
    ssize_t readLineTxt;

    f=fopen("process.txt","rt");
    while ((readLineTxt = getline(&line, &len, f)) != -1) {
        printf("%s\n", line);
    }

        //tambien iria un read(sock, pId, 1024); para recibir el pid e imprimirlo
    
    fclose(f);

    return 0; 
}