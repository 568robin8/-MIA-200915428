#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>

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
void createdisk(Cola *instruccion);

int main()
{

    inicio();

    return 0;
}

void inicio(){
 char line_instruction[1000];
 char copy_instruction[1000]; //copia de la linea de instruccion para enviarla como parametro al metodo
 char*c = "10";
 int b = atoi(c);
 printf("%d",b);
    printf("Proyecto[MIA]@200915428:> ");
    scanf(" %[^\n]s",line_instruction);

   // instruccion a minusculas
   int a;
   for (a=0; line_instruction[a]; a++){
    line_instruction[a] = tolower(line_instruction[a]);
   }

    strcpy(copy_instruction,line_instruction);

   // printf("%s /n",line_instruction);
   // printf("%s /n",copy_instruction);
    Cola *nueva;
    nueva = malloc(sizeof(Cola));
    CrearLista(nueva);

    char *first_word =strtok(line_instruction," ");

    while ( first_word != NULL ) {

        insertar_cola(nueva,first_word);
        first_word=strtok(NULL," ");
    }

/*
while (!Vacia(nueva)){
char *temp = consultar_cola(nueva);
printf("%s\n",temp);
if(strcmp(temp,"si")==0){
printf("hay un si\n");
}
eliminarnodo_cola(nueva);
}
*/

   if(strcmp(consultar_cola(nueva),"mkdisk")==0){

        eliminarnodo_cola(nueva);
      //printf("ahi vamos");
        createdisk(nueva);

        system("mkdir -p  carro/mama");

    }


   FILE* f = fopen("prueba.sdk","wb");
   if(f==NULL){
    printf("error");
   }else{

   fseek( f, 10239998, SEEK_SET );
   fputs("/0", f);
   fclose(f);
   }

   //imprimir();

}

void createdisk(Cola * instrucciones){
    char name[16];
    char unit[2];
    char path[200];
    int size;
    size = 0;



    while (!Vacia(instrucciones)){

    char* token = consultar_cola(instrucciones);
    //printf("%s",token);

    token =strtok(token,"::");

     if(strcmp(token,"-size")==0){

        token=strtok(NULL,"::");

            size = atoi(token);

     }else if(strcmp(token,"-path")==0){

            token=strtok(NULL,"::");
            token=strtok(token,"\"");

            strcpy(path,token);

     }else if(strcmp(token,"+unit")==0){

            token=strtok(NULL,"::");
            token=strtok(token,"\"");
            strcpy(unit,token);
     }else if(strcmp(token,"-name")==0){

            token=strtok(NULL,"::");
            token=strtok(token,"\"");
            strcpy(name,token);
     }

    eliminarnodo_cola(instrucciones);
    }

     printf( "%d\n",size );
      printf( "%s\n",path );
       printf( "%s\n",unit );
        printf( "%s\n",name );


}


