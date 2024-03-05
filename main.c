//
// Created by cobalto on 10/10/23.
//
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>


pthread_mutex_t inj_eletro, display, ABS, V, Cinto, Choque, OverHeat;
bool pedal = false;
bool pedal_display = false;

bool freio = false;
bool freio_display = false;

bool vidro = false;
bool vidro_display = false;

bool cinto_seguranca = false;
bool cinto_display = false;

bool airbag = false;
bool airbag_display = false;

bool temp = false;
bool temp_display = false;

void ler_teclado();
void refresh_display();
void freios();
void motor();
void vidros();
void cinto();
void airbags();
void temperatura();

void main(){
    pthread_t t1, t2, t3, t4, t5, t6, t7, t8;

    pthread_create(&t1, NULL, (void *) ler_teclado, NULL);
    pthread_create(&t2, NULL, (void *) motor, NULL);
    pthread_create(&t3, NULL, (void *) freios, NULL);
    pthread_create(&t4, NULL, (void *) refresh_display, NULL);
    pthread_create(&t5, NULL, (void *) vidros, NULL);
    pthread_create(&t6, NULL, (void *) cinto, NULL);
    pthread_create(&t7, NULL, (void *) airbags, NULL);
    pthread_create(&t8, NULL, (void *) temperatura, NULL);

    pthread_join(t1, NULL);
    pthread_join(t8, NULL);
    pthread_join(t7, NULL);
    pthread_join(t6, NULL);
    pthread_join(t5, NULL);
    pthread_join(t4, NULL);
    pthread_join(t3, NULL);
    pthread_join(t2, NULL);

}
//thread que fiscaliza input
void ler_teclado() {
    int tecla;
    time_t user_input;
    while (1) {
        if (kbhit()) {
            tecla = getch();
            if (tecla == 'w'){// w minúsculo pisa no acelerador
                pthread_mutex_lock(&inj_eletro);
                pedal = (pedal)? false : true ;
                pthread_mutex_unlock(&inj_eletro);
            }
            if (tecla == 'f'){// f minúsculo aciona os freios
                pthread_mutex_lock(&ABS);
                freio = (freio)? false : true ;
                pthread_mutex_unlock(&ABS);
            }
            if (tecla == 'v'){// v minúsculo aciona os vidros
                pthread_mutex_lock(&V);
                vidro = (vidro)? false : true ;
                pthread_mutex_unlock(&V);
            }
            if (tecla == 'c'){// c minúsculo aciona os cintos
                pthread_mutex_lock(&Cinto);
                cinto_seguranca = (cinto_seguranca)? false : true ;
                pthread_mutex_unlock(&Cinto);
            }
            if (tecla == 'b'){// b minúsculo aciona os airbags
                pthread_mutex_lock(&Choque);
                airbag = (airbag)? false : true ;
                pthread_mutex_unlock(&Choque);
            }
            if (tecla == 't'){// t minúsculo aciona a verificação de temperatura
                pthread_mutex_lock(&OverHeat);
                temp = (temp)? false : true ;
                pthread_mutex_unlock(&OverHeat);
            }
            user_input = clock();
        }
    }
}
// thread para atualizar o display ao usuario
void refresh_display(){
    bool freio_ = false;
    char freios_ = 'f';

    bool pedal_ = false;
    char pedais_ = 'w';

    bool temp_ = false;
    char temps_ = 't';

    bool air_ = false;
    char airs_ = 'b';

    bool cinto_ = false;
    char cintos_ = 'c';

    bool vidro_ = false;
    char vidros_ = 'v';
    while(1) {
        pthread_mutex_lock(&display);
        freio_ = freio_display;
        pedal_ = pedal_display;
        temp_ = temp_display;
        air_ = airbag_display;
        cinto_ = cinto_display;
        vidro_ = vidro_display;
        pthread_mutex_unlock(&display);

        freios_ = (freio_)? 'F':'f';
        pedais_ = (pedal_)? 'W':'w';
        temps_ = (temp_)? 'T':'t';
        airs_ = (air_)? 'B':'b';
        cintos_ = (cinto_)? 'C':'c';
        vidros_ = (vidro_)? 'V':'v';

        printf("Pedal: %c  , Freios: %c  , Aviso de Temperatura: %c  , Airbag: %c  , Cintos: %c  , Vidros: %c\n", pedais_, freios_, temps_, airs_, cintos_, vidros_);
        sleep(1/2);
        system("cls");
    }

}

// thread para injeção eletronica
void motor(){
    time_t motor_end;
    bool estado;
    while(1){
        pthread_mutex_lock(&inj_eletro);
        estado = pedal;
        pthread_mutex_unlock(&inj_eletro);
        if (estado){
            pthread_mutex_lock(&display);
            pedal_display = true;
            pthread_mutex_unlock(&display);
            time(&motor_end);
        }else {
            pthread_mutex_lock(&display);
            pedal_display = false;
            pthread_mutex_unlock(&display);
        }
    }
}
// thread para o ABS
void freios(){
    time_t freio_end;
    bool estado;
    while(1){
        pthread_mutex_lock(&ABS);
        estado = freio;
        pthread_mutex_unlock(&ABS);
        if (estado){
            pthread_mutex_lock(&display);
            freio_display = true;
            pthread_mutex_unlock(&display);
            time(&freio_end);
        } else {
            pthread_mutex_lock(&display);
            freio_display = false;
            pthread_mutex_unlock(&display);
        }
    }
}
// thread para o cinto de segurança
void cinto(){
    time_t cinto_end;
    bool estado;
    while(1){
        pthread_mutex_lock(&Cinto);
        estado = cinto_seguranca;
        pthread_mutex_unlock(&Cinto);
        if (estado){
            pthread_mutex_lock(&display);
            cinto_display = true;
            pthread_mutex_unlock(&display);
            time(&cinto_end);
        }else {
            pthread_mutex_lock(&display);
            cinto_display = false;
            pthread_mutex_unlock(&display);
        }
    }
}
// thread para o acionamento dos vidros
void vidros(){
    time_t vidro_end;
    bool estado;
    while(1){
        pthread_mutex_lock(&V);
        estado = vidro;
        pthread_mutex_unlock(&V);
        if (estado){
            pthread_mutex_lock(&display);
            vidro_display = true;
            pthread_mutex_unlock(&display);
            time(&vidro_end);
        }else {
            pthread_mutex_lock(&display);
            vidro_display = false;
            pthread_mutex_unlock(&display);
        }
    }
}
// thread para o acionamento dos airbags
void airbags(){
    time_t air_end;
    bool estado;
    while(1){
        pthread_mutex_lock(&Choque);
        estado = airbag;
        pthread_mutex_unlock(&Choque);
        if (estado){
            pthread_mutex_lock(&display);
            airbag_display = true;
            pthread_mutex_unlock(&display);
            time(&air_end);
        }else {
            pthread_mutex_lock(&display);
            airbag_display = false;
            pthread_mutex_unlock(&display);
        }
    }
}

// thread para o acionamento dos airbags
void temperatura(){
    time_t temp_end;
    bool estado;
    while(1){
        pthread_mutex_lock(&OverHeat);
        estado = temp;
        pthread_mutex_unlock(&OverHeat);
        if (estado){
            pthread_mutex_lock(&display);
            temp_display = true;
            pthread_mutex_unlock(&display);
            time(&temp_end);
        }
    }
}