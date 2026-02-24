#include "server.hpp"

#define PORT 8080

int main(){
    Server server(PORT);
    server.start();
    return 0;
}