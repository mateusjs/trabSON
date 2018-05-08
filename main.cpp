#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

typedef std::vector<int> Buffer;

using namespace std;

int countCozinha = 0;
int countBuffet = 0;

class Cozinha{
public:
    Cozinha(); //in e tamanho de buffer por parametro
    int getBandeija();
    void setBandeija(int bandeja);
    Buffer bufferCozinha;
    int tamBufferCozinha;
    int in;
}cozinha;

class Cozinheiros{
    public:
        Cozinheiros();
        void acessaBufferCozinha();
        void produzCozinha(Cozinha cozinha);

}cozinheiro;

class Garcons{
    public:
        Garcons();
    private:
        void acessaBufferCozinha();
        void consomeCozinha();
        void acessaBufferBuffet();
        void produzBuffet();
}garcon;

class Buffet{
    public:
    Buffet();
    int getBandeja();
    void setBandeja(int bandeja);
    Buffer bufferBuffet;
    int tamBufferBuffet;
    int out;
};

class Clientes{
    public:
        Clientes();
        void acessaBufferBuffet();
        void consomeBuffet(Buffet buffet);
}cliente;

void Cozinheiros::produzCozinha(Cozinha cozinha) {
    while(countCozinha == cozinha.tamBufferCozinha);

    cozinha.bufferCozinha[cozinha.in] = 1;
    cozinha.in = (cozinha.in + 1) % cozinha.tamBufferCozinha;
    countCozinha++;

}

void Garcons::consomeCozinha() {

}

void Garcons::acessaBufferBuffet() {

}

void Garcons::produzBuffet() {

}

void Clientes::acessaBufferBuffet() {

}

void  Clientes::consomeBuffet(Buffet buffet) {
    while(countBuffet == 0);

    buffet.bufferBuffet[buffet.out] = 0;
    buffet.out = (buffet.out + 1) % buffet.tamBufferBuffet;
    countBuffet--;

}

int main() {

   return 0;
}