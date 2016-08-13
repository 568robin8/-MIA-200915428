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

typedef struct nodo_mount{
    struct nodo_mount *sig;
    struct nodo_mount *ant;
    char path_mount[200];
    char name_mount[16];
    char id_mount[4];
}node_mount;

typedef struct{
    node_mount *inicio, *fin;
    int tamanio;
}lista_mount;



void CrearLista(Cola *c);
int Vacia(Cola *c);
void insertar_cola(Cola *c, char *Contenido);
char *consultar_cola(Cola *c);
void eliminarnodo_cola(Cola *c);

void inicializar_lista_mount(lista_mount* lista);
void insertar_lista(lista_mount *lista, char* path, char *name, char*id);
int consultar_existe(lista_mount *lista, char* id);

char *consultar_id(lista_mount *lista, char* id);
int consultar_letramount(lista_mount *lista, char *letra);
int consultar_path(lista_mount *lista, char *path);
void eliminar_lista(lista_mount *lista, char* id);
void print_mounts(lista_mount *lista);
#endif
