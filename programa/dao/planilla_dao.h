#ifndef PLANILLA_DAO_H
#define PLANILLA_DAO_H

#include "./empleado_dao.h"

struct fecha {
    int anio;
    int mes;
    int dia;
};

typedef struct fecha Fecha;

struct planilla {
    int id;
    Fecha fecha;
    double monto_carga_social;
    EmpleadoConRol* empleadosConRol;
};

typedef struct planilla Planilla;

static MYSQL_RES *res;
static MYSQL_ROW row;

bool RegistrarPlanilla(Planilla planilla) {
    MYSQL *conn = Conectar();
    char query[256];
    char fecha_str[25];
    sprintf(
        fecha_str, "%d-%d-%d", 
        planilla.fecha.anio, 
        planilla.fecha.mes, 
        planilla.fecha.dia
    );

    sprintf(
        query, "CALL registrar_planilla('%s')",
        fecha_str
    );
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        mysql_free_result(res);
        mysql_close(conn);
        return false;
    }
    
    mysql_free_result(res);
    mysql_close(conn);

    return true;
}

bool RegistrarEmpleadoAPlanilla(Fecha fecha, char *cedula_empleado) {
    MYSQL *conn = Conectar();
    char query[256];
    char fecha_str[25];
    sprintf(
        fecha_str, "%d-%d-%d", 
        fecha.anio, 
        fecha.mes, 
        fecha.dia
    );

    sprintf(
        query, "CALL create_planXemp_fecha('%s', '%s')",
        fecha_str, cedula_empleado
    );
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        mysql_close(conn);
        return false;
    }
    
    mysql_close(conn);

    return true;
}

bool EliminarEmpleadoDePlanilla(Fecha fecha, char *cedula_empleado) {
    MYSQL *conn = Conectar();
    char query[256];
    char fecha_str[25];
    sprintf(
        fecha_str, "%d-%d-%d", 
        fecha.anio, 
        fecha.mes, 
        fecha.dia
    );

    sprintf(
        query, "CALL remove_planXemp_fecha('%s', '%s')",
        fecha_str, cedula_empleado
    );
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        mysql_close(conn);
        return false;
    }
    
    mysql_close(conn);

    return true;
}

Planilla ObtenerPlanillaPorFecha(Fecha fecha) {
    MYSQL *conn = Conectar();
    char query[256];
    char fecha_str[25];
    sprintf(
        fecha_str, "%d-%d-%d", 
        fecha.anio, 
        fecha.mes, 
        fecha.dia
    );

    sprintf(
        query, "CALL get_planilla_por_fecha('%s')",
        fecha_str
    );

    Planilla planilla;
    planilla.id = -1;
    
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return planilla;
    }
    
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    if (row == NULL) {
        return planilla;
    }
    
    planilla.id = atoi(row[0]);
    char *fecha_plan_bd = row[1];
    sscanf(
        fecha_plan_bd, "%d-%d-%d", 
        &planilla.fecha.anio, 
        &planilla.fecha.mes, 
        &planilla.fecha.dia
    );
    planilla.monto_carga_social = atof(row[2]);
    
    mysql_free_result(res);
    mysql_close(conn);

    return planilla;
}

int ObtenerCantEmpleadosPorPlanilla(int idPlanilla) {
    MYSQL *conn = Conectar();
    char query[256];
    sprintf(
        query, "CALL get_cant_empleados_planilla(%d)",
        idPlanilla
    );

    int cant_empleados = 0;
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return cant_empleados;
    }
    
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    if (row == NULL) {
        return cant_empleados;
    }
    
    cant_empleados = atoi(row[0]);
    
    mysql_free_result(res);
    mysql_close(conn);

    return cant_empleados;
}

int ObtenerCantEmpleadosPorFecha(Fecha fecha) {
    MYSQL *conn = Conectar();
    char query[256];
    char fecha_str[25];
    sprintf(
        fecha_str, "%d-%d-%d", 
        fecha.anio, 
        fecha.mes, 
        fecha.dia
    );

    sprintf(
        query, "CALL get_cant_empleados_por_fecha('%s')",
        fecha_str
    );

    int cant_empleados = 0;
    if (mysql_query(conn, query)) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return cant_empleados;
    }
    
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    if (row == NULL) {
        return cant_empleados;
    }
    
    cant_empleados = atoi(row[0]);
    
    mysql_free_result(res);
    mysql_close(conn);

    return cant_empleados;
}

EmpleadoConRol* ObtenerEmpleadosDePlanilla(Fecha fecha) {
    MYSQL *conn = Conectar();
    char query[256];
    char fecha_str[25];
    sprintf(
        fecha_str, "%d-%d-%d", 
        fecha.anio, 
        fecha.mes, 
        fecha.dia
    );

    sprintf(
        query, "CALL get_planXemp_por_fecha('%s')",
        fecha_str
    );

    EmpleadoConRol* empleados = NULL;

    if (mysql_query(conn, "CALL get_planXemp_por_fecha('2019-12-01');")) {
        printf("Error al ejecutar el query: %s\n", mysql_error(conn));
        return empleados;
    }

    res = mysql_store_result(conn);

    if (res) {
        int cant_empleados = mysql_num_rows(res);
        
        empleados = malloc(cant_empleados * sizeof *empleados);
        int i = 0;
        while ((row = mysql_fetch_row(res))) {
            strcpy(empleados[i].cedula, row[0]);
            strcpy(empleados[i].nombre_completo, row[1]);
            empleados[i].salario_mensual = atof(row[2]);
            strcpy(empleados[i].nombre_rol, row[3]);
            i++;
        }
    }

    mysql_free_result(res);
    mysql_close(conn);

    return empleados;
}

#endif // PLANILLA_DAO_H