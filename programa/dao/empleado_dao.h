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