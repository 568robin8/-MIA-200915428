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
