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
#include <time.h>
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
bool CargarFacturasDesdeBdd();
bool StringSoloConLetras(char* cadena);
bool CargarAreasDesdeBdd();
bool CargarEmpleadosDesdeBdd();
bool CargarProductosDesdeRuta();
bool CargarPlanillasDesdeBdd();
bool VerificarCedulaEmpleado(char* cedula_elegida);
bool CargarCargasSocialesDesdeBdd();
void CambiarValorDeCargaSocial();
bool VerificarDouble(char* texto);
bool CedulaFueElegida(char* cedula, EmpleadoConRol* empleados_elegidos, int cant_empleados_restantes);
char** SepararLineas(char* texto, char* delimitador);

void MenuEliminarNomina();
void MostrarListaDeEmpleados();
void ImprimirListaDeEmpleados(EmpleadoConRol* empleados_elegidos, int cantidad_empleados);
void EliminarEmpleadoDeNomina(char* cedula, EmpleadoConRol* empleados_elegidos, int cant_empleados_restantes);
void MostrarListaDeProductos();
void MostrarListaDeAreas();
void MostrarTodasLasNominasConSalarios();
void MostrarInformacionDelNegocio();
void MostrarTodasLasNominas();
void MenuRegistroVentaProducto();
void ConsultarFacturasCargadas();
Area ObtenerAreaProduccion(int id_area);
bool ExisteAreaProduccion(int id_area_produccion);

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
void AgregarEmpleadosANomina(int mes, int anio);
void AgregarEmpleadosANominaAux(Planilla plan, EmpleadoConRol* empleados_elegidos, int cant_empleados_restantes);
char* Leer(FILE* archivo, int cont);
bool Revisar01_Comas(char* texto, int cont);
int ContarCaracteres(FILE *archivo);
int ContarLineas(char* texto);
bool Revisar(char* texto, int cont);
bool VerificarNumero(char num[]);
bool ExisteNomina(int id_nomina);
bool EliminarNomina(int id_nomina);
void PauseSinTimer(int cantidadEnters);
void Pause();

LineaFactura* AgregarLineaFactura(LineaFactura* lineasFactura, int cant_prod_elegidos, Producto producto, int cantidad);

static Operador operador; // Operador del sistema
static Comercio comercio; // Comercio del sistema
static Producto* listaProductos; // Lista de productos del sistema
static Area* listaAreas; // Lista de areas del sistema
static EmpleadoConRol* listaEmpleados; // Lista de empleados del sistema
static CargoSocial cargoSocial; // Cargo social para los empleados
static int cantidadProductos; // Cantidad de productos del sistema
static int cantidadAreas; // Cantidad de areas del sistema
static int cantidadEmpleados; // Cantidad de empleados en el sistema
static PlanillaConCantEmpleados* planillas; // Lista de planillas del sistema

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

    // Cargas Sociales
    if (!CargarCargasSocialesDesdeBdd()) {
        return main();
    }
    printf("[OK] Cargas Sociales caMostrarTodasLasNominasrgadas desde base de datos\n");

    
    // Planillas - planillas del negocio (si hay alguna)
    CargarPlanillasDesdeBdd();

    // Productos - lista de productos del comercio (si encontró alguno)
    CargarProductosDesdeBdd();

    Menu_01();
    return 0;
}

bool CargarPlanillasDesdeBdd() {
    planillas = ObtenerPlanillasConCantEmpleados();
    if (planillas == NULL) {
        return false;
    }
    return true;
}

// Se carga la información de las cargas sociales de los empleados
bool CargarCargasSocialesDesdeBdd() {
    CargoSocial cargo = ObtenerCargoSocial((char*)"General");
    if (cargo.id == 0) {
        return false;
    }
    cargoSocial = cargo;
    return true;
}

// Se cargan de forma inicial los empleados de la bdd
bool CargarEmpleadosDesdeBdd() {
    listaEmpleados = ObtenerEmpleadosConRol();
    cantidadEmpleados = ObtenerCantidadEmpleados();
    if (listaEmpleados == NULL) {
        return false;
    }
    return true;
}

bool CargarFacturasDesdeBdd() {
    Factura* facturasBdd = ObtenerFacturasComercio((char*)"001");
    int cantidadFacturasBdd = ObtenerCantidadFacturas((char*)"001");
    if (facturasBdd == NULL || cantidadFacturasBdd == -1 ) {
        return false;
    }
    facturas = facturasBdd;
    cantidadFacturas = cantidadFacturasBdd;
    return true;
}

// Se cargan de forma inicial las areas del negocio de la bdd
bool CargarAreasDesdeBdd() {
    listaAreas = ObtenerAreas();
    cantidadAreas = ObtenerCantidadAreas();
    if (listaAreas == NULL) {
        return false;
    }
    return true;
}

// Se cargan de forma inicial los productos del comercio de la bdd
void CargarProductosDesdeBdd() {
    listaProductos = ObtenerProductos();
    cantidadProductos = ObtenerCantidadProductos();
}

// Se carga la informacion inicial del comercio en la bdd
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
            PauseSinTimer(2);
            break;
        case 3:
            MostrarListaDeEmpleados();
            PauseSinTimer(2);
            break;
        case 4:
            MostrarListaDeProductos();
            PauseSinTimer(2);
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

/*****Nombre***************************************
* MostrarListaDeProductos
*****Descripción**********************************
* Muestra toda la lista de productos en pantalla
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
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

                PauseSinTimer(2);
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
    while (*puntero != '\0' || *puntero != '\n') {
        if (*puntero == 10) {
            break;
        }
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
    printf("[MENU DE OPCIONES ADMINISTRATIVAS]\n");
    printf("1.Valores iniciales\n2.Registro de nomina\n3.Eliminar nomina\n4.Registro de venta de productos\n5.Consulta de nominas\n6.Consulta de ventas\n7.Balance anual\n0.Volver\n>");
    char opcion_01[BUFFER];
    scanf("%s",&opcion_01);
    if (!VerificarNumero(opcion_01)) {
        printf("[ERROR] = POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
        PauseSinTimer(2);
        Menu_OA1();
        return;
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
            planillas = ObtenerPlanillasConCantEmpleados();
            if (planillas == NULL) {
                printf("[ERROR] = NO HAY NOMINAS REGISTRADAS!\n");
                PauseSinTimer(2);
                Menu_OA1();
                return;
            }
            MenuEliminarNomina();
            break;
        case 4:
            break;
        case 5:
            planillas = ObtenerPlanillasConCantEmpleados();
            if (planillas == NULL) {
                printf("[ERROR] = NO HAY NOMINAS REGISTRADAS!\n");
                PauseSinTimer(2);
                Menu_OA1();
                return;
            }
            MostrarTodasLasNominasConSalarios();
            PauseSinTimer(2);
            break;
        case 6:
            Menu_CV01();
            break;
        case 7:
            Menu_BA01();
            break;
        case 0:
            Menu_01();
            break;
        default:
            printf("[ERROR] = POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
            Pause();
            Menu_OA1();
            break;
    }
}

void MenuEliminarNomina() {
    system("clear");
    printf("[ELIMINAR NOMINA]\n");

    MostrarTodasLasNominas();
    printf("\n------------------------------\n");
    printf("Ingrese el [numero] de la nomina a eliminar: ");
    printf("\n(O presione 0 para volver al menu anterior)");
    printf("\n>");
    char opcion_01[BUFFER];
    scanf("%s",&opcion_01);
    if (!VerificarNumero(opcion_01)) {
        printf("[ERROR] = POR FAVOR DIGITE UN NUMERO VALIDO!\n");
        PauseSinTimer(2);
        MenuEliminarNomina();
        return;
    }
    int opcion_02 = strtol(opcion_01,NULL,10);
    if (opcion_02 == 0) {
        Menu_OA1();
        return;
    }
    if (ExisteNomina(opcion_02)) {
        if (EliminarNomina(opcion_02)) {
            printf("[EXITO] = NOMINA (%i) ELIMINADA CON EXITO!\n", opcion_02);
            PauseSinTimer(2);
        }
        else {
            printf("[ERROR] = NO SE PUDO ELIMINAR LA NOMINA (%i)!\n", opcion_02);
            PauseSinTimer(2);
        }
    }
    else {
        printf("[ERROR] = NO EXISTE UNA NOMINA CON EL NUMERO (%i)!\n", opcion_02);
        PauseSinTimer(2);
        MenuEliminarNomina();
        return;
    }
}

bool ExisteNomina(int id_nomina) {
    int i = 0;
    while (planillas[i].id != -1) {
        if (planillas[i].id == id_nomina) return true;
        i++;
    }
    return false;
}

bool EliminarNomina(int id_nomina) {
    int i = 0;
    while (planillas[i].id != -1) {
        if (planillas[i].id == id_nomina) {
            Fecha fecha = planillas[i].fecha;
            if (EliminarPlanillaPorFecha(fecha)) {
                planillas = ObtenerPlanillasConCantEmpleados();
                return true;
            }
        }
        i++;
    }
    return false;
}

void MostrarTodasLasNominasConSalarios() {
    PlanillaConCantEmpleados* p = ObtenerPlanillasConCantEmpleados();
    if (p == NULL) {
        printf("[ERROR] = NO HAY NOMINAS REGISTRADAS!\n");
        PauseSinTimer(2);
        return;
    }
    int i = 0;
    printf("\n>>> Lista de nominas\n");
    while (p[i].id != -1) {
        printf("----------------------------------\n");
        printf("Nomina: %i\n", p[i].id);
        printf("Fecha: 1/%i/%i\n", p[i].fecha.mes, p[i].fecha.anio);
        printf("Cantidad de empleados: %i\n", p[i].cantidad_empleados);
        printf("Porcentaje de carga social: %.2f%\n", p[i].monto_carga_social * 100);
        printf("\n ~~ Empleados ~~\n");

        double subtotal_salarios = 0;
        double total_salarios = 0;

        for (int j = 0; j < p[i].cantidad_empleados; j++) {
            printf("\n\n");
            printf("Nombre: %s", listaEmpleados[j].nombre_completo);
            printf("\n");
            printf("Cedula: %s", listaEmpleados[j].cedula);
            printf("\n");
            printf("Sueldo: %.2f", listaEmpleados[j].salario_mensual);
            printf("\n");
            printf("Puesto: %s", listaEmpleados[j].nombre_rol);

            subtotal_salarios += listaEmpleados[j].salario_mensual;
            total_salarios += (listaEmpleados[j].salario_mensual * (p[i].monto_carga_social));
        }

        printf("\nSubtotal de salarios (sin cargas): %.2f\n", subtotal_salarios);
        printf("\nTotal de salarios (con cargas): %.2f\n\n", total_salarios);

        i++;
    }
}

void MostrarTodasLasNominas() {
    PlanillaConCantEmpleados* planillas = ObtenerPlanillasConCantEmpleados();
    printf("\n>>> Lista de nominas\n");
    printf("\n");
    int i = 0;
    while (planillas[i].id != -1) {
        printf(
            "[%i] Fecha: 1/%i/%i | Carga social: %.2f% | Cantidad de empleados: %i\n",
            planillas[i].id,
            planillas[i].fecha.mes,
            planillas[i].fecha.anio,
            (planillas[i].monto_carga_social * 100),
            planillas[i].cantidad_empleados
        );
        i++;
    }
}


void MenuRegistroVentaProducto(LineaFactura* lineasFactura, int cant_prod_elegidos) {
    system("clear");
    printf("[REGISTRO DE VENTA DE PRODUCTOS]\n");
    printf("Cantidad de productos elegidos: %d\n",cant_prod_elegidos);
    printf("\n>>> Productos disponibles <<<\n");
    MostrarListaDeProductos();
    if (cant_prod_elegidos > 0) {
        printf("\n----------------------------------------\n");
        printf(">>> Detalles de facturacion");
        printf("\n----------------------------------------\n");
        double subtotal = 0;
        double iva = 0;
        double total = 0;
        for (int i = 0;i < cant_prod_elegidos;i++) {
            char xd[51];
            strcpy(xd, lineasFactura[i].id_producto);
            Producto p = ObtenerProductoExistente((char*)xd);
            double subtotal_linea = (p.costo * lineasFactura[i].cantidad);
            double iva_linea = (p.costo * lineasFactura[i].cantidad) * (p.impuesto / 100);
            double total_linea = subtotal_linea + iva_linea;
            printf(
                "[%s / %15s] Cantidad: %5d\t | Subtotal: %10.2f\t | IVA(%.2f%): %10.2f\t | Total: %12.2f\n",
                p.id,
                p.nombre,
                lineasFactura[i].cantidad,
                subtotal_linea,
                p.impuesto,
                iva_linea,
                total_linea
            );
            subtotal += subtotal_linea;
            iva += iva_linea;
            total += total_linea;
        }
        printf("\n----------------------------------------\n");
        printf(">>> Totales (montos finales)\n");
        printf("Subtotal = %.2f | IVA = %.2f | Total = %.2f", subtotal, iva, total);
    }
    printf("\n-------------------------------------\n");
    printf("1.Agregar producto\n2.Eliminar producto\n3.Finalizar venta\n0.Cancelar\n>");
    char opcion_01[BUFFER];
    scanf("%s",&opcion_01);
    if (!VerificarNumero(opcion_01)) {
        printf("[ERROR] = POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
        PauseSinTimer(2);
        MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
        return;
    }
    int opcion_02 = strtol(opcion_01,NULL,10);

    char id_prod[BUFFER];
    char confirmacion[BUFFER];
    switch(opcion_02) {
        case 1:
            printf("[!] Digite el [id] del producto a agregar: ");
            scanf("%s",&id_prod);
            if (strlen(id_prod) == 0) {
                printf("[ERROR] = POR FAVOR DIGITE UN ID VALIDO\n");
                PauseSinTimer(2);
                MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
                return;
            }
            if (!ExisteProducto(id_prod)) {
                printf("[ERROR] = EL PRODUCTO NO EXISTE\n");
                PauseSinTimer(2);
                MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
                return;
            }
            printf("Id >>>> %s\n",id_prod);
            Producto prod = ObtenerProducto((char*)id_prod);
            if (strcmp(prod.id,"ND") == 0) {
                printf("[ERROR] = EL PRODUCTO NO EXISTE\n");
                PauseSinTimer(2);
                MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
                return;
            }
            printf("\n");
            printf("[!] Digite la cantidad de [%s] a agregar: ", prod.nombre);
            char cant_prod[BUFFER];
            scanf("%s",&cant_prod);
            if (strlen(cant_prod) == 0) {
                printf("[ERROR] = POR FAVOR DIGITE UNA CANTIDAD VALIDA\n");
                PauseSinTimer(2);
                MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
                return;
            }
            if (!VerificarNumero(cant_prod)) {
                printf("[ERROR] = POR FAVOR DIGITE UNA CANTIDAD VALIDA\n");
                PauseSinTimer(2);
                MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
                return;
            }  
            int cant_prod_int = strtol(cant_prod,NULL,10);
            if (cant_prod_int <= 0) {
                printf("[ERROR] = POR FAVOR DIGITE UNA CANTIDAD VALIDA\n");
                PauseSinTimer(2);
                MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
                return;
            }
            if (ExisteProductoEnLinea(lineasFactura, cant_prod_elegidos, prod)) {
                // Agregar repetido
                AgregarRepetidoLineaFactura(lineasFactura, cant_prod_elegidos, prod, cant_prod_int);
                printf("[!] SE AGREGARON (%i) MAS DE [%s] CON EXITO!\n", cant_prod_int, prod.nombre);
                PauseSinTimer(2);
                MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
                return;
            }
            // Agregar nuevo producto
            printf("[!] SE AGREGARON LAS  (%i) DE [%s] CON EXITO!\n", cant_prod_int, prod.nombre);
            lineasFactura = AgregarLineaFactura(lineasFactura, cant_prod_elegidos, prod, cant_prod_int);
            cant_prod_elegidos++;

            break;
        case 2: // Eliminar producto
            if (cant_prod_elegidos == 0) {
                printf("[ERROR] = NO HAY PRODUCTOS ELEGIDOS\n");
                PauseSinTimer(2);
                MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
                return;
            }
            printf("[!] Digite el [id] del producto a eliminar: ");
            scanf("%s",&id_prod);
            if (strlen(id_prod) == 0) {
                printf("[ERROR] = POR FAVOR DIGITE UN ID VALIDO\n");
                PauseSinTimer(2);
                MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
                return;
            }
            if (!ExisteProducto(id_prod)) {
                printf("[ERROR] = EL PRODUCTO NO EXISTE\n");
                PauseSinTimer(2);
                MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
                return;
            }
            Producto pro = ObtenerProducto((char*)id_prod);
            if (!ExisteProductoEnLinea(lineasFactura, cant_prod_elegidos, pro)) {
                printf("[ERROR] = EL PRODUCTO NO EXISTE EN LA LISTA DE ELEGIDOS\n");
                PauseSinTimer(2);
                MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
                return;
            }
            lineasFactura = EliminarLineaFactura(lineasFactura, cant_prod_elegidos, pro);
            cant_prod_elegidos--;
            printf("[!] SE ELIMINARON LAS [%s] CON EXITO!\n",pro.nombre);
            PauseSinTimer(2);
            MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
            return;
        case 3: // Cerrar venta
            if (cant_prod_elegidos == 0) {
                printf("[ERROR] = NO HAY PRODUCTOS ELEGIDOS\n");
                PauseSinTimer(2);
                MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
                return;
            }
            printf("[¿!?] ¿Esta seguro de cerrar la venta? (s/n): ");
            scanf("%s",&confirmacion);
            if (strlen(confirmacion) == 0) {
                printf("[ERROR] = POR FAVOR DIGITE UNA CONFIRMACION VALIDA\n");
                PauseSinTimer(2);
                MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
                return;
            }
            if (strcmp(confirmacion,"s") == 0 || strcmp(confirmacion,"S") == 0) {
                // Cerrar las ventas
                TerminarRegistroDeVenta(lineasFactura, cant_prod_elegidos);
                return;
            }
            break;
        case 0: // Cancelar
            if (cant_prod_elegidos == 0) {
                printf("[ERROR] SE CANCELO LA VENTA, VOLVIENDO AL MENU ANTERIOR\n");
                PauseSinTimer(2);
                Menu_OA1();
                return;
            }
            printf("[¿!!!?] SEGURO QUE DESEA CANCELAR LA VENTA? (s/n): ");
            scanf("%s",&confirmacion);
            if (strcmp(confirmacion,"s") == 0 || strcmp(confirmacion,"S") == 0) {
                printf("[!!!] SE CANCELO LA VENTA, VOLVIENDO AL MENU ANTERIOR\n");
                PauseSinTimer(2);
                Menu_OA1();
                return;
            }
            printf("[!] SE CONTINUA CON LA VENTA\n");
            break;
        default:
            printf("[ERROR] = POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
            break;
    }
    PauseSinTimer(2);
    MenuRegistroVentaProducto(lineasFactura, cant_prod_elegidos);
    return;
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
    system("clear");
    printf("[MENU DE VALORES INICIALES]\n");
    printf("1. Ver informacion del negocio\n");
    printf("2. Modificar el valor de las cargas sociales\n");
    printf("0. Volver\n");
    printf(">");

    char opcion_01[BUFFER];
    scanf("%s",&opcion_01);
    if (!VerificarNumero(opcion_01)) {
        printf("[ERROR] = POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
        PauseSinTimer(2);
        Menu_VI01();
        return;
    }
    int opcion_02 = strtol(opcion_01,NULL,10);
    switch(opcion_02) {
        case 1:
            MostrarInformacionDelNegocio();
            PauseSinTimer(2);
            break;
        case 2:
            CambiarValorDeCargaSocial();
            break;
        case 0:
            Menu_OA1();
            return;
        default:
            printf("[ERROR] = POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
            Pause();
            break;
    }
    Menu_VI01();
}

/*****Nombre***************************************
* CambiarValorDeCargaSocial
*****Descripción**********************************
* Cambia el valor de las cargas sociales con un
* valor nuevo digitado por el usuario
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void CambiarValorDeCargaSocial() {
    system("clear");
    printf("[CAMBIO DE VALOR DE CARGA SOCIAL]\n");
    printf(
        "[!] Valor actual porcentual: %.2f | %.2f%\n",
        cargoSocial.porcentaje,
        (cargoSocial.porcentaje * 100)
    );
    printf("[!] Ingrese el nuevo valor (porcentual), o presione enter para no cambiarlo: ");
    getchar();

    char opcion_01[BUFFER];
    fgets(opcion_01,BUFFER,stdin);

    printf("%s", opcion_01);
    if (opcion_01[0] == '\n' && opcion_01[1] == '\0') {
        printf("[!] No se ha cambiado el valor\n");
        PauseSinTimer(1);
        return;
    }
    if (!VerificarDouble(opcion_01)) {
        printf("[ERROR] = POR FAVOR DIGITE UN VALOR NUMERICO FLOTANTE!\n");
        PauseSinTimer(3);
        CambiarValorDeCargaSocial();
        return;
    }
    double valor = strtod(opcion_01,NULL);
    if (valor<1 || valor>5) {
        printf("[ERROR] = POR FAVOR DIGITE UN VALOR ENTRE 1 Y 5!\n");
        PauseSinTimer(2);
        CambiarValorDeCargaSocial();
        return;
    }
    cargoSocial.porcentaje = valor;
    ModificarCargoSocial((char*)"General", valor);
    printf("[!] Valor cambiado\n");
    PauseSinTimer(2);
}

/*****Nombre***************************************
* MostrarInformacionDelNegocio
*****Descripción**********************************
* Muestra la informacion del negocio en pantalla
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void MostrarInformacionDelNegocio() {
    printf("============[INFORMACION DEL COMERCIO]============\n");
    printf("Cedula del negocio: %s\n", comercio.cedulaJuridica);
    printf("Nombre del negocio: %s\n", comercio.nombre);
    printf("Telefono del negocio: %s\n", comercio.telefono);
    printf("Num. siguiente factura: %d\n", comercio.numSigFactura);

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
void Menu_RN01() {
    system("clear");
    printf("[REGISTRO DE NOMINAS]\n");
    
    char mes_nomina[3];
    printf("Ingrese el mes de la nomina: ");
    scanf("%s",&mes_nomina);
    if (!VerificarNumero(mes_nomina)) {
        printf("[ERROR] <= DEBE DIGITAR UN NUMERO PARA EL MES DE LA NOMINA!\n");
        PauseSinTimer(2);
        Menu_RN01();
        return;
    }
    int mes_nomina_int = strtol(mes_nomina,NULL,10);
    if (mes_nomina_int<1 || mes_nomina_int>12) {
        printf("[ERROR] <= DEBE DIGITAR UN NUMERO ENTRE 1 Y 12 PARA EL MES DE LA NOMINA!\n");
        PauseSinTimer(2);
        Menu_RN01();
        return;
    }

    char anio_nomina[3];
    printf("Ingrese el anio de la nomina: ");
    scanf("%s",&anio_nomina);
    if (!VerificarNumero(anio_nomina)) {
        printf("[ERROR] <= DEBE DIGITAR UN NUMERO PARA EL ANIO DE LA NOMINA!\n");
        PauseSinTimer(2);
        Menu_RN01();
        return;
    }
    int anio_nomina_int = strtol(anio_nomina,NULL,10);

    time_t tiempo;
    struct tm *tm;
    time(&tiempo);
    tm = localtime(&tiempo);

    int anio_actual = tm->tm_year + 1900;

    if (anio_nomina_int<(anio_actual-5) || anio_nomina_int>(anio_actual + 5)) {
        printf("[ERROR] <= DEBE DIGITAR UN NUMERO VALIDO PARA EL ANIO DE LA NOMINA!\n");
        printf("(Sin exceder de 5 anios antes y 5 anios despues del anio actual)\n");
        PauseSinTimer(2);
        Menu_RN01();
        return;
    }

    AgregarEmpleadosANomina(mes_nomina_int, anio_nomina_int);
}

/*****Nombre***************************************
* AgregarEmpleadosANomina
*****Descripción**********************************
* Agrega los empleados a la nomina, este primer
* menu es para agregar definir y registrar una
* nomina para posteriormente poder agregar los
* empleados a la nomina
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* int mes: mes de la nomina
* int anio: anio de la nomina
**************************************************/
void AgregarEmpleadosANomina(int mes, int anio) {
    system("clear");
    printf("[REGISTRO DE NOMINAS]\n");
    printf("Fecha (mes: %d/ anio: %d)\n", mes, anio);
    printf("Monto porcentual de cargas sociales: %.2f%\n", cargoSocial.porcentaje * 100);
    printf("¿Los valores son correctos? (s/n): ");
    char opcion_01[BUFFER];
    scanf("%s",opcion_01);
    if (strcmp(opcion_01,"s") == 0 || strcmp(opcion_01,"S") == 0) {
        Fecha fecha_planilla = {
            .dia = 1,
            .mes = mes,
            .anio = anio
        };

        Planilla plan = ObtenerPlanillaPorFecha(fecha_planilla);
        if (plan.id != -1) {
            printf("[ERROR] <= UNA PLANILLA YA SE ENCUENTRA REGISTRADA EN ESA FECHA!\n");
            printf("id planilla: %d\n", plan.id);
            Fecha f = plan.fecha;
            printf("fecha: %d/%d/%d\n", f.dia, f.mes, f.anio);
            PauseSinTimer(2);
            Menu_RN01();
            return;
        }

        plan.fecha = fecha_planilla;
        
        // La planilla no existe, se procede a crear.
        if (RegistrarPlanilla(plan)) {
            plan = ObtenerPlanillaPorFecha(fecha_planilla);
            EmpleadoConRol* empleados_elegidos = malloc(cantidadEmpleados * sizeof *empleados_elegidos);
            MostrarListaDeEmpleados();
            AgregarEmpleadosANominaAux(plan, empleados_elegidos, cantidadEmpleados);
            return;
        }
        printf("[ERROR] <= NO SE PUDO REGISTRAR LA PLANILLA!\n");
        PauseSinTimer(2);
        Menu_RN01();
        return;
    }
    if (strcmp(opcion_01,"n") == 0 || strcmp(opcion_01,"N") == 0) {
        printf("[!] Se cancelo el registro, volviendo al menu anterior\n");
        PauseSinTimer(2);
        Menu_RN01();
        return;
    }
    printf("[ERROR] <= DEBE DIGITAR S O N PARA CONFIRMAR O NO CONFIRMAR LA FECHA!\n");
    PauseSinTimer(2);
    Menu_RN01();
}

void ImprimirProductosElegidos(LineaFactura* lineasFactura, int cant_prod_elegidos) {
    printf("cantidad de productos elegidos: %d\n",cant_prod_elegidos);
    for (int i = 0; i < cant_prod_elegidos; i++) {
        printf("[%s] - Cantidad: %d\n", lineasFactura[i].id_producto, lineasFactura[i].cantidad);
    }
}


bool ExisteProductoEnLinea(LineaFactura* lineasFactura, int cant_prod_elegidos, Producto producto) {
    for (int i = 0; i < cant_prod_elegidos; i++) {
        if (strcmp(lineasFactura[i].id_producto, producto.id) == 0) {
            return true;
        }
    }
    return false;
}

void AgregarRepetidoLineaFactura(LineaFactura* lineasFactura, int cant_prod_elegidos, Producto producto, int cant_prod_int) {
    for (int i = 0; i < cant_prod_elegidos; i++) {
        if (strcmp(lineasFactura[i].id_producto, producto.id) == 0) {
            lineasFactura[i].cantidad += cant_prod_int;
            return;
        }
    }
}

/*****Nombre***************************************
* AgregarEmpleadosANominaAux
*****Descripción**********************************
* Una vez que se ha registrado la planilla o nomina,
* se procede a agregar los empleados a la nomina
* se les va a mostrar la lista de empleados registrados en
* el sistema y se le va a pedir que seleccione los empleados
* que se van a agregar a la nomina. Tambien, podra elegir
* empleados previamente elegidos para eliminarlos de la nomina.
* Y finalmente, registrar la nomina o cancelar el registro.
*
* El objetivo de esta funcion recursiva es que se puedan
* ir agregando los empleados en la lista que es pasada por
* referencia y que esta se pueda imprimir y actualizar a
* medida que el usuario lo desee
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Planilla plan: planilla a la cual se le van a agregar los empleados
* EmpleadoConRol* empleados_elegidos: lista de empleados elegidos para la nomina (se va modificando)
* int cantidad_empleados: cantidad de empleados que quedan por agregar a la nomina (servira para la recursion)
**************************************************/
void AgregarEmpleadosANominaAux(Planilla plan, EmpleadoConRol* empleados_elegidos, int cant_empleados_restantes) {
    system("clear");
    printf("[REGISTRO DE NOMINAS]\n");
    printf("[PLANILLA %d/%d]\n", plan.fecha.mes, plan.fecha.anio);
    printf("TOTAL DE EMPLEADOS: %d\n", cantidadEmpleados);
    printf("EMPLEADOS RESTANTES: %d\n", cant_empleados_restantes);

    MostrarListaDeEmpleados();
    printf("\n >>> Empleados elegidos <<<\n");
    ImprimirListaDeEmpleados(empleados_elegidos, cantidadEmpleados - cant_empleados_restantes);
    printf("----------------------------------------------------\n");
    printf("1. Agregar empleado |");
    printf("2. Eliminar empleado | ");
    printf("3. Terminar y registrar nomina |");
    printf("0. Cancelar");

    printf("\n>");
    char opcion_01[BUFFER];
    scanf("%s",opcion_01);
    if (VerificarNumero(opcion_01)) {
        int opcion_01_int = strtol(opcion_01,NULL,10);
        if (opcion_01_int == 1) {
            char cedula_elegida[BUFFER];
            printf("Ingrese la cedula del empleado: ");
            scanf("%s",cedula_elegida);
            if (VerificarCedulaEmpleado(cedula_elegida)) {
                // Verificar que no haya sido elegida
                int aux = cantidadEmpleados - cant_empleados_restantes;
                if (CedulaFueElegida(cedula_elegida, empleados_elegidos, aux)) {
                    printf("[ERROR] <= LA CEDULA YA FUE ELEGIDA!\n");
                    PauseSinTimer(2);
                    AgregarEmpleadosANominaAux(plan, empleados_elegidos, cant_empleados_restantes);
                    return;
                }
                EmpleadoConRol empleado_elegido = ObtenerEmpleadoConRol(cedula_elegida);
                if (strcmp(empleado_elegido.cedula,"ND") != 0) {
                    int aux = cantidadEmpleados - cant_empleados_restantes;
                    empleados_elegidos[aux] = empleado_elegido;
                    cant_empleados_restantes--;
                    AgregarEmpleadosANominaAux(plan, empleados_elegidos, cant_empleados_restantes);
                    return;
                }
            }
            printf("[ERROR] <= EL EMPLEADO NO EXISTE!\n");
            PauseSinTimer(1);
            AgregarEmpleadosANominaAux(plan, empleados_elegidos, cant_empleados_restantes);
            return;
        }
        if (opcion_01_int == 2) {
            char cedula_elegida[BUFFER];
            printf("Ingrese la cedula del empleado: ");
            scanf("%s",cedula_elegida);
            if (VerificarCedulaEmpleado(cedula_elegida)) {
                int aux = cantidadEmpleados - cant_empleados_restantes;
                if (CedulaFueElegida(cedula_elegida, empleados_elegidos, aux)) {
                    EliminarEmpleadoDeNomina(cedula_elegida, empleados_elegidos, cantidadEmpleados - cant_empleados_restantes);
                    cant_empleados_restantes++;
                    AgregarEmpleadosANominaAux(plan, empleados_elegidos, cant_empleados_restantes);
                    return;
                }
                printf("[ERROR] <= LA CEDULA NO FUE ELEGIDA!\n");
                PauseSinTimer(2);
                AgregarEmpleadosANominaAux(plan, empleados_elegidos, cant_empleados_restantes);
            }
            printf("[ERROR] <= EL EMPLEADO NO EXISTE!\n");
            PauseSinTimer(2);
            AgregarEmpleadosANominaAux(plan, empleados_elegidos, cant_empleados_restantes);
            return;
        }
        if (opcion_01_int == 3) {
            int cant_empleados_elegidos = cantidadEmpleados - cant_empleados_restantes;
            system("clear");
            if (cant_empleados_elegidos == 0) {
                printf("[ERROR] <= NO SE HA ELEGIDO NINGUN EMPLEADO!\n");
                PauseSinTimer(2);
                AgregarEmpleadosANominaAux(plan, empleados_elegidos, cant_empleados_restantes);
                return;
            }
            printf("[REGISTRO DE NOMINAS]\n");
            printf("[PLANILLA %d/%d]\n", plan.fecha.mes, plan.fecha.anio);
            printf("CANT. EMPLEADOS ELEGIDOS: %d\n", cant_empleados_elegidos);
            printf("-------------------------------");
            printf("\n >>> Empleados elegidos <<<\n");
            ImprimirListaDeEmpleados(empleados_elegidos, cant_empleados_elegidos);
            printf("\n-------------------------------");
            char confirmacion[BUFFER];
            printf("[!] ¿Esta seguro de que desea registrar la nomina? (s/n): ");
            printf("\n> ");
            scanf("%s",confirmacion);
            if (strcmp(confirmacion, "s") == 0 || strcmp(confirmacion, "S")) {
                // Registro de empleados en la bdd
                bool exito = true;
                for (int i = 0; i < cant_empleados_elegidos; i++) {
                    if (RegistrarEmpleadoAPlanilla(plan.fecha, empleados_elegidos[i].cedula)) {
                        continue;
                    }
                    exito = false;
                    break;
                }
                if (exito) {
                    printf(
                        "\n[EXITO] <= LOS %d EMPLEADOS FUERON REGISTRADOS EN LA NOMINA!\n",
                        cant_empleados_elegidos
                    );
                    PauseSinTimer(2);
                    system("clear");
                    return;
                }
                printf("[ERROR] <= NO SE PUDO REGISTRAR LA NOMINA CORRECTAMENTE!\n");
                EliminarPlanillaPorFecha(plan.fecha);
                PauseSinTimer(2);
            }
            AgregarEmpleadosANominaAux(plan, empleados_elegidos, cant_empleados_restantes);
            return;
        }
        if (opcion_01_int == 0) {
            printf("[!] Se cancelo el registro, volviendo al menu anterior\n");
            EliminarPlanillaPorFecha(plan.fecha);
            PauseSinTimer(2);
            Menu_RN01();
            return;
        }
        printf("[ERROR] <= DEBE DIGITAR UN NUMERO ENTRE 0 Y 3 PARA ELEGIR UNA OPCION!\n");
        PauseSinTimer(2);
        AgregarEmpleadosANominaAux(plan, empleados_elegidos, cant_empleados_restantes);
        return;
    }
}

/*****Nombre***************************************
* EliminarEmpleadoDeNomina
*****Descripción**********************************
* Elimina un empleado de la lista de empleados elegidos para la nomina
* utilizando la cedula del empleado como parametro
*****Retorno**************************************
* Sin retorno
*****Entradas*************************************
* char* cedula_elegida: cedula del empleado a eliminar
* EmpleadoConRol* empleados_elegidos: lista de empleados elegidos para la nomina
* int cant_empleados_restantes: cantidad de empleados restantes para la nomina
**************************************************/
void EliminarEmpleadoDeNomina(char* cedula, EmpleadoConRol* empleados_elegidos, int cant_empleados_elegidos) {
    EmpleadoConRol* nuevos_empleados = malloc(cantidadEmpleados * sizeof *nuevos_empleados);
    int indice = 0;

    for (int i = 0; i < cant_empleados_elegidos; i++) {
        if (strcmp(empleados_elegidos[i].cedula,cedula) == 0) {
            continue;
        }
        EmpleadoConRol emp;
        strcpy(emp.cedula, empleados_elegidos[i].cedula);
        strcpy(emp.nombre_completo, empleados_elegidos[i].nombre_completo);
        strcpy(emp.nombre_rol, empleados_elegidos[i].nombre_rol);
        emp.salario_mensual = empleados_elegidos[i].salario_mensual;
        nuevos_empleados[indice] = emp;
        indice++;
    }
    empleados_elegidos = nuevos_empleados;
}

bool StringSoloConLetras(char* cadena) {
    for (int i = 0;i < strlen(cadena);i++) {
        if (cadena[i] == ' ' || cadena[i] == '\n') {
            continue;
        }
        if (!isalpha(cadena[i]) && cadena[i] != ' ') {
            return false;
        }
    }
    return true;
}


void ConsultarFacturasCargadas() {
    printf("==================[ FACTURAS ]==================\n");
    for (int i = 0; i < cantidadFacturas; i++) {
        printf("Id de la factura: %i", facturas[i].id_factura);
        printf("\nNombre del comercio: %s", facturas[i].nombre_comercio);
        printf("\nCedula del comercio: %s", facturas[i].cedula_comercio);
        printf("\nTelefono del comercio: %s", facturas[i].telefono_comercio);
        printf("\nNombre del cliente: %s", facturas[i].nombre_cliente);
        printf("\nFecha de la factura: %i-%i-%i", facturas[i].fecha_facturacion.dia, facturas[i].fecha_facturacion.mes, facturas[i].fecha_facturacion.anio);
        printf("\nNombre del area de produccion: %s", facturas[i].nombre_area);
        printf("\n\n");
        
        DetalleFactura* detalleFactura = ObtenerDetallesFactura(facturas[i].id_factura);
        int cantidad_detalles = ObtenerCantidadDeDetalles(facturas[i].id_factura);

        printf("~~~~~~~~~~~~~~[ LINEAS DE FACTURA ]~~~~~~~~~~~~~\n");
        for (int j = 0; j < cantidad_detalles; j++) {
            Producto p = detalleFactura[j].producto;
            printf(
                "[%2i] %15s | Cantidad: %8i | Costo: %10.2f | IVA(%2.2f%): %10.2f |  Total: %10.2f\n", 
                j,
                p.nombre,
                detalleFactura[j].cantidad,
                (p.costo * detalleFactura[j].cantidad),
                (p.impuesto / 100),
                (p.costo * detalleFactura[j].cantidad) * (p.impuesto / 100),
                (p.costo * detalleFactura[j].cantidad) + ((p.costo * detalleFactura[j].cantidad) * (p.impuesto / 100))
            );
            printf("-------------------------------------------------------------------------------------------\n");
        }
        printf("\n");
        printf("Subtotal de factura: %10.2f\n", facturas[i].subtotal);
        printf("IVA de factura: %10.2f\n", facturas[i].impuesto);
        printf("Total de factura: %10.2f\n", facturas[i].total);
        printf("**********************************************************\n");
        printf("**********************************************************\n");
    }
}

/*****Nombre***************************************
* CedulaFueElegida
*****Descripción**********************************
* Pregunta si la cedula fue elegida para la nomina
* utilizando la cedula del empleado como parametro
*****Retorno**************************************
* Un valor booleano que indica si la cedula fue elegida
* true: si la cedula fue elegida
* false: si la cedula no fue elegida
*****Entradas*************************************
* char* cedula_elegida: cedula del empleado a verificar
* EmpleadoConRol* empleados_elegidos: lista de empleados elegidos para la nomina
**************************************************/
bool CedulaFueElegida(char* cedula, EmpleadoConRol* empleados_elegidos, int cant_empleados_restantes) {
    for (int i = 0; i < cant_empleados_restantes; i++) {
        if (strcmp(empleados_elegidos[i].cedula, cedula) == 0) {
            return true;
        }
    }
    return false;
}

LineaFactura* EliminarLineaFactura(LineaFactura* lineasFactura, int cant_prod_elegidos, Producto prod) {
    LineaFactura* lineasNuevas = malloc((cant_prod_elegidos-1) * sizeof(LineaFactura));
    int i = 0;
    int j = 0;
    while (i < cant_prod_elegidos) {
        if (strcmp(lineasFactura[i].id_producto, prod.id) != 0) {
            LineaFactura lf;
            strcpy(lf.id_producto, lineasFactura[i].id_producto);
            lf.cantidad = lineasFactura[i].cantidad;
            lineasNuevas[j] = lf;
            j++;
        }
        i++;
    }
    free(lineasFactura);
    return lineasNuevas;
}

/*****Nombre***************************************
* VerificarCedulaEmpleado
*****Descripción**********************************
* Verifica que la cedula del empleado exista en el sistema
*****Retorno**************************************
* Un valor booleano que indica si la cedula existe
* true: si la cedula existe
* false: si la cedula no existe
*****Entradas*************************************
* char *cedula: cedula del empleado a verificar
**************************************************/
bool VerificarCedulaEmpleado(char* cedula_elegida) {
    for (int i = 0; i < cantidadEmpleados; i++) {
        if (strcmp(listaEmpleados[i].cedula, cedula_elegida) == 0) {
            return true;
        }
    }
    return false;
}

Area ObtenerAreaProduccion(int id_area) {
    Area area;
    for (int i = 0;i < cantidadAreas;i++) {
        if (listaAreas[i].id == id_area) {
            area.id = listaAreas[i].id;
            strcpy(area.nombre, listaAreas[i].nombre);
            area.dimensiones = listaAreas[i].dimensiones;
            strcpy(area.producto_principal, listaAreas[i].producto_principal);
        }
    }
    return area;
}
bool ExisteAreaProduccion(int id_area_produccion) {
    for (int i = 0;i < cantidadAreas;i++) {
        if (listaAreas[i].id == id_area_produccion) {
            return true;
        }
    }
    return false;
}


Producto ObtenerProductoExistente(char* xd) {
    Producto p;
    for (int i = 0; i < cantidadProductos; i++) {
        if (strcmp(listaProductos[i].id, xd) == 0) {
            strcpy(p.id, listaProductos[i].id);
            strcpy(p.nombre, listaProductos[i].nombre);
            p.costo = listaProductos[i].costo;
            p.impuesto = listaProductos[i].impuesto;
            return p;
        }
    }
}


/*****Nombre***************************************
* ImprimirListaDeEmpleados
*****Descripción**********************************
* Muestra en pantalla a una lista de empleados
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* EmpleadoConRol* empleados_elegidos: lista de empleados
**************************************************/
void ImprimirListaDeEmpleados(EmpleadoConRol* empleados_elegidos, int cantidad_empleados) {
    for (int i = 0; i < cantidad_empleados; i++) {
        EmpleadoConRol empleado = empleados_elegidos[i];
        printf(
            " [%2i] Cedula: %9s\t | Nombre: %30s\t | Rol: %s\t | Salario: %.2f\n",
            i+1,
            empleado.cedula,
            empleado.nombre_completo,
            empleado.nombre_rol,
            empleado.salario_mensual
        );
    }
}

LineaFactura* AgregarLineaFactura(LineaFactura* lineasFactura, int cant_prod_elegidos, Producto producto, int cantidad) {
    LineaFactura* lineasNuevas = malloc((cant_prod_elegidos+1) * sizeof(LineaFactura));
    for (int i = 0; i < cant_prod_elegidos; i++) {
        lineasNuevas[i] = lineasFactura[i];
    }
    LineaFactura lf;
    strcpy(lf.id_producto, producto.id);
    lf.cantidad = cantidad;
    lineasNuevas[cant_prod_elegidos] = lf;
    free(lineasFactura);
    return lineasNuevas;
}


void TerminarRegistroDeVenta(LineaFactura* lineasFactura, int cant_prod_elegidos) {
    system("clear");
    printf("\n----------------------------------------\n");
    printf(">>> Detalles de facturacion");
    printf("\n----------------------------------------\n");
    double subtotal = 0;
    double iva = 0;
    double total = 0;
    for (int i = 0;i < cant_prod_elegidos;i++) {
        char xd[51];
        strcpy(xd, lineasFactura[i].id_producto);
        Producto p = ObtenerProductoExistente((char*)xd);
        double subtotal_linea = (p.costo * lineasFactura[i].cantidad);
        double iva_linea = (p.costo * lineasFactura[i].cantidad) * (p.impuesto / 100);
        double total_linea = subtotal_linea + iva_linea;
        printf(
            "[%s / %15s] Cantidad: %5d\t | Subtotal: %10.2f\t | IVA(%.2f%): %10.2f\t | Total: %12.2f\n",
            p.id,
            p.nombre,
            lineasFactura[i].cantidad,
            subtotal_linea,
            p.impuesto,
            iva_linea,
            total_linea
        );
        subtotal += subtotal_linea;
        iva += iva_linea;
        total += total_linea;
    }
    printf("\n----------------------------------------\n");
    printf(">>> Totales (montos finales)\n");
    printf("Subtotal = %.2f | IVA = %.2f | Total = %.2f", subtotal, iva, total);
    printf("\n----------------------------------------\n");
    char nombre_cliente[51];
    printf("\n[!] Digite el nombre con espacios del cliente: ");

    fflush(stdin);
    scanf(" %[^\n]s",&nombre_cliente);
    printf("\n");

    if (strlen(nombre_cliente) == 0 || strlen(nombre_cliente) > 50) {
        printf("[ERROR] = POR FAVOR DIGITE UN NOMBRE VALIDO\n");
        PauseSinTimer(2);
        TerminarRegistroDeVenta(lineasFactura, cant_prod_elegidos);
        return;
    }

    if (StringSoloConLetras(nombre_cliente) == 0) {
        printf("[ERROR] = POR FAVOR DIGITE UN NOMBRE VALIDO\n");
        PauseSinTimer(2);
        TerminarRegistroDeVenta(lineasFactura, cant_prod_elegidos);
        return;
    }

    int id_area_produccion = 0;
    printf("\n");
    MostrarListaDeAreas();

    printf("\n[!] Digite el id del area de produccion: ");
    scanf("%d",&id_area_produccion);
    if (id_area_produccion == 0) {
        printf("[ERROR] = POR FAVOR DIGITE UN ID VALIDO\n");
        PauseSinTimer(2);
        TerminarRegistroDeVenta(lineasFactura, cant_prod_elegidos);
        return;
    }

    if (!ExisteAreaProduccion(id_area_produccion)) {
        printf("[ERROR] = POR FAVOR DIGITE UN ID VALIDO\n");
        PauseSinTimer(2);
        TerminarRegistroDeVenta(lineasFactura, cant_prod_elegidos);
        return;
    }
    Area area_elegida = ObtenerAreaProduccion(id_area_produccion);

    int dia_factura = 0;
    int mes_factura = 0;
    int anio_factura = 0;

    printf("\n[!] Digite el dia de la factura: ");
    scanf("%d",&dia_factura);
    printf("\n[!] Digite el mes de la factura: ");
    scanf("%d",&mes_factura);
    printf("\n[!] Digite el anio de la factura: ");
    scanf("%d",&anio_factura);
    if (dia_factura > 31 || dia_factura < 1 || mes_factura > 12 || mes_factura < 1 || anio_factura > 2050 || anio_factura < 2000) {
        printf("[ERROR] = POR FAVOR DIGITE UNA FECHA VALIDA\n");
        PauseSinTimer(2);
        TerminarRegistroDeVenta(lineasFactura, cant_prod_elegidos);
        return;
    }

    Factura fac;
    Fecha fecha_factura;
    fecha_factura.dia = dia_factura;
    fecha_factura.mes = mes_factura;
    fecha_factura.anio = anio_factura;
    
    strcpy(fac.nombre_comercio, comercio.nombre);
    strcpy(fac.cedula_comercio, comercio.cedulaJuridica);
    strcpy(fac.telefono_comercio, comercio.telefono);
    strcpy(fac.nombre_cliente, nombre_cliente);
    strcpy(fac.nombre_area, area_elegida.nombre);
    fac.fecha_facturacion = fecha_factura;
    fac.id_area = id_area_produccion;
    fac.subtotal = subtotal;
    fac.impuesto = iva;
    fac.total = total;

    int id_factura = 0;
    id_factura = RegistrarFactura(fac);
    if (id_factura == 0) {
        printf("[ERROR] = POR FAVOR INTENTE NUEVAMENTE\n");
        PauseSinTimer(2);
        TerminarRegistroDeVenta(lineasFactura, cant_prod_elegidos);
        return;
    }

    Factura factura_registrada = ObtenerFactura(id_factura);

    // Registrar todos los productos a la factura
    RegistrarDetallesFacturaEnBdd(lineasFactura, cant_prod_elegidos, id_factura);

    system("clear");
    printf("============ [FACTURA] ============\n");
    printf("[i] Cedula juridica del comercio: %s", comercio.cedulaJuridica);
    printf("\n[i] Nombre del comercio: %s", comercio.nombre);
    printf("\n[i] Telefono del comercio: %s", comercio.telefono);
    printf("\n[i] Area de produccion: %s", area_elegida.nombre);
    printf("\n[i] Nombre del cliente: %s", nombre_cliente);
    printf("\n[i] Fecha de la factura: %d/%d/%d", dia_factura, mes_factura, anio_factura);
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    subtotal = 0;
    iva = 0;
    total = 0;
    for (int i = 0;i < cant_prod_elegidos;i++) {
        char xd[51];
        strcpy(xd, lineasFactura[i].id_producto);
        Producto p = ObtenerProductoExistente((char*)xd);
        double subtotal_linea = (p.costo * lineasFactura[i].cantidad);
        double iva_linea = (p.costo * lineasFactura[i].cantidad) * (p.impuesto / 100);
        double total_linea = subtotal_linea + iva_linea;
        printf(
            "[%s / %15s] Cantidad: %5d\t | Subtotal: %10.2f\t | IVA(%.2f%): %10.2f\t | Total: %12.2f\n",
            p.id,
            p.nombre,
            lineasFactura[i].cantidad,
            subtotal_linea,
            p.impuesto,
            iva_linea,
            total_linea
        );
        subtotal += subtotal_linea;
        iva += iva_linea;
        total += total_linea;
    }
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf(">>> Totales (montos finales)\n");
    printf("Subtotal = %.2f | IVA = %.2f | Total = %.2f", subtotal, iva, total);
    printf("\n==============================================\n");
    CargarFacturasDesdeBdd();
    PauseSinTimer(2);
}


void RegistrarDetallesFacturaEnBdd(LineaFactura* lineasFactura, int cant_prod_elegidos, int id_factura) {
    for (int i = 0; i < cant_prod_elegidos; i++) {
        char id_producto[51];
        strcpy(id_producto, lineasFactura[i].id_producto);
        Producto prod = ObtenerProducto((char*)id_producto);
        RegistrarDetalleFactura(id_factura, prod, lineasFactura[i].cantidad);
    }
}
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
* Sin retorno
*****Entradas*************************************
* int cantidadEnters - cantidad de enters a requeridos
**************************************************/
void PauseSinTimer(int cantidadEnters) {
    printf("[!] Presionar Cualquier tecla para para continuar...\n");
    for (int i=0;i<cantidadEnters;i++) {
        getchar();
    }
}