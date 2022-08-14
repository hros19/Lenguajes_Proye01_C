-- ===================================================
-- Instituto Tecnológico de Costa Rica.
-- Lenguajes de programación.
-- Creación: 13 de Agosto del 2022.
-- Autores: Alex Sánchez y Hansol Antay.
-- Descripción: Script de inicialización de la base de datos para una aplicación de punto de venta de un sistema agrícola.
-- ===================================================

DROP DATABASE IF EXISTS pdvAgricola;
CREATE DATABASE IF NOT EXISTS pdvAgricola;
USE pdvAgricola;
SET GLOBAL sql_mode = '';
ALTER DATABASE pdvAgricola CHARACTER SET utf8 COLLATE utf8_general_ci;

-- ===================================================
-- >> USUARIOS
-- ===================================================

-- ===================================================
-- Tabla: Usuarios
-- Descripcion: Tabla que almacenara las credenciales
-- de acceso para las opciones operativas del sistema.

-- Nota: La clave debe ser encriptada.
-- ===================================================
CREATE TABLE Usuarios (
  id INT NOT NULL AUTO_INCREMENT,
  usuario VARCHAR(50) NOT NULL,
  clave VARCHAR(255) NOT NULL,

  CONSTRAINT pk_usuarios PRIMARY KEY (id),
  CONSTRAINT uq_usuarios UNIQUE (usuario)
);

-- ===================================================
-- Tabla: Roles
-- Descripcion: Tabla que almacenara los roles que tienen
-- los empleados en el comercio.

-- Nota: Deben ser auto-generados y no se pueden editar.
-- ===================================================
CREATE TABLE Roles (
  id INT NOT NULL AUTO_INCREMENT,
  nombre VARCHAR(50) NOT NULL,
  descripcion VARCHAR(255) NOT NULL,
  CONSTRAINT pk_roles PRIMARY KEY (id),
  CONSTRAINT uq_roles UNIQUE (nombre)
);

-- ===================================================
-- Tabla: CargosSociales
-- Descripcion: Cargos sociales que pueden tener los
-- empleados.
-- Nota: Los valores deben ser tomados como porcentajes.
-- ===================================================
CREATE TABLE CargosSociales (
  id INT NOT NULL AUTO_INCREMENT,
  nombre VARCHAR(50) NOT NULL,
  double porcentaje_cargo DOUBLE(10,2) NOT NULL,

  CONSTRAINT pk_cargos_sociales PRIMARY KEY (id),
  CONSTRAINT uq_cargos_sociales UNIQUE (nombre)
);

-- ===================================================
-- Tabla: Productos
-- Descripcion: Productos que seran vendidos en el
-- comercio.
-- Nota: Estos deben ser auto-generados y solo pueden ser
-- cargados por el administrador. (Por el mismo programa)
-- ===================================================
CREATE TABLE Productos (
  id VARCHAR(50) NOT NULL,
  nombre VARCHAR(50) NOT NULL,
  costo DOUBLE(10,2) NOT NULL,
  impuesto DOUBLE(10,2) NOT NULL,
  CONSTRAINT pk_productos PRIMARY KEY (id),
);

-- ===================================================
-- Tabla: Areas
-- Descripcion: Areas que pueden tener los productos.
-- Nota: Estos deben ser auto-generados y no pueden ser
-- editados.
-- ===================================================
CREATE TABLE Areas (
  id INT NOT NULL AUTO_INCREMENT,
  nombre VARCHAR(50) NOT NULL,
  -- producto_principal VARCHAR(50) NOT NULL,
  CONSTRAINT pk_areas PRIMARY KEY (id),
  CONSTRAINT uq_areas UNIQUE (nombre)
);

-- ===================================================
-- Tabla: Empleados
-- Descripcion: Empleados que trabajan en el comercio.
-- Nota: Estos deben ser auto-generados y no pueden ser
-- editados.
-- ===================================================
CREATE TABLE Empleados (
  cedula VARCHAR(50) NOT NULL,
  nombre_completo VARCHAR(255) NOT NULL,
  id_rol INT NOT NULL,
  salario_mensual DOUBLE(10,2) NOT NULL,

  CONSTRAINT pk_empleados PRIMARY KEY (cedula),
  CONSTRAINT fk_emp_rol FOREIGN KEY (id_rol)
    REFERENCES Roles(id) ON DELETE RESTRICT,
);

-- ===================================================
-- Tabla: Comercios
-- Descripcion: Tabla para almacenar la informacion
-- del local comercial agrícola. (Indispensable para facturas)
-- ===================================================
CREATE TABLE Comercios (
  cedula VARCHAR(50) NOT NULL,
  nombre VARCHAR(50) NOT NULL,
  telefono VARCHAR(50) NOT NULL,
  num_sig_factura INT NOT NULL,
  CONSTRAINT pk_comercios PRIMARY KEY (id),
  CONSTRAINT uq_comercios UNIQUE (nombre)
);

-- ===================================================
-- Tabla: Planillas
-- Descripcion: Tabla para almacenar la informacion
-- de las planillas de los empleados.
-- ===================================================
CREATE TABLE Planillas (
  id INT NOT NULL AUTO_INCREMENT,
  fecha_nomina DATE NOT NULL,
  CONSTRAINT pk_planillas PRIMARY KEY (id),
  CONSTRAINT uq_planillas UNIQUE (fecha_nomina)
);

-- ===================================================
-- Tabla: PlanillasXEmpleados
-- Descripcion: Tabla para almacenar a los empleados
-- que trabajan en la planilla
CREATE TABLE PlanillasXEmpleados (
  id INT NOT NULL AUTO_INCREMENT,
  id_planilla INT NOT NULL,
  cedula_empleado VARCHAR(50) NOT NULL,
  CONSTRAINT pk_planXemp PRIMARY KEY (id),
  CONSTRAINT fk_planXemp_plan FOREIGN KEY (id_planilla)
    REFERENCES Planillas(id) ON DELETE RESTRICT,
  CONSTRAINT fk_planXemp_emp FOREIGN KEY (cedula_empleado)
    REFERENCES Empleados(cedula) ON DELETE RESTRICT
);

-- ===================================================
-- Tabla: Facturas
-- Descripcion: Tabla para almacenar las facturas
-- de los clientes.
-- ===================================================
CREATE TABLE Facturas (
  id INT NOT NULL AUTO_INCREMENT,
  nombre_cliente VARCHAR(50) NOT NULL,
  fecha_facturacion DATE NOT NULL,
  cedula_comercio VARCHAR(50) NOT NULL,
  subtotal DOUBLE(15,2) NOT NULL,
  impuesto DOUBLE(15,2) NOT NULL,
  total DOUBLE(15,2) NOT NULL,
  CONSTRAINT pk_facturas PRIMARY KEY (id),
  CONSTRAINT fk_fact_comercio FOREIGN KEY (cedula_comercio)
    REFERENCES Comercios(cedula) ON DELETE CASCADE,
);

-- ===================================================
-- Tabla: DetalleProducto
-- Descripcion: Tabla para almacenar los detalles de
-- los productos que se venden.
-- ===================================================
CREATE TABLE DetalleProducto (
  id INT NOT NULL AUTO_INCREMENT,
  id_producto VARCHAR(50) NOT NULL,
  cantidad INT NOT NULL,
  CONSTRAINT pk_detprod PRIMARY KEY (id),
  CONSTRAINT fk_detprod_prod FOREIGN KEY (id_producto)
    REFERENCES Productos(id) ON DELETE RESTRICT
);

-- ===================================================
-- Tabla: FacturasXDetalleProducto
-- Descripcion: Tabla para almacenar los detalles de
-- los productos que se venden y que se facturan.
-- ===================================================
CREATE TABLE FacturasXDetalleProducto (
  id INT NOT NULL AUTO_INCREMENT,
  id_detalle INT NOT NULL,
  id_factura INT NOT NULL,
  CONSTRAINT pk_factXdetprod PRIMARY KEY (id),
  CONSTRAINT fk_factXdetprod_dp FOREIGN KEY (id_detalle)
    REFERENCES DetalleProducto(id) ON DELETE RESTRICT,
  CONSTRAINT fk_factXdetprod_fact FOREIGN KEY (id_factura)
    REFERENCES Facturas(id) ON DELETE CASCADE,
  CONSTRAINT uq_factXdetprod UNIQUE (id_detalle, id_factura)
);
