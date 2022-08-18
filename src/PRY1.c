/*****Datos administrativos************************
* Nombre del archivo:PRY1
* Tipo de archivo:C
* Proyecto: Sistema de produccion agricola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Programa que realiza una gestion de produccion agricola
* Desarrollada en lenguaje C con conexion local a MySQL DB
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 14/08/2022
**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define BUFFER 250

void Menu_01();
void Menu_OP1();
void Menu_OP2();
void Menu_OA1();
void Menu_VI01();
void Menu_RN01();
void Menu_RV01();
void Menu_CN();
void Menu_CV01();
void Menu_BA01();
char* Leer(FILE* archivo);
bool VerificarNumero(char num[]);
void Pause();


/*****Nombre***************************************
* main
*****Descripción**********************************
* Metodo de inicio del programa
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
int main() {
    //revisar lo del fullscreen
    Menu_01();
    return 0;
}


/*****Nombre***************************************
* Menu_01
*****Descripción**********************************
* Menu principal del programa
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Menu_01() {
    system ("clear");
    fflush(stdin);
    printf("GESTIÓN DE PRODUCCIÓN AGRÍCOLA [GPA]");
    char opcion_01[BUFFER];
    printf("\n1.Opciones operativas\n2.Opciones administrativas\n>");
    scanf("%s",&opcion_01);
    if (!VerificarNumero(opcion_01)) {
        printf("[ERROR] = POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
        Pause();
        Menu_01();
    }
    int opcion_02 = strtol(opcion_01,NULL,10);
    switch (opcion_02) {
        case 1:
            Menu_OP1();
            break;
        case 2:
            Menu_OA1();
            break;
        default:
            printf("[ERROR] <= POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
            Pause();
            break;
    }

}


/*****Nombre***************************************
* Menu_OP1
*****Descripción**********************************
* Menu de gestion de opciones operativas
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Menu_OP1() {
    system("clear");
    fflush(stdin);
    printf("[MENÚ DE OPCIONES OPERATIVAS]\n");
    //se solicita la contraseña
    printf("Ingrese el nombre de usuario y contraseña\nNombre de usuario:");
    char nombreUsuario[BUFFER];
    scanf("%s",&nombreUsuario);
    //validamos nombre de nombreUsuario
    printf("Contraseña:");
    char contraseña[BUFFER];
    scanf("%s",&contraseña);
    //validamos contraseña
    //hacer proceso si da contraseña incorrecta
    Menu_OP2();

    
}

/*****Nombre***************************************
* Menu_OP2
*****Descripción********************************************************
* Menu de ingreso de la ubicacion del archivo para la carga de productos
*****Retorno************************************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Menu_OP2() {
    system("clear");
    fflush(stdin);
    printf("Ingrese la ruta de ubicacion del archivo de productos\n>");
    char ubicacion[BUFFER];
    scanf("%s",&ubicacion);
    FILE* ubicacionA = fopen(ubicacion,"r");
    if (!ubicacionA) {
        printf("[ERROR] <= EL ARCHIVO NO EXISTE O SE PRODUJO UN ERROR AL INTENTAR ABRIRLO");
        Pause();
        Menu_OP2();
    }
    else {
        char* texto = Leer(ubicacionA);
        exit(-1);
        //continuar con la revision del archivo
    }

}

/*****Nombre***************************************
* Leer
*****Descripción**********************************
* Metodo que lee un archivo
*****Retorno************************************************
* Cadena de caracteres que contiene el contenido del archivo
*****Entradas***********************************************
* El descriptor del archivo
**************************************************/
char* Leer(FILE *archivo) {
    int cont;
    do {
        int caracter = fgetc(archivo);
            if (caracter != EOF) {
                cont++;
            }
    }while (!feof(archivo));
    char *texto = malloc (sizeof(char) * cont);
    rewind(archivo);
    for (int i = 0; i!=cont;i++) {
        int caracter = fgetc(archivo);
            if (caracter != EOF) {
                texto[i] = (char) caracter;
            }
    }
    printf("%s",texto);
    fclose(archivo);
    return texto;
}


/*****Nombre***************************************
* Menu_OA1
*****Descripción**********************************
* Menu de gestion de opciones administrativas
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Menu_OA1() {
    system("clear");
    fflush(stdin);
    printf("MENU DE OPCIONES ADMINISTRATIVAS\n");
    printf("1.Valores iniciales\n2.Registro de nomina\n3.Registro de venta de productos\n4.Consulta de nominas\n5.Consulta de ventas\n6.Valance anual\n7.Volver atras\n8.Salir\n>");
    char opcion_01[BUFFER];
    scanf("%s",&opcion_01);
    if (!VerificarNumero(opcion_01)) {
        printf("[ERROR] = POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
        Pause();
        Menu_OA1();

    }
    int opcion_02 = strtol(opcion_01,NULL,10);
    switch(opcion_02) {
        case 1:
            Menu_VI01();
            break;
        case 2:
            Menu_RN01();
            break;
        case 3:
            Menu_RV01();
            break;
        case 4:
            Menu_CN();
            break;
        case 5:
            Menu_CV01();
            break;
        case 6:
            Menu_BA01();
            break;
        case 7:
            Menu_01();
            break;
        case 8:
            exit(-1);
        default:
            printf("[ERROR] = POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
            Pause();
            Menu_OA1();
            break;
    }
}
/*****Nombre***************************************
* Menu_VI01
*****Descripción**********************************
* Menu de gestion de valores iniciales
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Menu_VI01() {}

/*****Nombre***************************************
* Menu_RN01
*****Descripción**********************************
* Menu de gestion de regitro de nominas
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Menu_RN01() {}

/*****Nombre***************************************
* Menu_RV01
*****Descripción**********************************
* Menu de gestion de registro de ventas
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Menu_RV01() {}

/*****Nombre***************************************
* Menu_CN
*****Descripción**********************************
* Menu de consulta de nominas
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Menu_CN() {}

/*****Nombre***************************************
* Menu_CV01
*****Descripción**********************************
* Menu de consulta de ventas
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Menu_CV01() {}

/*****Nombre***************************************
* Menu_VA01
*****Descripción**********************************
* Menu de gestion de balance anual
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Menu_BA01() {}



/*****Nombre***************************************
* VerificarNumero
*****Descripción**********************************
* Programa que valida si la entrada es un numero
*****Retorno**************************************
* Valor booleano 
*****Entradas*************************************
* Cadena de caracteres
**************************************************/
bool VerificarNumero(char num[]) {
    for(int i = 0; i < strlen(num); i++) {if(isdigit(num[i])==0) return false;}
    return true;
}

/*****Nombre***************************************
* Pause
*****Descripción**********************************
* Metodo que realiza una pausa de 3 segundos 
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Pause() {
    for (int i=3;i!=0;i--) {
        printf("[%d]<===[SEGUNDOS PARA CONTINUAR]\n",i);
        fflush(stdout);
        sleep(1);
    }
}

