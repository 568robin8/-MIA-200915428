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

void inicializar_lista_mount(lista_mount* lista){
    lista->inicio = NULL;
    lista->fin = NULL;
    lista->tamanio = 0;
}

void insertar_lista(lista_mount *lista, char* path, char *name, char*id){
    node_mount *nuevo;
    nuevo = malloc(sizeof(node_mount));
    nuevo->sig = NULL;
    nuevo->ant = NULL;
    strcpy(nuevo->id_mount,"");
    strcat(nuevo->id_mount,id);
    strcpy(nuevo->name_mount,name);
    strcpy(nuevo->path_mount,path);

    node_mount *temp;
    temp = lista->inicio;
    if(temp == NULL){
        lista->inicio = nuevo;
        lista->fin = nuevo;
        lista->tamanio++;
    }else {
        while(temp->sig != NULL){
            temp= temp->sig;
        }

        temp->sig = nuevo;
        nuevo->ant = temp;
        lista->fin = nuevo;
        lista->tamanio++;
    }
}

void print_mounts(lista_mount *lista){
     if(lista->inicio!=NULL){
           node_mount *temp;
            temp = lista->inicio;

              while(temp!= NULL){
            printf("mounts : id:%s path: %s partition: %s\n",temp->id_mount, temp->path_mount, temp->name_mount);

            temp= temp->sig;
        }
    }else{
        printf("No hay particiones montadas\n");
    }
}

int consultar_existe(lista_mount *lista, char* id){
    int encontrado = 0;
    if(lista->inicio!=NULL){
           node_mount *temp;
            temp = lista->inicio;

              while(temp != NULL){
              if(strcmp(temp->id_mount,id)==0){
                encontrado = 1;
                break;
              }
            temp= temp->sig;
        }
    }
    return encontrado;
}

int consultar_path(lista_mount *lista, char *path){
   int encontrado = 0;
    if(lista->inicio!=NULL){
           node_mount *temp;
            temp = lista->inicio;

              while(temp != NULL){
              if(strcmp(temp->path_mount,path)==0){
                encontrado = 1;
                break;
              }
            temp= temp->sig;
        }
    }
    return encontrado;
}

int consultar_letramount(lista_mount *lista, char *letra){
      int encontrado = 0;
    if(lista->inicio!=NULL){
           node_mount *temp;
            temp = lista->inicio;

              while(temp != NULL){

              if(temp->id_mount[2]==letra[2]){
                encontrado = 1;
                break;
              }
            temp= temp->sig;
        }
    }
    return encontrado;
}



char *consultar_id(lista_mount *lista, char* id){
    char encontrado[200];
    if(lista->inicio!=NULL){
           node_mount *temp;
            temp = lista->inicio;

              while(temp != NULL){
              if(strcmp(temp->id_mount,id)==0){
                printf("PATH ANTES DE SER ENVIADO %s\n",temp->path_mount);
                strcpy(encontrado,temp->path_mount);
                return temp->path_mount;
                printf("PATH ENVIADO %s\n", encontrado);
                break;
              }
            temp= temp->sig;
        }
    }
    return encontrado;

}

void eliminar_lista(lista_mount *lista, char* id){

    node_mount *eliminar;
      if(lista->inicio!=NULL){
           node_mount *temp;
            temp = lista->inicio;

              while(temp != NULL){
              if(strcmp(temp->id_mount,id)==0){
                eliminar = temp;
                break;
              }
            temp= temp->sig;
        }
    }

    if(eliminar==lista->inicio){
        if(lista->tamanio==1){
            eliminar->ant = NULL;
            eliminar->sig = NULL;
            lista->inicio =NULL;
            lista->fin = NULL;
            free(eliminar);
            lista->tamanio--;
        }else{
            lista->inicio = eliminar->sig;
            lista->inicio->ant = NULL;
            free(eliminar);
            lista->tamanio--;
        }
    }else if (eliminar == lista->fin){
        lista->fin = eliminar->ant;
        lista->fin->sig = NULL;
        free(eliminar);
        lista->tamanio--;
    }else{
        eliminar->sig->ant = eliminar->ant;
        eliminar->ant->sig = eliminar->sig;
        free(eliminar);
        lista->tamanio--;
    }


}
/* REPOERTE MBR BETA

void Reporte_MBR(char *ruta_disco){
    FILE *Disco;

    if((Disco = fopen(ruta_disco,"rb"))){
        MBR mbr;
        rewind(Disco);
        fread(&mbr,sizeof(MBR),1,Disco);

        Cola cola;
        CrearLista(&cola);
        char _Ruta[250]="\0";
        sprintf(_Ruta,"%s",ruta_disco);
        ObtenerInstrucciones(&cola,_Ruta,"/");

        while(cola.Tamanho!=1){
            QuitarNodo(&cola);
        }

        FILE *Reporte;
        if((Reporte=fopen("mbr.dot","w")))
        {
            fprintf(Reporte,"digraph G{\nnode[shape=record];\nlabel=\"MBR del %s\"\n",Top(cola));
            fprintf(Reporte,"struct01 [label=\"{");
            fprintf(Reporte,"{Tamaño | %d} |",mbr.Tamanho);
            fprintf(Reporte,"{Fecha de Creacion | %s} |",mbr.Fecha);
            fprintf(Reporte,"{Firma | %d } |",mbr.Firma);
            int i;
            int p = 1;
            for(i=0;i<4;i++){
                if(mbr.particion[i].Estado=='1'){
                    fprintf(Reporte,"Particion %d |",p);
                    p++;
                    fprintf(Reporte,"{Estado | %c} |",mbr.particion[i].Estado);
                    fprintf(Reporte,"{Tipo | %c} |",mbr.particion[i].Tipo);
                    fprintf(Reporte,"{Ajuste | %c} |",mbr.particion[i].Ajuste);
                    fprintf(Reporte,"{Inicio | %d} |",mbr.particion[i].Inicio);
                    fprintf(Reporte,"{Tamaño | %d} |",mbr.particion[i].Tamanho);
                    fprintf(Reporte,"{Nombre | %s} |",mbr.particion[i].Nombre);
                }
            }
            fputs("}\"];\n",Reporte);
            fputs("}\n",Reporte);
            fflush(Reporte);
            fclose(Reporte);
            char instruccion[500]="\0";
            sprintf(instruccion,"dot -Tpdf mbr.dot -o %s",DISK_PATH);
            system(instruccion);
        }
        fclose(Disco);

    }

}
 */
