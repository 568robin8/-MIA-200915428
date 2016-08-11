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
void verificar_comandos(char *line_instruction);
void mkdisk(Cola *instruccion, char *copia_linea);
void creardisco(char* direccion, int size);
void fkdisk(Cola *nueva, char *copia_linea);
void leersh(nueva); // falta validar espacio
void reporte_mbr(nueva);

int main()
{
 //system("mkdir -p /home/mama papa");
//  system("xdg-open /home/rob/coldplay.jpg");
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

   // instruccion a minusculas
   int a;
   for (a=0; line_instruction[a]; a++){
    line_instruction[a] = tolower(line_instruction[a]);
   }

    char copia_linea[300];
    strcpy(copia_linea,line_instruction);

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
        //montar particion
    }else if(strcmp(consultar_cola(nueva),"rmdisk")==0){
        eliminarnodo_cola(nueva);
        //eliminar disco
    }else if(strcmp(consultar_cola(nueva),"umount")==0){
        eliminarnodo_cola(nueva);
        //desmontar particion
    }else if(strcmp(consultar_cola(nueva),"rep")==0){
        eliminarnodo_cola(nueva);
        reporte_mbr(nueva);

    }else if(strcmp(consultar_cola(nueva),"exec")==0){
        eliminarnodo_cola(nueva);
        leersh(nueva);
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


    int name_ = 0;
    int unit_ = 0;
    int path_ = 0;
    int size_ = 0;




    while (!Vacia(instrucciones)){

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
            printf("lo encontre %s",directorio);
            strcpy(path,directorio);
            break;
            }
            directorio = strtok(NULL,"\"");
            }

            path_ = 1;


     }else if(strcmp(token,"+unit")==0){

            token=strtok(NULL,"::");

            if(strcmp(token,"m")==0){
            strcpy(unit,token);
            unit_=1;
            }else if(strcmp(token,"k")==0){
            strcpy(unit,token);
            unit_=1;
            }else{
            printf("Error en el parametro +unit:\n");
            printf("La unidad no es valida solo se admite k o m\n");
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

    printf("token a evaluar antes del if %s\n",token);
            if(token[strlen(token)-1]=='"'){
            }else{
            name_ = 0;
            }
     }

    eliminarnodo_cola(instrucciones);
    }


    if(unit_ == 0){
    strcpy(unit,"m");
    unit_ = 1;
    }

    if(name_ == 1 && path_ == 1 && size_ == 1 && unit_ == 1) {

    char Instruccion[300]="\0";
    strcat(Instruccion,"mkdir -p ");

       printf( "path antes de ingresar al system %s\n",path );
 char copia_path [200];
 strcpy(copia_path,"");
 strcat(copia_path,"'");
 strcat(copia_path,path);
 strcat(copia_path,"'");


        strcat(Instruccion,copia_path);
       printf( "path modificado %s \n", copia_path);
        system(Instruccion);

         printf(" path sin comillas se supone %s\n",path);

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
                    if(size < 10000000){
                    printf("Error en la creacion: El tamaño del disco debe ser mayor a 10mb \n");
                    }else{
                    creardisco(direccionTMP,size);
                    }
                }else{
                //printf("%d\n",size);

                size = size*1000000;
                    if(size<10000000){
                     printf("Error en la creacion: El tamaño del disco debe ser mayor a 10mb \n");
                    }else{
                    creardisco(direccionTMP,size    );
                    }
                }

            }else{
            printf("Error en la creacion: \nYa existe el disco %s \n",direccionTMP);
            }

}else {
printf("parametros insuficientes o invalidos para realizar la creacion\n");

 printf( "%d\n",size );
     printf( "%s\n",path );
       printf( "%s\n",unit );
        printf( "%s\n",name );
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
    int sizep = 0;
    char unitp[2];
    char name[16] ="\0";
    char path_archivo[200] = "\0";
    char type[2];
    char ajuste[4];
    int valoradd = 0;
    char tipo_delete[10];


    int name_ = 0;
    int unit_ = 0;
    int path_ = 0;
    int size_ = 0;
    int type_ = 0;
    int fit_ajuste = 0;
    int delete_ = 0;
    int add_ = 0;




    while (!Vacia(nueva)){

    char* token = consultar_cola(nueva);
     token =strtok(token,"::");



     if(strcmp(token,"-size")==0){

        token=strtok(NULL,"::");

            sizep = atoi(token);
            if(sizep > 0){
            size_ = 1;
            }else{
            printf("Error en el parametro -size: \n");
            printf("El valor de size debe ser mayor a 0\n");
            }


     }else if(strcmp(token,"+unit")==0){

            token=strtok(NULL,"::");
           // token=strtok(token,"\"");

            if(strcmp(token,"m")==0){
            strcpy(unitp,token);
            unit_=1;
            }else if(strcmp(token,"k")==0){
            strcpy(unitp,token);
            unit_=1;
            }else if(strcmp(token,"b")==0){
            strcpy(unitp,token);
            unit_=1;
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
            printf("lo encontre %s",directorio);
            strcpy(path_archivo,directorio);
            break;
            }
            directorio = strtok(NULL,"\"");
            }

            path_ = 1;


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

            printf("este deberia ser el ajuste -%s-",token);

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
                name_ = 1;

      /*      if(strcmp(token,"")!=0){
                strcpy(name,token);
                name_ = 1;
            }else{
            printf("Error en el parametro -name: \n");
            printf("no se encontro ningun nombre \n");
            }
            */

     }else if(strcmp(token,"+add")==0){    // falta validar q no venga la extension (solo seria recorre cadena y ver si tiene punto)

            token=strtok(NULL,"::");

            if(strcmp(token,"0")==0){

                printf("Error en el parametro add el valor debe ser diferente de 0 \n");

            }else{
                valoradd= atoi(token);
                add_ = 1;
            }

     }else { // validar comandos invalidos
      //printf("parametros invalidos error \n");
     }

    eliminarnodo_cola(nueva);
    }


    printf("size %d\n",sizep);
    printf("unit %s\n",unitp);
    printf("name %s\n",name);
    printf("pat %s\n",path_archivo);
    printf("type %s\n",type);
    printf("ajuste %s\n",ajuste);
    printf("add %d\n", valoradd);
    printf("delete %s\n",tipo_delete);




}
void leersh(nueva){  // FALTA VALIDAR ESPACIO y quitarle el salto de linea sustituyendo las utlimas dos posiciones

     char* path_sh = consultar_cola(nueva);
     eliminarnodo_cola(nueva);

    path_sh =strtok(path_sh,"\"");
    printf("%s",path_sh);

    FILE *tmp;

        if((tmp = fopen(path_sh,"r"))){
            char Linea[300];
            while(fgets(Linea,300,tmp)!=NULL){

            if(Linea[0]=='#'){
             printf("\n%s\n",Linea);
            }else{
                if(strcmp(Linea,"\n")!=0){

                int fin_Linea = strlen(Linea)-1;
                    if(Linea[fin_Linea]=='\n'){
                        Linea[fin_Linea] = ' ';
                    }
                printf("\n\nComando: \n %s\n",Linea);
                verificar_comandos(Linea);
                }
             }
            }


            fclose(tmp);
        }else{
            printf("#El Archivo no existe!!!\n");
        }

}

void reporte_mbr(nueva){

     char* path_sh = consultar_cola(nueva);
     eliminarnodo_cola(nueva);

    path_sh =strtok(path_sh,"\"");
    printf("%s",path_sh);

    FILE *Disco;

    if((Disco = fopen(path_sh,"rb"))){
        mbr temporalmbr;
        rewind(Disco);
        fread(&temporalmbr,sizeof(mbr),1,Disco);
        printf("tamanio %d\n",temporalmbr.mbr_tamanio);
        printf("fecha %s\n",temporalmbr.mbr_fecha_creacion);
        }else{
        printf("error");
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
 */






