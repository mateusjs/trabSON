#include <vector>
#include <iostream>
#include <mutex>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <list>
#include <time.h>

typedef std::vector<int> Buffer;


using namespace std;

sem_t cozinha;
sem_t buffet;
sem_t fullCozinha;
sem_t fullBuffet;
sem_t emptyCozinha;
sem_t emptyBuffet;



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
    int countBuffet;
};

class Cozinheiro{ //Produtor do buffer cozinha
public:
    Cozinheiro(){};
    Cozinha *a;
    Cozinheiro(Cozinha *a);
    static void * cozinhando(void *pVoid);
    void * testing(void *pVoid);
};

class Clientes{ //Consumidor do buffer buffet
public:
    Clientes(){};
    Buffet *a;
    Clientes(Buffet *a);
    static void * servindo(void *pVoid);
    void * chamaServindo(void *pVoid);
};

class Garcons{ //Consumidor do buffer cozinha e produtor do buffer buffet
public:
    Garcons(){};
    Cozinha *a;
    Buffet *b;
    Garcons(Cozinha *a, Buffet *b);
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
	for(int i = 0; i < tamBufferCozinha; i++)
		bufferCozinha[i]=0;
}

void Cozinha::produzCozinha() {
   
	int sem_wait(sem_t &emptyCozinha);
	int sem_wait(sem_t &cozinha);
		//cout << "\nproduz em " << in << endl;
		//for(int i = 0; i < tamBufferCozinha; i++){
			//cout<<" "<< bufferCozinha[i];
		//}
		//cout << endl;
		sleep(3);
		bufferCozinha[in] = 1;
		in = (in + 1) % tamBufferCozinha;
    int sem_post(sem_t &cozinha);
	int sem_post(sem_t &fullCozinha);
}

void Cozinha::consomeCozinha() {
    int retira_bandeja = 0;
    int sem_wait(sem_t &fullCozinha);
    int sem_wait(sem_t &cozinha);

		//cout << "\nconsome em " << out << endl;
		//for(int i = 0; i < tamBufferCozinha; i++){
			//cout<<" "<< bufferCozinha[i];
		//}
		//cout << endl;
		sleep(1);
		retira_bandeja = bufferCozinha[out];
		out = (out + 1) % tamBufferCozinha;

    int sem_post(sem_t &cozinha);	
    int sem_post(sem_t &emptyCozinha);

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
	int sem_wait(sem_t &emptyBuffet);
	int sem_wait(sem_t &buffet);
		sleep(1);	
		bufferBuffet[in] = 1;
		in = (in + 1) % tamBufferBuffet;

    int sem_post(sem_t &buffet);
	int sem_post(sem_t &fullBuffet);
}

void  Buffet::consomeBuffet() {
    int retira_bandeja = 0;

    int sem_wait(sem_t &fullBuffet);
    int sem_wait(sem_t &buffet);
		sleep(3);
		retira_bandeja = bufferBuffet[out];
		out = (out + 1) % tamBufferBuffet;
    int sem_post(sem_t &buffet);	
    int sem_post(sem_t &emptyBuffet);

}

// ------------------------------------- METODOS DO CLASSE COZINHEIROS-----------------------------

Cozinheiro::Cozinheiro(Cozinha *a) {
    this->a = a;
}

void * Cozinheiro::cozinhando(void *pVoid) {
    reinterpret_cast<Cozinheiro*>(pVoid)->testing(nullptr);
}

void * Cozinheiro::testing(void *pVoid){
    for(int i = 0 ; i < 4; i++){
        a->produzCozinha();
        //cout << "\n" << "Cozinheiro cozinhou" << endl;
    }
}

// ------------------------------------- METODOS DO CLASSE CLIENTES-----------------------------

Clientes::Clientes(Buffet *a){
    this->a = a;
}

void * Clientes::servindo(void *pVoid){
    reinterpret_cast<Clientes*>(pVoid)->chamaServindo(nullptr);
}

void * Clientes::chamaServindo(void *pVoid){
    for(int i = 0 ; i < 4 ; i++){
        a->consomeBuffet();
        //cout << "Cliente se serviu!" << endl;
    }
}

// ------------------------------------- METODOS DO CLASSE GARCONS-----------------------------

Garcons::Garcons(Cozinha *a, Buffet *b){
    this->a = a;
    this->b = b;
}

void * Garcons::repondo(void * pVoid){
    reinterpret_cast<Garcons*>(pVoid)->teste(nullptr);
}

void * Garcons::teste(void *pVoid){
    for(int i = 0 ; i < 4; i++){
        a->consomeCozinha();
        //cout << "Garcon pegou da cozinha" << endl;
        b->produzBuffet();
        //cout << "Garcon repos no buffet" << endl;
    }
}

// ------------------------------------- Fim dos metodos-----------------------------
	// Coversão de nanosegundo para segundo



double timespecToSec(struct timespec *time){
   	return (double)(time->tv_sec * 1000000000 + time->tv_nsec)/1000000000.0;
}

int main() {
    vector<Cozinheiro> chefs;
    vector<Garcons> campeoes;
    vector<Clientes> donoDaRazao;

    int tam_bufferCozinha = 1;
    int tam_bufferBuffet = 1;
	float tempo	= 0.0;

	struct timespec startCPU, endCPU;
	struct timespec startWALL, endWALL;
	double timeCPU, timeWALL, mbs;

	clock_gettime(CLOCK_MONOTONIC,&startWALL); // relógio de "parede"
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&startCPU); // relógio de CPU

    Cozinha a(tam_bufferCozinha);
    Buffet b(tam_bufferBuffet);

    pthread_t cozinheiros[50], garcons[50], clientes[50]; //nome das threads

    sem_init(&cozinha, 0, 1); //semaforos
    sem_init(&buffet, 0, 1);

    sem_init(&fullCozinha, 0, 0); //semaforos
    sem_init(&fullBuffet, 0, 0);

    sem_init(&emptyCozinha, 0, tam_bufferCozinha); //semaforos
    sem_init(&emptyBuffet, 0, tam_bufferBuffet);



    for(int i = 0; i < 50; i++) { //insere o buffer no vetor
        chefs.push_back(Cozinheiro(&a));
        campeoes.push_back(Garcons(&a,&b));
        donoDaRazao.push_back(Clientes(&b));
    }

	clock_gettime(CLOCK_MONOTONIC,&startWALL); // relógio de "parede"
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&startCPU); // relógio de CPU

    for(int i = 0; i < 25 ; i++) //cria as threadas nos metodos escolhidos
        pthread_create(&cozinheiros[i], nullptr, &Cozinheiro::cozinhando, &chefs[i]);
 
	for(int i = 0; i < 25 ; i++)
    	pthread_create(&garcons[i], nullptr, &Garcons::repondo, &campeoes[i]);

	for(int i = 0; i < 25 ; i++)
		pthread_create(&clientes[i], nullptr, &Clientes::servindo, &donoDaRazao[i]);

    //JOINS --------------------------------------------------------------------------------

    for(int i=0 ; i < 25;i++) // espera as threads terminarem a execuçao
        pthread_join(cozinheiros[i], nullptr);

    for(int i = 0; i < 25; i++)
        pthread_join(garcons[i], nullptr);

    for(int i = 0; i < 25; i++)
        pthread_join(clientes[i], nullptr);
    

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&endCPU); // relógio de CPU
    clock_gettime(CLOCK_MONOTONIC,&endWALL); // relógio de "parede"

    timeCPU  = timespecToSec(&endCPU)  - timespecToSec(&startCPU) ;
    timeWALL = timespecToSec(&endWALL) - timespecToSec(&startWALL);

	cout<< timeCPU <<", "<< timeWALL<<endl;
	
    return 0;
}
