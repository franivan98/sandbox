#include "server_facade.hpp"

#define PORT 8080

int main(){
    ServerFacade server(PORT);
    server.start();
    return 0;
}