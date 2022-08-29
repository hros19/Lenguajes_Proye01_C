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
bool CargarFacturasDesdeBdd();
bool FacturaExiste(int idFactura);
void CargarProductosDesdeBdd();
bool CargarAreasDesdeBdd();
bool StringSoloConLetras(char* cadena);
bool CargarEmpleadosDesdeBdd();
bool CargarProductosDesdeRuta();
bool NumPerteneceALista(int* lista, int cantidad, int numero);
void MostrarReporteFacturaAnual(int anio);
bool CargarPlanillasDesdeBdd();
bool VerificarCedulaEmpleado(char* cedula_elegida);
bool CargarCargasSocialesDesdeBdd();
void CambiarValorDeCargaSocial();
void MostrarRendimientoDeAreaPorFecha();
void MostrarReportePlanillaAnual(int anio);
bool VerificarDouble(char* texto);
bool CedulaFueElegida(char* cedula, EmpleadoConRol* empleados_elegidos, int cant_empleados_restantes);
char** SepararLineas(char* texto, char* delimitador);
void MostrarReporteAnualCompleto(int anio);
void MenuEliminarNomina();
void MostrarListaDeEmpleados();
void ImprimirListaDeEmpleados(EmpleadoConRol* empleados_elegidos, int cantidad_empleados);
void EliminarEmpleadoDeNomina(char* cedula, EmpleadoConRol* empleados_elegidos, int cant_empleados_restantes);
void MostrarListaDeProductos();
void MostrarListaDeAreas();
void MostrarTodasLasNominasConSalarios();
void MostrarInformacionDelNegocio();
void MostrarTodasLasNominas();
void MostrarNominaEspecifica(int idNomina);
void ConsultarFacturasCargadas();

void Menu_01();
void Menu_OP1();
void Menu_OP2();
void MostrarTodasLasNominasResumidas();
void MostrarFacturaEspecifica(int idFactura);
void Menu_OA1();
void Menu_VI01();
void Menu_RN01();
void Menu_RV01();
void Menu_CN();
void Menu_CV01();
void Menu_BA01();

void MostrarFacturasResumidas();
void AgregarEmpleadosANomina(int mes, int anio);
void AgregarEmpleadosANominaAux(Planilla plan, EmpleadoConRol* empleados_elegidos, int cant_empleados_restantes);
Area ObtenerAreaProduccion(int id_area);
char* Leer(FILE* archivo, int cont);
bool Revisar01_Comas(char* texto, int cont);
int ContarCaracteres(FILE *archivo);
int ContarLineas(char* texto);

int ObtenerCantidadPlanillasPorMes(PlanillaConCantEmpleados* listaEmpleadosAnual, int cantidadEmpleadosAnual, int mes);
int* InterseccionListas(int* listaA, int largoA, int* listaB, int largoB);
bool Revisar(char* texto, int cont);
bool VerificarNumero(char num[]);
bool ComasJuntas(char* puntero);
bool ExisteAreaProduccion(int id_area_produccion);
bool ExisteNomina(int id_nomina);
bool EliminarNomina(int id_nomina);
void PauseSinTimer(int cantidadEnters);
void Pause();

static Factura* facturas; // Array de facturas
static Operador operador; // Operador del sistema
static Comercio comercio; // Comercio del sistema
static Producto* listaProductos; // Lista de productos del sistema
static Area* listaAreas; // Lista de areas del sistema
static EmpleadoConRol* listaEmpleados; // Lista de empleados del sistema
static CargoSocial cargoSocial; // Cargo social para los empleados
static int cantidadProductos; // Cantidad de productos del sistema
static int cantidadAreas; // Cantidad de areas del sistema
static int cantidadEmpleados; // Cantidad de empleados en el sistema
static int cantidadFacturas; // Cantidad de facturas en el sistema
static PlanillaConCantEmpleados* planillas; // Lista de planillas del sistema

// Struct definido aqui ya que no tiene sentido definirlo en otro archivo
struct lineaFactura {
    char id_producto[51];
    int cantidad;
};

typedef struct lineaFactura LineaFactura;

LineaFactura* AgregarLineaFactura(LineaFactura* lineasFactura, int cant_prod_elegidos, Producto producto, int cantidad);
void MenuRegistroVentaProducto(LineaFactura* lineasFactura, int cant_prod_elegidos);
void ImprimirProductosElegidos(LineaFactura* lineasFactura, int cant_prod_elegidos);
bool ExisteProductoEnLinea(LineaFactura* lineasFactura, int cant_prod_elegidos, Producto producto);
void AgregarRepetidoLineaFactura(LineaFactura* lineasFactura, int cant_prod_elegidos, Producto producto, int cant_prod_int);
LineaFactura* EliminarLineaFactura(LineaFactura* lineasFactura, int cant_prod_elegidos, Producto prod);
Producto ObtenerProductoExistente(char* xd);
void TerminarRegistroDeVenta(LineaFactura* lineasFactura, int cant_prod_elegidos);
void RegistrarDetallesFacturaEnBdd(LineaFactura* lineasFactura, int cant_prod_elegidos, int id_factura);
int ObtenerCantidadFacturasPorMes(Factura* listaFacturasAnual, int cantidadFacturasAnual, int mes);
Factura* ObtenerFacturasDeUnMes(Factura* facturaAnual, int cantidadFacturasDelAnio, int mes);
void MostrarReportePorMesDeAnio(int anio);

/*****Nombre***************************************
* main
*****Descripción y objetivo**********************************
* Metodo de inicio del programa con el objetivo
* de cargar los datos iniciales del sistema y 
* mostrar el menu principal.
*****Retorno**************************************
* int - Retorna 0 si el programa se ejecuta correctamente.
*****Entradas*************************************
* Sin entradas
**************************************************/
int main() {
    // Operador - usuario
    operador.id = 0;
    if (RegistrarOperadorInicial()) {
        return main();
    }
    if (operador.id == 0) {
        operador = ObtenerOperador((char*)"admin");
        if (operador.id == 0) {
            return main();
        }
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
    printf("[OK] Cargas Sociales cargadas desde base de datos\n");

    if (!CargarFacturasDesdeBdd()) {
        return main();
    }
    printf("[OK] Facturas cargadas desde base de datos\n");
    // Planillas - planillas del negocio (si hay alguna)
    CargarPlanillasDesdeBdd();

    // Productos - lista de productos del comercio (si encontró alguno)
    CargarProductosDesdeBdd();

    Menu_01();
    return 0;
}


/*****Nombre***************************************
* CargarFacturasDesdeBdd
*****Descripción y objetivo************************************
* Metodo para cargar facturas desde la base de datos
* en el sistema, para tenerlas listas en las consultas
* por el usuario.
*****Retorno****************************************
* Valor booleano
*****Entradas*************************************
* Sin entradas
**************************************************/
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

/*****Nombre***************************************
* CargarPlanillasDesdeBdd
*****Descripción y objetivo*************************************
* Metodo para cargar planillas desde la base de datos.
* Si no encuentra ninguna planilla, no carga ninguna.
* El objetivo es tener toda la información de las planillas
* en el sistema para poder mostrarla al usuario.
*****Retorno*****************************************
* Valor booleano
*****Entradas*************************************
* Sin entradas
**************************************************/
bool CargarPlanillasDesdeBdd() {
    planillas = ObtenerPlanillasConCantEmpleados();
    if (planillas == NULL) {
        return false;
    }
    return true;
}

/*****Nombre***************************************
* CargarCargasSocialesDesdeBdd
*****Descripción y objetivo*******************************************
* Metodo para cargar cargas sociales desde la base de datos
* en el sistema, con el objetivo de tenerlas listas 
* en las consultas por el usuario.
*****Retorno***********************************************
* Valor booleano
*****Entradas*************************************
* Sin entradas
**************************************************/
bool CargarCargasSocialesDesdeBdd() {
    CargoSocial cargo = ObtenerCargoSocial((char*)"General");
    if (cargo.id == 0) {
        return false;
    }
    cargoSocial = cargo;
    return true;
}

/*****Nombre***************************************
* CargarEmpleadosDesdeBdd
*****Descripción y objetivo*******************************************
* Metodo para cargar empleados desde la base de datos
* en el sistema, con el objetivo de tenerlos listos
* en las consultas por el usuario.
*****Retorno***********************************************
* Valor booleano
*****Entradas*************************************
* Sin entradas
**************************************************/
bool CargarEmpleadosDesdeBdd() {
    listaEmpleados = ObtenerEmpleadosConRol();
    cantidadEmpleados = ObtenerCantidadEmpleados();
    if (listaEmpleados == NULL) {
        return false;
    }
    return true;
}

/*****Nombre***************************************
* CargarAreasDesdeBdd
*****Descripción y objetivo**********************************
* Metodo para cargar areas desde la base de datos
* en el sistema, con el objetivo de tenerlas listas
* en las consultas por el usuario.
*****Retorno**************************************
* Valor booleano
*****Entradas*************************************
* Sin entradas
**************************************************/
bool CargarAreasDesdeBdd() {
    listaAreas = ObtenerAreas();
    cantidadAreas = ObtenerCantidadAreas();
    if (listaAreas == NULL) {
        return false;
    }
    return true;
}

/*****Nombre***************************************
* CargarProductosDesdeBdd
*****Descripción y objetivo*************************************
* Metodo para cargar productos desde la base de datos
* en el sistema, con el objetivo de tenerlos listos
* en las consultas por el usuario. Y para actualizar
* la lista de productos del comercio, en caso de cambios.
*****Retorno*****************************************
* Sin retorno
*****Entradas*************************************
* Sin entradas
**************************************************/
void CargarProductosDesdeBdd() {
    listaProductos = ObtenerProductos();
    cantidadProductos = ObtenerCantidadProductos();
}

/*****Nombre***************************************
* CargarComercioDesdeBdd
*****Descripción y objetivo*************************************
* Metodo para cargar comercio desde la base de datos
* en el sistema, con el objetivo de tenerlo listo
* en las consultas por el usuario y generacion de facturas.
*****Retorno*****************************************
* Valor booleano
*****Entradas*************************************
* Sin entradas
**************************************************/
bool CargarComercioDesdeBdd() {
    comercio = ObtenerInfoComercio((char*)"001");
    if (comercio.numSigFactura == -1) {
        return false;
    }
    return true;
}

/*****Nombre***************************************
* RegistrarOperadorInicial
*****Descripción y objetivo**********************************
* Método que registra un operador inicial en la base de datos.
* Retornará true si el registro fue exitoso, false en caso contrario.
* El objetivo es registrar al operador con la clave encriptada
* en caso de que sea el primer arranque del programa.
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
    char* clave_encriptada = Encriptar(clave);

    Operador op;
    strcpy(op.usuario, "admin");
    strcpy(op.clave, clave_encriptada);
    op.id = 1;

    return RegistrarOperador(op);
}

/*****Nombre***************************************
* Menu_01
*****Descripción y objetivo**********************************
* Menu principal del programa que permite al usuario
* elegir la opción que desea realizar.
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
    printf("\n1.Opciones operativas\n2.Opciones administrativas\n3.Salir\n>");
    scanf("%s",&opcion_01);
    if (!VerificarNumero(opcion_01)) {
        printf("[ERROR] <= POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
        PauseSinTimer(2);
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
        case 3:
            exit(-1);
        default:
            printf("[ERROR] <= POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
            PauseSinTimer(2);
            break;
    }
    Menu_01();
}

/*****Nombre***************************************
* Menu_OP1
*****Descripción y objetivo**********************************
* Funcion que despliega el login que debe realizar el usuario
* para poder acceder a las opciones operativas del programa.
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Menu_OP1() {
    system("clear");

    printf("[MENÚ DE OPCIONES OPERATIVAS]\n");
    
    printf("Ingrese el nombre de usuario y contrasena\nNombre de usuario:");
    char nombreUsuario[BUFFER];
    scanf("%s",&nombreUsuario);

    printf("contrasena:");
    char contrasena[BUFFER];
    scanf("%s",&contrasena);

    char* clave_encriptada = Encriptar((char*)contrasena);

    strcpy(operador.usuario, nombreUsuario);
    strcpy(operador.clave, clave_encriptada);
    
    if (ValidarOperador(operador)) {
        Menu_OP2();
    } else {
        printf("[ERROR] <= USUARIO O CONTRASEÑA INCORRECTOS!\n");
        PauseSinTimer(2);
        Menu_01();
    }
}

/*****Nombre***************************************
* Menu_OP2
*****Descripción y objetivo********************************************************
* Menu de gestion de opciones operativas del programa.
* El usuario podrá elegir entre las siguientes opciones:
* 1. Cargar productos
* 2. Listar áreas
* 3. Listar empleados
* 4. Listar productos
* 5. Volver
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
        PauseSinTimer(2);
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
                    PauseSinTimer(2);
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
            if (cantidadProductos == 0) {
                printf("[!] No hay PRODUCTOS!\n");
                    PauseSinTimer(2);
                    Menu_OP2();
                    return;
            }
            MostrarListaDeProductos();
            PauseSinTimer(2);
            break;
        default:
            printf("[ERROR] <= POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
            PauseSinTimer(2);
            break;
    }
    Menu_OP2();
}

/*****Nombre***************************************
* MostrarListaDeEmpleados
*****Descripción y objetivo**********************************
* Muestra la lista de empleados registrados en el sistema.
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
*****Descripción y objetivo**********************************
* Muestra toda la lista de areas en pantalla
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
*****Descripción y objetivo**********************************
* Muestra la lista de productos registrados en el sistema.
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
*****Descripción y objetivo**********************************
* Permite la carga de productos desde un archivo de texto
* en la ruta especificada por el usuario. Tiene como objetivo
* pedir, validar y cargar la información de los productos desde
* un archivo de texto.
*****Retorno**************************************
* bool - true si se cargaron los productos correctamente, false si no
*****Entradas*************************************
* Sin entradas
**************************************************/
bool CargarProductosDesdeRuta() {
    printf("\nIngrese la ruta de ubicacion del archivo de productos\n");
    printf("Digite (*) sin parentesis para cancelar y volver al menu anterior\n");
    printf(">");
    char ubicacion[BUFFER];
    fflush(stdin);
    scanf("%s",&ubicacion);
    
    // El usuario presiono ENTER
    if (strlen(ubicacion) == 0 || ubicacion[0] == '*') {
        return true;
    }

    FILE* ubicacionA = fopen(ubicacion,"r");
    if (!ubicacionA) {
        printf("\n[ERROR] <= EL ARCHIVO NO EXISTE O SE PRODUJO UN ERROR AL INTENTAR ABRIRLO!!\n");
        PauseSinTimer(2);
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
                PauseSinTimer(2);
                return false;
            }
            else {
                char** lineas = SepararLineas(texto, (char*)"\n");

                Producto* productos = malloc(sizeof(Producto)*cantLineas);
                for (int i = 0; i < cantLineas; i++) {
                    char copia_linea[BUFFER];
                    strcpy(copia_linea, lineas[i]);
                    if (ComasJuntas(copia_linea)) {
                        printf("\n[ERROR] <= NO DEBEN HABER ESPACIOS VACIOS!!\n");
                        PauseSinTimer(2);
                        return false;
                    }
                    char** campos = SepararLineas(lineas[i], (char*)",");

                    // Verificar cada uno de los campos

                    // Verificando el id
                    if (strlen(campos[0]) > 50) {
                        printf("\n[ERROR] <= EL CODIGO DEL PRODUCTO TIENE MAS DE 50 CARACTERES!!\n");
                        PauseSinTimer(2);
                        return false;
                    }

                    // Verificando el nombre
                    if (strlen(campos[1]) > 50) {
                        printf("\n[ERROR] <= EL NOMBRE DEL PRODUCTO TIENE MAS DE 50 CARACTERES!!\n");
                        PauseSinTimer(2);
                        return false;
                    }

                    // Verificando el costo (double)
                    if (!VerificarDouble(campos[2])) {
                        printf("%s\n",campos[2]);
                        printf("\n[ERROR] <= EL COSTO DEL PRODUCTO NO ES UN NUMERO VALIDO!!\n");
                        PauseSinTimer(2);
                        return false;
                    }

                    // Verificando el impuesto (double)
                    if (!VerificarDouble(campos[3])) {
                        printf("\n[ERROR] <= EL IMPUESTO DEL PRODUCTO NO ES UN NUMERO VALIDO!!\n");
                        PauseSinTimer(2);
                        return false;
                    }

                    // Mostrar que haya campos validos
                    if (strlen(campos[0])  < 2 || strlen(campos[0]) > 15) {
                        printf("\n[ERROR] <= LA LINEA NO ES VALIDA (3 - 15) CARACTERES PERMITIDOS!\n");
                        PauseSinTimer(2);
                        return false;
                    }
      
                    if (strlen(campos[1])  < 2 || strlen(campos[1]) > 15) {
                        printf("\n[ERROR] <= LA LINEA NO ES VALIDA (3 - 15) CARACTERES PERMITIDOS!\n");
                        PauseSinTimer(2);
                        return false;
                    }

                    if (strlen(campos[2])  < 2 || strlen(campos[2]) > 15) {
                        printf("\n[ERROR] <= LA LINEA NO ES VALIDA (3 - 15) CARACTERES PERMITIDOS!\n");
                        PauseSinTimer(2);
                        return false;
                    }

                    if (strlen(campos[3])  < 2 || strlen(campos[3]) > 15) {
                        printf("\n[ERROR] <= LA LINEA NO ES VALIDA (3 - 15) CARACTERES PERMITIDOS!\n");
                        PauseSinTimer(2);
                        return false;
                    }

                    double costo_producto = strtod(campos[2], NULL);
                    double impuesto_producto = strtod(campos[3], NULL);

                    if (costo_producto <= 0 || impuesto_producto <= 0) {
                        printf("\n[ERROR] <= EL COSTO O IMPUESTO NO ES VALIDO, SOLO VALORES POSITIVOS\n");
                        PauseSinTimer(2);
                        return false;
                    }

                    if (impuesto_producto >= 100) {
                        printf("\n[ERROR] <= EL IMPUESTO NO ES VALIDO (TIENE QUE SER MENOR QUE 100)!\n");
                        PauseSinTimer(2);
                        return false;
                    }

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
            PauseSinTimer(2);
            return false;
        }

    }
}

/*****Nombre***************************************
* VerificarDouble
*****Descripción y objetivo**********************************
* Verifica que el string pasado por parametro sea un double
*****Retorno**************************************
* True si es un double, false si no 
*****Entradas*************************************
* Sin entradas
**************************************************/
bool VerificarDouble(char* texto) {
    char* puntero = texto;
    int contador = 0;
    while (*puntero != '\0' || *puntero != '\n' || *puntero != '\r' || *puntero != 0) {
        if (*puntero == 10 || *puntero == 0 || *puntero == 13) {
            break;
        }
        if (!isdigit(*puntero) && *puntero != '.') {
            printf("A | %c | %i \n",*puntero, *puntero);
            return false;
        }
        if (!isdigit(*puntero) && *puntero != '.') {
            printf("E: %c, %i\n",*puntero, *puntero);
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

bool ComasJuntas(char* puntero) {
    while (*puntero != '\0' || *puntero != '\n' || *puntero != '\r' || *puntero != 0 || *puntero != EOF) {
        if (*puntero == ',') {
            if ((*puntero)++ == ',') {
                return true;
            } 
        }
        if (*puntero == '\0' || *puntero == '\n') { return false; }
        (*puntero)++;
    }
    return false;
} 

/*****Nombre***************************************
* SepararLineas
*****Descripción y objetivo**********************************
* Separa un string en lineas de acuerdo a un delimitador
* y devuelve un array de strings con cada una de las lineas.
* Como objetivo de separar la información de los productos
* del archivo de texto.
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
*****Descripción y objetivo**********************************
* Cuenta la cantidad de caracteres de un archivo, con el fin
* de saber cuantos caracteres hay en el archivo y poder
* procesarlo y validarlo.
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
*****Descripción y objetivo**********************************
* Metodo que lee un archivo y lo guarda en un string., con el fin
* de poder procesarlo y validarlo.
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
*****Descripción y objetivo************************************************
* Metodo que revisa el formato del archivo de carga de productos
* para verificar que cumpla con el formato necesario.
*****Retorno****************************************************
* Valor booleano
*****Entradas**********************************************************************************
* Cadena de caracteres que contiene el contenido del archivo - numero de caracteres del archivo
***********************************************************************************************/
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
*****Descripción y objetivo**********************************
* Revisa que el archivo tenga una cantidad de comas suficiente
* para poder separarlo en lineas.
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
*****Descripción y objetivo**********************************
* Cuenta todas las lineas de un archivo de texto. Para poder
* separarlo en lineas.
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
*****Descripción y objetivo**********************************
* Menu de gestion de opciones administrativas.
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Menu_OA1() {
    system("clear");
    fflush(stdin);
    printf("[MENU DE OPCIONES ADMINISTRATIVAS]\n");
    printf("1.Valores iniciales\n2.Registro de nomina\n3.Eliminar nomina\n4.Registro de venta de productos\n5.Consulta de nominas\n6.Consulta de ventas\n7.Balance anual\n8.Rendimiento de area por mes y anio\n0.Volver\n>");
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
            if (cantidadProductos == 0) {
                printf("[ERROR] = NO HAY PRODUCTOS REGISTRADOS!\n");
                PauseSinTimer(2);
                Menu_OA1();
                return;
            }
            LineaFactura* lineasFactura = malloc(cantidadProductos * sizeof *lineasFactura);

            MenuRegistroVentaProducto(lineasFactura, 0);
            break;
        case 5:
            CargarPlanillasDesdeBdd();
            planillas = ObtenerPlanillasConCantEmpleados();
            if (planillas == NULL) {
                printf("[ERROR] = NO HAY NOMINAS REGISTRADAS!\n");
                PauseSinTimer(2);
                Menu_OA1();
                return;
            }
            MostrarTodasLasNominasResumidas();
            PauseSinTimer(2);
            break;
        case 6:
            CargarFacturasDesdeBdd();
            if (cantidadFacturas == 0) {
                printf("[ERROR] = NO HAY FACTURAS REGISTRADAS!\n");
                PauseSinTimer(2);
                Menu_OA1();
                return;
            }
            MostrarFacturasResumidas();
            PauseSinTimer(2);
            break;
        case 7:
            Menu_BA01();
            break;
        case 8:
            CargarFacturasDesdeBdd();
            if (cantidadFacturas == 0) {
                printf("[ERROR] = NO HAY FACTURAS REGISTRADAS!\n");
                PauseSinTimer(2);
                Menu_OA1();
                return;
            }
            MostrarRendimientoDeAreaPorFecha();
            break;
        case 0:
            Menu_01();
            break;
        default:
            printf("[ERROR] = POR FAVOR DIGITE UNA DE LAS OPCIONES DISPONIBLES!\n");
            PauseSinTimer(2);
            Menu_OA1();
            break;
    }
}

/*****Nombre***************************************
* MostrarRenderimientoDeAreaPorFecha
*****Descripción y objetivo**********************************
* Solicita una fecha (mes y anio) para mostrar el
* reporte de rendimiento por cada área de dicha fecha.
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void MostrarRendimientoDeAreaPorFecha() {
    system("clear");
    printf("\n=========================[ Consulta de rendimiento de area por mes-anio ]=========================");
    printf("\n[i] Por favor ingrese una fecha [i]");
    
    Fecha fec;

    int mes = 0;
    printf("\n[i] Ingrese el mes (1 - 12) [i]\n");
    printf("> ");
    char opcion_mes[BUFFER];
    scanf("%s",&opcion_mes);
    if (!VerificarNumero(opcion_mes)) {
        printf("[ERROR] = POR FAVOR DIGITE UN MES VALIDO!\n");
        PauseSinTimer(2);
        MostrarRendimientoDeAreaPorFecha();
        return;
    }
    mes = strtol(opcion_mes,NULL,10);
    if (mes < 1 || mes > 12) {
        printf("[ERROR] = POR FAVOR DIGITE UN MES VALIDO!\n");
        PauseSinTimer(2);
        MostrarRendimientoDeAreaPorFecha();
        return;
    }

    int anio = 0;
    printf("\n[i] Ingrese el anio (YYYY) [i]\n");
    printf("> ");
    char opcion_anio[BUFFER];
    scanf("%s",&opcion_anio);
    if (!VerificarNumero(opcion_anio)) {
        printf("[ERROR] = POR FAVOR DIGITE UN ANIO VALIDO!\n");
        PauseSinTimer(2);
        MostrarRendimientoDeAreaPorFecha();
        return;
    }
    anio = strtol(opcion_anio,NULL,10);
    if (anio < 0) {
        printf("[ERROR] = POR FAVOR DIGITE UN ANIO VALIDO!\n");
        PauseSinTimer(2);
        MostrarRendimientoDeAreaPorFecha();
        return;
    }

    fec.mes = mes;
    fec.anio = anio;

    int indice = 0;
    RendimientoArea* rendimientosPorArea = ObtenerRendimientoPorAreas(mes, anio);

    system("clear");
    printf("\n====================[RENDIMIENTOS DEL MES %i DEL AÑO %i]====================\n",mes,anio);
    while (rendimientosPorArea[indice].subtotal_ventas != -1) {
        printf("\n[i] Area: %s [i]\n",rendimientosPorArea[indice].nombre_area);
        printf("[i] Subtotal de ventas: %.2f [i]\n",rendimientosPorArea[indice].subtotal_ventas);
        indice++;
    }
    printf("================================================================================\n");
    PauseSinTimer(2);
    free(rendimientosPorArea);
}

/*****Nombre***************************************
* MostrarFacturasResumidas
*****Descripción y objetivo**********************************
* Muestra las facturas resumidas y permite seleccionar una para ver su detalle
* o volver al menu anterior. Tiene como objetivo
* mostrar las facturas resumidas de las facturas registradas.
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void MostrarFacturasResumidas() {
    system("clear");
    printf("==================[ Resumen de facturas ] ==================\n");
    for (int i = 0; i < cantidadFacturas; i++) {
        printf("Id de la factura: %i", facturas[i].id_factura);
        printf("\nNombre del comercio: %s", facturas[i].nombre_comercio);
        printf("\nCedula del comercio: %s", facturas[i].cedula_comercio);
        printf("\nTelefono del comercio: %s", facturas[i].telefono_comercio);
        printf("\nNombre del cliente: %s", facturas[i].nombre_cliente);
        printf("\nFecha de la factura: %i-%i-%i", facturas[i].fecha_facturacion.dia, facturas[i].fecha_facturacion.mes, facturas[i].fecha_facturacion.anio);
        printf("\nNombre del area de produccion: %s", facturas[i].nombre_area);

        printf("\n");
        printf("Subtotal de factura: %10.2f\n", facturas[i].subtotal);
        printf("IVA de factura: %10.2f\n", facturas[i].impuesto);
        printf("Total de factura: %10.2f\n", facturas[i].total);
        printf("**********************************************************\n");
    }
    printf("[ i ] Elija el numero de la factura que desea consultar\n>");
    printf(" (O digite una letra para volver al menu anterior)\n>");
    char opcion_01[BUFFER];
    scanf("%s",&opcion_01);
    if (!VerificarNumero(opcion_01)) {
        printf("[ !! ] Volviendo al menu anterior!\n");
        PauseSinTimer(2);
        Menu_OA1();
        return;
    }
    int idFacturaElegida = strtol(opcion_01,NULL,10);
    if (FacturaExiste(idFacturaElegida)) {
        MostrarFacturaEspecifica(idFacturaElegida);
    } else {
        printf("[ !! ] La factura no existe!\n");
        PauseSinTimer(2);
        MostrarFacturasResumidas();
        return;
    }
}

/*****Nombre***************************************
* MostrarFacturaEspecifica
*****Descripción y objetivo**********************************
* Muestra la factura seleccionada por el usuario
* y permite volver al menu anterior.
*****Retorno**************************************
* Sin retorno
*****Entradas*************************************
* idFacturaElegida - id de la factura a mostrar
**************************************************/
void MostrarFacturaEspecifica(int idFacturaElegida) {
    system("clear");
    
    for (int i = 0; i < cantidadFacturas; i++) {
        if (facturas[i].id_factura == idFacturaElegida) {
            printf("==================[ Informacion de factura %i ] ==================\n", idFacturaElegida);
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
                    (p.impuesto),
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
}

/*****Nombre***************************************
* FacturaExiste
*****Descripción y objetivo**********************************
* Pregunta si una factura existe en el sistema.
* Con el objetivo de evitar errores de en las consultas.
*****Retorno**************************************
* True - Si la factura existe
* False - Si la factura no existe
*****Entradas*************************************
* idFactura - Id de la factura a consultar
**************************************************/
bool FacturaExiste(int idFactura) {
    CargarFacturasDesdeBdd();
    for (int i = 0; i < cantidadFacturas; i++) {
        if (facturas[i].id_factura == idFactura) return true;
    }
    return false;
}

/*****Nombre***************************************
* MostrarTodasLasNominasResumidas
*****Descripción y objetivo*******************************************
* Muestra todas las nominas resumidas en pantalla al usuario
* con una opcion que se le da para desplegar la informacion
* de una nomina en especifico.
*****Retorno************************************************
* No retorna
*****Entradas***********************************************
* No hay parametros
************************************************************/
void MostrarTodasLasNominasResumidas() {
    system("clear");
    PlanillaConCantEmpleados* p = ObtenerPlanillasConCantEmpleados();
    if (p == NULL) {
        printf("[ERROR] = NO HAY NOMINAS REGISTRADAS!\n");
        PauseSinTimer(2);
        return;
    }
    int i = 0;
    printf("\n=========================[ Lista de nominas ]================================\n");
    while (p[i].id != -1) {
        printf("----------------------------------\n");
        printf("Num. de nomina: %i\n", p[i].id);
        printf("Fecha: 1/%i/%i\n", p[i].fecha.mes, p[i].fecha.anio);
        printf("Cantidad de empleados: %i\n", p[i].cantidad_empleados);
        printf("Porcentaje de carga social: %.2f%\n", p[i].monto_carga_social * 100);
        double subtotal_salarios = 0;
        double total_salarios = 0;

        for (int j = 0; j < p[i].cantidad_empleados; j++) {
            subtotal_salarios += listaEmpleados[j].salario_mensual;
            total_salarios += (listaEmpleados[j].salario_mensual * (p[i].monto_carga_social));
        }

        printf("Subtotal de salarios (sin cargas): %.2f\n", subtotal_salarios);
        printf("Total de salarios (con cargas): %.2f\n\n", total_salarios);

        i++;
    }
    printf("============================================================\n");
    printf("[ i ] Digite el numero de la nomina a ver detalles: ");
    printf("\n(O digite una letra para volver al menu anterior)");
    printf("\n>");
    char opcion_01[BUFFER];
    scanf("%s",&opcion_01);
    if (!VerificarNumero(opcion_01)) {
        printf("[ !!! ] Volviendo al menu anterior!\n");
        PauseSinTimer(2);
        Menu_OA1();
        return;
    }
    int opcion_02 = strtol(opcion_01,NULL,10);
    if (ExisteNomina(opcion_02)) {
        MostrarNominaEspecifica(opcion_02);
    }
    else {
        printf("[ !!! ] No existe una nomina con el numero (%i)!\n", opcion_02);
        PauseSinTimer(2);
        MostrarTodasLasNominasResumidas();
    }

}

/*****Nombre***************************************
* MostrarNominaEspecifica
*****Descripción y objetivo*******************************************
* Metodo que recibe el id de una nomina para mostrar
* su info en pantalla
*****Retorno************************************************
* No tiene retorno
*****Entradas***********************************************
* int idNomina
************************************************************/
void MostrarNominaEspecifica(int idNomina) {
    system("clear");
    PlanillaConCantEmpleados* p = ObtenerPlanillasConCantEmpleados();
    if (p == NULL) {
        printf("[ERROR] = NO HAY NOMINAS REGISTRADAS!\n");
        PauseSinTimer(2);
        return;
    }
    int i = 0;

    while (p[i].id != -1) {
        if (p[i].id == idNomina) {
            printf("\n=========================[ NOMINA %i ]================================\n", p[i].id);
            printf("----------------------------------\n");
            printf("Fecha: 1/%i/%i\n", p[i].fecha.mes, p[i].fecha.anio);
            printf("Cantidad de empleados: %i\n", p[i].cantidad_empleados);
            printf("Porcentaje de carga social: %.2f%\n", p[i].monto_carga_social * 100);
            double subtotal_salarios = 0;
            double total_salarios = 0;
            printf("~~~~~~~~~~~~~~~~~~~ Lista de empleados ~~~~~~~~~~~~~~~~~~~\n");
            for (int j = 0; j < p[i].cantidad_empleados; j++) {
                printf("\n\n");
                printf("Nombre: %s", listaEmpleados[j].nombre_completo);
                printf("\n");
                printf("Cedula: %s", listaEmpleados[j].cedula);
                printf("\n");
                printf("Sueldo: %.2f", listaEmpleados[j].salario_mensual);
                printf("\n");
                printf("Puesto: %s", listaEmpleados[j].nombre_rol);
                printf("\n----------------------------------\n");
                subtotal_salarios += listaEmpleados[j].salario_mensual;
                total_salarios += (listaEmpleados[j].salario_mensual * (p[i].monto_carga_social));
            }
            printf("Subtotal de salarios (sin cargas): %.2f\n", subtotal_salarios);
            printf("Total de salarios (con cargas): %.2f\n\n", total_salarios);
            return;
        }
        i++;
    }
}

/*****Nombre***************************************
* ConsultarFacturasCargadas
*****Descripción y objetivo**********************************
* Menu de consulta de facturas
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
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
* MenuRegistroVentaProducto
*****Descripción y objetivo**********************************
* Menu de registro de venta de productos
*****Retorno**************************************
* Sin retorno 
*****Entradas****************************************
* LineaFactura* lineasFactura, int cant_prod_elegidos
*****************************************************/
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
* TerminarRegistroDeVenta
*****Descripción y objetivo*****************************************
* Metodo para finalizar proceso del registro de una venta.
* Tiene como objetivo finalizar el proceso de registro de una venta.
*****Retorno*********************************************
* Sin retorno 
*****Entradas****************************************
* LineaFactura* lineasFactura, int cant_prod_elegidos
*****************************************************/
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

    char id_digitado[BUFFER];
    printf("\n[!] Digite el id del area de produccion: ");
    scanf("%s", &id_digitado);
    if (!VerificarNumero(id_digitado)) {
        printf("[ERROR] = POR FAVOR DIGITE UN NUMERO\n");
        PauseSinTimer(2);
        TerminarRegistroDeVenta(lineasFactura, cant_prod_elegidos);
        return;
    }

    id_area_produccion = strtol(id_digitado,NULL,10);

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

    char dia_digitado[BUFFER];
    char mes_digitado[BUFFER];
    char anio_digitado[BUFFER];

    printf("\n[!] Digite el dia de la factura: ");
    scanf("%s",&dia_digitado);
    printf("\n[!] Digite el mes de la factura: ");
    scanf("%s",&mes_digitado);
    printf("\n[!] Digite el anio de la factura: ");
    scanf("%s",&anio_digitado);

    if (!VerificarNumero(dia_digitado) || !VerificarNumero(mes_digitado) || !VerificarNumero(anio_digitado)) {
        printf("[ERROR] = POR FAVOR DIGITE UNA FECHA VALIDA\n");
        PauseSinTimer(2);
        TerminarRegistroDeVenta(lineasFactura, cant_prod_elegidos);
        return;
    }

    dia_factura = strtol(dia_digitado,NULL,10);
    mes_factura = strtol(mes_digitado,NULL,10);
    anio_factura = strtol(anio_digitado,NULL,10);


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

/*****Nombre***************************************
* RegistrarDetallesFacturaEnBdd
*****Descripción y objetivo*******************************************
* Metodo para registrar los detalles de la factura en la BD.
* Con el objetivo de registrar los productos a la factura.
*****Retorno***********************************************
* Sin retorno 
*****Entradas********************************************************
* LineaFactura* lineasFactura, int cant_prod_elegidos, int id_factura
*********************************************************************/
void RegistrarDetallesFacturaEnBdd(LineaFactura* lineasFactura, int cant_prod_elegidos, int id_factura) {
    for (int i = 0; i < cant_prod_elegidos; i++) {
        char id_producto[51];
        strcpy(id_producto, lineasFactura[i].id_producto);
        Producto prod = ObtenerProducto((char*)id_producto);
        RegistrarDetalleFactura(id_factura, prod, lineasFactura[i].cantidad);
    }
}

/*****Nombre***************************************
* ObtenerAreaProduccion
*****Descripción y objetivo***************************
* Metodo para obtener el area de produccion de la BD.
*****Retorno*******************************
* Objeto area
*****Entradas*************************************
* int id_area
**************************************************/
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

/*****Nombre***************************************
* ExisteAreaProduccion
*****Descripción y objetivo***************************
* Metodo para verificar si existe una area
*****Retorno*******************************
* Valor booleano
*****Entradas*************************************
* int id_area_produccion
**************************************************/
bool ExisteAreaProduccion(int id_area_produccion) {
    for (int i = 0;i < cantidadAreas;i++) {
        if (listaAreas[i].id == id_area_produccion) {
            return true;
        }
    }
    return false;
}

/*****Nombre***************************************
* StringSoloConLetras
*****Descripción y objetivo*********************************************************
* Metodo para verificar si una cadena de caracteres contiene algun numero.
* El objetivo es validar que la cadena solo contenga letras.
*****Retorno*************************************************************
* Valor booleano
*****Entradas*************************************
* char* cadena
**************************************************/
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

/*****Nombre***************************************
* ObtenerProductoExistente
*****Descripción y objetivo***************************************************
* Metodo para obtener un producto existente en la lista de productos
*****Retorno*******************************************************
* Objeto producto
*****Entradas*************************************
* char* xd
**************************************************/
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
* EliminarLineaFactura
*****Descripción y objetivo******************************
* Metodo para eliminar una linea de la factura. El objetivo es eliminar una linea de la factura
* y actualizar el arreglo de lineas de factura.
*****Retorno**********************************
* Objeto lineafactura
*****Entradas*******************************************************
* LineaFactura* lineasFactura, int cant_prod_elegidos, Producto prod
********************************************************************/
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
* ImprimirProductosElegidos
*****Descripción y objetivo***************************************
* Metodo para imprimir productos elegidos en la factura
*****Retorno*******************************************
* Sin retorno
*****Entradas*******************************************************
* LineaFactura* lineasFactura, int cant_prod_elegidos
********************************************************************/
void ImprimirProductosElegidos(LineaFactura* lineasFactura, int cant_prod_elegidos) {
    printf("cantidad de productos elegidos: %d\n",cant_prod_elegidos);
    for (int i = 0; i < cant_prod_elegidos; i++) {
        printf("[%s] - Cantidad: %d\n", lineasFactura[i].id_producto, lineasFactura[i].cantidad);
    }
}

/*****Nombre***************************************
* ExisteProductoEnLinea
*****Descripción y objetivo*******************************************
* Metodo para verificar si existe un producto en la factura.
* Tiene como objetivo verificar si existe un producto en la factura y
* poder acumular la cantidad de productos en la factura.
*****Retorno************************************************
* Valor booleano
*****Entradas***********************************************************
* LineaFactura* lineasFactura, int cant_prod_elegidos, Producto producto
************************************************************************/
bool ExisteProductoEnLinea(LineaFactura* lineasFactura, int cant_prod_elegidos, Producto producto) {
    for (int i = 0; i < cant_prod_elegidos; i++) {
        if (strcmp(lineasFactura[i].id_producto, producto.id) == 0) {
            return true;
        }
    }
    return false;
}

/*****Nombre***************************************
* AgregarRepetidoLineaFactura
*****Descripción y objetivo*******************************************
* Metodo para sumar mas productos existentes en la factura
*****Retorno************************************************
* Sin retorno
*****Entradas******************************************************************************
* LineaFactura* lineasFactura, int cant_prod_elegidos, Producto producto, int cant_prod_int
*******************************************************************************************/
void AgregarRepetidoLineaFactura(LineaFactura* lineasFactura, int cant_prod_elegidos, Producto producto, int cant_prod_int) {
    for (int i = 0; i < cant_prod_elegidos; i++) {
        if (strcmp(lineasFactura[i].id_producto, producto.id) == 0) {
            lineasFactura[i].cantidad += cant_prod_int;
            return;
        }
    }
}

/*****Nombre***************************************
* AgregarLineaFactura
*****Descripción y objetivo*******************************************
* Metodo para agregar una linea a la factura
*****Retorno************************************************
* Objeto linea factura
*****Entradas******************************************************************************
* LineaFactura* lineasFactura, int cant_prod_elegidos, Producto producto, int cantidad
*******************************************************************************************/
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

/*****Nombre***************************************
* MenuEliminarNomina
*****Descripción y objetivo*******************************************
* Menu para eliminar nominas
*****Retorno************************************************
* Sin retorno
*****Entradas***********************************************
* Sin entradas
************************************************************/
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

/*****Nombre***************************************
* ExisteNomina
*****Descripción y objetivo*******************************************
* Metodo para verificar la existencia de una nomina
*****Retorno************************************************
* Valor booleano
*****Entradas***********************************************
* int id_nomina
************************************************************/
bool ExisteNomina(int id_nomina) {
    int i = 0;
    while (planillas[i].id != -1) {
        if (planillas[i].id == id_nomina) return true;
        i++;
    }
    return false;
}

/*****Nombre***************************************
* EliminarNomina
*****Descripción y objetivo*******************************************
* Metodo para eliminar una nomina
*****Retorno************************************************
* Valor booleano
*****Entradas***********************************************
* int id_nomina
************************************************************/
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

/*****Nombre***************************************
* MostrarTodasLasNominasConSalarios
*****Descripción y objetivo*******************************************
* Metodo para imprimir las nominas con salarios
*****Retorno************************************************
* Sin retorno
*****Entradas***********************************************
* Sin entradas
************************************************************/
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

        printf("\n\nSubtotal de salarios (sin cargas): %.2f\n", subtotal_salarios);
        printf("Total de salarios (con cargas): %.2f\n\n", total_salarios);

        i++;
    }
}

/*****Nombre***************************************
* MostrarTodasLasNominas
*****Descripción y objetivo**********************************
* Metodo para imprimir todas las nominas
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
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

/*****Nombre***************************************
* Menu_VI01
*****Descripción y objetivo**********************************
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
            PauseSinTimer(2);
            break;
    }
    Menu_VI01();
}

/*****Nombre***************************************
* CambiarValorDeCargaSocial
*****Descripción y objetivo**********************************
* Cambia el valor de las cargas sociales con un
* valor nuevo digitado por el usuario. El objetivo es
* que el usuario pueda cambiar el valor de las cargas
* sociales de la empresa.
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
    scanf("%s", &opcion_01);

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
*****Descripción y objetivo**********************************
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
}

/*****Nombre***************************************
* Menu_RN01
*****Descripción y objetivo**********************************
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
*****Descripción y objetivo**********************************
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

    Fecha fecha_nom = {1, mes, anio};

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
        EliminarPlanillaPorFecha(fecha_nom);
        Menu_RN01();
        return;
    }
    if (strcmp(opcion_01,"n") == 0 || strcmp(opcion_01,"N") == 0) {
        printf("[!] Se cancelo el registro, volviendo al menu anterior\n");
        PauseSinTimer(2);
        EliminarPlanillaPorFecha(fecha_nom);
        Menu_RN01();
        return;
    }
    printf("[ERROR] <= DEBE DIGITAR S O N PARA CONFIRMAR O NO CONFIRMAR LA FECHA!\n");
    EliminarPlanillaPorFecha(fecha_nom);
    PauseSinTimer(2);
    Menu_RN01();
}

/*****Nombre***************************************
* AgregarEmpleadosANominaAux
*****Descripción y objetivo**********************************
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
            if (strcmp(confirmacion, "s") == 0 || strcmp(confirmacion, "S") == 0) {
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
            else {
                printf("[!] <= LA NOMINA NO FUE REGISTRADA!\n");
                PauseSinTimer(2);
            }
            AgregarEmpleadosANominaAux(plan, empleados_elegidos, cant_empleados_restantes);
            return;
        }
        if (opcion_01_int == 0) {
            printf("[!] Se cancelo el registro, volviendo al menu anterior\n");
            EliminarPlanillaPorFecha(plan.fecha);
            PauseSinTimer(2);
            Menu_OA1();
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
*****Descripción y objetivo**********************************
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

/*****Nombre***************************************
* Menu_BA01
*****Descripción y objetivo**********************************
* Función para la impresión de los balances 
* anuales de la empresa.
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* Sin entradas
**************************************************/
void Menu_BA01() {
    system("clear");
    int* aniosConFacturas = ObtenerAniosConFacturas();
    int* aniosConPlanillas = ObtenerAniosConPlanillas();
    int cantidadAniosConFacturas = ObtenerCantidadAniosConFacturas();
    int cantidadAniosConPlanillas = ObtenerCantidadAniosConPlanillas();

    int* aniosConFacturasYPlanillas = InterseccionListas(
        aniosConFacturas, cantidadAniosConFacturas, 
        aniosConPlanillas, cantidadAniosConPlanillas
    );

    int indice = 0;
    printf("===================[ REPORTES ANUALES ]===================\n");

    // Se imprimen los que tienen facturas y planillas
    while (aniosConFacturasYPlanillas[indice] != -1) {
        MostrarReporteAnualCompleto(aniosConFacturasYPlanillas[indice]);
        indice++;
    }

    // Se imprimen los que tienen facturas pero no planillas
    for (int i = 0; i < cantidadAniosConFacturas; i++) {
        if (NumPerteneceALista(aniosConFacturasYPlanillas, cantidadAniosConFacturas, aniosConFacturas[i])) {
            continue;
        }
        MostrarReporteFacturaAnual(aniosConFacturas[i]);
    }

    // Se imprimen los que tienen planillas pero no facturas
    for (int i = 0; i < cantidadAniosConPlanillas; i++) {
        if (NumPerteneceALista(aniosConFacturasYPlanillas, cantidadAniosConPlanillas, aniosConPlanillas[i])) {
            continue;
        }
        MostrarReportePlanillaAnual(aniosConPlanillas[i]);
    }
    printf("\n========================================================\n");
    printf("[ i ] Seleccione un año para ver el reporte completo\n");
    printf("(Escriba una letra para volver al menu anterior\n");
    printf("> ");
    char opcion[BUFFER];
    scanf("%s", opcion);

    if (!VerificarNumero(opcion)) {
        printf("[!!] Volviendo al menu anterior");
        PauseSinTimer(2);
        Menu_OA1();
        return;
    }
    int anio = atoi(opcion);
    if (!NumPerteneceALista(aniosConFacturas, cantidadAniosConFacturas, anio) || !NumPerteneceALista(aniosConPlanillas, cantidadAniosConPlanillas, anio)) {
        printf("[!!] No hay reporte para el año %d\n", anio);
        PauseSinTimer(2);
        Menu_BA01();
        return;
    }

    // Se muestran los que tienen facturas y planillas
    MostrarReportePorMesDeAnio(anio);
    PauseSinTimer(2);


    /*
    Planilla* listaPlanillas;
    listaPlanillas = ObtenerTotalPlanillas(); // P

    printf("--------- [LISTA DE AÑOS CON PLANILLA REGISTRADA] ---------\n");
    int cantPlanillas = ObtenerCantidadPlanillas(); // P

    for (int i = 0;i<cantPlanillas;i++) {
        double cargas = ObtenerTotalNominas(listaPlanilas[i].fecha.anio);
        printf("\nAÑO = %d\nTOTAL CON CARGAS = %f",cargas,listaPlanillas[i].monto_carga_social);
    }

    //filtrar facturas por año
    Factura* listaFacturas;
    int cantFacturas = ObtenerCantidadFacturas(id del comercio);
    listaFacturas = ObtenerFacturasTotal(); // no esta el metodo en el dao


    double totalNom = ObtenerTotalNomina(Planilla* lista, int cantNominas, int anio);

    printf("--------- [LISTA DE AÑOS CON FACTURAS REGISTRADAS] ---------\n");
    for (int i = 0;i<cantFacturas;i++) {
        double subtotal = ObtenerSubtotalVentas(listaFacturas[i].fechaFacturacion.anio);
        double impuesto = ObtenerImpuestoVentas(listaFacturas[i].fechaFacturacion.anio);
        double balance = subtotal - ObtenerTotalNomina(listaFacturas[i].fechaFacturacion.anio);
        printf("AÑO = %d\nSUBTOTAL = %f\nTOTAL DE IMPUESTO = %f\nBALANCE = %f\n",listaFacturas[i].fecha_Facturacion.anio,subtotal,impuesto,balance);
    }
    
    printf("Digite el año por consultar\n>");
    char anio[BUFFER];
    scanf("%s",&anio);
    if (VerificarNumero(anio)) {
        int anio2 = strtol(anio, NULL, 10)
        Planilla* resPlanillas = ObtenerPlanillasPorAnio(anio2); no existe el metodo en el dao
        int cantPlanillasConsultadas = ObtenerCantidadPlanillasPorAnio(anio2); no existe el metodo en el dao
        printf("\n--------- [DETALLES DE LOS MOVIMIENTOS DEL AÑO %d] ---------\n",anio2);
        printf("=== [DETALLES DE LAS PLANILLAS] ===\n");
        for (int i=0;i<cantPlanillasConsultadas;i++) {
            printf("MES = %d\nCARGAS = %f\n",resPlanillas[i].fecha.mes,resPlanillas[i].monto_carga_social);
        }
        Factura* resFacturas = ObtenerFacturasPorAnio(anio2);
        int cantFacturasConsultadas = ObtenerCantFacturasPorAnio(anio2);

        // no se si hay que mostrar el balance porque en este caso no se si en lugar del total de la nomina sea la carga y seria hacer un metodo para obtener la carga por mes

        printf("\n\n=== [DETALLES DE LAS PLANILLAS] ===\n");
        for (int i=0;i<cantFacturasConsultadas;i++) {
            printf("MES = %d\nSUBTOTAL = %f\nIMPUESTO = %f",resFacturas[i].fechaFacturacion.mes,resFacturas[i].subtotal,resFacturas[i].impuesto);
        }


    }
    else {
        printf("[ERROR] <== INGRESE UN AÑO VALIDO!!\n");
        PauseSinTimer(1);
        Menu_BA01();
    }
    */
}

/*****Nombre***************************************
* MostrarReportePorMesDeAnio
*****Descripción y objetivo**********************************
* Muestra el reporte de un año en especifico
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* int anio - año del reporte a mostrar
**************************************************/
void MostrarReportePorMesDeAnio(int anio) {

    printf("\n====================[ REPORTE POR MES DE AÑO %d ]====================\n", anio);

    Factura* listaFacturasAnual = ObtenerFacturasPorAnio(anio);
    PlanillaConCantEmpleados* listaPlanillasAnual = ObtenerPlanillasPorAnio(anio);

    int cantidadFacturasAnual = ObtenerCantidadFacturasPorAnio(anio);
    int cantidadPlanillasAnual = ObtenerCantidadPlanillasPorAnio(anio);

    for (int i = 1; i <= 12; i++) {
        Factura* facturasDelMes;
        PlanillaConCantEmpleados* planillasDelMes;
        int cantidadFacturasDelMes = ObtenerCantidadFacturasPorMes(listaFacturasAnual, cantidadFacturasAnual, i);
        int cantidadPlanillasDelMes = ObtenerCantidadPlanillasPorMes(listaPlanillasAnual, cantidadPlanillasAnual, i);

        double subtotal_ventas_mensual = 0;
        double impuesto_ventas_mensual = 0;
        double total_nomina_mensual = 0;
        double balance_mensual = 0;

        if (cantidadFacturasDelMes > 0) {
            facturasDelMes = ObtenerFacturasDeUnMes(listaFacturasAnual, cantidadFacturasAnual, i);

            for (int j = 0; j < cantidadFacturasDelMes; j++) {
                subtotal_ventas_mensual += facturasDelMes[j].subtotal;
                impuesto_ventas_mensual += facturasDelMes[j].impuesto;
            }
        }

        if (cantidadPlanillasDelMes > 0) {
            for (int j = 0; j < cantidadPlanillasAnual; j++) {
                if (listaPlanillasAnual[j].fecha.mes == i) {
                    int cant_empleados = listaPlanillasAnual[j].cantidad_empleados;
                    Fecha fechaMes;
                    fechaMes.dia = 1;
                    fechaMes.mes = i;
                    fechaMes.anio = anio;
                    EmpleadoConRol* empDelMes = ObtenerEmpleadosDePlanilla(fechaMes);
                    for (int m = 0; m < cant_empleados; m++) {
                        total_nomina_mensual += empDelMes[m].salario_mensual * listaPlanillasAnual[j].monto_carga_social;
                    }
                }
            }
        }

        balance_mensual = subtotal_ventas_mensual - total_nomina_mensual;

        printf(
            "\n[MES (%2i)] Gasto de nomina: %10.2f | Subtotal de ventas: %10.2f | Impuestos: %10.2f | Balance: %10.2f", 
            i, 
            total_nomina_mensual, 
            subtotal_ventas_mensual, impuesto_ventas_mensual, 
            balance_mensual
        );

    }
    printf("\n========================================================================\n");
}

/*****Nombre***************************************
* ObtenerPlanillasDeUnMes
*****Descripción y objetivo**********************************
* Recorre la lista de planillas y devuelve una lista de planillas que corresponden al mes indicado.
* Tiene como objetivo obtener las planillas de un mes determinado.
*****Retorno**************************************
* PlanillaConCantEmpleados* - lista de planillas del mes indicado
*****Entradas*************************************
* PlanillaConCantEmpleados* listaPlanillas - lista de planillas a recorrer
* int cantPlanillas - cantidad de planillas a recorrer
* int mes - mes a buscar
**************************************************/
PlanillaConCantEmpleados* ObtenerPlanillasDeUnMes(PlanillaConCantEmpleados* planillasAnual, int cantidadPlanillaDelAnio, int mes) {
    PlanillaConCantEmpleados* planillasDelMes = (PlanillaConCantEmpleados*)malloc(sizeof(PlanillaConCantEmpleados) * cantidadPlanillaDelAnio);
    int cantidadPlanillasDelMes = ObtenerCantidadPlanillasPorMes(planillasAnual, cantidadPlanillaDelAnio, mes);
    int contador = 0;
    for (int i = 0; i < cantidadPlanillaDelAnio; i++) {
        if (planillasAnual[i].fecha.mes == mes) {
            planillasDelMes[contador] = planillasAnual[i];
            contador++;
        }
    }
    return planillasDelMes;
}

/*****Nombre***************************************
* ObtenerFacturasDeUnMes
*****Descripción y objetivo**********************************
* Recorre la lista de facturas y devuelve una lista de facturas que corresponden al mes indicado.
* Tiene como objetivo obtener las facturas de un mes determinado.
*****Retorno**************************************
* Factura* - lista de facturas del mes indicado
*****Entradas*************************************
* Factura* listaFacturas - lista de facturas a recorrer
* int cantFacturas - cantidad de facturas a recorrer
* int mes - mes a buscar
**************************************************/
Factura* ObtenerFacturasDeUnMes(Factura* facturaAnual, int cantidadFacturasDelAnio, int mes) {
    Factura* facturasDelMes = (Factura*)malloc(sizeof(Factura) * cantidadFacturasDelAnio);
    int cantidadFacturasDelMes = ObtenerCantidadFacturasPorMes(facturaAnual, cantidadFacturasDelAnio, mes);
    int contador = 0;
    for (int i = 0; i < cantidadFacturasDelAnio; i++) {
        if (facturaAnual[i].fecha_facturacion.mes == mes) {
            facturasDelMes[contador] = facturaAnual[i];
            contador++;
        }
    }
    return facturasDelMes;
}

/*****Nombre***************************************
* ObtenerCantidadPlanillasPorMes
*****Descripción y objetivo**********************************
* Obtiene la cantidad de planillas que corresponden al mes indicado.
*****Retorno**************************************
* int - cantidad de planillas del mes indicado
*****Entradas*************************************
* PlanillaConCantEmpleados* listaPlanillas - lista de planillas a recorrer
* int cantPlanillas - cantidad de planillas a recorrer
* int mes - mes a buscar
**************************************************/
int ObtenerCantidadPlanillasPorMes(PlanillaConCantEmpleados* listaEmpleadosAnual, int cantidadEmpleadosAnual, int mes) {
    int cantidadPlanillas = 0;
    for (int i = 0; i < cantidadEmpleadosAnual; i++) {
        if (listaEmpleadosAnual[i].fecha.mes == mes) {
            cantidadPlanillas++;
        }
    }
    return cantidadPlanillas;
}

/*****Nombre***************************************
* ObtenerCantidadDeFacturasPorMes
*****Descripción y objetivo**********************************
* Obtiene la cantidad de facturas que corresponden al mes indicado.
*****Retorno**************************************
* int - cantidad de facturas del mes indicado
*****Entradas*************************************
* Factura* listaFacturasAnual - lista de facturas a recorrer
* int cantFacturasAnual - cantidad de facturas a recorrer
* int mes - mes a buscar
**************************************************/
int ObtenerCantidadFacturasPorMes(Factura* listaFacturasAnual, int cantidadFacturasAnual, int mes) {
    int cant = 0;
    for (int i = 0; i < cantidadFacturasAnual; i++) {
        if (listaFacturasAnual[i].fecha_facturacion.mes == mes) {
            cant++;
        }
    }
    return cant;
}

/*****Nombre***************************************
* MostrarReportePlanillaAnual
*****Descripción y objetivo**********************************
* Muestra el reporte de planillas anual según el año indicado.
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* int anio - año a buscar
**************************************************/
void MostrarReportePlanillaAnual(int anio) {

    double total_nomina = ObtenerTotalNominasConCargasPorAnio(anio);

    double subtotal_ventas = 0;
    double impuestos = 0;

    double balance = subtotal_ventas - total_nomina;

    printf(
        "\n [%d] Gasto de nomina: %10.2f | Subtotal venta: %10.2f | Impuestos: %10.2f | Balance: %10.2f", 
        anio, 
        total_nomina, 
        subtotal_ventas, 
        impuestos,
        balance
    );
}

/*****Nombre***************************************
* MostrarReporteFacturasAnual
*****Descripción y objetivo**********************************
* Muestra el reporte de facturas anual según el año indicado.
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* int anio - año a buscar
**************************************************/
void MostrarReporteFacturaAnual(int anio) {
    ReporteAnual reporte_ventas = ObtenerReporteAnual((char*)"001", anio);

    double total_nomina = 0;

    double balance = reporte_ventas.subtotal_ventas - total_nomina;

    printf("\n [%d] Gasto de nomina: %10.2f | Subtotal venta: %10.2f | Impuestos: %10.2f | Balance: %10.2f\n", anio, total_nomina, reporte_ventas.subtotal_ventas, reporte_ventas.total_impuesto, balance);
}

/*****Nombre***************************************
* MostrarReporteAnualCompleto
*****Descripción y objetivo**********************************
* Muestra en pantalla el reporte anual completo. De forma que
* se muestre el reporte de planillas anual y el reporte de facturas anual.
*****Retorno**************************************
* Sin retorno 
*****Entradas*************************************
* int anio - año a buscar
**************************************************/
void MostrarReporteAnualCompleto(int anio) {
    // Llegarán tanto las planillas como las facturas para el año indicado
    double total_nomina = ObtenerTotalNominasConCargasPorAnio(anio);
    ReporteAnual reporte_ventas = ObtenerReporteAnual((char*)"001", anio);

    double balance = reporte_ventas.subtotal_ventas - total_nomina;

    printf("\n [%d] Gasto de nomina: %10.2f | Subtotal venta: %10.2f | Impuestos: %10.2f |  Balance: %10.2f\n", anio, total_nomina, reporte_ventas.subtotal_ventas, reporte_ventas.total_impuesto, balance);
}

/*****Nombre***************************************
* NumPerteneceAlista
*****Descripción y objetivo**********************************
* Pregunta si un número pertenece a una lista de números.
*****Retorno**************************************
* true - si el número pertenece a la lista
* false - si el número no pertenece a la lista
*****Entradas*************************************
* lista - lista de números a recorrer
* int cantidad - cantidad de números a recorrer
* int numero - número a buscar
**************************************************/
bool NumPerteneceALista(int* lista, int cantidad, int numero) {
    for (int i = 0; i < cantidad; i++) {
        if (lista[i] == -1) {
            break;
        }
        if (lista[i] == numero) {
            return true;
        }
    }
    return false;
}

/*****Nombre***************************************
* InterseccionListas
*****Descripción y objetivo**********************************
* Obtiene la intersección de dos listas de números.
*****Retorno**************************************
* int* - lista de números que pertenecen a ambas listas
*****Entradas*************************************
* int* listaA - lista de números a recorrer
* int tamanioListaA - cantidad de números a recorrer de la lista B
* int* listaB - lista de números a recorrer
* int tamanioListaB - cantidad de números a recorrer de la listaB
**************************************************/
int* InterseccionListas(int* listaA, int tamanioListaA, int* listaB, int tamanioListaB) {
    int* listaInterseccion = malloc(sizeof(int) * (tamanioListaA + tamanioListaB));
    int indice = 0;
    for (int i = 0; i < tamanioListaA; i++) {
        for (int j = 0; j < tamanioListaB; j++) {
            if (listaA[i] == listaB[j]) {
                listaInterseccion[indice] = listaA[i];
                indice++;
            }
        }
    }
    listaInterseccion[indice] = -1;
    return listaInterseccion;
}

/*****Nombre***************************************
* ObtenerSubtotalVentas
*****Descripción y objetivo**********************************
* Obtiene el subtotal de ventas por un año en particular
*****Retorno**************************************
* double - subtotal de ventas del año
*****Entradas*************************************
* int anio - año a buscar
**************************************************/
double ObtenerSubtotalVentas(int anio) {
    Factura* listaFacturas = ObtenerFacturasPorAnio(anio);
    int cantFacturas = ObtenerCantidadFacturasPorAnio(anio);
    double res = 0;
    for (int i = 0; i<cantFacturas;i++)
        res+=listaFacturas[i].subtotal;
    return res;
}

/*****Nombre***************************************
* ObtenerImpuestosVentas
*****Descripción y objetivo**********************************
* Obtener el total de impuestos de ventas por un año en particular
*****Retorno**************************************
* double - total de impuestos de ventas del año
*****Entradas*************************************
* int anio - año a buscar
**************************************************/
double ObtenerImpuestoVentas(int anio) {
    Factura* listaFacturas = ObtenerFacturasPorAnio(anio);
    int cantFacturas = ObtenerCantidadFacturasPorAnio(anio);
    double res = 0;
    for (int i = 0; i<cantFacturas; i++)
        res+=listaFacturas[i].impuesto;
    return res;
}

/*****Nombre***************************************
* ObtenerTotalNomina
*****Descripción y objetivo**********************************
* Obtiene el total de la nomina de un año en particular
*****Retorno**************************************
* double - total de la nomina del año
*****Entradas*************************************
* int anio - año a buscar
**************************************************/
double ObtenerTotalNomina(int anio) {
    PlanillaConCantEmpleados* listaPlanillas = ObtenerPlanillasPorAnio(anio);
    int cantNominas = ObtenerCantidadPlanillasPorAnio(anio);
    double res = 0;
    for (int i = 0; i<cantNominas;i++)
        res+=listaPlanillas[i].monto_carga_social;
    return res;
}

/*****Nombre***************************************
* CedulaFueElegida
*****Descripción y objetivo**********************************
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

/*****Nombre***************************************
* VerificarCedulaEmpleado
*****Descripción y objetivo**********************************
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

/*****Nombre***************************************
* ImprimirListaDeEmpleados
*****Descripción y objetivo**********************************
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

/*****Nombre***************************************
* VerificarNumero
*****Descripción y objetivo**********************************
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
*****Descripción y objetivo**********************************
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
*****Descripción y objetivo**********************************
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

/*****Datos administrativos************************
* Nombre del archivo: area_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para el acceso a los datos de las áreas que
* servirán para las facturas.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**************************************************/

#ifndef AREA_DAO_H
#define AREA_DAO_H

#include "./conexion_mysql.h"

/**
 * @struct area
 * @brief Áreas de producción que irán en las facturas.
 */
struct area {
    int id; // Id en la tabla.
    char nombre[51]; // Nombre de la área.
    double dimensiones; // Dimensiones en mts. cuadrados.
    char producto_principal[51]; // Producto principal del área.
};

typedef struct area Area;

static MYSQL_RES *res;
static MYSQL_ROW row;

/*****Nombre************************************************************
* ObtenerCantidadAreas
*****Descripción********************************************************
* Obtiene la cantidad de áreas que está registradas en la base de datos.
* Tiene como objetivo utilizar esto en los ciclos recorriendo la lista
* de áreas.
*****Retorno************************************************************
* Un número (int) que será la cantidad de áreas de registradas en la
* base de datos.
*****Entradas***********************************************************
* Sin entradas.
************************************************************************/
int ObtenerCantidadAreas() {
    MYSQL *conn = Conectar();
    int cantidad = -1;
    char query[100];
    sprintf(query, "CALL get_areas()");

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return cantidad;
    }

    res = mysql_store_result(conn);
    cantidad = mysql_num_rows(res);

    mysql_free_result(res);
    mysql_close(conn);

    return cantidad;
}

/*****Nombre************************************************************
* ObtenerAreas
*****Descripción********************************************************
* Obiene la lista de todas las áreas registradas en la base de datos.
*****Retorno************************************************************
* (Area*) Lista de áreas en la base de datos.
*****Entradas***********************************************************
* Sin entradas.
************************************************************************/
Area* ObtenerAreas() {
    int cantidad = ObtenerCantidadAreas();
    if (cantidad == -1) { return NULL; }

    char query[100];
    sprintf(query, "CALL get_areas()");

    MYSQL *conn = Conectar();
    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return NULL;
    }

    res = mysql_store_result(conn);
    Area* areas = malloc(cantidad * sizeof *areas);

    int i = 0;
    while ((row = mysql_fetch_row(res))) {
        areas[i].id = atoi(row[0]);
        strcpy(areas[i].nombre, row[1]);
        areas[i].dimensiones = atof(row[2]);
        strcpy(areas[i].producto_principal, row[3]);
        i++;
    }

    mysql_free_result(res);
    mysql_close(conn);

    return areas;
}

#endif // AREA_DAO_H

/*****Datos administrativos*******************************
* Nombre del archivo: cargoSocial_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para el acceso a los datos del cargo social que
* que corresponderá a las cargas de los sueldos de los
* empleados de la empresa.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef CARGOSOCIAL_DAO_H
#define CARGOSOCIAL_DAO_H

#include "./conexion_mysql.h"

/**
 * @struct cargo_social
 * @brief Porcentaje de cargas sociales de los sueldos de los empleados.
 */
struct cargo_social {
    int id; // Id en la tabla.
    char nombre[51]; // Nombre de la carga social.
    double porcentaje; // Porcentaje del cargo social.
};

typedef struct cargo_social CargoSocial;

static MYSQL_RES *res;
static MYSQL_ROW row;

/*****Nombre************************************************************
* ObtenerCargoSocial
*****Descripción********************************************************
* Obtiene el cargo social buscando por el nombre, el programa por default
* utilizará solamente una carga social ´General´, pero se realizó de
* forma que se pueda extender más adelante.
*****Retorno************************************************************
* Un objeto/struct CargaSocial con toda la información de dicha
* carga social para calcular el sueldo de los empleados.
*****Entradas***********************************************************
* - (char *nombre) Nombre de la carga social a consultar.
************************************************************************/
CargoSocial ObtenerCargoSocial(char *nombre) {
    MYSQL *conn = Conectar();
    CargoSocial cargoSocial;
    cargoSocial.id = 0;
    
    char query[100];
    sprintf(query, "CALL get_cargo_social('%s')", nombre);

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return cargoSocial;
    }

    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res))) {
        cargoSocial.id = atoi(row[0]);
        strcpy(cargoSocial.nombre, row[1]);
        cargoSocial.porcentaje = atof(row[2]);
    }

    mysql_free_result(res);
    mysql_close(conn);

    return cargoSocial;
}

/*****Nombre************************************************************
* ModificarCargoSocial
*****Descripción********************************************************
* Busca un cargo social por el nombre y modifica el valor porcentual
* que corresponderá a dicha carga social.
*****Retorno************************************************************
* Un valor booleano que indicará el éxito o error de dicha modificación.
*****Entradas***********************************************************
* - (char *nombre) Nombre de la carga social a modificar.
* - (double porcentaje) El nuevo porcentaje de la carga social.
************************************************************************/
bool ModificarCargoSocial(char *nombre, double porcentaje) {
    MYSQL *conn = Conectar();
    char query[100];
    sprintf(query, "CALL modif_cargo_social('%s', %f)", nombre, porcentaje);

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return false;
    }
    mysql_close(conn);
    return true;
}


#endif // CARGOSOCIAL_DAO_H

/*****Datos administrativos*******************************
* Nombre del archivo: comercio_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para el acceso a los datos del comercio.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef COMERCIO_DAO_H
#define COMERCIO_DAO_H

#include "./conexion_mysql.h"

/**
 * @struct comercio
 * @brief Características o información básica del comercio.
 */
struct comercio {
    char cedulaJuridica[51];
    char nombre[51];
    char telefono[51];
    int numSigFactura;
};

typedef struct comercio Comercio;

static MYSQL_RES *res;
static MYSQL_ROW row;

/*****Nombre************************************************************
* RegistrarComercio
*****Descripción********************************************************
* Permite registrar un nuevo comercio en el sistema, originalmente
* la aplicación no requiere registrar comercios pero se da para
* la posibilidad de extender el programa más adelante.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito o fallo del registro del
* comercio en la base de datos.
*****Entradas***********************************************************
* - (Comercio comercio) el objeto/estructura comercio que se desea
*                       registrar en la base de datos.
************************************************************************/
bool RegistrarComercio(Comercio comercio) {
    MYSQL* conn = Conectar();

    char query[256];
    sprintf(
        query,
        "CALL registrar_comercio('%s', '%s', '%s', %d)",
        comercio.cedulaJuridica,
        comercio.nombre,
        comercio.telefono,
        comercio.numSigFactura
    );

    if (mysql_query(conn, query)) {
        fprintf(stderr, "\n[Error] %s\n", mysql_error(conn));
        mysql_close(conn);
        return false;
    }
    mysql_close(conn);
    return true;
}

/*****Nombre************************************************************
* ObtenerInfoComercio
*****Descripción********************************************************
* Busca el comercio utilizando la cédula jurídica del mismo y retorna 
* toda la información del mismo en un objeto/estructura Comercio.
*
* Nota: recordar que la aplicación está pensada para utilizar un solo
*       comercio para las facturas.
*****Retorno************************************************************
* Un (Comercio) con toda la información que se almacenó en la base de
* datos.
*****Entradas***********************************************************
* - (char *cedulaJuridica) La cédula del comercio que estamos buscando.
************************************************************************/
Comercio ObtenerInfoComercio(char *cedulaJuridica) {
    MYSQL *conn = Conectar();
    Comercio comercio;
    comercio.numSigFactura = -1; // -1 para indicar que no se encontró el comercio
    
    char query[100];
    sprintf(query, "CALL get_comercio('%s')", cedulaJuridica);
    
    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return comercio;
    }
    
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res))) {
        strcpy(comercio.cedulaJuridica, row[0]);
        strcpy(comercio.nombre, row[1]);
        strcpy(comercio.telefono, row[2]);
        comercio.numSigFactura = atoi(row[3]);
    }
    
    mysql_free_result(res);
    mysql_close(conn);
    
    return comercio;
}

#endif // COMERCIO_DAO_H

/*****Datos administrativos*******************************
* Nombre del archivo: conexion_mysql
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para la administración para la conexión a la
* base de datos local.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef CONEXION_MYSQL_H
#define CONEXION_MYSQL_H

#include <stdio.h>
#include <mysql/mysql.h>

static const char* SERVER = "localhost";
static const char* USUARIO = "root";
static const char* CLAVE = "root";
static const char* BASEDEDATOS = "pdvAgricola";

/*****Nombre************************************************************
* Conectar
*****Descripción********************************************************
* Realiza y retorna la conexión a la base de datos local.
*****Retorno************************************************************
* La conexión (MYSQL*) de la base datos.
*****Entradas***********************************************************
* No tiene entradas.
************************************************************************/
MYSQL* Conectar() {
    MYSQL* conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, SERVER, USUARIO, CLAVE, BASEDEDATOS, 0, NULL, 0)) {
        fprintf(stderr, "[Error] %s\n", mysql_error(conn));
        exit(1);
    }
    return conn;
}

#endif // CONEXION_MYSQL_H

/*****Datos administrativos*******************************
* Nombre del archivo: empleado_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo de acceso para los datos de los empleados
* del comercio.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef EMPLEADO_DAO_H
#define EMPLEADO_DAO_H

#include "./conexion_mysql.h"

/**
 * @struct empleado
 * @brief Abstracción que corresponde a los empleados del comercio.
 */
struct empleado {
    char cedula[51]; // Cédula del empleado
    char nombre_completo[256]; // Nombre del empleado
    int rol; // Id del rol del empleado
    double salario_mensual; // Salario del empleado (sin el cargo)
};

typedef struct empleado Empleado;

/**
 * @struct empleado_con_rol
 * @brief Abstracción de empleado con su respectivo rol (nombre del rol)
 */
struct empleado_con_rol {
    char cedula[51]; // Cédula del empleado
    char nombre_completo[256]; // Nombre del empleado.
    char nombre_rol[51]; // Nombre del rol (Administrador, etc...)
    double salario_mensual; // Salario del empleado (sin el cargo)
};

typedef struct empleado_con_rol EmpleadoConRol;

static MYSQL_RES *res;
static MYSQL_ROW row;

/*****Nombre************************************************************
* ObtenerCantidadEmpleados
*****Descripción********************************************************
* Retorna la cantidad de empleados que hay registrados en el comercio.
*
* Nota: como se utilizará de forma plana 10 empleados, está función
*       está pensando en la futura extensibilidad en caso de que se
*       quiera agregar más empleados en la base de datos.
*****Retorno************************************************************
* Un número (int) que corresponderá a la cantidad de empleados
* registrados en la base de datos.
*****Entradas***********************************************************
* No tiene entradas.
************************************************************************/
int ObtenerCantidadEmpleados() {
    MYSQL *conn = Conectar();
    int cantidad = 0;
    char query[256];
    sprintf(query, "SELECT COUNT(*) FROM Empleados");
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return -1;
    }

    res = mysql_store_result(conn);
    if (res) {
        row = mysql_fetch_row(res);
        cantidad = atoi(row[0]);
    }

    mysql_free_result(res);
    mysql_close(conn);

    return cantidad;
}

/*****Nombre************************************************************
* ObtenerEmpleadosConRol
*****Descripción********************************************************
* Retorna una lista (puntero) con todos los empleados con su rol (el
* nombre del puesto).
*****Retorno************************************************************
* Un puntero (EmpleadoConRol*) con todos los empleados registrados
* con el respectivo rol o cargo de la empresa.
*****Entradas***********************************************************
* No tiene entradas.
************************************************************************/
EmpleadoConRol* ObtenerEmpleadosConRol() {
    MYSQL *conn = Conectar();
    EmpleadoConRol *empleados = NULL;
    char query[256];
    sprintf(query, "CALL get_empleados_con_rol()");
    
    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return NULL;
    }
    
    res = mysql_store_result(conn);
    if (res) {
        int cantidad = mysql_num_rows(res);
        empleados = malloc(cantidad * sizeof *empleados);
        int i = 0;
        while ((row = mysql_fetch_row(res))) {
            strcpy(empleados[i].cedula, row[0]);
            strcpy(empleados[i].nombre_completo, row[1]);
            strcpy(empleados[i].nombre_rol, row[2]);
            empleados[i].salario_mensual = atof(row[3]);
            i++;
        }
    }
    
    mysql_free_result(res);
    mysql_close(conn);

    return empleados;
}

/*****Nombre************************************************************
* ObtenerEmpleados
*****Descripción********************************************************
* Retorna una lista (puntero) con todos los empleados.
*****Retorno************************************************************
* Un puntero (Empleado*) con todos los empleados registrados.
*****Entradas***********************************************************
* No tiene entradas.
************************************************************************/
Empleado* ObtenerEmpleados() {
    MYSQL *conn = Conectar();
    char query[256];
    sprintf(query, "CALL get_empleados()");
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return NULL;
    }

    res = mysql_store_result(conn);
    int cantidad = mysql_num_rows(res);
    Empleado* empleados = (Empleado*)malloc(sizeof(Empleado) * cantidad);
    int i = 0;
    while ((row = mysql_fetch_row(res))) {
        strcpy(empleados[i].cedula, row[0]);
        strcpy(empleados[i].nombre_completo, row[1]);
        empleados[i].rol = atoi(row[2]);
        empleados[i].salario_mensual = atof(row[3]);
        i++;
    }
    mysql_free_result(res);
    mysql_close(conn);

    return empleados;
}

/*****Nombre************************************************************
* ObtenerEmpleadoConRol
*****Descripción********************************************************
* Busca un empleado utilizando la cédula de este en base datos y
* retorna al empleado con su respectivo rol o cargo en la empresa.
*****Retorno************************************************************
* Un (EmpleadoConRol) con sus atributos.
*****Entradas***********************************************************
* No tiene entradas.
************************************************************************/
EmpleadoConRol ObtenerEmpleadoConRol(char *cedula) {
    MYSQL *conn = Conectar();
    EmpleadoConRol empleado;
    strcpy(empleado.cedula, "ND");
    char query[256];
    sprintf(query, "CALL get_empleado_con_rol('%s')", cedula);
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return empleado;
    }
    
    res = mysql_store_result(conn);
    if (res) {
        row = mysql_fetch_row(res);
        strcpy(empleado.cedula, row[0]);
        strcpy(empleado.nombre_completo, row[1]);
        strcpy(empleado.nombre_rol, row[2]);
        empleado.salario_mensual = atof(row[3]);
    }
    
    mysql_free_result(res);
    mysql_close(conn);

    return empleado;
}

/*****Nombre************************************************************
* ObtenerEmpleado
*****Descripción********************************************************
* Busca un empleado utilizando la cédula de este en base datos y retorna
* su información en una estructura (Empleado).
*****Retorno************************************************************
* Un (Empleado) con sus atributos.
*
* Nota: en caso de que no existe un empleado con dicha cédula
*       se retornará la estructura empleado con cédula "ND" que indica
*       que no está definido.
*****Entradas***********************************************************
* - (char *cedula) La cédula del empleado a consultar.
************************************************************************/
Empleado ObtenerEmpleado(char *cedula) {
    MYSQL *conn = Conectar();
    Empleado empleado;
    strcpy(empleado.cedula, "ND");
    char query[256];
    sprintf(query, "CALL get_empleado('%s')", cedula);
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return empleado;
    }
    
    res = mysql_store_result(conn);
    if (res) {
        row = mysql_fetch_row(res);
        strcpy(empleado.cedula, row[0]);
        strcpy(empleado.nombre_completo, row[1]);
        empleado.rol = atoi(row[2]);
        empleado.salario_mensual = atof(row[3]);
    }
    
    mysql_free_result(res);
    mysql_close(conn);

    return empleado;
}

/*****Nombre************************************************************
* ExisteEmpleado
*****Descripción********************************************************
* Consulta si existe un empleado específico en la base de datos
* utilizando la cédula como criterio de búsqueda.
*****Retorno************************************************************
* Un valor booleano que indica si existe (true) o si no (false).
*****Entradas***********************************************************
* - (char *cedula) La cédula del empleado a consultar.
************************************************************************/
bool ExisteEmpleado(char *cedula) {
    Empleado empleado = ObtenerEmpleado(cedula);
    return strcmp(empleado.cedula, "ND") != 0;
}

#endif // EMPLEADO_DAO_H

/*****Datos administrativos*******************************
* Nombre del archivo: factura_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para el acceso a los datos de las facturas
* del comercio en la base de datos.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef FACTURA_DAO_H
#define FACTURA_DAO_H

#include "./producto_dao.h"

/**
 * @struct detalleFactura
 * @brief Corresponde a los detalles de la factura.
 */
struct detalleFactura {
    Producto producto;
    int cantidad;
};

typedef struct detalleFactura DetalleFactura;

/**
 * @struct factura
 * @brief Abstracción que corresponde a las facturas
 *        almacenadas en base de datos.
 */
struct factura {
    char nombre_comercio[51]; // Nombre del comercio.
    char cedula_comercio[51]; // Default (Solo se usa una) 001.
    char telefono_comercio[51]; // Teléfono del comercio.
    int id_factura; // Id de la factura en la tabla.
    char nombre_cliente[51]; // Nombre del cliente en la factura.
    Fecha fecha_facturacion; // Fecha de la facturación.
    char nombre_area[51]; // Nombre del área del cual.
    int id_area; // Id del área.
    double subtotal; // Monto del subtotal.
    double impuesto; // Monto del impuesto (Porcentual).
    double total; // Monto total (subtotal + impuesto).
    DetalleFactura* detalles; // Lista con los detalles de la factura.
};

typedef struct factura Factura;

struct reporteAnual {
    double subtotal_ventas;
    double total_impuesto;
};

typedef struct reporteAnual ReporteAnual;

struct rendimientoArea {
    char nombre_area[51];
    double subtotal_ventas;
};

typedef struct rendimientoArea RendimientoArea;

/*****Nombre************************************************************
* ObtenerReporteAnual
*****Descripción********************************************************
* Obtiene el reporte anual de ventas de un negocio en particular.
*****Retorno************************************************************
* Un ReporteAnual de las ventas del negocio.
*****Entradas***********************************************************
* - char* cedula_comercio: Cedula del comercio.
* - int anio: Año de las facturas.
************************************************************************/
ReporteAnual ObtenerReporteAnual(char* cedula_comercio, int anio) {
    ReporteAnual reporte;
    reporte.subtotal_ventas = -1;
    reporte.total_impuesto = -1;
    
    MYSQL *conn = Conectar();
    if (conn == NULL) {
        return reporte;
    }

    char query[200];
    sprintf(query, "CALL get_ventas_por_anio('%s', %d)", cedula_comercio, anio);

    int result = mysql_query(conn, query);
    if (result != 0) {
        return reporte;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL) {
        return reporte;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (row == NULL) {
        return reporte;
    }

    reporte.subtotal_ventas = atof(row[0]);
    reporte.total_impuesto = atof(row[1]);
    return reporte;
}

/*****Nombre************************************************************
 * ObtenerRendimientoAreas
 * ***********************************************************************
 * Obtiene el rendimiento de las areas del local segun el mes y el año.
 * ***********************************************************************
 * Retorno: Un arreglo de RendimientoArea.
 * ***********************************************************************
 * Entradas:
 * - int mes: Mes de las facturas.
 * - int anio: Año de las facturas.
 * ***********************************************************************/
RendimientoArea* ObtenerRendimientoPorAreas(int mes, int anio) {
    RendimientoArea* rendimiento = NULL;
    MYSQL *conn = Conectar();
    if (conn == NULL) {
        return rendimiento;
    }
    
    char query[200];
    sprintf(query, "CALL get_rendimiento_de_areas_por_fecha(%d, %d)", mes, anio);
    
    int result = mysql_query(conn, query);
    if (result != 0) {
        return rendimiento;
    }
    
    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL) {
        return rendimiento;
    }
    
    rendimiento = malloc(mysql_num_rows(res) * sizeof *rendimiento);
    if (rendimiento == NULL) {
        return rendimiento;
    }

    int i = 0;
    while ( (row = mysql_fetch_row(res)) != NULL) {
        strcpy(rendimiento[i].nombre_area, row[0]);
        rendimiento[i].subtotal_ventas = atof(row[1]);
        i++;
    }

    rendimiento[i].subtotal_ventas = -1;

    mysql_close(conn);
    
    return rendimiento;
}

/*****Nombre************************************************************
* ObtenerAniosConFacturas
*****Descripción********************************************************
* Obtiene todos los años en los que hay facturas en la base de datos.
*****Retorno************************************************************
* Un arreglo de enteros con los años.
*****Entradas***********************************************************
* Sin entradas
************************************************************************/
int* ObtenerAniosConFacturas() {
    int* anios = NULL;
    int anios_count = 0;
    MYSQL *conn = Conectar();
    if (conn == NULL) {
        return anios;
    }
    char query[200];
    sprintf(query, "CALL get_anios_con_facturas()");
    int result = mysql_query(conn, query);
    if (result != 0) {
        return anios;
    }
    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL) {
        return anios;
    }
    MYSQL_ROW row = mysql_fetch_row(res);
    while (row != NULL) {
        anios = realloc(anios, sizeof(int) * (anios_count + 1));
        anios[anios_count] = atoi(row[0]);
        anios_count++;
        row = mysql_fetch_row(res);
    }
    return anios;
}

/*****Nombre************************************************************
* ObtenerCantidadAniosConFacturas
*****Descripción********************************************************
* Obtiene la cantidad de años en los que hay facturas en la base de datos.
*****Retorno************************************************************
* Un entero con la cantidad de años.
*****Entradas***********************************************************
* No tiene
************************************************************************/
int ObtenerCantidadAniosConFacturas() {
    MYSQL* conn = Conectar();
    if (conn == NULL) {
        return 0;
    }
    char query[200];
    sprintf(query, "CALL get_cant_anios_con_facturas()");
    int result = mysql_query(conn, query);
    if (result != 0) {
        return 0;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    if (res == NULL) {
        return 0;
    }
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row == NULL) {
        return 0;
    }
    int cantidad = atoi(row[0]);
    mysql_close(conn);
    return cantidad;
}

/*****Nombre************************************************************
* RegistrarFactura
*****Descripción********************************************************
* Registra una factura en la base de datos.
*****Retorno************************************************************
* Un número (int) que corresponderá a la id en la tabla de la nueva
* factura que se ha registrado. En caso de existir algún error en el
* registro, se retornará un id (-1) indicando que no se registró la
* factura.
*****Entradas***********************************************************
* - (Factura factura) La factura como tal que se desea registrar.
************************************************************************/
int RegistrarFactura(Factura factura) {
    MYSQL *conn = Conectar();

    Fecha fechaFactura = factura.fecha_facturacion;
    char fecha_str[25];
    sprintf(fecha_str, "%d-%d-%d", fechaFactura.anio, fechaFactura.mes, fechaFactura.dia);
    char query[1000];
    sprintf(
        query,
        "CALL create_factura('%s', '%s', '%d', '%s', '%f', '%f', '%f')",
        factura.nombre_cliente, fecha_str, factura.id_area, factura.cedula_comercio, factura.subtotal, factura.impuesto, factura.total
    );

    int result = mysql_query(conn, query);
    if (result != 0) {
        printf("Error al registrar la factura: %s\n", mysql_error(conn));
        return -1;
    }
    
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    int id_factura = atoi(row[0]);

    mysql_free_result(res);
    mysql_close(conn);
    
    return id_factura;
}

/*****Nombre************************************************************
* ObtenerCantidadDeDetalles
*****Descripción********************************************************
* Obtiene la cantidad de detalles que tiene una factura, utilizando
* el id de la factura.
*****Retorno************************************************************
* Un número (int) que corresponde a la cantidad de detalles que tiene
* la factura.
*****Entradas***********************************************************
* - (ind id_factura) El id de la factura la cual se desea consultar.
************************************************************************/
int ObtenerCantidadDeDetalles(int id_factura) {
    MYSQL *conn = Conectar();
    int cantidad = -1;
    char query[100];
    sprintf(query, "CALL get_cantidad_detalles_factura(%d)", id_factura);

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return cantidad;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    cantidad = atoi(row[0]);

    mysql_free_result(res);
    mysql_close(conn);

    return cantidad;
}

/*****Nombre************************************************************
* ObtenerFactura
*****Descripción********************************************************
* Obtiene toda la información de una (Factura) que se encuentre
* que se encuentre registrada en la base de datos. Buscando la factura
* por su id en la tabla.
*****Retorno************************************************************
* Un objeto (Factura) con toda su información obtenida de la base
* de datos.
*****Entradas***********************************************************
* - (int id_factura) El id de la factura en la tabla.
************************************************************************/
Factura ObtenerFactura(int id_factura) {

    Factura factura;
    factura.id_factura = -1;

    MYSQL *conn = Conectar();
    char query[250];
    sprintf(query, "CALL get_factura_por_id(%d)", id_factura);

    int result = mysql_query(conn, query);
    if (result != 0) {
        printf("Error al obtener la factura: %s\n", mysql_error(conn));
        return factura;
    }
    
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (row == NULL) {
        printf("No se encontro la factura\n");
        return factura;
    }
    
    strcpy(factura.nombre_comercio, row[0]);
    strcpy(factura.cedula_comercio, row[1]);
    strcpy(factura.telefono_comercio, row[2]);
    strcpy(factura.nombre_cliente, row[3]);
    factura.id_factura = id_factura;

    char *fecha_str = row[4];
    Fecha fecha;
    sscanf(fecha_str, "%d-%d-%d", &fecha.dia, &fecha.mes, &fecha.anio);
    factura.fecha_facturacion = fecha;

    strcpy(factura.nombre_area, row[5]);
    factura.id_area = atoi(row[6]);
    factura.subtotal = atof(row[7]);
    factura.impuesto = atof(row[8]);
    factura.total = atof(row[9]);

    mysql_free_result(res);
    mysql_close(conn);

    return factura;
}

/*****Nombre************************************************************
* RegistrarDetalleFactura
*****Descripción********************************************************
* Registra un nuevo detalle a una factura existente en la base de
* datos.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso del 
* registro del detalle.
*****Entradas***********************************************************
* - (int id_factura) El id de la factura en la tabla.
* - (Producto producto) El producto que se desea registrar en el detalle.
* - (int cantidad) La cantidad de producto que se está registrando.
************************************************************************/
bool RegistrarDetalleFactura(int id_factura, Producto producto, int cantidad) {
    MYSQL *conn = Conectar();
    char query[250];
    sprintf(query, "CALL create_factXdetprod('%s', %d, %d)", producto.id, id_factura, cantidad);
    int result = mysql_query(conn, query);
    if (result != 0) {
        printf("Error al registrar el detalle de la factura: %s\n", mysql_error(conn));
        return false;
    }
    mysql_close(conn);
    return true;
}

/*****Nombre************************************************************
* ObtenerDetallesFactura
*****Descripción********************************************************
* Obtiene todos los detalles de una factura, buscandola por su id.
*****Retorno************************************************************
* Una lista (puntero) de todos los detalles (DetalleFactura) de una
* (Factura) que se encuentre registrada en la base de datos.
*****Entradas***********************************************************
* - (int id_factura) El id de la factura en la tabla.
************************************************************************/
DetalleFactura* ObtenerDetallesFactura(int id_factura) {
    MYSQL *conn = Conectar();
    char query[250];
    sprintf(query, "CALL get_factXdetprod(%d)", id_factura);
    int result = mysql_query(conn, query);
    if (result != 0) {
        printf("Error al obtener los detalles de la factura: %s\n", mysql_error(conn));
        return NULL;
    }
    
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    int cantidad = mysql_num_rows(res);
    DetalleFactura *detalles = malloc(cantidad * sizeof *detalles);
    for (int i = 0; i < cantidad; i++) {
        Producto p;
        strcpy(p.id, row[0]);
        strcpy(p.nombre, row[1]);
        p.costo = atof(row[2]);
        p.impuesto = atof(row[3]);
        detalles[i].producto = p;
        detalles[i].cantidad = atoi(row[4]);
        row = mysql_fetch_row(res);
    }
    mysql_free_result(res);
    mysql_close(conn);
    return detalles;
}

/*****Nombre************************************************************
* ObtenerFacturasComercio
*****Descripción********************************************************
* Obtiene todas las facturas de un comercio, buscando por su cedula.
*****Retorno************************************************************
* Una lista (puntero) de todas las facturas (Factura) de un comercio.
*****Entradas***********************************************************
* - char* cedula_comercio: Cedula del comercio.
************************************************************************/
Factura* ObtenerFacturasComercio(char* cedula_comercio) {
    MYSQL *conn = Conectar();
    char query[250];
    sprintf(query, "CALL get_facturas('%s')", cedula_comercio);
    int result = mysql_query(conn, query);
    if (result != 0) {
        printf("Error al obtener las facturas del comercio: %s\n", mysql_error(conn));
        return NULL;
    }
    
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    int cantidad = mysql_num_rows(res);
    Factura *facturas = malloc(cantidad * sizeof *facturas);
    for (int i = 0; i < cantidad; i++) {
        Factura f;
        strcpy(f.nombre_comercio, row[0]);
        strcpy(f.cedula_comercio, row[1]);
        strcpy(f.telefono_comercio, row[2]);
        f.id_factura = atoi(row[3]);
        strcpy(f.nombre_cliente, row[4]);
        char *fecha_str = row[5];
        Fecha fecha;
        sscanf(fecha_str, "%d-%d-%d", &fecha.dia, &fecha.mes, &fecha.anio);
        f.fecha_facturacion = fecha;
        strcpy(f.nombre_area, row[6]);
        f.subtotal = atof(row[7]);
        f.impuesto = atof(row[8]);
        f.total = atof(row[9]);
        facturas[i] = f;
        row = mysql_fetch_row(res);
    }
    mysql_free_result(res);
    mysql_close(conn);
    return facturas;
}

/*****Nombre************************************************************
* ObtenerFacturasPorAnio
*****Descripción********************************************************
* Obtiene todas las facturas de un comercio, buscando por un año.
*****Retorno************************************************************
* Una lista (puntero) de todas las facturas (Factura) de un comercio.
*****Entradas***********************************************************
* - int anio: Año de las facturas.
************************************************************************/
Factura* ObtenerFacturasPorAnio(int p_anio) {
    MYSQL *conn = Conectar();
    char query[250];
    sprintf(query, "CALL get_facturas_por_anio(%i)", p_anio);
    int result = mysql_query(conn, query);
    if (result != 0) {
        printf("Error al obtener las facturas del comercio: %s\n", mysql_error(conn));
        return NULL;
    }
    
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    int cantidad = mysql_num_rows(res);
    Factura *facturas = malloc(cantidad * sizeof *facturas);
    for (int i = 0; i < cantidad; i++) {
        Factura f;
        strcpy(f.nombre_comercio, row[0]);
        strcpy(f.cedula_comercio, row[1]);
        strcpy(f.telefono_comercio, row[2]);
        f.id_factura = atoi(row[3]);
        strcpy(f.nombre_cliente, row[4]);
        char *fecha_str = row[5];
        Fecha fecha;
        sscanf(fecha_str, "%d-%d-%d", &fecha.dia, &fecha.mes, &fecha.anio);
        f.fecha_facturacion = fecha;
        strcpy(f.nombre_area, row[6]);
        f.subtotal = atof(row[7]);
        f.impuesto = atof(row[8]);
        f.total = atof(row[9]);
        facturas[i] = f;
        row = mysql_fetch_row(res);
    }
    mysql_free_result(res);
    mysql_close(conn);
    return facturas;
}

/*****Nombre************************************************************
* ObtenerReporteAnual
*****Descripción********************************************************
* Obtiene el reporte anual de ventas de un negocio en particular.
*****Retorno************************************************************
* Un ReporteAnual de las ventas del negocio.
*****Entradas***********************************************************
* - char* cedula_comercio: Cedula del comercio.
* - int anio: Año de las facturas.
************************************************************************/
int ObtenerCantidadFacturas(char* cedula_comercio) {
    MYSQL *conn = Conectar();
    char query[250];
    sprintf(query, "CALL get_cantidad_facturas('%s')", cedula_comercio);
    int result = mysql_query(conn, query);
    if (result != 0) {
        printf("Error al obtener la cantidad de facturas del comercio: %s\n", mysql_error(conn));
        return -1;
    }
    
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    int cantidad = atoi(row[0]);
    mysql_free_result(res);
    mysql_close(conn);
    return cantidad;
}

/*****Nombre************************************************************
* ObtenerCantidadFacturasPorAnio
*****Descripción********************************************************
* Obtiene la cantidad de facturas de un comercio, buscando por un año.
*****Retorno************************************************************
* UL
*****Entradas***********************************************************
* - char* cedula_comercio: Cedula del comercio.
* - int anio: Año de las facturas.
************************************************************************/
int ObtenerCantidadFacturasPorAnio(int anio) {
    MYSQL *conn = Conectar();
    char query[250];
    sprintf(query, "CALL get_cant_facturas_por_anio(%i)", anio);
    int result = mysql_query(conn, query);
    if (result != 0) {
        return -1;
    }
    
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    int cantidad = atoi(row[0]);
    mysql_free_result(res);
    mysql_close(conn);
    return cantidad;
}

#endif // FACTURA_DAO_H

/*****Datos administrativos*******************************
* Nombre del archivo: operador_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para el acceso a los datos de los operadores
* del comercio en la base de datos.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef OPERADOR_DAO_H
#define OPERADOR_DAO_H

#include "./conexion_mysql.h"

/**
 * @struct operador
 * @brief Los operadores o usuarios del sistema.
 */
struct operador {
    int id; // Id en la tabla
    char usuario[51]; // Nombre de usuario
    char clave[256]; // Clave del usuario
};

typedef struct operador Operador;

static MYSQL_RES *res;
static MYSQL_ROW row;

/*****Nombre************************************************************
* RegistrarOperador
*****Descripción********************************************************
* Registra un nuevo operador en la base de datos.
*
* No es requerido utilizar ya que habrá un solo operador en la base
* de datos, pero se puede utilizar en una futura extensión del programa.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso (false)
* del registro.
*****Entradas***********************************************************
* - (Operador operador) El nuevo operador que se desee registrar.
************************************************************************/
bool RegistrarOperador(Operador operador) {
    MYSQL* conn = Conectar();

    char query[256];
    sprintf(
        query,
        "CALL create_usuario('%s', '%s')",
        operador.usuario,
        operador.clave
    );

    if (mysql_query(conn, query)) {
        // Está bien que no se imprima el error porque es una operación
        // para validar el usuario y la clave.
        mysql_close(conn);
        return false;
    }
    mysql_close(conn);
    return true;
}

/*****Nombre************************************************************
* ObtenerOperador
*****Descripción********************************************************
* Obtiene toda la información del (Operador) que se obtiene de la
* base de datos, buscándolo por su nombre de usuario.
*****Retorno************************************************************
* Una estructura (Operador) con toda la información obtenida de la base
* de datos.
*****Entradas***********************************************************
* - (char *usuario) Nombre de usuario del operador.
************************************************************************/
Operador ObtenerOperador(char* usuario) {
    MYSQL *conn = Conectar();

    Operador operador;
    operador.id = 0;
    char query[256];

    sprintf(
        query,
        "CALL get_usuario('%s')",
        usuario
    );
    if (mysql_query(conn, query)) {
        return operador;
    }
    res = mysql_use_result(conn);

    row = mysql_fetch_row(res);
    if (row == NULL) {
        return operador;
    }

    operador.id = atoi(row[0]);
    strcpy(operador.usuario, row[1]);
    strcpy(operador.clave, row[2]);

    mysql_free_result(res);
    mysql_close(conn);

    return operador;
}

/*****Nombre************************************************************
* ValidarOperador
*****Descripción********************************************************
* Valida la información de registro o credenciales en la base de datos.
*****Retorno************************************************************
* Un valor booleano que indica si se dieron las credenciales
* correctas (true) o si no coinciden (false) el usuario-contraseña.
*****Entradas***********************************************************
* - (Operador operador) El operador que se desea logear/verificar.
************************************************************************/
bool ValidarOperador(Operador operador) {
    Operador operador_db = ObtenerOperador(operador.usuario);

    // Simplemente, no existe el usuario.
    if (operador_db.id == 0) {
        return false;
    }

    // El usuario existe, pero la clave no coincide.
    if (strcmp(operador.clave, operador_db.clave) != 0) {
        return false;
    }
    return true;
}

#endif // OPERADOR_DAO_H

/*****Datos administrativos*******************************
* Nombre del archivo: planilla_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para el acceso a los datos de las planillas
* del comercio en la base de datos.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef PLANILLA_DAO_H
#define PLANILLA_DAO_H

#include "./empleado_dao.h"

/**
 * @struct fecha
 * @brief Una simple estructura de fecha.
 */
struct fecha {
    int anio;
    int mes;
    int dia;
};

typedef struct fecha Fecha;

/**
 * @struct planilla
 * @brief Una planilla registrada del comercio
 */
struct planilla {
    int id; // Id de la planilla en la tabla.
    Fecha fecha; // Fecha correspondiente a la planilla.
    double monto_carga_social; // El monto de la carga social para la planilla.
    EmpleadoConRol* empleadosConRol; // Lista de empleados de la planilla.
};

typedef struct planilla Planilla;

struct planillaCantEmpleados {
    int id; // Id de la planilla en la tabla.
    Fecha fecha; // Fecha correspondiente a la planilla.
    double monto_carga_social; // El monto de la carga social para la planilla.
    int cantidad_empleados; // La cantidad de empleados de la planilla.
    EmpleadoConRol* empleadosConRol; // Lista de empleados de la planilla.
};

typedef struct planillaCantEmpleados PlanillaConCantEmpleados;

static MYSQL_RES *res;
static MYSQL_ROW row;

/*****Nombre************************************************************
* ObtenerAniosConPlanillas
*****Descripción********************************************************
* Obtiene los años con planillas registradas en la base de datos.
*****Retorno************************************************************
* int* - Un arreglo de enteros con los años con planillas registradas.
*****Entradas***********************************************************
* No tiene entradas
************************************************************************/
int* ObtenerAniosConPlanillas() {
    MYSQL *conn = Conectar();
    int* anios = NULL;
    int cantidad = 0;
    char query[200];
    sprintf(query, "CALL get_anios_con_planillas()");
    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return anios;
    }
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res))) {
        anios = realloc(anios, sizeof(int) * (cantidad + 1));
        anios[cantidad] = atoi(row[0]);
        cantidad++;
    }
    mysql_close(conn);
    return anios;
}

/*****Nombre************************************************************
* ObtenerCantidadAniosConPlanillas
*****Descripción********************************************************
* Obtiene la cantidad de años con planillas registradas en la base de datos.
*****Retorno************************************************************
* Un entero con la cantidad de años con planillas registradas.
*****Entradas***********************************************************
* Sin entradas
************************************************************************/
int ObtenerCantidadAniosConPlanillas() {
    MYSQL *conn = Conectar();
    int cantidad = 0;
    char query[200];
    sprintf(query, "CALL get_cant_anios_con_planillas()");
    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return cantidad;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    cantidad = atoi(row[0]);
    mysql_close(conn);
    return cantidad;
}

/*****Nombre************************************************************
* ObtenerCantidadPlanillasPorAnio
*****Descripción********************************************************
* Obtiene la cantidad de planillas registradas en un año en la base de datos.
*****Retorno************************************************************
* int - La cantidad de planillas registradas en el año.
*****Entradas***********************************************************
* int anio - El año del cual se quiere obtener la cantidad de planillas.
************************************************************************/
int ObtenerCantidadPlanillasPorAnio(int anio) {
    MYSQL *conn = Conectar();
    int cantidad = 0;
    char query[200];
    sprintf(query, "CALL get_cantidad_planillas_por_anio(%i)", anio);
    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return -1;
    }
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    cantidad = atoi(row[0]);
    mysql_close(conn);
    return cantidad;
}

/*****Nombre************************************************************
* ObtenerTotalNominasConCargasPorAnio
*****Descripción********************************************************
* Obtiene el total de las nominas de los empleados de un anio dado.
* Calculado con el monto de la carga social.
*****Retorno************************************************************
* double - El total de las nominas de los empleados de un anio dado.
*****Entradas***********************************************************
* - int anio: El anio de las nominas a obtener.
************************************************************************/
double ObtenerTotalNominasConCargasPorAnio(int anio) {
    MYSQL *conn = Conectar();
    char query[200];
    sprintf(query, "CALL get_total_nomina_con_cargas_por_anio(%d)", anio);
    
    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return -1;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    double total = atof(row[0]);
    mysql_close(conn);
    return total;
}

/*****Nombre************************************************************
* RegistrarPlanilla
*****Descripción********************************************************
* Registra una nueva planilla en la base de datos.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso (false)
* del registro.
*****Entradas***********************************************************
* - (Planilla planilla) La nueva planilla que se desea registrar.
************************************************************************/
bool RegistrarPlanilla(Planilla planilla) {
    MYSQL *conn = Conectar();
    char query[256];
    char fecha_str[25];
    sprintf(
        fecha_str, "%d-%d-%d", 
        planilla.fecha.anio, 
        planilla.fecha.mes, 
        planilla.fecha.dia
    );

    sprintf(
        query, "CALL registrar_planilla('%s')",
        fecha_str
    );
    
    if (mysql_query(conn, query)) {
        mysql_free_result(res);
        mysql_close(conn);
        return false;
    }
    
    mysql_free_result(res);
    mysql_close(conn);

    return true;
}

/*****Nombre************************************************************
* EliminarPlanillaPorFecha
*****Descripción********************************************************
* Elimina una planilla de la base de datos.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso (false)
* del eliminado.
*****Entradas***********************************************************
* - (Fecha fecha) La fecha de la planilla que se desea eliminar.
************************************************************************/
bool EliminarPlanillaPorFecha(Fecha fecha) {
    MYSQL *conn = Conectar();
    char query[256];

    sprintf(
        query, "CALL eliminar_planilla_por_fecha('%i', '%i')",
        fecha.mes, fecha.anio
    );
    
    if (mysql_query(conn, query)) {
        printf("%s\n", mysql_error(conn));
        mysql_close(conn);
        
        return false;
    }
    mysql_close(conn);

    return true;
}

/*****Nombre************************************************************
* RegistrarEmpleadoAPlanilla
*****Descripción********************************************************
* Registra un empleado a una planilla existente, utilizando la fecha
* que se indicó en la planilla.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso (false)
* del registro.
*****Entradas***********************************************************
* - (Fecha fecha) La fecha de la planilla de la cuál se desea registrar
*                 al empleado.
* - (char *cedula_empleado) La cédula del empleado que se desea
*                           registrar.
************************************************************************/
bool RegistrarEmpleadoAPlanilla(Fecha fecha, char *cedula_empleado) {
    MYSQL *conn = Conectar();
    char query[256];
    char fecha_str[25];
    sprintf(
        fecha_str, "%d-%d-%d", 
        fecha.anio, 
        fecha.mes, 
        fecha.dia
    );

    sprintf(
        query, "CALL create_planXemp_fecha('%s', '%s')",
        fecha_str, cedula_empleado
    );
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        mysql_close(conn);
        return false;
    }
    
    mysql_close(conn);

    return true;
}

/*****Nombre************************************************************
* EliminarEmpleadoDePlanilla
*****Descripción********************************************************
* Elimina un empleado en específico de una planilla existente.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso (false)
* del borrado.
*****Entradas***********************************************************
* - (Fecha fecha) La fecha de la planilla que se desea modificar.
* - (char *cedula_empleado) La cédula del empleado que se desea remover.
************************************************************************/
bool EliminarEmpleadoDePlanilla(Fecha fecha, char *cedula_empleado) {
    MYSQL *conn = Conectar();
    char query[256];
    char fecha_str[25];
    sprintf(
        fecha_str, "%d-%d-%d", 
        fecha.anio, 
        fecha.mes, 
        fecha.dia
    );

    sprintf(
        query, "CALL remove_planXemp_fecha('%s', '%s')",
        fecha_str, cedula_empleado
    );
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        mysql_close(conn);
        return false;
    }
    
    mysql_close(conn);

    return true;
}

/*****Nombre************************************************************
* ObtenerPlanillaPorFecha
*****Descripción********************************************************
* Obtiene la estructura (Planilla) por medio de una fecha indicada.
*****Retorno************************************************************
* Una estructura (Planilla) con toda la información obtenida desde la
* base de datos.
*****Entradas***********************************************************
* - (Fecha fecha) La fecha de la planilla que se desea buscar.
************************************************************************/
Planilla ObtenerPlanillaPorFecha(Fecha fecha) {
    MYSQL *conn = Conectar();
    char query[256];

    sprintf(
        query, "CALL get_planilla_por_fecha('%i', '%i')",
        fecha.mes, fecha.anio
    );

    Planilla planilla;
    planilla.id = -1;
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return planilla;
    }
    
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    if (row == NULL) {
        return planilla;
    }
    
    planilla.id = atoi(row[0]);
    char *fecha_plan_bd = row[1];
    sscanf(
        fecha_plan_bd, "%d-%d-%d", 
        &planilla.fecha.anio, 
        &planilla.fecha.mes, 
        &planilla.fecha.dia
    );
    planilla.monto_carga_social = atof(row[2]);
    
    mysql_free_result(res);
    mysql_close(conn);

    return planilla;
}

/*****Nombre************************************************************
* ObtenerPlanillasConCantEmpleados
*****Descripción********************************************************
* Obtiene las planillas con la cantidad de empleados registrados en
* cada una.
*****Retorno************************************************************
* Un arreglo (PlanillaConCantEmpleados *) con todas las planillas obtenidas.
*****Entradas***********************************************************
Sin entradas
************************************************************************/
PlanillaConCantEmpleados* ObtenerPlanillasConCantEmpleados() {
    MYSQL *conn = Conectar();
    char query[256];
    sprintf(
        query,
        "CALL get_planillas_con_cant_empleados()"
    );

    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return NULL;
    }


    res = mysql_store_result(conn);
    if (res) {
        int cant_planillas = mysql_num_rows(res) + 1;
        if (mysql_num_rows(res) == 0) {
            mysql_free_result(res);
            mysql_close(conn);
            return NULL;
        }
        PlanillaConCantEmpleados *planillas = malloc(
            cant_planillas * sizeof *planillas
        );
        int i = 0;
        while ((row = mysql_fetch_row(res)) && i < cant_planillas) {
            planillas[i].id = atoi(row[0]);
            char *fecha_plan_bd = row[1];
            sscanf(
                fecha_plan_bd, "%d-%d-%d", 
                &planillas[i].fecha.anio, 
                &planillas[i].fecha.mes, 
                &planillas[i].fecha.dia
            );
            planillas[i].monto_carga_social = atof(row[2]);
            planillas[i].cantidad_empleados = atoi(row[3]);
            i++;
        }
        planillas[i].id = -1;
        mysql_free_result(res);
        mysql_close(conn);
        return planillas;
    }
    mysql_close(conn);
    return NULL;
}

/*****Nombre************************************************************
* ObtenerPlanillasPorAnio
*****Descripción********************************************************
* Tiene todas las planillas de un año determinado.
*****Retorno************************************************************
* Un arreglo de estructuras (Planilla) con todas las planillas de un año.
*****Entradas***********************************************************
* - int anio: Año de las facturas.
************************************************************************/
PlanillaConCantEmpleados* ObtenerPlanillasPorAnio(int p_anio) {
    MYSQL *conn = Conectar();
    char query[256];
    sprintf(
        query,
        "CALL get_planillas_por_anio_con_cant_empleados(%i)",
        p_anio
    );

    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return NULL;
    }


    res = mysql_store_result(conn);
    if (res) {
        int cant_planillas = mysql_num_rows(res) + 1;
        if (mysql_num_rows(res) == 0) {
            mysql_free_result(res);
            mysql_close(conn);
            return NULL;
        }
        PlanillaConCantEmpleados *planillas = malloc(
            cant_planillas * sizeof *planillas
        );
        int i = 0;
        while ((row = mysql_fetch_row(res)) && i < cant_planillas) {
            planillas[i].id = atoi(row[0]);
            char *fecha_plan_bd = row[1];
            sscanf(
                fecha_plan_bd, "%d-%d-%d", 
                &planillas[i].fecha.anio, 
                &planillas[i].fecha.mes, 
                &planillas[i].fecha.dia
            );
            planillas[i].monto_carga_social = atof(row[2]);
            planillas[i].cantidad_empleados = atoi(row[3]);
            i++;
        }
        planillas[i].id = -1;
        mysql_free_result(res);
        mysql_close(conn);
        return planillas;
    }
    mysql_close(conn);
    return NULL;
}

/*****Nombre************************************************************
* ObtenerCantEmpleadosPorPlanilla
*****Descripción********************************************************
* Obtiene la cantidad de empleados que están asignados a una planilla
* en específico.
*****Retorno************************************************************
* Un número (int) con la cantidad de empleados de la planilla.
*****Entradas***********************************************************
* - (int idPlanilla) El identificador de la planilla en la tabla.
************************************************************************/
int ObtenerCantEmpleadosPorPlanilla(int idPlanilla) {
    MYSQL *conn = Conectar();
    char query[256];
    sprintf(
        query, "CALL get_cant_empleados_planilla(%d)",
        idPlanilla
    );

    int cant_empleados = 0;
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return cant_empleados;
    }
    
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    if (row == NULL) {
        return cant_empleados;
    }
    
    cant_empleados = atoi(row[0]);
    
    mysql_free_result(res);
    mysql_close(conn);

    return cant_empleados;
}

/*****Nombre************************************************************
* EliminarPlanilla
*****Descripción********************************************************
* Obtiene el reporte anual de ventas de un negocio en particular.
*****Retorno************************************************************
* Un ReporteAnual de las ventas del negocio.
*****Entradas***********************************************************
* - char* cedula_comercio: Cedula del comercio.
* - int anio: Año de las facturas.
************************************************************************/
bool EliminarPlanilla(int mes, int anio) {
    MYSQL *conn = Conectar();
    char query[256];
    sprintf(
        query, "CALL eliminar_planilla_por_fecha(%d, %d)",
        mes, anio
    );

    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return false;
    }
    
    mysql_close(conn);
    return true;
}

/*****Nombre************************************************************
* ObtenerCantEmpleadosPorFecha
*****Descripción********************************************************
* Obtiene la cantidad de empleados que están registrados en una
* planilla por medio de una fecha indicada.
*****Retorno************************************************************
* Un número (int) con la cantidad de empleados registrados en dicha
* planilla.
*****Entradas***********************************************************
* - (Fecha fecha) La fecha de la planilla que se desea consultar.
************************************************************************/
int ObtenerCantEmpleadosPorFecha(Fecha fecha) {
    MYSQL *conn = Conectar();
    char query[256];
    char fecha_str[25];
    sprintf(
        fecha_str, "%d-%d-%d", 
        fecha.anio, 
        fecha.mes, 
        fecha.dia
    );

    sprintf(
        query, "CALL get_cant_empleados_por_fecha('%s')",
        fecha_str
    );

    int cant_empleados = 0;
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return cant_empleados;
    }
    
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    if (row == NULL) {
        return cant_empleados;
    }
    
    cant_empleados = atoi(row[0]);
    
    mysql_free_result(res);
    mysql_close(conn);

    return cant_empleados;
}

/*****Nombre************************************************************
* ObtenerEmpleadoDePlanilla
*****Descripción********************************************************
* Obtiene una lista (puntero) con todos los (EmpleadoConRol - empleado_dao.h)
* que están asignados a una planilla en específico buscando la planilla
* por una fecha en específico.
*****Retorno************************************************************
* La lista de (EmpleadoConRol) que están asignados a la planilla.
*****Entradas***********************************************************
* - (Fecha fecha) La fecha de la planilla que se desea buscar.
************************************************************************/
EmpleadoConRol* ObtenerEmpleadosDePlanilla(Fecha fecha) {
    MYSQL *conn = Conectar();
    char query[256];
    char fecha_str[25];
    sprintf(
        fecha_str, "%d-%d-%d", 
        fecha.anio, 
        fecha.mes, 
        fecha.dia
    );

    sprintf(
        query, "CALL get_planXemp_por_fecha('%s')",
        fecha_str
    );

    EmpleadoConRol* empleados = NULL;

    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return empleados;
    }

    res = mysql_store_result(conn);

    if (res) {
        int cant_empleados = mysql_num_rows(res);
        
        empleados = malloc(cant_empleados * sizeof *empleados);
        int i = 0;
        while ((row = mysql_fetch_row(res))) {
            strcpy(empleados[i].cedula, row[0]);
            strcpy(empleados[i].nombre_completo, row[1]);
            empleados[i].salario_mensual = atof(row[2]);
            strcpy(empleados[i].nombre_rol, row[3]);
            i++;
        }
    }

    mysql_free_result(res);
    mysql_close(conn);

    return empleados;
}

#endif // PLANILLA_DAO_H

/*****Datos administrativos*******************************
* Nombre del archivo: producto_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para el acceso a los datos de los productos
* del comercio en la base de datos.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef PRODUCTO_DAO_H
#define PRODUCTO_DAO_H

#include "./conexion_mysql.h"

/**
 * @struct producto
 * @brief Productos que vende el comercio.
 */
struct producto {
    char id[51]; // Id del producto en la tabla.
    char nombre[51]; // Nombre del producto.
    double costo; // Costo del producto (sin impuesto)
    double impuesto; // Impuesto agregado (porcentual)
};

typedef struct producto Producto;

static MYSQL_RES *res;
static MYSQL_ROW row;

/*****Nombre************************************************************
* ObtenerCantidadProductos
*****Descripción********************************************************
* Obtiene el total de productos que están registrados en la base
* de datos.
*****Retorno************************************************************
* Un número (int) con la cantidad total de productos registrados.
*****Entradas***********************************************************
* No tiene entradas.
************************************************************************/
int ObtenerCantidadProductos() {
    MYSQL *conn = Conectar();
    int cantidad = -1;
    char query[100];
    sprintf(query, "CALL get_productos()");

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return cantidad;
    }

    res = mysql_store_result(conn);
    cantidad = mysql_num_rows(res);

    mysql_free_result(res);
    mysql_close(conn);

    return cantidad;
}

/*****Nombre************************************************************
* ObtenerProducto
*****Descripción********************************************************
* Obtiene la información de un (Producto) en específico desde la
* base de datos.
*****Retorno************************************************************
* Un (Producto) con toda su información, en caso de que no exista el
* producto simplemente su identificador será "ND".
*****Entradas***********************************************************
* - (char *idProducto) Identificador alfanumérico del producto.
************************************************************************/
Producto ObtenerProducto(char *idProducto) {
    MYSQL *conn = Conectar();
    Producto producto;
    strcpy(producto.id, "ND");
    
    char query[100];
    sprintf(query, "CALL get_producto('%s')", idProducto);
    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return producto;
    }
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res))) {
        strcpy(producto.id, row[0]);
        strcpy(producto.nombre, row[1]);
        producto.costo = atof(row[2]);
        producto.impuesto = atof(row[3]);
    }

    mysql_free_result(res);
    mysql_close(conn);

    return producto;
}

/*****Nombre************************************************************
* RegistrarProducto
*****Descripción********************************************************
* Registra un nuevo producto en la base de datos.
*
* No se requiere utilizar está función, pensando más en futura extensión
* de este programa.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso (false)
* del registro del producto.
*****Entradas***********************************************************
* - (Producto producto) El producto que se desea registrar.
************************************************************************/
bool RegistrarProducto(Producto producto) {
    MYSQL *conn = Conectar();
    char query[100];
    sprintf(
        query, 
        "CALL registrar_producto('%s', '%s', %f, %f)", 
        producto.id, producto.nombre, 
        producto.costo, producto.impuesto
    );

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return false;
    }
    mysql_close(conn);
    return true;
}

/*****Nombre************************************************************
* EliminarProductos
*****Descripción********************************************************
* Elimina todos los productos registrados en la base de datos.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso (false)
* de la eliminación de los productos.
*****Entradas***********************************************************
* - Sin entradas.
************************************************************************/
bool EliminarProductos() {
    MYSQL *conn = Conectar();
    char query[100];
    sprintf(query, "CALL remove_productos()");

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return false;
    }

    mysql_close(conn);
    return true;
}

/*****Nombre************************************************************
* ExisteProducto
*****Descripción********************************************************
* Consulta en la base de datos si existe un producto en específico o no.
*****Retorno************************************************************
* Un valor booleano (bool) que indica si existe el producto (true) o
* si no existe (false).
*****Entradas***********************************************************
* - (char *idProducto) El identificador del producto que se está
*                      buscando.
************************************************************************/
bool ExisteProducto(char *idProducto) {
    Producto prod = ObtenerProducto(idProducto);
    if (strcmp(prod.id, "ND") == 0) {
        return false;
    }
    return true;
}

/*****Nombre************************************************************
* ObtenerProductos
*****Descripción********************************************************
* Obtiene todos los productos registrados en la base de datos.
*****Retorno************************************************************
* Una lista (puntero) de todos los productos registrados en base de datos.
*****Entradas***********************************************************
* No tiene entradas.
************************************************************************/
Producto* ObtenerProductos() {
    int cantidad = ObtenerCantidadProductos();
    if (cantidad == -1) { return NULL; }

    char query[100];
    sprintf(query, "CALL get_productos()");

    MYSQL *conn = Conectar();
    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return NULL;
    }

    res = mysql_store_result(conn);
    Producto* productos = malloc(cantidad * sizeof *productos);

    int i = 0;
    while ((row = mysql_fetch_row(res))) {
        strcpy(productos[i].id, row[0]);
        strcpy(productos[i].nombre, row[1]);
        productos[i].costo = atof(row[2]);
        productos[i].impuesto = atof(row[3]);
        i++;
    }

    mysql_free_result(res);
    mysql_close(conn);

    return productos;
}

#endif // PRODUCTO_DAO_H

/*****Datos administrativos*******************************
* Nombre del archivo: rol_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para el acceso a los datos de los roles/puestos
* de los empleados del comercio.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef ROL_DAO_H
#define ROL_DAO_H

#include "./conexion_mysql.h"

/**
 * @struct rol
 * @brief Rol o puesto de los empleados en la base de datos.
 */
struct rol {
    int id; // Id del rol en la tabla.
    char nombre[51]; // Nombre del rol.
    char descripcion[256]; // Breve descripción del rol.
};

typedef struct rol Rol;

static MYSQL_RES *res;
static MYSQL_ROW row;

/*****Nombre************************************************************
* ObtenerRoles
*****Descripción********************************************************
* Obtiene todos los roles que estén registrados en la base de datos.
*****Retorno************************************************************
* Una lista (puntero) de los roles en base de datos.
*****Entradas***********************************************************
* No tiene entradas.
************************************************************************/
Rol* ObtenerRoles() {

    MYSQL *conn = Conectar();

    int cantidad_roles = 0;
    char query[100];
    sprintf(query, "CALL get_roles()");

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return NULL;
    }

    res = mysql_store_result(conn);
    cantidad_roles = mysql_num_rows(res);
    Rol* roles = malloc(cantidad_roles * sizeof *roles);

    int i = 0;
    while ((row = mysql_fetch_row(res))) {
        roles[i].id = atoi(row[0]);
        strcpy(roles[i].nombre, row[1]);
        strcpy(roles[i].descripcion, row[2]);
        i++;
    }

    mysql_free_result(res);
    mysql_close(conn);

    return roles;
}

/*****Nombre************************************************************
* ObtenerRol
*****Descripción********************************************************
* Obtiene la información registrada en base de datos de un rol en
* específico.
*****Retorno************************************************************
* Un (Rol) con toda su información.
*****Entradas***********************************************************
* - (int idRol) El id del rol en la tabla.
************************************************************************/
Rol ObtenerRol(int idRol) {
    MYSQL *conn = Conectar();
    Rol rol;
    strcpy(rol.nombre, "ND");
    strcpy(rol.descripcion, "ND");
    char query[100];
    sprintf(query, "CALL get_rol('%d')", idRol);

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return rol;
    }

    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res))) {
        rol.id = atoi(row[0]);
        strcpy(rol.nombre, row[0]);
        strcpy(rol.descripcion, row[1]);
    }
    mysql_free_result(res);
    mysql_close(conn);
    return rol;
}

/*****Nombre************************************************************
* ObtenerCantidadRoles
*****Descripción********************************************************
* Obtiene la cantidad de roles que están registrados en la base de
* datos.
*****Retorno************************************************************
* Un número (int) con la cantidad total de roles registrados.
*****Entradas***********************************************************
* No tiene entradas.
************************************************************************/
int ObtenerCantidadRoles() {
    MYSQL *conn = Conectar();

    int cantidad_roles = -1;
    char query[100];
    sprintf(query, "CALL get_roles()");

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return cantidad_roles;
    }

    res = mysql_store_result(conn);
    cantidad_roles = mysql_num_rows(res);

    mysql_free_result(res);
    mysql_close(conn);

    return cantidad_roles;
}

#endif // ROL_DAO_H