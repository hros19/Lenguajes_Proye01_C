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
#include "./dao/comercio_dao.h"
#include "./dao/operador_dao.h"
#include "./dao/planilla_dao.h"
#include "./dao/producto_dao.h"
#include "./dao/factura_dao.h"
#include "./dao/rol_dao.h"
#include "./dao/area_dao.h"
#include "./dao/empleado_dao.h"
#include "./dao/cargoSocial_dao.h"
#include "./utilities/crypt.h"

#define BUFFER 250

bool RegistrarOperadorInicial();
bool CargarComercioDesdeBdd();
void CargarProductosDesdeBdd();
bool CargarAreasDesdeBdd();
bool CargarEmpleadosDesdeBdd();
bool CargarProductosDesdeRuta();
bool VerificarDouble(char* texto);
char** SepararLineas(char* texto, char* delimitador);

void MostrarListaDeEmpleados();
void MostrarListaDeProductos();
void MostrarListaDeAreas();

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
bool Revisar01_Comas(char* texto, int cont);
int ContarCaracteres(FILE *archivo);
int ContarLineas(char* texto);
bool Revisar(char* texto, int cont);
bool VerificarNumero(char num[]);
void PauseSinTimer();
void Pause();

static Operador operador; // Operador del sistema
static Comercio comercio; // Comercio del sistema
static Producto* listaProductos; // Lista de productos del sistema
static Area* listaAreas; // Lista de areas del sistema
static EmpleadoConRol* listaEmpleados; // Lista de empleados del sistema
static int cantidadProductos; // Cantidad de productos del sistema
static int cantidadAreas; // Cantidad de areas del sistema
static int cantidadEmpleados; // Cantidad de empleados en el sistema

/*****Nombre***************************************
* main
*****Descripción**********************************
* Metodo de inicio del programa
*****Retorno**************************************
* int - Retorna 0 si el programa se ejecuta correctamente.
*****Entradas*************************************
* Sin entradas
**************************************************/
int main() {
    // Operador - usuario
    if (RegistrarOperadorInicial()) {
        return main();
    }
    printf("[OK] Operador cargado desde base de datos\n");

    // Comercio - datos del comercio
    if (!CargarComercioDesdeBdd()) {
        return main();
    }
    printf("[OK] Comercio cargado desde base de datos\n");

    // Areas - areas del negocio
    if (!CargarAreasDesdeBdd()) {
        return main();
    }
    printf("[OK] Areas cargadas desde base de datos\n");

    // Empleados - empleados del negocio
    if (!CargarEmpleadosDesdeBdd()) {
        return main();
    }
    printf("[OK] Empleados cargados desde base de datos\n");

    // Productos - lista de productos del comercio (si encontró alguno)
    CargarProductosDesdeBdd();

    Menu_01();
    return 0;
}

bool CargarEmpleadosDesdeBdd() {
    listaEmpleados = ObtenerEmpleadosConRol();
    cantidadEmpleados = ObtenerCantidadEmpleados();
    if (listaEmpleados == NULL) {
        return false;
    }
    return true;
}

bool CargarAreasDesdeBdd() {
    listaAreas = ObtenerAreas();
    cantidadAreas = ObtenerCantidadAreas();
    if (listaAreas == NULL) {
        return false;
    }
    return true;
}

void CargarProductosDesdeBdd() {
    listaProductos = ObtenerProductos();
    cantidadProductos = ObtenerCantidadProductos();
}

bool CargarComercioDesdeBdd() {
    comercio = ObtenerInfoComercio((char*)"001");
    if (comercio.numSigFactura == -1) {
        return false;
    }
    return true;
}

/*****Nombre***************************************
* RegistrarOperadorInicial
*****Descripción**********************************
* Método que registra un operador inicial en la base de datos.
* Retornará true si el registro fue exitoso, false en caso contrario.
*
* Nota: El operador inicial se registrará con el nombre "admin" y la
* contraseña "admin".
*****Retorno**************************************
* True si el registro fue exitoso, false en caso contrario.
*****Entradas*************************************
* Sin entradas
**************************************************/
bool RegistrarOperadorInicial() {
    char clave[] = "admin";
    char clave_encriptada[BUFFER];
    Encriptar(clave, clave_encriptada);

    Operador op;
    strcpy(op.usuario, "admin");
    strcpy(op.clave, clave_encriptada);
    op.id = 1;

    return RegistrarOperador(op);
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
        printf("[ERROR] <= POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
        Pause();
        Menu_01();
        return;
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
    Menu_01();
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
    
    printf("Ingrese el nombre de usuario y contrasena\nNombre de usuario:");
    char nombreUsuario[BUFFER];
    scanf("%s",&nombreUsuario);

    printf("contrasena:");
    char contrasena[BUFFER];
    scanf("%s",&contrasena);

    char clave_encriptada[BUFFER];
    Encriptar(contrasena, clave_encriptada);

    printf(">> %s\n>> %s\n",nombreUsuario,clave_encriptada);

    PauseSinTimer();

    strcpy(operador.usuario, nombreUsuario);
    strcpy(operador.clave, clave_encriptada);
    
    if (ValidarOperador(operador)) {
        Menu_OP2();
    } else {
        printf("\n[ERROR] Usuario o contrasena incorrectos\n");
        Pause();
        Menu_01();
    }
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

    printf("[MENÚ DE OPCIONES OPERATIVAS]\n");
    printf("\n1. Cargar productos");
    printf("\n2. Listas de areas");
    printf("\n3. Listas de empleados");
    printf("\n4. Listas de productos");
    printf("\n0. Volver");

    printf("\n>");
    char input[BUFFER];
    scanf("%s",&input);
    if (!VerificarNumero(input)) {
        printf("[ERROR] <= POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
        Pause();
        Menu_OP2();
        return;
    }

    char c = 0; // Variable para el ciclo while
    bool termino = false;
    int opcion = strtol(input,NULL,10);
    switch (opcion) {
        case 0:
            Menu_01();
            return;
        case 1:

            if (cantidadProductos != 0) {
                printf("[!] Ya hay productos cargados en la base de datos\n");
                printf("[!] ¿Desea continuar? (s/n): ");
                
                fflush(stdin);
                scanf(" %c",&c);
                
                if (c == 's' || c == 'S') {
                    termino = false;
                    do {
                        termino = CargarProductosDesdeRuta();
                    } while (!termino);
                    break;
                }
                else {
                    printf("[!] Se cancelo la carga de productos\n");
                    Pause();
                    Menu_OP2();
                    return;
                }
            }

            termino = false;
            do {
                termino = CargarProductosDesdeRuta();
            } while (!termino);
            break;
        case 2:
            MostrarListaDeAreas();
            PauseSinTimer();
            break;
        case 3:
            MostrarListaDeEmpleados();
            PauseSinTimer();
            break;
        case 4:
            MostrarListaDeProductos();
            PauseSinTimer();
            break;
        default:
            printf("[ERROR] <= POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
            Pause();
            break;
    }
    Menu_OP2();
}

/*****Nombre***************************************
* MostrarListaDeEmpleados
*****Descripción**********************************
* Muestra la lista de empleados
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void MostrarListaDeEmpleados() {
    printf("============[Lista de empleados]============\n");
    for (int i = 0; i < cantidadEmpleados; i++) {
        printf(
            "[%6s]\t%30s\t | Puesto: %30s\t | Salario: %.2f\n",
            listaEmpleados[i].cedula,
            listaEmpleados[i].nombre_completo,
            listaEmpleados[i].nombre_rol,
            listaEmpleados[i].salario_mensual
        );
    }
}

void MostrarListaDeAreas() {
    printf("============[Lista de areas]============\n");
    for (int i = 0; i < cantidadAreas; i++) {
        printf(
            "[%d]\t%15s\t | Dimensiones (mts. cds.): %.2f\t | Producto estelar: %s\n", 
            listaAreas[i].id, 
            listaAreas[i].nombre, 
            listaAreas[i].dimensiones, 
            listaAreas[i].producto_principal
        );
    }
}

/*****Nombre***************************************
* MostrarListaDeProductos
*****Descripción**********************************
* Muestra la lista de productos
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void MostrarListaDeProductos() {
    printf("============[Lista de productos]============\n");
    for (int i = 0; i < cantidadProductos; i++) {
        printf(
            "[%4s]\t%15s\t | Costo: %.2f\t | Porc. impuesto: %.2f\n", 
            listaProductos[i].id,
            listaProductos[i].nombre,
            listaProductos[i].costo,
            listaProductos[i].impuesto
        );
    }
}

/*****Nombre***************************************
* CargarProductosDesdeRuta
*****Descripción**********************************
* Permite la carga de productos desde un archivo de texto
* en la ruta especificada por el usuario
*****Retorno**************************************
* bool - true si se cargaron los productos correctamente, false si no
*****Entradas*************************************
* Sin entradas
**************************************************/
bool CargarProductosDesdeRuta() {
    printf("\nIngrese la ruta de ubicacion del archivo de productos\n");
    printf("Presione ENTER para cancelar y volver al menu anterior\n");
    printf(">");
    char ubicacion[BUFFER];
    fflush(stdin);
    scanf("%s",&ubicacion);
    
    // El usuario presiono ENTER
    if (strlen(ubicacion) == 0) {
        return true;
    }

    FILE* ubicacionA = fopen(ubicacion,"r");
    if (!ubicacionA) {
        printf("\n[ERROR] <= EL ARCHIVO NO EXISTE O SE PRODUJO UN ERROR AL INTENTAR ABRIRLO!!\n");
        Pause();
        return false;
    }
    else {
        int cantCaracteres = ContarCaracteres(ubicacionA);
        char* texto = Leer(ubicacionA, cantCaracteres);
        if (Revisar(texto,cantCaracteres)) {
            
            // Verificar que no hayan mas de 100 lineas
            int cantLineas = ContarLineas(texto);
            cantidadProductos = cantLineas;

            if (cantLineas > 100) {
                printf("\n[ERROR] <= EL ARCHIVO CONTIENE MAS DE 100 LINEAS!!\n");
                Pause();
                return false;
            }
            else {
                char** lineas = SepararLineas(texto, (char*)"\n");

                Producto* productos = malloc(sizeof(Producto)*cantLineas);
                for (int i = 0; i < cantLineas; i++) {
                    printf("Linea: %s\n",lineas[i]);
                    char** campos = SepararLineas(lineas[i], (char*)",");

                    // Verificar cada uno de los campos

                    // Verificando el id
                    if (strlen(campos[0]) > 50) {
                        printf("\n[ERROR] <= EL CODIGO DEL PRODUCTO TIENE MAS DE 50 CARACTERES!!\n");
                        Pause();
                        return false;
                    }

                    // Verificando el nombre
                    if (strlen(campos[1]) > 50) {
                        printf("\n[ERROR] <= EL NOMBRE DEL PRODUCTO TIENE MAS DE 50 CARACTERES!!\n");
                        Pause();
                        return false;
                    }

                    // Verificando el costo (double)
                    if (!VerificarDouble(campos[2])) {
                        printf("\n[ERROR] <= EL COSTO DEL PRODUCTO NO ES UN NUMERO VALIDO!!\n");
                        Pause();
                        return false;
                    }

                    // Verificando el impuesto (double)
                    if (!VerificarDouble(campos[3])) {
                        printf("\n[ERROR] <= EL IMPUESTO DEL PRODUCTO NO ES UN NUMERO VALIDO!!\n");
                        Pause();
                        return false;
                    }

                    double costo_producto = strtod(campos[2], NULL);
                    double impuesto_producto = strtod(campos[3], NULL);

                    Producto producto;
                    strcpy(producto.id, campos[0]);
                    strcpy(producto.nombre, campos[1]);
                    producto.costo = costo_producto;
                    producto.impuesto = impuesto_producto;

                    productos[i] = producto;
                }
                // Si todo esta bien, se cargan los productos a la lista del programa
                listaProductos = productos;
                printf("\n[!] PRODUCTOS CARGADOS CON EXITO!!\n");
                if (EliminarProductos()) {
                    for (int i = 0; i < cantLineas; i++) {
                        RegistrarProducto(listaProductos[i]);
                    }
                }

                PauseSinTimer();
                return true;
            }
        }   
        else {
            printf("\n[ERROR] <= EL ARCHIVO NO CUENTA CON EL FORMATO NECESARIO PARA LA CARGA DE PRODUCTOS!!\n\n");
            Pause();
            return false;
        }

    }
}

/*****Nombre***************************************
* VerificarDouble
*****Descripción**********************************
* Verifica que el string pasado por parametro sea un double
*****Retorno**************************************
* True si es un double, false si no 
*****Entradas*************************************
* Sin entradas
**************************************************/
bool VerificarDouble(char* texto) {
    char* puntero = texto;
    int contador = 0;
    while (*puntero != '\0') {
        if (!isdigit(*puntero) && *puntero != '.') {
            return false;
        }
        // Contar cada punto decimal
        if (*puntero == '.') {
            contador++;
        }
        puntero++;
    }
    if (contador > 1) {
        return false;
    }
    return true;
}

/*****Nombre***************************************
* SepararLineas
*****Descripción**********************************
* Separa un string en lineas de acuerdo a un delimitador
* y devuelve un array de strings con cada una de las lineas
*****Retorno**************************************
* char** - Array de strings con cada una de las lineas
*****Entradas*************************************
* char* - String a separar
* char* - Delimitador para separar el string
**************************************************/
char** SepararLineas(char* texto, char* delimitador) {
    char** lineas = (char**)malloc(sizeof(char*)*100);
    int i = 0;
    char* token = strtok(texto, delimitador);
    while (token != NULL) {
        lineas[i] = token;
        token = strtok(NULL, delimitador);
        i++;
    }
    return lineas;
}

/*****Nombre***************************************
* ContarCaracteres
*****Descripción**********************************
* Cuenta la cantidad de caracteres de un archivo
*****Retorno**************************************
* int - Cantidad de caracteres
*****Entradas*************************************
* FILE* - Archivo a contar
**************************************************/
int ContarCaracteres(FILE *archivo) {
    int cont = 0;
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
    //printf("%s",texto);
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
    int lineas = ContarLineas(texto);
    if (Revisar01_Comas(texto,cont)) {
        char arregloID[lineas][BUFFER];
        int posID = 0;
        int pos = 0;
        char palabra[BUFFER];
        bool bandera = true;
        for (int i = 0;i!=cont;i++) {
            if (bandera) {
                if (texto[i]!=',') {
                    palabra[pos] = texto[i];
                    pos++;
                }
                else {
                    palabra[pos] = '\0';
                    strcpy(arregloID[posID],palabra);
                    posID++;
                    pos = 0;
                    bandera = false;
                }
            }
            else if(texto[i] == '\n') {
                bandera = true;
            }
        }

        for (int i = 0;i!=lineas;i++) {
            for (int j = i+1;j!=lineas;j++) {
                if (strcmp(arregloID[i],arregloID[j])==0) {
                    return false;
                }
            }
        }

        return true;
    }
    return false;
}

/*****Nombre***************************************
* Revisar01_Comas
*****Descripción**********************************
* Revisa que el archivo tenga una cantidad de comas suficiente
*****Retorno**************************************
* bool - True si es correcto, false si no
*****Entradas*************************************
* char* - Texto a revisar
* int - Cantidad de comas permitidas
**************************************************/
bool Revisar01_Comas(char* texto, int cont) {
    int cantComas = 0;
    for (int i = 0;i!=cont;i++) {
        if (texto[i]==',') cantComas++;
        if (texto[i]=='\n' || texto[i] == '\0') {
            if (cantComas!=3) return false;
            cantComas = 0;
        }
    }
    return true;
}

/*****Nombre***************************************
* ContarLineas
*****Descripción**********************************
* Revisa que el texto tenga una cantidad de lineas suficiente
*****Retorno**************************************
* int - Cantidad de lineas del texto
*****Entradas*************************************
* char* - Texto a revisar
**************************************************/
int ContarLineas(char* texto) {
    int i = 0;
    int cantLineas = 0;
    while (texto[i]!='\0') {
        if (texto[i]=='\n') cantLineas++;
        i++;
    }
    return cantLineas;
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
void Menu_VI01() {
    char id[] = "001";
    Comercio comercio = ObtenerInfoComercio(id);
    printf("[CEDULA JURIDICA] = %s\n",comercio.cedulaJuridica);
    printf("[NOMBRE DEL LOCAL] = %S\n",comercio.nombre);
    printf("[TELEFONO] = %s\n",comercio.telefono);
    printf("[NUMERO DE SIGUIENTE FACTURA] = %d\n",comercio.numSigFactura);
}

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

/*****Nombre***************************************
* PauseSinTimer
*****Descripción**********************************
* Pausa la ejecucion del programa sin timer establecido
*****Retorno**************************************
* No tiene
*****Entradas*************************************
* No tiene
**************************************************/
void PauseSinTimer() {
    printf("[!] Presionar Cualquier tecla para para continuar...\n");
    getchar();
    getchar();
}