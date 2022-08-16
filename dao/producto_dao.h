#ifndef PRODUCTO_DAO_H
#define PRODUCTO_DAO_H

#include "./conexion_mysql.h"

struct producto {
    char id[51];
    char nombre[51];
    double costo;
    double impuesto;
};

typedef struct producto Producto;

static MYSQL_RES *res;
static MYSQL_ROW row;

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

bool ExisteProducto(char *idProducto) {
    Producto prod = ObtenerProducto(idProducto);
    if (strcmp(prod.id, "ND") == 0) {
        return false;
    }
    return true;
}

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