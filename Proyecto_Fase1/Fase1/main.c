#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>

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
void createdisk(char *instruction);

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

    char *first_word =strtok(line_instruction," ");

    if(strcmp(first_word,"mkdisk")==0){
        createdisk(copy_instruction);
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

void createdisk(char* instruction){
    char name[16];
    char unit[2];
    char path[200];
    int size;
    size = 0;




    char *token = strtok(instruction," ");
/*
    while ( token != NULL ) {
      printf( "%s\n", token );

        //token=strtok(NULL,":");
        token=strtok(NULL,"::");
    }
    */
    //printf("%s\n",token);
    //token = strtok(NULL, " ");
    printf("%s\n",token);
    token = strtok(NULL, "::" );

    printf("%s\n",token);
    token = strtok( NULL, " " );
    printf("%s\n",token);

     token = strtok(NULL, "::" );

    printf("%s\n",token);
    token = strtok( NULL, " " );
    printf("%s\n",token);


/*
     while ( token != NULL ) {
     // printf( "%s\n", token );
     if(strcmp(token,"-size")==0){
        //token=strtok(NULL,":");
        token=strtok(token," ");
            size = atoi(token);
            printf("%s\n",token);
           //printf( "%d\n",size );

     }else if(strcmp(token,"-path")==0){

            //token=strtok(NULL,":");
            strcpy(path,token);
     }else if(strcmp(token,"-unit")==0){

            token=strtok(NULL,":");
            strcpy(unit,token);
     }else if(strcmp(token,"-name")==0){

            token=strtok(NULL,":");
            strcpy(name,token);
     }
      token = strtok( NULL, " " );
    //  token = strtok( NULL, "::" );
   }

     printf( "%d\n",size );
      printf( "%s\n",path );
       printf( "%s\n",unit );
        printf( "%s\n",name );
*/

}
