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