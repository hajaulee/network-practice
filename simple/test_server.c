#include "clientServer.c"
#include <string.h>

void callback(char* request, char * response){
    strcpy(response, request);
}
int main(int argc, char const *argv[])
{
    createServer(&callback);
    return 0;
}
