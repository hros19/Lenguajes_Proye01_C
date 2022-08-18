#ifndef OPERADOR_DAO_H
#define OPERADOR_DAO_H

#include "./conexion_mysql.h"

struct operador {
    int id;
    char usuario[51];
    char clave[256];
};

typedef struct operador Operador;

static MYSQL_RES *res;
static MYSQL_ROW row;

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
        fprintf(stderr, "\n[Error] %s\n", mysql_error(conn));
        mysql_close(conn);
        return false;
    }
    mysql_close(conn);
    return true;
}

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
        fprintf(stderr, "[Error] %s\n", mysql_error(conn));
        exit(1);
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

/*int main() {

    MYSQL *conn = conectar();

    if (mysql_query(conn, "CALL get_x")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("%s | %s\n", row[0], row[1]);
    }
}*/

#endif // OPERADOR_DAO_H