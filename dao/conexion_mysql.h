#ifndef CONEXION_MYSQL_H
#define CONEXION_MYSQL_H

#include <stdio.h>
#include <mysql/mysql.h>

static const char* SERVER = "localhost";
static const char* USUARIO = "root";
static const char* CLAVE = "root";
static const char* BASEDEDATOS = "pdvAgricola";

/*
int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    char *server = "localhost";
    char *user = "root";
    char *password = "root";
    char *database = "pdvAgricola";


    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }


    if (mysql_query(conn, "CALL get_x")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("%s | %s\n", row[0], row[1]);
    }
    
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}*/

MYSQL* Conectar() {
    MYSQL* conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, SERVER, USUARIO, CLAVE, BASEDEDATOS, 0, NULL, 0)) {
        fprintf(stderr, "[Error] %s\n", mysql_error(conn));
        exit(1);
    }
    return conn;
}

#endif // CONEXION_MYSQL_H