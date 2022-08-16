#ifndef CARGOSOCIAL_DAO_H
#define CARGOSOCIAL_DAO_H

#include "./conexion_mysql.h"

struct cargo_social {
    int id;
    char nombre[51];
    double porcentaje;
};

typedef struct cargo_social CargoSocial;

static MYSQL_RES *res;
static MYSQL_ROW row;

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