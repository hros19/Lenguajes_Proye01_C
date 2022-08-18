/*****Datos administrativos*******************************
* Nombre del archivo: factura_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para el acceso a los datos de las facturas
* del comercio en la base de datos.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef FACTURA_DAO_H
#define FACTURA_DAO_H

#include "./producto_dao.h"

/**
 * @struct detalleFactura
 * @brief Corresponde a los detalles de la factura.
 */
struct detalleFactura {
    Producto producto;
    int cantidad;
};

typedef struct detalleFactura DetalleFactura;

/**
 * @struct factura
 * @brief Abstracción que corresponde a las facturas
 *        almacenadas en base de datos.
 */
struct factura {
    char nombre_comercio[51]; // Nombre del comercio.
    char cedula_comercio[51]; // Default (Solo se usa una) 001.
    char telefono_comercio[51]; // Teléfono del comercio.
    int id_factura; // Id de la factura en la tabla.
    char nombre_cliente[51]; // Nombre del cliente en la factura.
    Fecha fecha_facturacion; // Fecha de la facturación.
    char nombre_area[51]; // Nombre del área del cual.
    int id_area; // Id del área.
    double subtotal; // Monto del subtotal.
    double impuesto; // Monto del impuesto (Porcentual).
    double total; // Monto total (subtotal + impuesto).
    DetalleFactura* detalles; // Lista con los detalles de la factura.
};

typedef struct factura Factura;

/*****Nombre************************************************************
* RegistrarFactura
*****Descripción********************************************************
* Registra una factura en la base de datos.
*****Retorno************************************************************
* Un número (int) que corresponderá a la id en la tabla de la nueva
* factura que se ha registrado. En caso de existir algún error en el
* registro, se retornará un id (-1) indicando que no se registró la
* factura.
*****Entradas***********************************************************
* - (Factura factura) La factura como tal que se desea registrar.
************************************************************************/
int RegistrarFactura(Factura factura) {
    MYSQL *conn = Conectar();

    Fecha fechaFactura = factura.fecha_facturacion;
    char fecha_str[25];
    sprintf(fecha_str, "%d-%d-%d", fechaFactura.anio, fechaFactura.mes, fechaFactura.dia);
    char query[1000];
    sprintf(
        query,
        "CALL create_factura('%s', '%s', '%d', '%s', '%f', '%f', '%f')",
        factura.nombre_cliente, fecha_str, factura.id_area, factura.cedula_comercio, factura.subtotal, factura.impuesto, factura.total
    );

    int result = mysql_query(conn, query);
    if (result != 0) {
        printf("Error al registrar la factura: %s\n", mysql_error(conn));
        return -1;
    }
    
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    int id_factura = atoi(row[0]);

    mysql_free_result(res);
    mysql_close(conn);
    
    return id_factura;
}

/*****Nombre************************************************************
* ObtenerCantidadDeDetalles
*****Descripción********************************************************
* Obtiene la cantidad de detalles que tiene una factura, utilizando
* el id de la factura.
*****Retorno************************************************************
* Un número (int) que corresponde a la cantidad de detalles que tiene
* la factura.
*****Entradas***********************************************************
* - (ind id_factura) El id de la factura la cual se desea consultar.
************************************************************************/
int ObtenerCantidadDeDetalles(int id_factura) {
    MYSQL *conn = Conectar();
    int cantidad = -1;
    char query[100];
    sprintf(query, "CALL get_cantidad_detalles_factura(%d)", id_factura);

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return cantidad;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    cantidad = atoi(row[0]);

    mysql_free_result(res);
    mysql_close(conn);

    return cantidad;
}

/*****Nombre************************************************************
* ObtenerFactura
*****Descripción********************************************************
* Obtiene toda la información de una (Factura) que se encuentre
* que se encuentre registrada en la base de datos. Buscando la factura
* por su id en la tabla.
*****Retorno************************************************************
* Un objeto (Factura) con toda su información obtenida de la base
* de datos.
*****Entradas***********************************************************
* - (int id_factura) El id de la factura en la tabla.
************************************************************************/
Factura ObtenerFactura(int id_factura) {

    Factura factura;
    factura.id_factura = -1;

    MYSQL *conn = Conectar();
    char query[250];
    sprintf(query, "CALL get_factura_por_id(%d)", id_factura);

    int result = mysql_query(conn, query);
    if (result != 0) {
        printf("Error al obtener la factura: %s\n", mysql_error(conn));
        return factura;
    }
    
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (row == NULL) {
        printf("No se encontro la factura\n");
        return factura;
    }
    
    strcpy(factura.nombre_comercio, row[0]);
    strcpy(factura.cedula_comercio, row[1]);
    strcpy(factura.telefono_comercio, row[2]);
    strcpy(factura.nombre_cliente, row[3]);
    factura.id_factura = id_factura;

    char *fecha_str = row[4];
    Fecha fecha;
    sscanf(fecha_str, "%d-%d-%d", &fecha.dia, &fecha.mes, &fecha.anio);
    factura.fecha_facturacion = fecha;

    strcpy(factura.nombre_area, row[5]);
    factura.id_area = atoi(row[6]);
    factura.subtotal = atof(row[7]);
    factura.impuesto = atof(row[8]);
    factura.total = atof(row[9]);

    mysql_free_result(res);
    mysql_close(conn);

    return factura;
}

/*****Nombre************************************************************
* RegistrarDetalleFactura
*****Descripción********************************************************
* Registra un nuevo detalle a una factura existente en la base de
* datos.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso del 
* registro del detalle.
*****Entradas***********************************************************
* - (int id_factura) El id de la factura en la tabla.
* - (Producto producto) El producto que se desea registrar en el detalle.
* - (int cantidad) La cantidad de producto que se está registrando.
************************************************************************/
bool RegistrarDetalleFactura(int id_factura, Producto producto, int cantidad) {
    MYSQL *conn = Conectar();
    char query[250];
    sprintf(query, "CALL create_factXdetprod('%s', %d, %d)", producto.id, id_factura, cantidad);
    int result = mysql_query(conn, query);
    if (result != 0) {
        printf("Error al registrar el detalle de la factura: %s\n", mysql_error(conn));
        return false;
    }
    mysql_close(conn);
    return true;
}

/*****Nombre************************************************************
* ObtenerDetallesFactura
*****Descripción********************************************************
* Obtiene todos los detalles de una factura, buscandola por su id.
*****Retorno************************************************************
* Una lista (puntero) de todos los detalles (DetalleFactura) de una
* (Factura) que se encuentre registrada en la base de datos.
*****Entradas***********************************************************
* - (int id_factura) El id de la factura en la tabla.
************************************************************************/
DetalleFactura* ObtenerDetallesFactura(int id_factura) {
    MYSQL *conn = Conectar();
    char query[250];
    sprintf(query, "CALL get_factXdetprod(%d)", id_factura);
    int result = mysql_query(conn, query);
    if (result != 0) {
        printf("Error al obtener los detalles de la factura: %s\n", mysql_error(conn));
        return NULL;
    }
    
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    int cantidad = mysql_num_rows(res);
    DetalleFactura *detalles = malloc(cantidad * sizeof *detalles);
    for (int i = 0; i < cantidad; i++) {
        Producto p;
        strcpy(p.id, row[0]);
        strcpy(p.nombre, row[1]);
        p.costo = atof(row[2]);
        p.impuesto = atof(row[3]);
        detalles[i].producto = p;
        detalles[i].cantidad = atoi(row[1]);
        row = mysql_fetch_row(res);
    }
    mysql_free_result(res);
    mysql_close(conn);
    return detalles;
}

#endif // FACTURA_DAO_H