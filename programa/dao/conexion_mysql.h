/*****Datos administrativos*******************************
* Nombre del archivo: conexion_mysql
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para la administración para la conexión a la
* base de datos local.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef CONEXION_MYSQL_H
#define CONEXION_MYSQL_H

#include <stdio.h>
#include <mysql/mysql.h>

static const char* SERVER = "localhost";
static const char* USUARIO = "root";
static const char* CLAVE = "root";
static const char* BASEDEDATOS = "pdvAgricola";

/*****Nombre************************************************************
* Conectar
*****Descripción********************************************************
* Realiza y retorna la conexión a la base de datos local.
*****Retorno************************************************************
* La conexión (MYSQL*) de la base datos.
*****Entradas***********************************************************
* No tiene entradas.
************************************************************************/
MYSQL* Conectar() {
    MYSQL* conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, SERVER, USUARIO, CLAVE, BASEDEDATOS, 0, NULL, 0)) {
        fprintf(stderr, "[Error] %s\n", mysql_error(conn));
        exit(1);
    }
    return conn;
}

#endif // CONEXION_MYSQL_H