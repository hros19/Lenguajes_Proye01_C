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