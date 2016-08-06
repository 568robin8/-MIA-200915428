#include <stdio.h>
#include <stdlib.h>
//#include <nuevo.h>
#include <string.h>
#include <sys/types.h>



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
    char *token = strtok(instruction," ");

     while ( token != NULL ) {
      printf( "%s\n", token );
      token = strtok( NULL, " " ); /* obtiene el siguiente token */
   } /* fin de while */

}
