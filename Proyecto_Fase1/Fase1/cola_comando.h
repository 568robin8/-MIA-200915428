#ifndef COLA_COMANDO_H
#define COLA_COMANDO_H
#include <stdio.h>
#include <stdlib.h>

typedef struct nodo{
  struct nodo *sig;
  char comando[100];

}node;

typedef struct
{
    int Tamanho;
    node *inicio, *fin;
}Cola;


void CrearLista(Cola *c);
int Vacia(Cola *c);
void insertar_cola(Cola *c, char *Contenido);
char *consultar_cola(Cola *c);
void eliminarnodo_cola(Cola *c);
#endif

