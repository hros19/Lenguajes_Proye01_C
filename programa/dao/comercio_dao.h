#ifndef COMERCIO_DAO_H
#define COMERCIO_DAO_H

#include "./conexion_mysql.h"

struct comercio {
    char cedulaJuridica[51];
    char nombre[51];
    char telefono[51];
    int numSigFactura;
};

typedef struct comercio Comercio;

static MYSQL_RES *res;
static MYSQL_ROW row;

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