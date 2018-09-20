#include "clientServer.c"
#include <string.h>

void callback(char* request, char * response){
    puts(response);
}
int main(int argc, char const *argv[])
{
    createClient(&callback);
    return 0;
}