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