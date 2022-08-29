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

struct planillaCantEmpleados {
    int id; // Id de la planilla en la tabla.
    Fecha fecha; // Fecha correspondiente a la planilla.
    double monto_carga_social; // El monto de la carga social para la planilla.
    int cantidad_empleados; // La cantidad de empleados de la planilla.
    EmpleadoConRol* empleadosConRol; // Lista de empleados de la planilla.
};

typedef struct planillaCantEmpleados PlanillaConCantEmpleados;

static MYSQL_RES *res;
static MYSQL_ROW row;

/*****Nombre************************************************************
* ObtenerAniosConPlanillas
*****Descripción********************************************************
* Obtiene los años con planillas registradas en la base de datos.
*****Retorno************************************************************
* int* - Un arreglo de enteros con los años con planillas registradas.
*****Entradas***********************************************************
* No tiene entradas
************************************************************************/
int* ObtenerAniosConPlanillas() {
    MYSQL *conn = Conectar();
    int* anios = NULL;
    int cantidad = 0;
    char query[200];
    sprintf(query, "CALL get_anios_con_planillas()");
    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return anios;
    }
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res))) {
        anios = realloc(anios, sizeof(int) * (cantidad + 1));
        anios[cantidad] = atoi(row[0]);
        cantidad++;
    }
    mysql_close(conn);
    return anios;
}

/*****Nombre************************************************************
* ObtenerCantidadAniosConPlanillas
*****Descripción********************************************************
* Obtiene la cantidad de años con planillas registradas en la base de datos.
*****Retorno************************************************************
* Un entero con la cantidad de años con planillas registradas.
*****Entradas***********************************************************
* Sin entradas
************************************************************************/
int ObtenerCantidadAniosConPlanillas() {
    MYSQL *conn = Conectar();
    int cantidad = 0;
    char query[200];
    sprintf(query, "CALL get_cant_anios_con_planillas()");
    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return cantidad;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    cantidad = atoi(row[0]);
    mysql_close(conn);
    return cantidad;
}

/*****Nombre************************************************************
* ObtenerCantidadPlanillasPorAnio
*****Descripción********************************************************
* Obtiene la cantidad de planillas registradas en un año en la base de datos.
*****Retorno************************************************************
* int - La cantidad de planillas registradas en el año.
*****Entradas***********************************************************
* int anio - El año del cual se quiere obtener la cantidad de planillas.
************************************************************************/
int ObtenerCantidadPlanillasPorAnio(int anio) {
    MYSQL *conn = Conectar();
    int cantidad = 0;
    char query[200];
    sprintf(query, "CALL get_cantidad_planillas_por_anio(%i)", anio);
    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return -1;
    }
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    cantidad = atoi(row[0]);
    mysql_close(conn);
    return cantidad;
}

/*****Nombre************************************************************
* ObtenerTotalNominasConCargasPorAnio
*****Descripción********************************************************
* Obtiene el total de las nominas de los empleados de un anio dado.
* Calculado con el monto de la carga social.
*****Retorno************************************************************
* double - El total de las nominas de los empleados de un anio dado.
*****Entradas***********************************************************
* - int anio: El anio de las nominas a obtener.
************************************************************************/
double ObtenerTotalNominasConCargasPorAnio(int anio) {
    MYSQL *conn = Conectar();
    char query[200];
    sprintf(query, "CALL get_total_nomina_con_cargas_por_anio(%d)", anio);
    
    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return -1;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    double total = atof(row[0]);
    mysql_close(conn);
    return total;
}

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
        mysql_free_result(res);
        mysql_close(conn);
        return false;
    }
    
    mysql_free_result(res);
    mysql_close(conn);

    return true;
}

/*****Nombre************************************************************
* EliminarPlanillaPorFecha
*****Descripción********************************************************
* Elimina una planilla de la base de datos.
*****Retorno************************************************************
* Un valor booleano (bool) que indica el éxito (true) o fracaso (false)
* del eliminado.
*****Entradas***********************************************************
* - (Fecha fecha) La fecha de la planilla que se desea eliminar.
************************************************************************/
bool EliminarPlanillaPorFecha(Fecha fecha) {
    MYSQL *conn = Conectar();
    char query[256];

    sprintf(
        query, "CALL eliminar_planilla_por_fecha('%i', '%i')",
        fecha.mes, fecha.anio
    );
    
    if (mysql_query(conn, query)) {
        printf("%s\n", mysql_error(conn));
        mysql_close(conn);
        
        return false;
    }
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

    sprintf(
        query, "CALL get_planilla_por_fecha('%i', '%i')",
        fecha.mes, fecha.anio
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
* ObtenerPlanillasConCantEmpleados
*****Descripción********************************************************
* Obtiene las planillas con la cantidad de empleados registrados en
* cada una.
*****Retorno************************************************************
* Un arreglo (PlanillaConCantEmpleados *) con todas las planillas obtenidas.
*****Entradas***********************************************************
Sin entradas
************************************************************************/
PlanillaConCantEmpleados* ObtenerPlanillasConCantEmpleados() {
    MYSQL *conn = Conectar();
    char query[256];
    sprintf(
        query,
        "CALL get_planillas_con_cant_empleados()"
    );

    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return NULL;
    }


    res = mysql_store_result(conn);
    if (res) {
        int cant_planillas = mysql_num_rows(res) + 1;
        if (mysql_num_rows(res) == 0) {
            mysql_free_result(res);
            mysql_close(conn);
            return NULL;
        }
        PlanillaConCantEmpleados *planillas = malloc(
            cant_planillas * sizeof *planillas
        );
        int i = 0;
        while ((row = mysql_fetch_row(res)) && i < cant_planillas) {
            planillas[i].id = atoi(row[0]);
            char *fecha_plan_bd = row[1];
            sscanf(
                fecha_plan_bd, "%d-%d-%d", 
                &planillas[i].fecha.anio, 
                &planillas[i].fecha.mes, 
                &planillas[i].fecha.dia
            );
            planillas[i].monto_carga_social = atof(row[2]);
            planillas[i].cantidad_empleados = atoi(row[3]);
            i++;
        }
        planillas[i].id = -1;
        mysql_free_result(res);
        mysql_close(conn);
        return planillas;
    }
    mysql_close(conn);
    return NULL;
}

/*****Nombre************************************************************
* ObtenerPlanillasPorAnio
*****Descripción********************************************************
* Tiene todas las planillas de un año determinado.
*****Retorno************************************************************
* Un arreglo de estructuras (Planilla) con todas las planillas de un año.
*****Entradas***********************************************************
* - int anio: Año de las facturas.
************************************************************************/
PlanillaConCantEmpleados* ObtenerPlanillasPorAnio(int p_anio) {
    MYSQL *conn = Conectar();
    char query[256];
    sprintf(
        query,
        "CALL get_planillas_por_anio_con_cant_empleados(%i)",
        p_anio
    );

    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return NULL;
    }


    res = mysql_store_result(conn);
    if (res) {
        int cant_planillas = mysql_num_rows(res) + 1;
        if (mysql_num_rows(res) == 0) {
            mysql_free_result(res);
            mysql_close(conn);
            return NULL;
        }
        PlanillaConCantEmpleados *planillas = malloc(
            cant_planillas * sizeof *planillas
        );
        int i = 0;
        while ((row = mysql_fetch_row(res)) && i < cant_planillas) {
            planillas[i].id = atoi(row[0]);
            char *fecha_plan_bd = row[1];
            sscanf(
                fecha_plan_bd, "%d-%d-%d", 
                &planillas[i].fecha.anio, 
                &planillas[i].fecha.mes, 
                &planillas[i].fecha.dia
            );
            planillas[i].monto_carga_social = atof(row[2]);
            planillas[i].cantidad_empleados = atoi(row[3]);
            i++;
        }
        planillas[i].id = -1;
        mysql_free_result(res);
        mysql_close(conn);
        return planillas;
    }
    mysql_close(conn);
    return NULL;
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
* EliminarPlanilla
*****Descripción********************************************************
* Obtiene el reporte anual de ventas de un negocio en particular.
*****Retorno************************************************************
* Un ReporteAnual de las ventas del negocio.
*****Entradas***********************************************************
* - char* cedula_comercio: Cedula del comercio.
* - int anio: Año de las facturas.
************************************************************************/
bool EliminarPlanilla(int mes, int anio) {
    MYSQL *conn = Conectar();
    char query[256];
    sprintf(
        query, "CALL eliminar_planilla_por_fecha(%d, %d)",
        mes, anio
    );

    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return false;
    }
    
    mysql_close(conn);
    return true;
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

    if (mysql_query(conn, query)) {
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