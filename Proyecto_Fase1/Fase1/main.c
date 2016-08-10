#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include "cola_comando.h"

int size = 0;
int correlativo = 0;
char unit[2] = "\0";

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
void mkdisk(Cola *instruccion);
void creardisco(char* direccion, int size);

int main()
{

    inicio();

    return 0;
}

void inicio(){
 char line_instruction[1000];
 char copy_instruction[1000]; //copia de la linea de instruccion para enviarla como parametro al metodo

    printf("Proyecto[MIA]@200915428:> ");
    scanf(" %[^\n]s",line_instruction);

   // system("mkdir -p /home/rob/escritorio");

   // instruccion a minusculas
   int a;
   for (a=0; line_instruction[a]; a++){
    line_instruction[a] = tolower(line_instruction[a]);
   }

    strcpy(copy_instruction,line_instruction);


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
          mkdisk(nueva);
    }else if(strcmp(consultar_cola(nueva),"fkdisk")==0){
        eliminarnodo_cola(nueva);
        // crear particiones
    }else if(strcmp(consultar_cola(nueva),"mount")==0){
        eliminarnodo_cola(nueva);
        //montar particion
    }else if(strcmp(consultar_cola(nueva),"rmdisk")==0){
        eliminarnodo_cola(nueva);
        //eliminar disco
    }else if(strcmp(consultar_cola(nueva),"umount")==0){
        eliminarnodo_cola(nueva);
        //desmontar particion
    }else if(strcmp(consultar_cola(nueva),"mbr")==0){
        eliminarnodo_cola(nueva);
        //mostrar reporte mbr
    }else if(strcmp(consultar_cola(nueva),"disk")==0){
        eliminarnodo_cola(nueva);
        //mostrar reporete disk
    }else if(strcmp(consultar_cola(nueva),"exec")==0){
        eliminarnodo_cola(nueva);
        //leer script
    }else if(strcmp(consultar_cola(nueva),"exit")==0){
        eliminarnodo_cola(nueva);
        exit(0);
    }




   inicio();

}

void mkdisk(Cola * instrucciones){
    char name[16] ="\0";
    char nameaux[16] ="\0";
    char path[200] = "\0";


    int name_ = 0;
    int unit_ = 0;
    int path_ = 0;
    int size_ = 0;



    while (!Vacia(instrucciones)){

    char* token = consultar_cola(instrucciones);
    //printf("%s",token);

    token =strtok(token,"::");

     if(strcmp(token,"-size")==0){

        token=strtok(NULL,"::");

            size = atoi(token);
            if(size > 0){
            size_ = 1;
            }else{
            printf("Error en el parametro -size: \n");
            printf("El valor de size debe ser mayor a 0\n\n");
            }


     }else if(strcmp(token,"-path")==0){

            token=strtok(NULL,"::");
            token=strtok(token,"\"");

            strcpy(path,token);
            path_ = 1;


     }else if(strcmp(token,"+unit")==0){

            token=strtok(NULL,"::");
            token=strtok(token,"\"");

            if(strcmp(token,"m")==0){
            strcpy(unit,token);
            unit_=1;
            }else if(strcmp(token,"k")==0){
            strcpy(unit,token);
            unit_=1;
            }else{
            printf("Error en el parametro +unit:\n");
            printf("La unidad no es valida solo se admite k o m\n\n");
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
            printf("%s extension no valida, solo se permite .sdk \n\n",extension);
            }

     }

    eliminarnodo_cola(instrucciones);
    }


    if(unit_ == 0){
    strcpy(unit,"m");
    unit_ = 1;
    }

    if(name_ == 1 && path_ == 1 && size_ == 1 && unit_ == 1) {
    // printf( "%d\n",size );
    //  printf( "%s\n",path );
    //   printf( "%s\n",unit );
    //    printf( "%s\n",name );



    char Instruccion[300]="\0";
    strcat(Instruccion,"mkdir -p ");
    strcat(Instruccion,path);
       printf( "%s\n",path );
        system(Instruccion);

    struct stat st = {0};
      char direccionTMP[0];
      strcpy(direccionTMP,path);
      strcat(direccionTMP,name);

    if(stat(direccionTMP, &st)==-1){ //verificar si el directorio existe
                printf("disponible");
                printf("unidad es %s\n",unit);
                if(strcmp(unit,"k")==0){

                // calcular size actualizar mbr y enviarlo como parametro..
               //  printf("%d\n",size);
                size = size * 1000;
                creardisco(direccionTMP,size);
                }else{
                //printf("%d\n",size);

                size = size*1000000;
                creardisco(direccionTMP,size    );
                }

            }else{
            printf("Error en la creacion: \nYa existe el disco %s ",direccionTMP);
            }

}


    //end if
} //end for

void creardisco(char* direccion, int size){

 mbr nuevo;
 struct tm fecha=*localtime(&(time_t){time(NULL)});
 strcpy(nuevo.mbr_fecha_creacion,asctime(&fecha));
 printf(nuevo.mbr_fecha_creacion);
 //nuevo.mbr_disk_asignature = 1;
 nuevo.mbr_disk_asignature = correlativo;
 nuevo.mbr_tamanio = size;
 //nuevo.mbr_partition1 = NULL;
 //nuevo.mbr_partition2 = NULL;
 //nuevo.mbr_partition3 = NULL;
 //nuevo.mbr_partition4 = NULL;


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
*/




