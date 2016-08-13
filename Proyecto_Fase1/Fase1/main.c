#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "cola_comando.h"

    int name_ = 0;
    int unit_ = 0;
    int path_ = 0;
    int size_ = 0;

int correlativo = 0;
int size = 0;
int sizep= 0;
int valoradd = 0;
int type_ = 0;
int fit_ajuste = 0;
int name_f = 0;
int unit_f = 0;
int path_f = 0;
int size_f = 0;
int delete_ = 0;
int add_ = 0;
int name_m = 0;
int path_m = 0;
int id_r = 0;
int name_r = 0;
int path_r = 0;
int id_u= 0;

char unit[2] = "\0";
char unitp[2] = "\0";
char type[2] = "\0";

lista_mount *nuevalista;


typedef struct{
    char status[10];
    char part_type[2];
    char part_fit[3];
    int part_start;
    int part_size;
    char part_name[16];
}particion;

typedef struct{
    int mbr_tamanio;
    char mbr_fecha_creacion[200];
    int mbr_disk_asignature;
    particion mbr_partition1;
    particion mbr_partition2;
    particion mbr_partition3;
    particion mbr_partition4;

}mbr;

typedef struct{
    char part_status[20];
    char part_fit[4];
    int part_start;
    int part_size;
    int part_next;
    char part_name[16];
}ebr;

void inicio();
void verificar_comandos(char *line_instruction);
void mkdisk(Cola *instruccion, char *copia_linea);
void creardisco(char* direccion, int size);
void fkdisk(Cola *nueva, char *copia_linea);
void crearparticion(int size_partition, char *nombre_partition, char *path_validado, char* type_partition, char* ajuste_partition);
void mount(Cola *nueva,char *copia_linea);
void montar_particion(char *path_mount, char *name_particion);
void umount(Cola *nueva, char* copia_linea);
void rmdisk(Cola *nueva,char* copia_linea);

void rep(Cola *nueva,char *copia_linea);
void reporte_mbr(char *path_rep,char *id, char*pathdot);
void reporte_disk(char *path_rep,char *id, char*pathdot);

void leersh(Cola *nueva, char *copia_linea);

int main()
{
nuevalista = malloc(sizeof(lista_mount));
inicializar_lista_mount(nuevalista);
  //system("dot '/home/rob/chute a/mbr.dot' -o '/home/rob/chute a/image3.jpg' -Tjpg");


 inicio();

    return 0;
}

void inicio(){
 char line_instruction[1000];

    printf("\nProyecto[MIA]@200915428:> ");
    scanf(" %[^\n]s",line_instruction);
    verificar_comandos(line_instruction);

   inicio();

}


void verificar_comandos(char *line_instruction){


   sizep = 0;
    strcpy(unitp," ");
  // unitp[2] = "\0";
  strcpy(type," ");
    //type[2] = "\0";
    char copia_linea[300];
    strcpy(copia_linea,line_instruction);
   int a;
   for (a=0; line_instruction[a]; a++){
    line_instruction[a] = tolower(line_instruction[a]);
   }




    Cola *nueva;
    nueva = malloc(sizeof(Cola));
    CrearLista(nueva);

    char *first_word =strtok(line_instruction," ");

    while ( first_word != NULL ) {

        insertar_cola(nueva,first_word);
        first_word=strtok(NULL," ");
    }



   if(strcmp(consultar_cola(nueva),"mkdisk")==0){
        eliminarnodo_cola(nueva);
          mkdisk(nueva,copia_linea);
    }else if(strcmp(consultar_cola(nueva),"fdisk")==0){
        eliminarnodo_cola(nueva);
        fkdisk(nueva,copia_linea);
        // crear particiones
    }else if(strcmp(consultar_cola(nueva),"mount")==0){
        eliminarnodo_cola(nueva);
        if(Vacia(nueva)){
        printf("Las particiones montadas son: \n");
        //llamar metodo q las imprima
        print_mounts(nuevalista);
        }else{
        mount(nueva,copia_linea);
        }

    }else if(strcmp(consultar_cola(nueva),"rmdisk")==0){
        eliminarnodo_cola(nueva);
        rmdisk(nueva,copia_linea);
    }else if(strcmp(consultar_cola(nueva),"umount")==0){
        eliminarnodo_cola(nueva);
        umount(nueva,copia_linea);
    }else if(strcmp(consultar_cola(nueva),"rep")==0){
        eliminarnodo_cola(nueva);
        rep(nueva,copia_linea);

    }else if(strcmp(consultar_cola(nueva),"exec")==0){
        eliminarnodo_cola(nueva);
        leersh(nueva,copia_linea);
    }else if(strcmp(consultar_cola(nueva),"exit")==0){
        eliminarnodo_cola(nueva);
        exit(0);
    }else{
        eliminarnodo_cola(nueva);
        printf("comando no valido \n");
    }



}


void mkdisk(Cola * instrucciones,char *copia_linea){
    char name[16] ="\0";
    char nameaux[16] ="\0";
    char path[200] = "\0";
    strcpy(unit," ");
    size = 0;


    name_ = 0;
    unit_ = 0;
    path_ = 0;
    size_ = 0;




    while (!Vacia(instrucciones)){

   // printf("EXPRECIONES A EVALUAR %s\n\n",consultar_cola(instrucciones));

    char* token = consultar_cola(instrucciones);
     token =strtok(token,"::");


     if(strcmp(token,"-size")==0){

        token=strtok(NULL,"::");

            size = atoi(token);
            if(size > 0){
            size_ = 1;
            }else{
            printf("Error en el parametro -size: \n");
            printf("El valor de size debe ser mayor a 0\n");
            }


     }else if(strcmp(token,"-path")==0){

            token=strtok(NULL,"::");
            //token=strtok(token,"\"");

            char* directorio = strtok(copia_linea,"\"");
            while(directorio != NULL){
            //printf("posibles paths %s\n",path);
            if(directorio[0]=='/'){
          //  printf("lo encontre %s",directorio);
            strcpy(path,directorio);
            break;
            }
            directorio = strtok(NULL,"\"");
            }

            path_ = 1;


     }else if(strcmp(token,"+unit")==0){

            token=strtok(NULL,"::");

         //   printf("UNIT A EVALUAR--%s--\n",token);

            if(strcmp(token,"m")==0){
            strcpy(unit,token);
            unit_=1;
            }else if(strcmp(token,"k")==0){
            strcpy(unit,token);
            unit_=1;
            }else{
            printf("Error en el parametro +unit:\n");
            printf("La unidad no es valida solo se admite k o m\n");
            unit_=1;
            }


     }else if(strcmp(token,"-name")==0){    // falta validar q no venga la extension (solo seria recorre cadena y ver si tiene punto)

            token=strtok(NULL,"::");
            token=strtok(token,"\"");

            strcpy(nameaux,token);

            char *extension = nameaux;
            extension = strtok(extension,".");
            extension = strtok(NULL,".");
            if(strcmp(extension,"dsk")==0){
                strcpy(name,token);
                name_ = 1;
            }else{
            printf("Error en el parametro -name: \n");
            printf("%s extension no valida, solo se permite .sdk \n",extension);

            }

     }else {

    //printf("token a evaluar antes del if %s\n",token);
            if(token[strlen(token)-1]=='"'){
            }else{
          //  name_ = 0;
            }
     }

    eliminarnodo_cola(instrucciones);
    }


    if(unit_ == 0){
    strcpy(unit,"m");
 //   printf("NO TIENE Q ENTRAR ACA");
    unit_ = 1;
    }
  //  printf("NAME %d PATH%d SIZE%d UNIT%d\n",name_,path_,size_,unit_);
    if(name_ == 1 && path_ == 1 && size_ == 1 && unit_ == 1) {

    char Instruccion[300]="\0";
    strcat(Instruccion,"mkdir -p ");

       //printf( "path antes de ingresar al system %s\n",path );
 char copia_path [200];
 strcpy(copia_path,"");
 strcat(copia_path,"'");
 strcat(copia_path,path);
 strcat(copia_path,"'");


        strcat(Instruccion,copia_path);
   //    printf( "path modificado %s \n", copia_path);
        system(Instruccion);

   //      printf(" path sin comillas se supone %s\n",path);

    struct stat st = {0};
      char direccionTMP[0];
      strcpy(direccionTMP,path);
      strcat(direccionTMP,name);

    if(stat(direccionTMP, &st)==-1){ //verificar si el directorio existe
             //   printf("disponible");
             //   printf("unidad es %s\n",unit);
                if(strcmp(unit,"k")==0){

                // calcular size actualizar mbr y enviarlo como parametro..
               //  printf("%d\n",size);
                size = size * 1000;
                    if(size < 10000000){
                    printf("Error en la creacion: El tamaño del disco debe ser mayor a 10mb \n");
                    }else{
                    creardisco(direccionTMP,size);
                    printf("Disco creado exitosamente\n tamaño: %d\n",size );
                    printf( "path: %s\n",path );
                    printf( "unidad: %s\n",unit );
                    printf( "nombre: %s\n",name );
                    }
                }else if(strcmp(unit,"m")==0){
                //printf("%d\n",size);

                size = size*1000000;
                    if(size<10000000){
                     printf("Error en la creacion: El tamaño del disco debe ser mayor a 10mb \n");
                    }else{
                    creardisco(direccionTMP,size);
                    printf("Disco creado exitosamente\n tamaño: %d\n",size );
                    printf( "path: %s\n",path );
                    printf( "unidad: %s\n",unit );
                    printf( "nombre: %s\n",name );

                    }
                }else{
                    printf("verifique unidad\n");
                }

            }else{
            printf("Error en la creacion: \nYa existe el disco %s \n",direccionTMP);
            }

}else {
printf("parametros insuficientes o invalidos para realizar la creacion\n");


}


}

void creardisco(char* direccion, int size){

 mbr nuevo;
 struct tm fecha=*localtime(&(time_t){time(NULL)});
 strcpy(nuevo.mbr_fecha_creacion,asctime(&fecha));
 printf(nuevo.mbr_fecha_creacion);
 //nuevo.mbr_disk_asignature = 1;
 nuevo.mbr_disk_asignature = correlativo;
 nuevo.mbr_tamanio = size;

 strcpy(nuevo.mbr_partition1.part_name,"-");
 strcpy(nuevo.mbr_partition2.part_name,"-");
 strcpy(nuevo.mbr_partition3.part_name,"-");
 strcpy(nuevo.mbr_partition4.part_name,"-");
 strcpy(nuevo.mbr_partition1.part_type,"-");
 strcpy(nuevo.mbr_partition2.part_type,"-");
 strcpy(nuevo.mbr_partition3.part_type,"-");
 strcpy(nuevo.mbr_partition4.part_type,"-");
    nuevo.mbr_partition1.part_size = 0;
    nuevo.mbr_partition2.part_size = 0;
    nuevo.mbr_partition3.part_size = 0;
    nuevo.mbr_partition4.part_size = 0;
    nuevo.mbr_partition1.part_start = 0;
    nuevo.mbr_partition2.part_start = 0;
    nuevo.mbr_partition3.part_start = 0;
    nuevo.mbr_partition4.part_start = 0;

    printf("direccion antes de crear disco %s\n",direccion);
   FILE* f = fopen(direccion,"wb");
   if(f==NULL){
    printf("error");
   }else{

   fseek( f, (size-2), SEEK_SET );
   fputs("/0", f);
   fclose(f);
   }


   f = fopen(direccion,"r+b");
   if(f==NULL){
   printf("error al escribir");

    }else{
    rewind(f);
    fwrite(&nuevo,sizeof(mbr),1,f);

    }
    fclose(f);
}

void  fkdisk(Cola *nueva, char *copia_linea){
 //   int sizep = 0;
    char name[16] ="\0";
    char path_archivo[200] = "\0";
    char ajuste[4] = "\0";
    valoradd = 0;
    char tipo_delete[10] = "\0";


    name_f = 0;
    unit_f = 0;
    path_f = 0;
    size_f = 0;
    type_ = 0;
    fit_ajuste = 0;
    delete_ = 0;
    add_ = 0;




    while (!Vacia(nueva)){

    char* token = consultar_cola(nueva);
     token =strtok(token,"::");



     if(strcmp(token,"-size")==0){

        token=strtok(NULL,"::");

            sizep = atoi(token);
            if(sizep > 0){
            size_f = 1;
            }else{
            printf("Error en el parametro -size: \n");
            printf("El valor de size debe ser mayor a 0\n");
            }


     }else if(strcmp(token,"+unit")==0){

            token=strtok(NULL,"::");

            if(strcmp(token,"m")==0){
            strcpy(unitp,token);
            unit_f=1;
            }else if(strcmp(token,"k")==0){
            strcpy(unitp,token);
            unit_f=1;
            }else if(strcmp(token,"b")==0){

            strcpy(unitp,token);
            unit_f=1;
            }else{
            printf("Error en el parametro +unit:\n");
            printf("La unidad no es valida solo se admite k o m\n");
            }


     }else if(strcmp(token,"-path")==0){

            token=strtok(NULL,"::");
            //token=strtok(token,"\"");

            char* directorio = strtok(copia_linea,"\"");
            while(directorio != NULL){
            //printf("posibles paths %s\n",path);
            if(directorio[0]=='/'){
            //printf("path a crear %s",directorio);
            int a;
             for (a=0; directorio[a]; a++){
            directorio[a] = tolower(directorio[a]);
            }
            strcpy(path_archivo,directorio);
             path_f = 1;
            break;
            }
            directorio = strtok(NULL,"\"");
            }




     }else if(strcmp(token,"+type")==0){

            token=strtok(NULL,"::");

            if(strcmp(token,"p")==0){
            strcpy(type,token);
            type_=1;
            }else if(strcmp(token,"e")==0){
            strcpy(type,token);
            type_=1;
            }else if(strcmp(token,"l")==0){
            strcpy(type,token);
            type_=1;
            }else{
            printf("El parametro type es invalido solo puede ser P,E,L \n");
            }

     }else if(strcmp(token,"+fit")==0){

            token=strtok(NULL,"::");

            if(strcmp(token,"bf")==0){
            strcpy(ajuste,token);
            fit_ajuste=1;
            }else if(strcmp(token,"ff")==0){
            strcpy(ajuste,token);
            fit_ajuste=1;
            }else if(strcmp(token,"wf")==0){
            strcpy(ajuste,token);
            fit_ajuste=1;
            }else{
            printf("El parametro fit es invalido solo puede ser bf, ff, wf\n");
            }

     }else if(strcmp(token,"+delete")==0){

            token=strtok(NULL,"::");


            if(strcmp(token,"fast")==0){
            strcpy(tipo_delete,token);
            delete_=1;
            }else if(strcmp(token,"full")==0){
            strcpy(tipo_delete,token);
            delete_=1;
            }else{
            printf("El parametro delete es invalido solo puede ser fast o full \n");
            }

     }else if(strcmp(token,"-name")==0){    // falta validar q no venga la extension (solo seria recorre cadena y ver si tiene punto)

            token=strtok(NULL,"::");
            token=strtok(token,"\"");
              strcpy(name,token);
                name_f = 1;

      /*      if(strcmp(token,"")!=0){
                strcpy(name,token);
                name_ = 1;
            }else{
            printf("Error en el parametro -name: \n");
            printf("no se encontro ningun nombre \n");
            }
            */

     }else if(strcmp(token,"+add")==0){
            token=strtok(NULL,"::");

            if(strcmp(token,"0")==0){

                printf("Error en el parametro add el valor debe ser diferente de 0 \n");

            }else{
                valoradd= atoi(token);
                add_ = 1;
                printf("debe enrar a add aqui \n");
            }

     }else { // validar comandos invalidos
      //printf("parametros invalidos error \n");
     }

    eliminarnodo_cola(nueva);
    }

    if(unit_f == 0){
    strcpy(unitp,"k");
    unit_f=1;
    }

    struct stat st = {0};
      char direccionTMP[0];
      strcpy(direccionTMP,path_archivo);

    if(stat(direccionTMP, &st)==-1){ //verificar si el directorio existe
                printf("fkdisk error en path no existe \n");
                path_f = 0;
    }

    if(type_==0){
    strcpy(type,"p");
    type_ = 1;
    }
    if(fit_ajuste==0){
    strcpy(ajuste,"wf");
    fit_ajuste = 1;
    }

    // validar parametros existentes
   // printf("fi y%d y asujte%d ysize %d y path %d y name %d add %d delete %d\n",type_,fit_ajuste,size_f,path_f,name_f,add_,delete_);

    if(size_f==1 && path_f==1 && name_f == 1 && delete_ == 0 && add_ == 0){
        printf("se va crear una particion\n");
        if(strcmp(unitp,"m")==0){
        int total = sizep*1000000;
        crearparticion(total,name,path_archivo,type,ajuste);
        printf("Particion creada con exito\n");

        }else if(strcmp(unitp,"k")==0){
        int total = sizep*1000;
        crearparticion(total,name,path_archivo,type,ajuste);
        printf("Particion creada con exito\n");

        }else{
        crearparticion(sizep,name,path_archivo,type,ajuste);
        printf("Particion creada con exito\n");

        }

    }else if(delete_==1 && name_f==1 && path_f == 1){
    printf("se va borrar una particion\n");
    }else if(add_ ==1 && name_f ==1 && path_f == 1){
    printf("se puede agregar tañano a una particion existente\n");
    }else {
    printf("parametros insuficientes o extras para realizar alguna accion permitida por fdisk\n");
    }





}
// falta validar nombre repetidoo
void crearparticion(int size_partition, char *nombre_partition, char *path_validado, char *type_partition, char* ajuste_partition){
     FILE *Disco;

    if((Disco = fopen(path_validado,"rb+"))){
        mbr temporalmbr;
        rewind(Disco);
        fread(&temporalmbr,sizeof(mbr),1,Disco);
        printf("tamanio particion: %d\n",size_partition);
        printf("tipo particion: %s\n",type_partition);
        printf("nombre particion: %s\n",nombre_partition);
        printf("tamanio disco %d\n",temporalmbr.mbr_tamanio);
       // printf("fecha %s\n",temporalmbr.mbr_fecha_creacion);
        // calcular tamaño disponible
        int tamanio_ocupado = 0;
        int particionesprimarias = 0;
        int particionesextendidas = 0;
        int boolean_disponible = 0;
        int tamanio_disco = temporalmbr.mbr_tamanio;
        int tamanio_disponible = 0;

        if(strcmp(temporalmbr.mbr_partition1.part_name,"-")==0){
            boolean_disponible = 1;
        }else{
            tamanio_ocupado = tamanio_ocupado + temporalmbr.mbr_partition1.part_size;
            if(strcmp(temporalmbr.mbr_partition1.part_type,"p")==0){
                particionesprimarias ++;
            }else{
                particionesextendidas ++;
            }
        }

        if(strcmp(temporalmbr.mbr_partition2.part_name,"-")==0){
            boolean_disponible = 1;
        }else{
            tamanio_ocupado = tamanio_ocupado + temporalmbr.mbr_partition2.part_size;
             if(strcmp(temporalmbr.mbr_partition2.part_type,"p")==0){
                particionesprimarias ++;
            }else{
                particionesextendidas ++;
            }
        }

        if(strcmp(temporalmbr.mbr_partition3.part_name,"-")==0){
            boolean_disponible = 1;
        }else{
            tamanio_ocupado = tamanio_ocupado + temporalmbr.mbr_partition3.part_size;
             if(strcmp(temporalmbr.mbr_partition3.part_type,"p")==0){
                particionesprimarias ++;
            }else{
                particionesextendidas ++;
            }
        }

        if(strcmp(temporalmbr.mbr_partition4.part_name,"-")==0){
            boolean_disponible = 1;
        }else{
            tamanio_ocupado = tamanio_ocupado + temporalmbr.mbr_partition4.part_size;
             if(strcmp(temporalmbr.mbr_partition4.part_type,"p")==0){
                particionesprimarias ++;
            }else{
                particionesextendidas ++;
            }
        }

        tamanio_disponible = tamanio_disco-tamanio_ocupado;

        if(boolean_disponible == 1){
            if(tamanio_disponible < size_partition){
                printf("El tamaño de la particion:%d bytes sobrepasa el tamaño disponible:%d bytes\n No es posible crear la particion \n", size_partition,tamanio_disponible);
            }else{
                //printf("primarias %d extendidas %d tipo %s\n",particionesprimarias,particionesextendidas,type_partition);
                int insertada = 0;
                if(particionesprimarias!=3 && strcmp(type_partition,"p")==0){
                    int posicion = tamanio_disco - tamanio_disponible;

                     if(strcmp(temporalmbr.mbr_partition1.part_name,"-")==0){
                        strcpy(temporalmbr.mbr_partition1.part_fit,ajuste_partition);
                        strcpy(temporalmbr.mbr_partition1.part_name,nombre_partition);
                        temporalmbr.mbr_partition1.part_size = size_partition;
                        temporalmbr.mbr_partition1.part_start = posicion;
                        strcpy(temporalmbr.mbr_partition1.part_type,type_partition);
                        strcpy(temporalmbr.mbr_partition1.status,"vacia");
                        insertada = 1;
                     }
                     if(strcmp(temporalmbr.mbr_partition2.part_name,"-")==0){
                        if(insertada == 0){
                        strcpy(temporalmbr.mbr_partition2.part_fit,ajuste_partition);
                        strcpy(temporalmbr.mbr_partition2.part_name,nombre_partition);
                        temporalmbr.mbr_partition2.part_size = size_partition;
                        temporalmbr.mbr_partition2.part_start = posicion;
                        strcpy(temporalmbr.mbr_partition2.part_type,type_partition);
                        strcpy(temporalmbr.mbr_partition2.status,"vacia");
                        insertada = 1;
                        }
                     }
                     if(strcmp(temporalmbr.mbr_partition3.part_name,"-")==0){
                        if(insertada == 0){
                        strcpy(temporalmbr.mbr_partition3.part_fit,ajuste_partition);
                        strcpy(temporalmbr.mbr_partition3.part_name,nombre_partition);
                        temporalmbr.mbr_partition3.part_size = size_partition;
                        temporalmbr.mbr_partition3.part_start = posicion;
                        strcpy(temporalmbr.mbr_partition3.part_type,type_partition);
                        strcpy(temporalmbr.mbr_partition3.status,"vacia");
                        insertada = 1;
                        }
                     }
                     if(strcmp(temporalmbr.mbr_partition4.part_name,"-")==0){
                        if(insertada == 0){
                        strcpy(temporalmbr.mbr_partition4.part_fit,ajuste_partition);
                        strcpy(temporalmbr.mbr_partition4.part_name,nombre_partition);
                        temporalmbr.mbr_partition4.part_size = size_partition;
                        temporalmbr.mbr_partition4.part_start = posicion;
                        strcpy(temporalmbr.mbr_partition4.part_type,type_partition);
                        strcpy(temporalmbr.mbr_partition4.status,"vacia");
                        insertada = 1;
                        }
                     }

                }else if(particionesextendidas!=1 && strcmp(type_partition,"e")==0){
                     int posicion = tamanio_disco - tamanio_disponible;
                      if(strcmp(temporalmbr.mbr_partition1.part_name,"-")==0){
                        strcpy(temporalmbr.mbr_partition1.part_fit,ajuste_partition);
                        strcpy(temporalmbr.mbr_partition1.part_name,nombre_partition);
                        temporalmbr.mbr_partition1.part_size = size_partition;
                        temporalmbr.mbr_partition1.part_start = posicion;
                        strcpy(temporalmbr.mbr_partition1.part_type,type_partition);
                        strcpy(temporalmbr.mbr_partition1.status,"vacia");
                        insertada = 1;
                     }
                     if(strcmp(temporalmbr.mbr_partition2.part_name,"-")==0){
                        if(insertada == 0){
                        strcpy(temporalmbr.mbr_partition2.part_fit,ajuste_partition);
                        strcpy(temporalmbr.mbr_partition2.part_name,nombre_partition);
                        temporalmbr.mbr_partition2.part_size = size_partition;
                        temporalmbr.mbr_partition2.part_start = posicion;
                        strcpy(temporalmbr.mbr_partition2.part_type,type_partition);
                        strcpy(temporalmbr.mbr_partition2.status,"vacia");
                        insertada =1;
                        }
                     }
                     if(strcmp(temporalmbr.mbr_partition3.part_name,"-")==0){
                        if(insertada == 0){
                        strcpy(temporalmbr.mbr_partition3.part_fit,ajuste_partition);
                        strcpy(temporalmbr.mbr_partition3.part_name,nombre_partition);
                        temporalmbr.mbr_partition3.part_size = size_partition;
                        temporalmbr.mbr_partition3.part_start = posicion;
                        strcpy(temporalmbr.mbr_partition3.part_type,type_partition);
                        strcpy(temporalmbr.mbr_partition3.status,"vacia");
                        insertada = 1;
                        }
                     }
                     if(strcmp(temporalmbr.mbr_partition4.part_name,"-")==0){
                        if(insertada == 0){
                        strcpy(temporalmbr.mbr_partition4.part_fit,ajuste_partition);
                        strcpy(temporalmbr.mbr_partition4.part_name,nombre_partition);
                        temporalmbr.mbr_partition4.part_size = size_partition;
                        temporalmbr.mbr_partition4.part_start = posicion;
                        strcpy(temporalmbr.mbr_partition4.part_type,type_partition);
                        strcpy(temporalmbr.mbr_partition4.status,"vacia");
                        insertada = 1;
                        }
                     }


                }else{
                    printf("Ya no puede crear mas particiones consulte el reporte del disco\n Puede tener ya 3 primarias o una extendida\n");
                }
            }
        }else{
            printf("ya no puede crear mas particiones existen 4 creadas\n");
        }

        rewind(Disco);
        fwrite(&temporalmbr,sizeof(mbr),1,Disco);
        fclose(Disco);
        }else{
        printf("error garrafal no se encuentra el disco");
        }

}

void mount(Cola *nueva,char *copia_linea){
    char name_particion[16] ="\0";
    char path_mount[200] = "\0";
    name_m = 0;
    path_m = 0;


     while (!Vacia(nueva)){

    char* token = consultar_cola(nueva);
     token =strtok(token,"::");


    if(strcmp(token,"-path")==0){

            token=strtok(NULL,"::");
            //token=strtok(token,"\"");

            char* directorio = strtok(copia_linea,"\"");
            while(directorio != NULL){
            //printf("posibles paths %s\n",path);
            if(directorio[0]=='/'){
            printf("lo encontre %s\n",directorio);
            int a;
             for (a=0; directorio[a]; a++){
            directorio[a] = tolower(directorio[a]);
            }
            strcpy(path_mount,directorio);
            path_m = 1;
            break;
            }
            directorio = strtok(NULL,"\"");
            }

     }else if(strcmp(token,"-name")==0){

            token=strtok(NULL,"::");
            token=strtok(token,"\"");
              strcpy(name_particion,token);
                name_m = 1;

     }else{

     }

    eliminarnodo_cola(nueva);
    }


        //validar path
        struct stat st = {0};
        char direccionTMP[0];
        strcpy(direccionTMP,path_mount);

        if(stat(direccionTMP, &st)==-1){ //verificar si el directorio existe
                printf("mount error en path no existe \n");
                path_m = 0;
        }

    if(path_m==1 && name_m ==1){
        montar_particion(path_mount,name_particion);
    }else{
    printf("mount error parametros insuficientes o invalidos\n");
    }
        printf("%s\n",name_particion);
        printf("%s\n",path_mount);


}

void montar_particion(char *path_mount, char *name_particion){

    printf("%s",path_mount);

    FILE *Disco;

    if((Disco = fopen(path_mount,"rb"))){
        mbr temporalmbr;
        rewind(Disco);
        fread(&temporalmbr,sizeof(mbr),1,Disco);
            int nombrevalido = 0;
            if(strcmp(temporalmbr.mbr_partition1.part_name,name_particion)==0){
            nombrevalido = 1;
            }
            if(strcmp(temporalmbr.mbr_partition2.part_name,name_particion)==0){
            nombrevalido = 1;
            }
            if(strcmp(temporalmbr.mbr_partition3.part_name,name_particion)==0){
            nombrevalido = 1;
            }
            if(strcmp(temporalmbr.mbr_partition4.part_name,name_particion)==0){
            nombrevalido = 1;
            }

            if(nombrevalido == 0){
                printf("El nombre de la particion no existe dentro del directorio\n");
            }else{
                    char letras[9] = "abcdefghi";
                    char numeros[10] ="0123";
                    char id[4] ="\0";
                    id[0] = 'v';
                    id[1] = 'd';
                    id[4] = '\0';
                    int i;
                    int insertado = 0;
                    for(i = 0; i<9; i++){
                    //char *tem = cab[i];
                    id[2]=letras[i];
                   // printf("%c \n",letras[i]);
                        int j;
                        for(j=0; j<4; j++){
                            id[3] = numeros[j];
                            // PENDIENTE VALIDAR LETRA POR DISCO
                           //printf("valor de consultar letra %d",consultar_letramount(nuevalista,id));
                          // if(consultar_letramount(nuevalista,id)==0 || j<4){
                                if(consultar_existe(nuevalista,id)!=1){
                                insertar_lista(nuevalista,path_mount,name_particion,id);
                                insertado = 1;
                                break;
                                }
                           // }
                        }
                        if(insertado == 1){
                        break;
                        }
                    }

            }



         fclose(Disco);
        }else{
        printf("error");
        }



}

void umount(Cola *nueva, char* copia_linea){
    char id_umount[4] ="\0";
    id_u= 0;


     while (!Vacia(nueva)){

    char* token = consultar_cola(nueva);
     token =strtok(token,"::");

    if(strcmp(token,"-id")==0){

            token=strtok(NULL,"::");
           // token=strtok(token,"\"");
              strcpy(id_umount,token);
              if(consultar_existe(nuevalista,id_umount)==1){
              eliminar_lista(nuevalista,id_umount);

              }else{
              printf("No existe una particion montada con el nombre %s\n",id_umount);
              }
                id_u = 1;

     }else{
        printf("parametro invalido a sido ignorado: %s\n",token);
     }

    eliminarnodo_cola(nueva);
    }

}

void rmdisk(Cola *nueva,char* copia_linea){
    char path_eliminar[200] = "\0";

     while (!Vacia(nueva)){

    char* token = consultar_cola(nueva);
     token =strtok(token,"::");


    if(strcmp(token,"-path")==0){

            token=strtok(NULL,"::");
            //token=strtok(token,"\"");

            char* directorio = strtok(copia_linea,"\"");
            while(directorio != NULL){
            //printf("posibles paths %s\n",path);
            if(directorio[0]=='/'){
            printf("lo encontre %s\n",directorio);
            int a;
             for (a=0; directorio[a]; a++){
            directorio[a] = tolower(directorio[a]);
            }

            strcpy(path_eliminar,directorio);
         //   path_m = 1;
            break;
            }
            directorio = strtok(NULL,"\"");
            }

     }else{

     }

    eliminarnodo_cola(nueva);
    }


        //validar path
        struct stat st = {0};
        char direccionTMP[0];
        strcpy(direccionTMP,path_eliminar);

        if(stat(direccionTMP, &st)==-1){ //verificar si el directorio existe
                printf("rmdisk error en path no existe \n");

        }else{
            if(consultar_path(nuevalista,path_eliminar)==1){
                printf("rmdisk error no se puede eliminar debe desmontar primero particiones que utilizen este disco\n");
            }else{
                printf("Ingrese s para confirmar eliminar el disco \nIngrese cualquier otro caracter para cancelar\n");
                char entrada[4] ="\0";
                scanf("%s",entrada);
                if(strcmp(entrada,"s")==0){
                    if(remove(path_eliminar)==0){
                        printf("elminado exitosamente");
                    }else{
                        printf("error inesperado es posible q el disco este en uso");
                    }
                }else{

                printf("Se cancelo la eliminacion del disco\n");

                }

            }
        }

        //printf("Disco a eliminar: %s",path_eliminar);

}

void rep(Cola *nueva,char *copia_linea){
    id_r = 0;
    name_r = 0;
    path_r = 0;

    char name_tipo[16] ="\0";
    char path_rep[200] = "\0";
    char parth_copy [200] ="\0";
    char id[4] ="\0";

     while (!Vacia(nueva)){

    char* token = consultar_cola(nueva);
     token =strtok(token,"::");


    if(strcmp(token,"-path")==0){

            token=strtok(NULL,"::");
            //token=strtok(token,"\"");

            char* directorio = strtok(copia_linea,"\"");
            while(directorio != NULL){
            //printf("posibles paths %s\n",path);
            if(directorio[0]=='/'){
            printf("lo encontre %s\n",directorio);
            strcpy(path_rep,directorio);
            path_r = 1;


            strcpy(parth_copy,path_rep);

                int i;
                int remplazo = 0;
                for (i=strlen(parth_copy);parth_copy[i]!='/';i--){ //recorremos la cadena
                parth_copy[i] ='\0';
                }

                printf("copia pat sin image %s\n",parth_copy);

                char copia_path [200];
                 strcpy(copia_path,"");
                 strcat(copia_path,"'");
                 strcat(copia_path,parth_copy);
                 strcat(copia_path,"'");

                char Instruccion[300]="\0";
                strcat(Instruccion,"mkdir -p ");

                strcat(Instruccion,copia_path);
                printf( "path modificado %s \n", copia_path);
                system(Instruccion);

            break;
            }
            directorio = strtok(NULL,"\"");
            }

     }else if(strcmp(token,"-name")==0){

            token=strtok(NULL,"::");
            token=strtok(token,"\"");

            if(strcmp(token,"mbr")==0){
             strcpy(name_tipo,token);
                name_r = 1;
            }else if(strcmp(token,"disk")==0){
             strcpy(name_tipo,token);
                name_r = 1;
            }else{
            printf("rep error: el tipo de reporte no existe\n");
            }

     }else if(strcmp(token,"-id")==0){

            token=strtok(NULL,"::");

            strcpy(id,token);
                id_r= 1;

     }else{

     }

    eliminarnodo_cola(nueva);
    }


    if(path_r==1 && name_r ==1){
        //montar_particion(path_mount,name_particion);

        if(strcmp(name_tipo,"mbr")==0){
            reporte_mbr(path_rep,id,parth_copy);
        }else if(strcmp(name_tipo,"disk")==0){
            reporte_disk(path_rep,id,parth_copy);
        }
    }else{
    printf("rep error parametros insuficientes o invalidos\n");
    }
        printf("%s\n",name_tipo);
        printf("%s\n",path_rep);
        printf("%s\n",id);

}
void leersh(Cola *nueva, char* copia_linea){

char path_leer[200] ="\0";

       while (!Vacia(nueva)){



    char* token = consultar_cola(nueva);
     token =strtok(token,"::");


    if(strcmp(token,"-path")==0){

            token=strtok(NULL,"::");
            //token=strtok(token,"\"");

            char* directorio = strtok(copia_linea,"\"");
            while(directorio != NULL){
            //printf("posibles paths %s\n",path);
            if(directorio[0]=='/'){
            printf("lo encontre %s\n",directorio);
            strcpy(path_leer,directorio);
         //   path_m = 1;
            break;
            }
            directorio = strtok(NULL,"\"");
            }

     }else{

     }

    eliminarnodo_cola(nueva);
    }

      struct stat st = {0};
        char direccionTMP[0];
        strcpy(direccionTMP,path_leer);

        if(stat(direccionTMP, &st)==-1){ //verificar si el directorio existe
                printf("rmdisk error en path no existe \n");
        }
        else{


        FILE *tmp;

            if((tmp = fopen(path_leer,"r"))){
                char Linea[300];
                while(fgets(Linea,300,tmp)!=NULL){

                if(Linea[0]=='#'){
                 printf("\n%s\n",Linea);
                }else{
                    if(strcmp(Linea,"\n")!=0){

                    int fin_Linea = strlen(Linea)-1;
                        if(Linea[fin_Linea]=='\n'){
                            //printf("deberia entrassdalfjaslñdkjfsalñkdjfñaskdjfñasdkjñ\n");
                            Linea[fin_Linea-1]='\0';
                        }

                       if(Linea[0]=='\0'){
                       }else{
                       printf("\n\nComando: \n %s\n",Linea);
                        verificar_comandos(Linea);

                       }
                    }
                 }
                }


                fclose(tmp);
            }else{
                printf("#Error al abrir el archivo !!!\n");
            }

        }




}

void reporte_mbr(char *path_rep,char *id, char*pathdot){
int existe_id = 0;
char path_dot[200] = "\0";
strcpy(path_dot,pathdot);

printf("path rep es %s\n",path_rep);
char copia_pathrep [200] = "\0";
strcpy(copia_pathrep,path_rep);
char*  token =strtok(copia_pathrep,".");
     token = strtok(NULL,".");

     printf("el formato es%s\n",token);

    if(nuevalista->inicio == NULL){
        printf("Error no hay particiones montadas para realizar el reporte\n");
    }else{
        if(consultar_existe(nuevalista,id)==1){
        existe_id = 1;

        }else{
        printf("Error el nombre de la particion es incorrecto no existe\n");
        }
    }

    printf("existe id es%d\n",existe_id);
    if(existe_id==1){

    FILE *Disco;
    printf("path de consultar id%s\n",consultar_id(nuevalista,id));

    if((Disco = fopen(consultar_id(nuevalista,id),"rb"))){
        mbr temporalmbr;
        rewind(Disco);
        fread(&temporalmbr,sizeof(mbr),1,Disco);

        FILE *Reporte;
        strcat(path_dot,"mbr.dot");
        if((Reporte=fopen(path_dot,"w")))
        {
            fprintf(Reporte,"digraph G{\nnode[shape=record rankdir=LR];\nlabel=\"MBR\"\n");
            fprintf(Reporte,"struct01 [label= \" {");
            fprintf(Reporte,"{Tamaño | %d} |",temporalmbr.mbr_tamanio);
           // fprintf(Reporte,"{Ubicacion | %s} |",consultar_id(nuevalista,id));
            fprintf(Reporte,"{Fecha de Creacion | %s} |",temporalmbr.mbr_fecha_creacion);
            fprintf(Reporte,"{disk signatura| %d } |",temporalmbr.mbr_disk_asignature);

            if(strcmp(temporalmbr.mbr_partition1.part_name,"-")==0){
                fprintf(Reporte,"{partition1 | null }|");
            }else{
                    fprintf(Reporte,"Particion 1 |");
                    fprintf(Reporte,"{Estado | %s} |",temporalmbr.mbr_partition1.status);
                    fprintf(Reporte,"{Tipo | %s} |",temporalmbr.mbr_partition1.part_type);
                    fprintf(Reporte,"{Ajuste | %s} |",temporalmbr.mbr_partition1.part_fit);
                    fprintf(Reporte,"{Inicio | %d} |",temporalmbr.mbr_partition1.part_start);
                    fprintf(Reporte,"{Tamaño | %d} |",temporalmbr.mbr_partition1.part_size);
                    fprintf(Reporte,"{Nombre | %s} |",temporalmbr.mbr_partition1.part_name);
            }

             if(strcmp(temporalmbr.mbr_partition2.part_name,"-")==0){
                fprintf(Reporte,"{partition2 | null }|");
            }else{
                    fprintf(Reporte,"Particion 2 |");
                    fprintf(Reporte,"{Estado | %s} |",temporalmbr.mbr_partition2.status);
                    fprintf(Reporte,"{Tipo | %s} |",temporalmbr.mbr_partition2.part_type);
                    fprintf(Reporte,"{Ajuste | %s} |",temporalmbr.mbr_partition2.part_fit);
                    fprintf(Reporte,"{Inicio | %d} |",temporalmbr.mbr_partition2.part_start);
                    fprintf(Reporte,"{Tamaño | %d} |",temporalmbr.mbr_partition2.part_size);
                    fprintf(Reporte,"{Nombre | %s} |",temporalmbr.mbr_partition2.part_name);
            }
            if(strcmp(temporalmbr.mbr_partition3.part_name,"-")==0){
                fprintf(Reporte,"{partition3 | null }|");
            }else{
                    fprintf(Reporte,"Particion 3 |");
                    fprintf(Reporte,"{Estado | %s} |",temporalmbr.mbr_partition3.status);
                    fprintf(Reporte,"{Tipo | %s} |",temporalmbr.mbr_partition3.part_type);
                    fprintf(Reporte,"{Ajuste | %s} |",temporalmbr.mbr_partition3.part_fit);
                    fprintf(Reporte,"{Inicio | %d} |",temporalmbr.mbr_partition3.part_start);
                    fprintf(Reporte,"{Tamaño | %d} |",temporalmbr.mbr_partition3.part_size);
                    fprintf(Reporte,"{Nombre | %s} |",temporalmbr.mbr_partition3.part_name);
            }
            if(strcmp(temporalmbr.mbr_partition4.part_name,"-")==0){
                fprintf(Reporte,"{partition4 | null }|");
            }else{
                    fprintf(Reporte,"Particion 4 |");
                    fprintf(Reporte,"{Estado | %s} |",temporalmbr.mbr_partition4.status);
                    fprintf(Reporte,"{Tipo | %s} |",temporalmbr.mbr_partition4.part_type);
                    fprintf(Reporte,"{Ajuste | %s} |",temporalmbr.mbr_partition4.part_fit);
                    fprintf(Reporte,"{Inicio | %d} |",temporalmbr.mbr_partition4.part_start);
                    fprintf(Reporte,"{Tamaño | %d} |",temporalmbr.mbr_partition4.part_size);
                    fprintf(Reporte,"{Nombre | %s} |",temporalmbr.mbr_partition4.part_name);
            }

            fputs("}\"];\n",Reporte);
            fputs("}\n",Reporte);
            fflush(Reporte);
            fclose(Reporte);

    }

         fclose(Disco);
        }else{
        printf("error");
        }
            char instructio [100] = "\0";
            strcpy(instructio,"dot ");
            strcat(instructio,"'");
            strcat(instructio,path_dot);
            strcat(instructio,"'");
            strcat(instructio," -o ");
            strcat(instructio,"'");
            strcat(instructio,path_rep);
            strcat(instructio,"' ");
            strcat(instructio,"-T");
            strcat(instructio,token);
            //strcat(instructio,"\"");
           // strcpy(instructio,"\"dot '/home/rob/chute a/mbr.dot' -o '/home/rob/chute a/mbr.jpg' -Tjpg\"");
            system(instructio);
            strcpy(instructio,"xdg-open '");
            strcat(instructio,path_rep);
            strcat(instructio,"'");
            system(instructio);
           //  system("display salida.png");
 }

}
void reporte_disk(char *path_rep,char *id, char*pathdot){
int existe_id = 0;
char path_dot[200] = "\0";
strcpy(path_dot,pathdot);

printf("path rep es %s\n",path_rep);
char copia_pathrep [200] = "\0";
strcpy(copia_pathrep,path_rep);
char*  token =strtok(copia_pathrep,".");
     token = strtok(NULL,".");

     printf("el formato es%s\n",token);

    if(nuevalista->inicio == NULL){
        printf("Error no hay particiones montadas para realizar el reporte\n");
    }else{
        if(consultar_existe(nuevalista,id)==1){
        existe_id = 1;

        }else{
        printf("Error el nombre del id es incorrecto no existe\n");
        }
    }



    if(existe_id==1){

    FILE *Disco;
            char *parhdisco = consultar_id(nuevalista,id);
           printf("path de consultar id%s\n",parhdisco);

    if((Disco = fopen(consultar_id(nuevalista,id),"rb"))){
        mbr temporalmbr;
        rewind(Disco);
        fread(&temporalmbr,sizeof(mbr),1,Disco);

        FILE *Reporte;
        strcat(path_dot,"mbr2.dot");
        if((Reporte=fopen(path_dot,"w")))
        {
              fprintf(Reporte,"digraph G{");
            fprintf(Reporte,"\n\trankdir=LR\n");

            fprintf(Reporte,"\n\tsubgraph cluster{\n");

            fprintf(Reporte,"\n\tnode[shape=record];");
            fprintf(Reporte,"\n\tlabel=\"Reporte del disco\"\n");

            fprintf(Reporte,"\t\tstruct0 [label=\"{MBR |\n");
            int ocupada = 0;
              if(strcmp(temporalmbr.mbr_partition1.part_name,"-")!=0){
              ocupada++;
                if(strcmp(temporalmbr.mbr_partition1.part_type,"p")==0){
                fprintf(Reporte," Primaria |");
                }else{
                fprintf(Reporte," Extendida | {EBR|libre}|");
                }
            }
            if(strcmp(temporalmbr.mbr_partition2.part_name,"-")!=0){
                ocupada++;
                if(strcmp(temporalmbr.mbr_partition2.part_type,"p")==0){
                fprintf(Reporte," Primaria |");
                }else{
                fprintf(Reporte," Extendida | {EBR|libre}|");
                }


            }
            if(strcmp(temporalmbr.mbr_partition3.part_name,"-")!=0){
            ocupada++;
                if(strcmp(temporalmbr.mbr_partition3.part_type,"p")==0){
                fprintf(Reporte," Primaria |");
                }else{
                fprintf(Reporte," Extendida | {EBR|libre}|");
                }
            }
            if(strcmp(temporalmbr.mbr_partition4.part_name,"-")!=0){
            ocupada++;
                if(strcmp(temporalmbr.mbr_partition4.part_type,"p")==0){
                fprintf(Reporte," Primaria |");
                }else{
                fprintf(Reporte," Extendida | {EBR|libre}|");
                }
            }

            if(ocupada<4){
                fprintf(Reporte,"Libre");
            }


            fputs("}\"];}\n",Reporte);
            fputs("}\n",Reporte);

            fflush(Reporte);
            fclose(Reporte);

    }

         fclose(Disco);
        }else{
        printf("error");
        }
            char instructio [100] = "\0";
            strcpy(instructio,"dot ");
            strcat(instructio,"'");
            strcat(instructio,path_dot);
            strcat(instructio,"'");
            strcat(instructio," -o ");
            strcat(instructio,"'");
            strcat(instructio,path_rep);
            strcat(instructio,"' ");
            strcat(instructio,"-T");
            strcat(instructio,token);
            //strcat(instructio,"\"");
           // strcpy(instructio,"\"dot '/home/rob/chute a/mbr.dot' -o '/home/rob/chute a/mbr.jpg' -Tjpg\"");
           printf("Comando reporte 2%s\n",instructio);
            system(instructio);
            strcpy(instructio,"xdg-open '");
            strcat(instructio,path_rep);
            strcat(instructio,"'");
            system(instructio);
           //  system("display salida.png");
 }
}
/* recorrer cola
while (!Vacia(nueva)){
char *temp = consultar_cola(nueva);
printf("%s\n",temp);
if(strcmp(temp,"si")==0){
printf("hay un si\n");
}
eliminarnodo_cola(nueva);
}

// reeemplazar caracter
 int i;
    int remplazo = 0;
    for (i=0;path[i]!='\0';i++){ //recorremos la cadena
    if (path[i]=='/'&&remplazo==0){ //compara
        path[i]=' ';

        remplazo = 1;
        printf("si entro"); //cambia el valor si lo encuentra
    }
    }


  // verificar si existe disco
struct stat st = {0};
#include <sys/stat.h> es la libreria

if (stat("/home/iveth/Documentos/carpet/mig.dsk", &st) == -1) {
//direccion = copia;
printf("no la encontre \n");
   //  val = mkdir("carpeta/", 0700);
}else{
printf("ahi esta\n");
}


/// remove(path) para remover..
int verifica = remove(path) "si es cero lo elimina"


char id[4] = "vda0";
char ide[1];
ide[0] = id[3];
     int n;
     n = atoi(ide);

     printf("%d",n);


     char ida = id[3];
     int na;
     na = (int) (ida - 48);
 */






