#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "cola_comando.h"

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
      //printf("ahi vamos");
        mkdisk(nueva);



    }




   inicio();

}

void createdisk(Cola * instrucciones){
    char name[16] ="\0";
    char nameaux[16] ="\0";
    char unit[2] = "\0";
    char path[200] = "\0";
    int size;
    size = 0;

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
            if(strcmp(extension,"sdk")==0){
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
     printf( "%d\n",size );
      printf( "%s\n",path );
       printf( "%s\n",unit );
        printf( "%s\n",name );



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


                if(strcmp(unit,"k")==0){
                // calcular size actualizar mbr y enviarlo como parametro..
                }else{



                }

            }else{
            printf("Error en la creacion: \nYa existe el disco %s ",direccionTMP1);
            }

}


    //end if
} //end for

void creardisco(char* direccion, int size){


   FILE* f = fopen(direccion,"wb");
   if(f==NULL){
    printf("error");
   }else{

   fseek( f, size, SEEK_SET );
   fputs("/0", f);
   fclose(f);
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
*/




