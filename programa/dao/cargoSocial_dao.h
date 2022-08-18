/*****Datos administrativos*******************************
* Nombre del archivo: cargoSocial_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para el acceso a los datos del cargo social que
* que corresponderá a las cargas de los sueldos de los
* empleados de la empresa.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef CARGOSOCIAL_DAO_H
#define CARGOSOCIAL_DAO_H

#include "./conexion_mysql.h"

/**
 * @struct cargo_social
 * @brief Porcentaje de cargas sociales de los sueldos de los empleados.
 */
struct cargo_social {
    int id; // Id en la tabla.
    char nombre[51]; // Nombre de la carga social.
    double porcentaje; // Porcentaje del cargo social.
};

typedef struct cargo_social CargoSocial;

static MYSQL_RES *res;
static MYSQL_ROW row;

/*****Nombre************************************************************
* ObtenerCargoSocial
*****Descripción********************************************************
* Obtiene el cargo social buscando por el nombre, el programa por default
* utilizará solamente una carga social ´General´, pero se realizó de
* forma que se pueda extender más adelante.
*****Retorno************************************************************
* Un objeto/struct CargaSocial con toda la información de dicha
* carga social para calcular el sueldo de los empleados.
*****Entradas***********************************************************
* - (char *nombre) Nombre de la carga social a consultar.
************************************************************************/
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

/*****Nombre************************************************************
* ModificarCargoSocial
*****Descripción********************************************************
* Busca un cargo social por el nombre y modifica el valor porcentual
* que corresponderá a dicha carga social.
*****Retorno************************************************************
* Un valor booleano que indicará el éxito o error de dicha modificación.
*****Entradas***********************************************************
* - (char *nombre) Nombre de la carga social a modificar.
* - (double porcentaje) El nuevo porcentaje de la carga social.
************************************************************************/
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