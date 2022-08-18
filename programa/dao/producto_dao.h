/*****Datos administrativos*******************************
* Nombre del archivo: producto_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para el acceso a los datos de los productos
* del comercio en la base de datos.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef PRODUCTO_DAO_H
#define PRODUCTO_DAO_H

#include "./conexion_mysql.h"

/**
 * @struct producto
 * @brief Productos que vende el comercio.
 */
struct producto {
    char id[51]; // Id del producto en la tabla.
    char nombre[51]; // Nombre del producto.
    double costo; // Costo del producto (sin impuesto)
    double impuesto; // Impuesto agregado (porcentual)
};

typedef struct producto Producto;

static MYSQL_RES *res;
static MYSQL_ROW row;

/*****Nombre************************************************************
* ObtenerCantidadProductos
*****Descripción********************************************************
* Obtiene el total de productos que están registrados en la base
* de datos.
*****Retorno************************************************************
* Un número (int) con la cantidad total de productos registrados.
*****Entradas***********************************************************
* No tiene entradas.
************************************************************************/
int ObtenerCantidadProductos() {
    MYSQL *conn = Conectar();
    int cantidad = -1;
    char query[100];
    sprintf(query, "CALL get_productos()");

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return cantidad;
    }

    res = mysql_store_result(conn);
    cantidad = mysql_num_rows(res);

    mysql_free_result(res);
    mysql_close(conn);

    return cantidad;
}

/*****Nombre************************************************************
* ObtenerProducto
*****Descripción********************************************************
* Obtiene la información de un (Producto) en específico desde la
* base de datos.
*****Retorno************************************************************
* Un (Producto) con toda su información, en caso de que no exista el
* producto simplemente su identificador será "ND".
*****Entradas***********************************************************
* - (char *idProducto) Identificador alfanumérico del producto.
************************************************************************/
Producto ObtenerProducto(char *idProducto) {
    MYSQL *conn = Conectar();
    Producto producto;
    strcpy(producto.id, "ND");
    
    char query[100];
    sprintf(query, "CALL get_producto('%s')", idProducto);

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return producto;
    }

    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res))) {
        strcpy(producto.id, row[0]);
        strcpy(producto.nombre, row[1]);
        producto.costo = atof(row[2]);
        producto.impuesto = atof(row[3]);
    }

    mysql_free_result(res);
    mysql_close(conn);

    return producto;
}

/*****Nombre************************************************************
* RegistrarProducto
*****Descripción********************************************************
* Registra un nuevo producto en la base de datos.
*
* No se requiere utilizar está función, pensando más en futura extensión
* de este programa.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso (false)
* del registro del producto.
*****Entradas***********************************************************
* - (Producto producto) El producto que se desea registrar.
************************************************************************/
bool RegistrarProducto(Producto producto) {
    MYSQL *conn = Conectar();
    char query[100];
    sprintf(
        query, 
        "CALL registrar_producto('%s', '%s', %f, %f)", 
        producto.id, producto.nombre, 
        producto.costo, producto.impuesto
    );

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return false;
    }
    mysql_close(conn);
    return true;
}

/*****Nombre************************************************************
* EliminarProductos
*****Descripción********************************************************
* Elimina todos los productos registrados en la base de datos.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso (false)
* de la eliminación de los productos.
*****Entradas***********************************************************
* - Sin entradas.
************************************************************************/
bool EliminarProductos() {
    MYSQL *conn = Conectar();
    char query[100];
    sprintf(query, "CALL remove_productos()");

    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return false;
    }

    mysql_close(conn);
    return true;
}

/*****Nombre************************************************************
* ExisteProducto
*****Descripción********************************************************
* Consulta en la base de datos si existe un producto en específico o no.
*****Retorno************************************************************
* Un valor booleano (bool) que indica si existe el producto (true) o
* si no existe (false).
*****Entradas***********************************************************
* - (char *idProducto) El identificador del producto que se está
*                      buscando.
************************************************************************/
bool ExisteProducto(char *idProducto) {
    Producto prod = ObtenerProducto(idProducto);
    if (strcmp(prod.id, "ND") == 0) {
        return false;
    }
    return true;
}

/*****Nombre************************************************************
* ObtenerProductos
*****Descripción********************************************************
* Obtiene todos los productos registrados en la base de datos.
*****Retorno************************************************************
* Una lista (puntero) de todos los productos registrados en base de datos.
*****Entradas***********************************************************
* No tiene entradas.
************************************************************************/
Producto* ObtenerProductos() {
    int cantidad = ObtenerCantidadProductos();
    if (cantidad == -1) { return NULL; }

    char query[100];
    sprintf(query, "CALL get_productos()");

    MYSQL *conn = Conectar();
    if (mysql_query(conn, query)) {
        printf("[Error] %s\n", mysql_error(conn));
        return NULL;
    }

    res = mysql_store_result(conn);
    Producto* productos = malloc(cantidad * sizeof *productos);

    int i = 0;
    while ((row = mysql_fetch_row(res))) {
        strcpy(productos[i].id, row[0]);
        strcpy(productos[i].nombre, row[1]);
        productos[i].costo = atof(row[2]);
        productos[i].impuesto = atof(row[3]);
        i++;
    }

    mysql_free_result(res);
    mysql_close(conn);

    return productos;
}

#endif // PRODUCTO_DAO_H