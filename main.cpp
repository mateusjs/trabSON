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
    int countCozinha;
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
    int countBuffet;
};

class Cozinheiro{ //Produtor do buffer cozinha
public:
    Cozinheiro(){};
    Cozinha a;
    Cozinheiro(Cozinha a);
    void * cozinhando(void *pVoid);
    string panela;

};

class Clientes{ //Consumidor do buffer buffet
public:
    Clientes(){};
    Buffet a;
    Clientes(Buffet a);
    void servindo();
};

class Garcons{ //Consumidor do buffer cozinha e produtor do buffer buffet
public:
    Garcons(){};
    Cozinha a;
    Buffet b;
    Garcons(Cozinha a, Buffet b);
    void repondo(Cozinha a, Buffet b);

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
    cout<< "Poduziu Cozinha" << endl;
    int sem_post(sem_t &cozinha);
}

void Cozinha::consomeCozinha() {
    int retira_bandeja = 0;
    while(countCozinha == 0);

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
    panela = "tramontina";
}

void * Cozinheiro::cozinhando(void *pVoid) {
    for(int i = 0 ; i < 10 ; i++){
        a.produzCozinha();
        cout << "Cozinheiro cozinhou" << endl;
    }
}

// ------------------------------------- METODOS DO CLASSE CLIENTES-----------------------------

Clientes::Clientes(Buffet a){
    this->a = a;
}

void Clientes::servindo(){
    for(int i = 0 ; i < 10 ; i++){
        a.consomeBuffet();
        cout << "Cliente serviu" << endl;
    }
}

// ------------------------------------- METODOS DO CLASSE GARCONS-----------------------------

Garcons::Garcons(Cozinha a, Buffet b){
    this->a = a;
    this->b = b;
}

void Garcons::repondo(Cozinha a, Buffet b){
    for(int i = 0 ; i < 10; i++){
        a.consomeCozinha();
        cout << "Garcon pegou da cozinha" << endl;
        b.produzBuffet();
        cout << "Garcon repos no buffet" << endl;
    }
}

void * teste(void *pVoid){
    cout << "XABLAU";
}

// ------------------------------------- Fim dos metodos-----------------------------
int main() {
    vector<Cozinheiro> chefs;
    vector<Garcons> campeoes;
    vector<Clientes> usuarios;



    int tam_bufferCozinha = 5;
    int tam_bufferBuffet = 5;
//
//    cout << "Tamanho do buffer Cozinha: ";
//    cin >> tam_bufferCozinha << endl;
//
//    cout << "Tamanho do buffer Buffet: ";
//    cin >> tam_bufferBuffet << endl;

    Cozinha a = Cozinha(tam_bufferCozinha);
    Buffet b = Buffet(tam_bufferBuffet);

    pthread_t cozinheiros[3], garcons[3], clientes[3];

    sem_init(&cozinha, 0, tam_bufferCozinha);
    sem_init(&buffet, 0, tam_bufferBuffet);


    for(int i = 0; i < 10; i++) {
        chefs.push_back(Cozinheiro(a));
        cout<<chefs[i].panela;
    }

    pthread_create(&cozinheiros[0], NULL,breno.cozinhando , NULL);
    pthread_create(&cozinheiros[1], NULL,chico.cozinhando , NULL);
    pthread_create(&cozinheiros[2], NULL,morilu.cozinhando , NULL);


    //}
//    for(int i=0;i < 3;i++)
//        pthread_join(cozinheiros[i], NULL);

//    for(int i = 0; i < 10; i++)
//        pthread_create(&garcons, NULL, (void)repondo , NULL);



    return 0;
}
