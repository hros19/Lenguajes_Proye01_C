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
//#include "./dao/comercio_dao.h" //revisar ubicacion
//#include "./dao/operador_dao.h"

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
char* Leer(FILE* archivo, int cont);
int ContarCaracteres(FILE *archivo);
bool Revisar(char* texto, int cont);
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
    
    printf("Ingrese el nombre de usuario y contraseña\nNombre de usuario:");
    char nombreUsuario[BUFFER];
    scanf("%s",&nombreUsuario);

    printf("Contraseña:");
    char contraseña[BUFFER];
    scanf("%s",&contraseña);
    

    /*Operador operador;
    strcpy(operador.usuario,nombreUsuario); //posibles problemas de copiado
    strcpy(operador.clave,contraseña);

    if (ValidarOperador(operador)) {
        printf("INICIO DE SESION CORRECTO");
        Pause();
        Menu_OP2();
    }
    else {
        printf("\n[ERROR] <= NOMBRE DE USUARIO O CONTRASEÑA INCORRECTO, POR FAVOR INTENTE DE NUEVO\n");
        Pause();
        Menu_OP1();
    }*/
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
        int cantCaracteres = ContarCaracteres(ubicacionA);
        printf("%d",cantCaracteres);
        char* texto = Leer(ubicacionA, cantCaracteres);
        if (Revisar(texto,cantCaracteres)) {
            printf("todo correcto");
        }
        
        else {printf("todo mal");}
    }

}

int ContarCaracteres(FILE *archivo) {
    int cont;
    do {
        int caracter = fgetc(archivo);
            if (caracter != EOF) {
                cont++;
            }
    }while (!feof(archivo));
    return cont;
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
char* Leer(FILE *archivo,int cont) {
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
* Revisar
*****Descripción************************************************
* Metodo que revisa el formato del archivo de carga de productos
*****Retorno***********************************************************************************
* Valor booleano
*****Entradas**********************************************************************************
* Cadena de caracteres que contiene el contenido del archivo - numero de caracteres del archivo
**************************************************/
bool Revisar(char* texto, int cont) {
    int cantComas = 0;
    for (int i = 0;i!=cont;i++) {
        if (texto[i]==',') {cantComas++;}
        if (texto[i]=='\n' || texto[i] == '\0') {
            if (cantComas!=3) {return false;}
            cantComas = 0;
            printf("%d",cantComas);
        }
    }
    return true;
}




   /* bool bandera = true;
    char arregloID[cont][BUFFER];
    int cantID;
    for (int i = 0;i!=cont;i++) {
        int pos2;
        if (bandera) {
            char palabra[BUFFER];
            int pos;
            pos2 = i;
            while (texto[pos2]!=',') {
                palabra[pos] = texto[pos2];
                pos++;
                pos2++;
            }
            palabra[pos] = '\0';
            bandera = false;
            strcpy(arregloID[cantID],palabra);
            cantID++;
        }
        else if(texto[i] == '\n') {
            bandera = true;
        }
    }
}*/

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
void Menu_VI01() {}/*
    char id[] = "001";
    Comercio comercio = ObtenerInfoComercio(id);
    printf("[CEDULA JURIDICA] = %s\n",comercio.cedulaJuridica);
    printf("[NOMBRE DEL LOCAL] = %S\n",comercio.nombre);
    printf("[TELEFONO] = %s\n",comercio.telefono);
    printf("[NUMERO DE SIGUIENTE FACTURA] = %d\n",comercio.numSigFactura);
}*/

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

