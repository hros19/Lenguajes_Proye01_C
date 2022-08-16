#ifndef AREA_DAO_H
#define AREA_DAO_H

#include "./conexion_mysql.h"

struct area {
    int id;
    char nombre[51];
    double dimensiones;
    char producto_principal[51];
};

typedef struct area Area;

static MYSQL_RES *res;
static MYSQL_ROW row;

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