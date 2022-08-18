/*****Datos administrativos*******************************
* Nombre del archivo: comercio_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para el acceso a los datos del comercio.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef COMERCIO_DAO_H
#define COMERCIO_DAO_H

#include "./conexion_mysql.h"

/**
 * @struct comercio
 * @brief Características o información básica del comercio.
 */
struct comercio {
    char cedulaJuridica[51];
    char nombre[51];
    char telefono[51];
    int numSigFactura;
};

typedef struct comercio Comercio;

static MYSQL_RES *res;
static MYSQL_ROW row;

/*****Nombre************************************************************
* RegistrarComercio
*****Descripción********************************************************
* Permite registrar un nuevo comercio en el sistema, originalmente
* la aplicación no requiere registrar comercios pero se da para
* la posibilidad de extender el programa más adelante.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito o fallo del registro del
* comercio en la base de datos.
*****Entradas***********************************************************
* - (Comercio comercio) el objeto/estructura comercio que se desea
*                       registrar en la base de datos.
************************************************************************/
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

/*****Nombre************************************************************
* ObtenerInfoComercio
*****Descripción********************************************************
* Busca el comercio utilizando la cédula jurídica del mismo y retorna 
* toda la información del mismo en un objeto/estructura Comercio.
*
* Nota: recordar que la aplicación está pensada para utilizar un solo
*       comercio para las facturas.
*****Retorno************************************************************
* Un (Comercio) con toda la información que se almacenó en la base de
* datos.
*****Entradas***********************************************************
* - (char *cedulaJuridica) La cédula del comercio que estamos buscando.
************************************************************************/
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