#import "cola_comando.h"

void CrearLista(Cola *c){
c->fin = NULL;
c->inicio=NULL;
c->Tamanho = 0;
}

int Vacia(Cola *c)
{
    return c->inicio==NULL;
}

void insertar_cola(Cola *c, char *Contenido)
{
    node *nuevo;
    nuevo = malloc(sizeof(node));
    nuevo->sig = NULL;

    strcpy(nuevo->comando,Contenido);
    if(Vacia(c)){
        c->inicio = nuevo;
        c->fin = nuevo;
    }else{
        c->fin->sig = nuevo;
        c->fin = nuevo;
    }
    c->Tamanho++;
}

char *consultar_cola(Cola *c){
    return c->inicio->comando;
}

void eliminarnodo_cola(Cola *c){
    node *temporal;

    temporal = c->inicio;

    if(!Vacia(c)){
        if(c->Tamanho ==1){
        c->inicio = NULL;
        c->fin = NULL;
        free(temporal);
        }else{
        c->inicio = c->inicio->sig;
        free(temporal);
        }
    }
}
