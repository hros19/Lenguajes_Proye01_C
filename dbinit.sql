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

DELIMITER $$
-- GET Usuarios
CREATE PROCEDURE get_usuarios()
BEGIN
  SELECT * FROM Usuarios;
END $$
DELIMITER ;

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

INSERT INTO Roles (nombre, descripcion) 
  VALUES ('Administrador', 'Administrador de la productora');
INSERT INTO Roles (nombre, descripcion) 
  VALUES ('Agricultor', 'Encargado de árboles, cultivos y suelos');
INSERT INTO Roles (nombre, descripcion) 
  VALUES ('Científico agrónomo', 'Gestor de producción e impacto ambiental');
INSERT INTO Roles (nombre, descripcion)
  VALUES ('Inspector', 'Encargado de la inspección del producto y suelos');
INSERT INTO Roles (nombre, descripcion)
  VALUES ('Operario', 'Encargado del manejo del equipo agropecuario');
INSERT INTO Roles (nombre, descripcion)
  VALUES ('Recolector', 'Encargado de la recolección del producto');
INSERT INTO Roles (nombre, descripcion)
  VALUES ('Supervisor de calidad', 'Encargado de mantener y procurar la calidad del producto');
INSERT INTO Roles (nombre, descripcion)
  VALUES ('Técnico agrónomo', 'Encargado del manejo de cultivos, árboles y suelos');

-- ===================================================
-- Tabla: CargosSociales
-- Descripcion: Cargos sociales que pueden tener los
-- empleados.
-- Nota: Los valores deben ser tomados como porcentajes.
-- ===================================================
CREATE TABLE CargosSociales (
  id INT NOT NULL AUTO_INCREMENT,
  nombre VARCHAR(50) NOT NULL,
	porcentaje_cargo DOUBLE(10,2) NOT NULL,

  CONSTRAINT pk_cargos_sociales PRIMARY KEY (id),
  CONSTRAINT uq_cargos_sociales UNIQUE (nombre)
);

INSERT INTO CargosSociales (nombre, porcentaje_cargo) 
  VALUES ('General', '1.5');

DELIMITER $$
CREATE FUNCTION get_cargos_sociales()
  RETURNS INT DETERMINISTIC
BEGIN
  RETURN (
    SELECT porcentaje_cargo 
    FROM CargosSociales WHERE nombre = 'General'
  );
END $$

-- MODIFY CargosSociales
CREATE PROCEDURE modify_cargos_sociales(
  IN porcentaje_cargo DOUBLE(10,2)
)
BEGIN
  UPDATE CargosSociales 
  SET porcentaje_cargo = porcentaje_cargo
  WHERE nombre = 'General';
END $$
DELIMITER ;

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
  CONSTRAINT pk_productos PRIMARY KEY (id)
);

DELIMITER $$

-- GET Productos
CREATE PROCEDURE get_productos()
BEGIN
  SELECT * FROM Productos;
END $$

-- CREATE Productos
CREATE PROCEDURE create_productos(
    IN id VARCHAR(50),
    IN nombre VARCHAR(50), 
    IN costo DOUBLE(10,2), 
    IN impuesto DOUBLE(10,2)
  )
BEGIN
  INSERT INTO Productos (nombre, costo, impuesto)
    VALUES (nombre, costo, impuesto);
END $$

-- REMOVE Productos
CREATE PROCEDURE remove_productos()
BEGIN
  DELETE FROM Productos;
END $$

DELIMITER ;

-- ===================================================
-- Tabla: Areas
-- Descripcion: Areas que pueden tener los productos.
-- Nota: Estos deben ser auto-generados y no pueden ser
-- editados.
-- ===================================================
CREATE TABLE Areas (
  id INT NOT NULL AUTO_INCREMENT,
  nombre VARCHAR(50) NOT NULL,
  dimensiones DOUBLE(15,2) NOT NULL,
  -- producto_principal VARCHAR(50) NOT NULL,
  CONSTRAINT pk_areas PRIMARY KEY (id),
  CONSTRAINT uq_areas UNIQUE (nombre)
);

INSERT INTO Areas (nombre, dimensiones) 
  VALUES ('Cereales', 15.2);
INSERT INTO Areas (nombre, dimensiones)
  VALUES ('Leguminosas', 20.5);
INSERT INTO Areas (nombre, dimensiones)
  VALUES ('Frutas', 12.4);
INSERT INTO Areas (nombre, dimensiones)
  VALUES ('Hortalizas', 18.7);
INSERT INTO Areas (nombre, dimensiones)
  VALUES ('Verduras', 15.6);
INSERT INTO Areas (nombre, dimensiones)
  VALUES ('Oleaginosas', 19.5);
INSERT INTO Areas (nombre, dimensiones)
  VALUES ('Ornamentales', 11.4);
INSERT INTO Areas (nombre, dimensiones)
  VALUES ('Tuberculosas', 12.5);
INSERT INTO Areas (nombre, dimensiones)
  VALUES ('Pastos', 11.5);
INSERT INTO Areas (nombre, dimensiones)
  VALUES ('Medicinales', 19.5);

DELIMITER $$

-- GET Areas
CREATE PROCEDURE get_areas()
BEGIN
  SELECT * FROM Areas;
END $$

DELIMITER ;

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
    REFERENCES Roles(id) ON DELETE RESTRICT
);

INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual) 
  VALUES ('12345678', 'Juan Perez Diaz', 1, '300000.00');
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345679', 'Maria Perez Rojas', 2, '350000.00');
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345670', 'Pedro Rodriguez Diaz', 3, '280000.00');
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345671', 'Juanito Salas Mendez', 4, '750000.00');
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345672', 'Luis Robles Fernandez', 5, '250000.00');
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345673', 'Charlie Perez Diaz', 6, '300000.00');
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345674', 'Juanito Chavarria Diaz', 7, '350000.00');
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345675', 'Marcos Escalante Salas', 8, '280000.00');
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345676', 'Sara Mendez Perez', 3, '650000.00');
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345677', 'Juanito Perez Mendez', 2, '250000.00');

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
  CONSTRAINT pk_comercios PRIMARY KEY (cedula),
  CONSTRAINT uq_comercios UNIQUE (nombre)
);

INSERT INTO Comercios (cedula, nombre, telefono, num_sig_factura) 
  VALUES ('001', 'Comercio Agrícola', '88888888', 1);

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

DELIMITER $$

-- GET Planillas
CREATE PROCEDURE get_planillas()
BEGIN
  SELECT * FROM Planillas;
END $$

-- GET Planillas por año
CREATE PROCEDURE get_planillas_por_anio(anio INT)
BEGIN
  SELECT * FROM Planillas 
  WHERE YEAR(fecha_nomina) = anio;
END $$

-- CREATE Planilla
CREATE PROCEDURE create_planilla(IN fecha_nomina DATE)
BEGIN
  INSERT INTO Planillas (fecha_nomina) VALUES (fecha_nomina);
END $$

DELIMITER ;

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
    REFERENCES Empleados(cedula) ON DELETE RESTRICT,
  CONSTRAINT uq_planXemp UNIQUE (id_planilla, cedula_empleado)
);

DELIMITER $$

-- GET PlanillasXEmpleados
-- Obtiene todos los empleados de una planilla
CREATE PROCEDURE get_planXemp(IN id_planilla INT)
BEGIN
  DECLARE strQuery VARCHAR(555);
  SET @strQuery = CONCAT(
    'SELECT Emp.cedula, Emp.nombre_completo, Emp.salario_mensual, ', 
    'Rol.nombre, '
    'FROM Empleados Emp ',
    'INNER JOIN Roles Rol ON Emp.id_rol = Rol.id ',
    'INNER JOIN PlanillasXEmpleados PlaXEmp ON Emp.cedula = PlaXEmp.cedula_empleado ',
    'WHERE PlaXEmp.id_planilla = ', id_planilla
  );
  PREPARE stmt FROM @strQuery;
  EXECUTE stmt;
END $$

DELIMITER ;

-- ===================================================
-- Tabla: Facturas
-- Descripcion: Tabla para almacenar las facturas
-- de los clientes.
-- ===================================================
CREATE TABLE Facturas (
  id INT NOT NULL AUTO_INCREMENT,
  nombre_cliente VARCHAR(50) NOT NULL,
  fecha_facturacion DATE NOT NULL,
  area_produccion VARCHAR(50) NOT NULL,
  cedula_comercio VARCHAR(50) NOT NULL,
  subtotal DOUBLE(15,2) NOT NULL,
  impuesto DOUBLE(15,2) NOT NULL,
  total DOUBLE(15,2) NOT NULL,
  CONSTRAINT pk_facturas PRIMARY KEY (id),
  CONSTRAINT fk_fact_comercio FOREIGN KEY (cedula_comercio)
    REFERENCES Comercios(cedula) ON DELETE CASCADE
);

DELIMITER $$

-- GET Facturas
CREATE PROCEDURE get_facturas()
BEGIN
  SELECT Fac.id, Fac.nombre_cliente, Fac.fecha_facturacion, 
         Fac.area_produccion, Fac.cedula_comercio,
         Com.nombre, Com.telefono, Com.num_sig_factura, 
         Fac.subtotal, Fac.impuesto, Fac.total
  FROM Facturas Fac
  INNER JOIN Comercios Com ON Fac.cedula_comercio = Com.cedula;
END $$

-- CREATE Factura
CREATE PROCEDURE create_factura(
  IN nombre_cliente VARCHAR(50), 
  IN fecha_facturacion DATE, 
  IN cedula_comercio VARCHAR(50), 
  IN subtotal DOUBLE, 
  IN impuesto DOUBLE, 
  IN total DOUBLE
)
BEGIN
  INSERT INTO Facturas (
    nombre_cliente, fecha_facturacion, cedula_comercio, 
    subtotal, impuesto, total
  )
  VALUES (
    nombre_cliente, fecha_facturacion, cedula_comercio,
    subtotal, impuesto, total
  );
  SET @id = LAST_INSERT_ID();
  UPDATE Comercios SET num_sig_factura = num_sig_factura + 1 WHERE cedula = cedula_comercio;
  SELECT * FROM Facturas WHERE id = @id;
END $$

DELIMITER ;

-- ===================================================
-- Tabla: FacturasXDetalleProducto
-- Descripcion: Tabla para almacenar los detalles de
-- los productos que se venden y que se facturan.
-- ===================================================
CREATE TABLE FacturasXDetalleProducto (
  id INT NOT NULL AUTO_INCREMENT,
  id_producto VARCHAR(50) NOT NULL,
  id_factura INT NOT NULL,
  cantidad INT NOT NULL,
  CONSTRAINT pk_factXdetprod PRIMARY KEY (id),
  CONSTRAINT fk_factXdetprod_fact FOREIGN KEY (id_factura)
    REFERENCES Facturas(id) ON DELETE CASCADE,
  CONSTRAINT fk_factXdetprod_prod FOREIGN KEY (id_producto)
    REFERENCES Productos(id) ON DELETE CASCADE,
  CONSTRAINT uq_factXdetprod UNIQUE (id_producto, id_factura)
);

DELIMITER $$

-- get FacturasXDetalleProducto
CREATE PROCEDURE get_factXdetprod(IN id_factura INT)
BEGIN
  DECLARE strQuery VARCHAR(555);
  SET @strQuery = CONCAT(
    'SELECT Prod.id, Prod.nombre, Prod.precio, ', 
    'FactXDetProd.cantidad, FactXDetProd.id_factura ',
    'FROM Productos Prod ',
    'INNER JOIN FacturasXDetalleProducto FactXDetProd ON Prod.id = FactXDetProd.id_producto ',
    'WHERE FactXDetProd.id_factura = ', id_factura
  );
  PREPARE stmt FROM @strQuery;
  EXECUTE stmt;
END $$

-- create FacturasXDetalleProducto
CREATE PROCEDURE create_factXdetprod(
  IN id_producto INT, 
  IN id_factura INT, 
  IN cantidad INT
)
BEGIN
  INSERT INTO FacturasXDetalleProducto (
    id_producto, id_factura, cantidad
  )
  VALUES (
    id_producto, id_factura, cantidad
  );
END $$
