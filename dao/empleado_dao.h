#ifndef EMPLEADO_DAO_H
#define EMPLEADO_DAO_H

#include "./conexion_mysql.h"

struct empleado {
    char cedula[51];
    char nombre_completo[256];
    int rol;
    double salario_mensual;
};

typedef struct empleado Empleado;

struct empleado_con_rol {
    char cedula[51];
    char nombre_completo[256];
    char nombre_rol[51];
    double salario_mensual;
};

typedef struct empleado_con_rol EmpleadoConRol;

static MYSQL_RES *res;
static MYSQL_ROW row;

int ObtenerCantidadEmpleados() {
    MYSQL *conn = Conectar();
    int cantidad = 0;
    char query[256];
    sprintf(query, "SELECT COUNT(*) FROM Empleados");
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return -1;
    }

    res = mysql_store_result(conn);
    if (res) {
        row = mysql_fetch_row(res);
        cantidad = atoi(row[0]);
    }

    mysql_free_result(res);
    mysql_close(conn);

    return cantidad;
}

EmpleadoConRol* ObtenerEmpleadosConRol() {
    MYSQL *conn = Conectar();
    EmpleadoConRol *empleados = NULL;
    char query[256];
    sprintf(query, "CALL get_empleados_con_rol()");
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return NULL;
    }
    
    res = mysql_store_result(conn);
    if (res) {
        int cantidad = mysql_num_rows(res);
        empleados = malloc(cantidad * sizeof *empleados);
        int i = 0;
        while ((row = mysql_fetch_row(res))) {
            strcpy(empleados[i].cedula, row[0]);
            strcpy(empleados[i].nombre_completo, row[1]);
            strcpy(empleados[i].nombre_rol, row[2]);
            empleados[i].salario_mensual = atof(row[3]);
            i++;
        }
    }
    
    mysql_free_result(res);
    mysql_close(conn);

    return empleados;
}

Empleado* ObtenerEmpleados() {
    MYSQL *conn = Conectar();
    char query[256];
    sprintf(query, "CALL get_empleados()");
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return NULL;
    }

    res = mysql_store_result(conn);
    int cantidad = mysql_num_rows(res);
    Empleado* empleados = (Empleado*)malloc(sizeof(Empleado) * cantidad);
    int i = 0;
    while ((row = mysql_fetch_row(res))) {
        strcpy(empleados[i].cedula, row[0]);
        strcpy(empleados[i].nombre_completo, row[1]);
        empleados[i].rol = atoi(row[2]);
        empleados[i].salario_mensual = atof(row[3]);
        i++;
    }
    mysql_free_result(res);
    mysql_close(conn);

    return empleados;
}

EmpleadoConRol ObtenerEmpleadoConRol(char *cedula) {
    MYSQL *conn = Conectar();
    EmpleadoConRol empleado;
    empleado.cedula = "ND";
    char query[256];
    sprintf(query, "CALL get_empleado_con_rol('%s')", cedula);
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return empleado;
    }
    
    res = mysql_store_result(conn);
    if (res) {
        row = mysql_fetch_row(res);
        strcpy(empleado.cedula, row[0]);
        strcpy(empleado.nombre_completo, row[1]);
        strcpy(empleado.nombre_rol, row[2]);
        empleado.salario_mensual = atof(row[3]);
    }
    
    mysql_free_result(res);
    mysql_close(conn);

    return empleado;
}

Empleado ObtenerEmpleado(char *cedula) {
    MYSQL *conn = Conectar();
    Empleado empleado;
    empleado.cedula = "ND";
    char query[256];
    sprintf(query, "CALL get_empleado('%s')", cedula);
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return empleado;
    }
    
    res = mysql_store_result(conn);
    if (res) {
        row = mysql_fetch_row(res);
        strcpy(empleado.cedula, row[0]);
        strcpy(empleado.nombre_completo, row[1]);
        empleado.rol = atoi(row[2]);
        empleado.salario_mensual = atof(row[3]);
    }
    
    mysql_free_result(res);
    mysql_close(conn);

    return empleado;
}

bool ExisteEmpleado(char *cedula) {
    Empleado empleado = ObtenerEmpleado(cedula);
    return strcmp(empleado.cedula, "ND") != 0;
}

#endif // EMPLEADO_DAO_H