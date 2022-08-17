#ifndef FACTURA_DAO_H
#define FACTURA_DAO_H

#include "./producto_dao.h"

struct detalleFactura {
    Producto producto;
    int cantidad;
};

typedef struct detalleFactura DetalleFactura;

struct factura {
    char nombre_comercio[51]; // 
    char cedula_comercio[51]; // Default (Solo se usa una) 001
    char telefono_comercio[51];
    int id_factura;
    char nombre_cliente[51];
    Fecha fecha_facturacion;
    char nombre_area[51];
    int id_area;
    double subtotal;
    double impuesto;
    double total;
    DetalleFactura* detalles;
};

typedef struct factura Factura;

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