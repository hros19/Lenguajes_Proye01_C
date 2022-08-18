/*****Datos administrativos*******************************
* Nombre del archivo: planilla_dao
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: Alex Sánchez Céspedes - Hansol Antay
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para el acceso a los datos de las planillas
* del comercio en la base de datos.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

#ifndef PLANILLA_DAO_H
#define PLANILLA_DAO_H

#include "./empleado_dao.h"

/**
 * @struct fecha
 * @brief Una simple estructura de fecha.
 */
struct fecha {
    int anio;
    int mes;
    int dia;
};

typedef struct fecha Fecha;

/**
 * @struct planilla
 * @brief Una planilla registrada del comercio
 */
struct planilla {
    int id; // Id de la planilla en la tabla.
    Fecha fecha; // Fecha correspondiente a la planilla.
    double monto_carga_social; // El monto de la carga social para la planilla.
    EmpleadoConRol* empleadosConRol; // Lista de empleados de la planilla.
};

typedef struct planilla Planilla;

static MYSQL_RES *res;
static MYSQL_ROW row;

/*****Nombre************************************************************
* RegistrarPlanilla
*****Descripción********************************************************
* Registra una nueva planilla en la base de datos.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso (false)
* del registro.
*****Entradas***********************************************************
* - (Planilla planilla) La nueva planilla que se desea registrar.
************************************************************************/
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

/*****Nombre************************************************************
* RegistrarEmpleadoAPlanilla
*****Descripción********************************************************
* Registra un empleado a una planilla existente, utilizando la fecha
* que se indicó en la planilla.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso (false)
* del registro.
*****Entradas***********************************************************
* - (Fecha fecha) La fecha de la planilla de la cuál se desea registrar
*                 al empleado.
* - (char *cedula_empleado) La cédula del empleado que se desea
*                           registrar.
************************************************************************/
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

/*****Nombre************************************************************
* EliminarEmpleadoDePlanilla
*****Descripción********************************************************
* Elimina un empleado en específico de una planilla existente.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso (false)
* del borrado.
*****Entradas***********************************************************
* - (Fecha fecha) La fecha de la planilla que se desea modificar.
* - (char *cedula_empleado) La cédula del empleado que se desea remover.
************************************************************************/
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

/*****Nombre************************************************************
* ObtenerPlanillaPorFecha
*****Descripción********************************************************
* Obtiene la estructura (Planilla) por medio de una fecha indicada.
*****Retorno************************************************************
* Una estructura (Planilla) con toda la información obtenida desde la
* base de datos.
*****Entradas***********************************************************
* - (Fecha fecha) La fecha de la planilla que se desea buscar.
************************************************************************/
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

/*****Nombre************************************************************
* ObtenerCantEmpleadosPorPlanilla
*****Descripción********************************************************
* Obtiene la cantidad de empleados que están asignados a una planilla
* en específico.
*****Retorno************************************************************
* Un número (int) con la cantidad de empleados de la planilla.
*****Entradas***********************************************************
* - (int idPlanilla) El identificador de la planilla en la tabla.
************************************************************************/
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

/*****Nombre************************************************************
* ObtenerCantEmpleadosPorFecha
*****Descripción********************************************************
* Obtiene la cantidad de empleados que están registrados en una
* planilla por medio de una fecha indicada.
*****Retorno************************************************************
* Un número (int) con la cantidad de empleados registrados en dicha
* planilla.
*****Entradas***********************************************************
* - (Fecha fecha) La fecha de la planilla que se desea consultar.
************************************************************************/
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

/*****Nombre************************************************************
* ObtenerEmpleadoDePlanilla
*****Descripción********************************************************
* Obtiene una lista (puntero) con todos los (EmpleadoConRol - empleado_dao.h)
* que están asignados a una planilla en específico buscando la planilla
* por una fecha en específico.
*****Retorno************************************************************
* La lista de (EmpleadoConRol) que están asignados a la planilla.
*****Entradas***********************************************************
* - (Fecha fecha) La fecha de la planilla que se desea buscar.
************************************************************************/
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