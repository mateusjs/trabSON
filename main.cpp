#include <vector>
#include <iostream>
#include <mutex>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <list>

typedef std::vector<int> Buffer;


using namespace std;

sem_t cozinha;
sem_t buffet;
int countCozinha;
int countBuffet;

class Cozinha{ //buffer
public:
    //in e tamanho de buffer por parametro
    Cozinha();
    Cozinha(int tam_bufferCozinha);
    void produzCozinha();
    void consomeCozinha();
    Buffer bufferCozinha;
    int tamBufferCozinha;
    int in;
    int out;
//    volatile int countCozinha;
};

class Buffet{ //Buffer
public:
    Buffet();
    Buffet(int tamBufferBuffet);
    void produzBuffet();
    void consomeBuffet();
    Buffer bufferBuffet;
    int tamBufferBuffet;
    int in;
    int out;
//    int countBuffet;
};

class Cozinheiro{ //Produtor do buffer cozinha
public:
    Cozinheiro(){};
    Cozinha a;
    Cozinheiro(Cozinha a);
    static void * cozinhando(void *pVoid);
    void * testing(void *pVoid);
};

class Clientes{ //Consumidor do buffer buffet
public:
    Clientes(){};
    Buffet a;
    Clientes(Buffet a);
    static void * servindo(void *pVoid);
    void * chamaServindo(void *pVoid);
};

class Garcons{ //Consumidor do buffer cozinha e produtor do buffer buffet
public:
    Garcons(){};
    Cozinha a;
    Buffet b;
    Garcons(Cozinha a, Buffet b);
    static void *repondo( void *pVoid);
    void *teste(void *pVoid);
};


// ------------------------------------- METODOS-----------------------------
// ------------------------------------- METODOS DO CLASSE COZINHA-----------------------------
Cozinha::Cozinha(){}

Cozinha::Cozinha(int tamBufferCozinha) {
    this->tamBufferCozinha = tamBufferCozinha;
    countCozinha = 0;
    in = 0;
    out = 0;
    bufferCozinha = Buffer(tamBufferCozinha);
}

void Cozinha::produzCozinha() {
    while(countCozinha == tamBufferCozinha);

    bufferCozinha[in] = 1;
    in = (in + 1) % tamBufferCozinha;
    int sem_wait(sem_t &cozinha);
    countCozinha++;
    cout<< "Poduziu Cozinha" << countCozinha << this << endl;
    int sem_post(sem_t &cozinha);
}

void Cozinha::consomeCozinha() {
    int retira_bandeja = 0;
    while(countCozinha == 0) {
        cout << countCozinha <<  this;  sleep(1);
        fflush(stdout);
    }

    retira_bandeja = bufferCozinha[out];
    out = (out + 1) % tamBufferCozinha;
    int sem_wait(sem_t &cozinha);
    countCozinha--;
    cout<< "Consumiu Cozinha" << endl;
    int sem_post(sem_t &cozinha);

}

// ------------------------------------- METODOS DO CLASSE BUFFET-----------------------------
Buffet::Buffet() {}

Buffet::Buffet(int tamBufferBuffet) {
    this->tamBufferBuffet = tamBufferBuffet;
    countBuffet = 0;
    in = 0;
    out = 0;
    bufferBuffet = Buffer(tamBufferBuffet);
}

void Buffet::produzBuffet() {
    while(countBuffet == tamBufferBuffet);

    bufferBuffet[in] = 1;
    in = (in + 1) % tamBufferBuffet;
    int sem_wait(sem_t &buffet);
    countBuffet++;
    cout<< "Produziu Buffet" << endl;
    int sem_post(sem_t &buffet);
}

void  Buffet::consomeBuffet() {
    int retira_bandeja = 0;
    while(countBuffet == 0);

    retira_bandeja = bufferBuffet[out];
    out = (out + 1) % tamBufferBuffet;
    int sem_wait(sem_t &buffet);
    countBuffet--;
    cout<< "Consumiu Buffet" << endl;
    int sem_post(sem_t &buffet);

}

// ------------------------------------- METODOS DO CLASSE COZINHEIROS-----------------------------

Cozinheiro::Cozinheiro(Cozinha a) {
    this->a = a;
}

void * Cozinheiro::cozinhando(void *pVoid) {
    reinterpret_cast<Cozinheiro*>(pVoid)->testing(nullptr);
}

void * Cozinheiro::testing(void *pVoid){
    for(int i = 0 ; i < 10 ; i++){
        a.produzCozinha();
        cout << "Cozinheiro cozinhou" << endl;
    }
}

// ------------------------------------- METODOS DO CLASSE CLIENTES-----------------------------

Clientes::Clientes(Buffet a){
    this->a = a;
}

void * Clientes::servindo(void *pVoid){
    reinterpret_cast<Clientes*>(pVoid)->chamaServindo(nullptr);
}

void * Clientes::chamaServindo(void *pVoid){
    for(int i = 0 ; i < 10 ; i++){
        a.consomeBuffet();
        cout << "Cliente se serviu!" << endl;
    }
}

// ------------------------------------- METODOS DO CLASSE GARCONS-----------------------------

Garcons::Garcons(Cozinha a, Buffet b){
    this->a = a;
    this->b = b;
}

void * Garcons::repondo(void * pVoid){
    reinterpret_cast<Garcons*>(pVoid)->teste(nullptr);
}

void * Garcons::teste(void *pVoid){
    for(int i = 0 ; i < 10; i++){
        a.consomeCozinha();
        cout << "Garcon pegou da cozinha" << endl;
        b.produzBuffet();
        cout << "Garcon repos no buffet" << endl;
    }
}

// ------------------------------------- Fim dos metodos-----------------------------
int main() {
    vector<Cozinheiro> chefs;
    vector<Garcons> campeoes;
    vector<Clientes> donoDaRazao;

    int tam_bufferCozinha = 5;
    int tam_bufferBuffet = 5;

    Cozinha a = Cozinha(tam_bufferCozinha);
    Buffet b = Buffet(tam_bufferBuffet);

    pthread_t cozinheiros[3], garcons[3], clientes[3]; //nome das threads

    sem_init(&cozinha, 0, tam_bufferCozinha); //iniciaçao das threads
    sem_init(&buffet, 0, tam_bufferBuffet);


    for(int i = 0; i < 10; i++) { //insere o buffer no vetor
        chefs.push_back(Cozinheiro(a));
        campeoes.push_back(Garcons(a,b));
        donoDaRazao.push_back(Clientes(b));
    }

    for(int i = 0; i < 2 ; i++){ //cria as threadas nos metodos escolhidos
        pthread_create(&cozinheiros[i], nullptr, &Cozinheiro::cozinhando, &chefs[i]);
        pthread_create(&garcons[i], nullptr, &Garcons::repondo, &campeoes[i]);
        pthread_create(&clientes[i], nullptr, &Clientes::servindo, &donoDaRazao[i]);

    }

    for(int i=0 ;i < 2;i++) { // espera as threads terminarem a execuçao
        pthread_join(cozinheiros[i], nullptr);
        pthread_join(garcons[i], nullptr);
        pthread_join(clientes[i], nullptr);
    }
    return 0;
}
