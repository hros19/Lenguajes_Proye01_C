#ifndef ROL_DAO_H
#define ROL_DAO_H

#include "./conexion_mysql.h"

struct rol {
    int id;
    char nombre[51];
    char descripcion[256];
};

typedef struct rol Rol;

static MYSQL_RES *res;
static MYSQL_ROW row;

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