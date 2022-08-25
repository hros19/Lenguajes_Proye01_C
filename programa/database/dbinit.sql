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
SET SQL_SAFE_UPDATES = 0;
-- ALTER DATABASE pdvAgricola CHARACTER SET utf8 COLLATE utf8_unicode_ci;

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

-- ===================================================
-- PROCEDURE: get_usuarios
-- Descripción: Obtiene todos los usuarios o operadores
--              del sistema.

-- Nota: La clave se almacena encriptada.
-- ===================================================
CREATE PROCEDURE get_usuarios()
BEGIN
  SELECT * FROM Usuarios;
END $$

-- ===================================================
-- PROCEDURE: get_usuario
-- Descripción: Obtiene un usuario en específico
--              buscándolo por su usuario en la tabla.
-- Entradas:
--    - IN usuario VARCHAR(50): nombre de usuario
-- 
-- ===================================================
CREATE PROCEDURE get_usuario(IN usuario VARCHAR(50))
BEGIN
  SELECT * FROM Usuarios WHERE usuario = usuario;
END $$

-- ===================================================
-- PROCEDURE: create_usuario
-- Descripción: Crea o registra un nuevo usuario
--              en la tabla.
-- Entradas:
--    - IN usuario VARCHAR(50): nombre de usuario
--    - IN clave VARCHAR(255): clave encriptada.
-- 
-- ===================================================
CREATE PROCEDURE create_usuario(IN usuario VARCHAR(50), IN clave VARCHAR(255))
BEGIN
  INSERT INTO Usuarios (usuario, clave) VALUES (usuario, clave);
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

-- Valores por defecto de la aplicación.

INSERT INTO Roles (nombre, descripcion) 
  VALUES ('Administrador', 'Administrador de la productora');
INSERT INTO Roles (nombre, descripcion) 
  VALUES ('Agricultor', 'Encargado de arboles, cultivos y suelos');
INSERT INTO Roles (nombre, descripcion) 
  VALUES ('Cientifico agronomo', 'Gestor de produccion e impacto ambiental');
INSERT INTO Roles (nombre, descripcion)
  VALUES ('Inspector', 'Encargado de la inspeccion del producto y suelos');
INSERT INTO Roles (nombre, descripcion)
  VALUES ('Operario', 'Encargado del manejo del equipo agropecuario');
INSERT INTO Roles (nombre, descripcion)
  VALUES ('Recolector', 'Encargado de la recoleccion del producto');
INSERT INTO Roles (nombre, descripcion)
  VALUES ('Supervisor de calidad', 'Encargado de mantener y procurar la calidad del producto');
INSERT INTO Roles (nombre, descripcion)
  VALUES ('Tecnico agrónomo', 'Encargado del manejo de cultivos, arboles y suelos');

DELIMITER $$

-- ===================================================
-- PROCEDURE: get_roles
-- Descripción: Obtiene todos los roles de la tabla.
-- ===================================================
CREATE PROCEDURE get_roles()
BEGIN
  SELECT * FROM Roles;
END $$

-- GET Rol por id
CREATE PROCEDURE get_rol(IN id INT)
BEGIN
  SELECT * FROM Roles WHERE id = id;
END $$

DELIMITER ;

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

-- Valor por defecto de la carga social de los salarios.

INSERT INTO CargosSociales (nombre, porcentaje_cargo) 
  VALUES ('General', '1.5');

DELIMITER $$

-- ===================================================
-- PROCEDURE: get_cargo_social
-- Descripción: Obtiene el cargo social buscándolo por
--              su nombre en la tabla.
-- Entradas:
--    - IN usuario VARCHAR(50): el nombre del cargo social.
-- 
-- ===================================================
CREATE PROCEDURE get_cargo_social(IN nombre VARCHAR(50))
BEGIN
  SELECT * FROM CargosSociales WHERE nombre = nombre;
END $$

-- ===================================================
-- PROCEDURE: modif_cargo_social
-- Descripción: Modifica el valor de un cargo social
--              en específico.
-- Entradas:
--    - IN nombre VARCHAR(50): nombre del cargo social.
--    - IN porcentaje_cargo DOUBLE(10, 2): nuevo porcentaje
--           del cargo social.
-- 
-- ===================================================
CREATE PROCEDURE modif_cargo_social(
  IN nombre VARCHAR(50),
  IN porcentaje_cargo DOUBLE(10,2)
)
BEGIN
  UPDATE CargosSociales 
  SET porcentaje_cargo = porcentaje_cargo
  WHERE nombre = nombre;
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
  CONSTRAINT pk_productos PRIMARY KEY (id),
  CONSTRAINT uq_productos UNIQUE (nombre)
);

DELIMITER $$

-- ===================================================
-- PROCEDURE: get_productos.
-- Descripción: Obtiene todos los productos registrados
--              en la base de datos.
-- ===================================================
CREATE PROCEDURE get_productos()
BEGIN
  SELECT * FROM Productos;
END $$

-- ===================================================
-- PROCEDURE: registrar_producto
-- Descripción: Crea o registra un nuevo producto
--              en la tabla.
-- Entradas:
--    - IN id VARCHAR(50): Id del producto.
--    - IN nombre VARCHAR(50): Nombre del producto.
--    - IN costo DOUBLE(10,2): Costo del producto.
--    - IN impuesto DOUBLE(10,2): Impuesto porcentual.
-- 
-- ===================================================
CREATE PROCEDURE registrar_producto(
    IN id VARCHAR(50),
    IN nombre VARCHAR(50), 
    IN costo DOUBLE(10,2), 
    IN impuesto DOUBLE(10,2)
  )
BEGIN
  INSERT INTO Productos (id, nombre, costo, impuesto)
    VALUES (id, nombre, costo, impuesto);
END $$

-- ===================================================
-- PROCEDURE: get_producto
-- Descripción: Obtiene un producto en específico.
-- Entradas:
--    - IN id VARCHAR(50): Id del producto.
-- 
-- ===================================================
CREATE PROCEDURE get_producto(IN pId VARCHAR(50))
BEGIN
  SELECT * FROM Productos WHERE id = pId;
END $$

-- ===================================================
-- PROCEDURE: remove_productos
-- Descripción: Elimina todos los productos de la tabla.
-- 
-- ===================================================
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
  producto_principal VARCHAR(50) NOT NULL,
  CONSTRAINT pk_areas PRIMARY KEY (id),
  CONSTRAINT uq_areas UNIQUE (nombre)
);

-- Valores por defecto de las áreas.

INSERT INTO Areas (nombre, dimensiones, producto_principal) 
  VALUES ('Cereales', 15.2, 'Trigo');
INSERT INTO Areas (nombre, dimensiones, producto_principal)
  VALUES ('Leguminosas', 20.5, 'Maiz');
INSERT INTO Areas (nombre, dimensiones, producto_principal)
  VALUES ('Frutas', 12.4, 'Manzana');
INSERT INTO Areas (nombre, dimensiones, producto_principal)
  VALUES ('Hortalizas', 18.7, 'Papa');
INSERT INTO Areas (nombre, dimensiones, producto_principal)
  VALUES ('Verduras', 15.6, 'Pepino');
INSERT INTO Areas (nombre, dimensiones, producto_principal)
  VALUES ('Oleaginosas', 19.5, 'Aguacate');
INSERT INTO Areas (nombre, dimensiones, producto_principal)
  VALUES ('Ornamentales', 11.4, 'Cebolla');
INSERT INTO Areas (nombre, dimensiones, producto_principal)
  VALUES ('Tuberculosas', 12.5, 'Puerro');
INSERT INTO Areas (nombre, dimensiones, producto_principal)
  VALUES ('Pastos', 11.5, 'Lechuga');
INSERT INTO Areas (nombre, dimensiones, producto_principal)
  VALUES ('Medicinales', 19.5, 'Algodon');

DELIMITER $$

-- ===================================================
-- PROCEDURE: get_areas
-- Descripción: Obtiene todas las áreas registradas
-- 
-- ===================================================
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

-- Valores por defecto de los empleados.

INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual) 
  VALUES ('12345678', 'Juan Perez Diaz', 1, 350000);
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345679', 'Maria Perez Rojas', 2, 320000);
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345670', 'Pedro Rodriguez Diaz', 3, 280000);
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345671', 'Juanito Salas Mendez', 4, 650000);
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345672', 'Luis Robles Fernandez', 5, 250000);
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345673', 'Charlie Perez Diaz', 6, 300000);
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345674', 'Juanito Chavarria Diaz', 7, 350000);
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345675', 'Marcos Escalante Salas', 8, 280000);
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345676', 'Sara Mendez Perez', 3, 650000);
INSERT INTO Empleados (cedula, nombre_completo, id_rol, salario_mensual)
  VALUES ('12345677', 'Juanito Perez Mendez', 2, 250000);

DELIMITER $$

-- ===================================================
-- PROCEDURE: get_empleados
-- Descripción: Obtiene todos los empleados registrados.
-- 
-- ===================================================
CREATE PROCEDURE get_empleados()
BEGIN
  SELECT * FROM Empleados;
END $$

-- ===================================================
-- PROCEDURE: get_empleados_por_rol
-- Descripción: Obtiene todos los empleados registrados
--              con su rol.
-- 
-- ===================================================
CREATE PROCEDURE get_empleados_con_rol()
BEGIN
  SELECT Emp.cedula, Emp.nombre_completo,
         Rol.nombre, Emp.salario_mensual 
  FROM Empleados Emp 
  INNER JOIN Roles Rol ON Emp.id_rol = Rol.id;
END $$

-- ===================================================
-- PROCEDURE: get_empleado
-- Descripción: Obtiene un empleado registrado.
-- Entradas:
--    - IN cedula VARCHAR(50): cedula del empleado a buscar.
-- 
-- ===================================================
CREATE PROCEDURE get_empleado(IN cedula VARCHAR(50))
BEGIN
  SELECT * FROM Empleados WHERE cedula = cedula;
END $$

-- ===================================================
-- PROCEDURE: get_empleado_con_rol
-- Descripción: Obtiene la información de un empleado
--              con su rol respectivo.
-- Entradas:
--    - IN cedula VARCHAR(50): cedula del empleado a buscar.
-- 
-- ===================================================
CREATE PROCEDURE get_empleado_con_rol(IN cedula VARCHAR(50))
BEGIN
  SELECT Emp.cedula, Emp.nombre_completo, Rol.nombre, Emp.salario_mensual 
  FROM Empleados Emp
  INNER JOIN Roles Rol ON Emp.id_rol = Rol.id
  WHERE Emp.cedula = cedula;
END $$

DELIMITER ;

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

-- Valores por defecto del comercio.

INSERT INTO Comercios (cedula, nombre, telefono, num_sig_factura) 
  VALUES ('001', 'Comercio Agrícola', '88888888', 1);

DELIMITER $$

-- ===================================================
-- PROCEDURE: get_comercio
-- Descripción: Obtienen la información del local comercial agrícola.
-- Entradas:
--    - IN cedula VARCHAR(50): cedula del local comercial agrícola.
-- 
-- ===================================================
CREATE PROCEDURE get_comercio(IN cedula VARCHAR(50))
BEGIN
  SELECT * FROM Comercios WHERE cedula = cedula;
END $$

-- ===================================================
-- PROCEDURE: registrar_comercio
-- Descripción: Registra un local comercial agrícola.
-- Entradas:
--    - IN cedula VARCHAR(50): cedula del local comercial agrícola.
--    - IN nombre VARCHAR(50): nombre del local comercial agrícola.
--    - IN telefono VARCHAR(50): telefono del local comercial agrícola.
--    - IN num_sig_factura: numero de la siguiente factura.
-- 
-- ===================================================
CREATE PROCEDURE registrar_comercio(
    IN cedula VARCHAR(50),
    IN nombre VARCHAR(50),
    IN telefono VARCHAR(50),
    IN num_sig_factura INT
)
BEGIN
  INSERT INTO Comercios (cedula, nombre, telefono, num_sig_factura)
    VALUES (cedula, nombre, telefono, num_sig_factura);
END $$


DELIMITER ;

-- ===================================================
-- Tabla: Planillas
-- Descripcion: Tabla para almacenar la informacion
-- de las planillas de los empleados.
-- ===================================================
CREATE TABLE Planillas (
  id INT NOT NULL AUTO_INCREMENT,
  fecha_nomina DATE NOT NULL,
  carga_social DOUBLE(10,2) NOT NULL,
  CONSTRAINT pk_planillas PRIMARY KEY (id),
  CONSTRAINT uq_planillas UNIQUE (fecha_nomina)
);

DELIMITER $$

-- ===================================================
-- PROCEDURE: get_planillas
-- Descripción: Obtiene todas las planillas registradas.
-- 
-- ===================================================
CREATE PROCEDURE get_planillas()
BEGIN
  SELECT * FROM Planillas;
END $$

-- ===================================================
-- PROCEDURE: get_planillas_con_cant_empleados
-- Descripción: Obtiene la información de las planillas
--              con la cantidad de empleados que la contiene.
-- ===================================================
CREATE PROCEDURE get_planillas_con_cant_empleados()
BEGIN
  SELECT P.id, P.fecha_nomina, P.carga_social, COUNT(PE.id) AS cant_empleados
  FROM Planillas P
  INNER JOIN PlanillasXEmpleados PE ON P.id = PE.id_planilla
  WHERE P.id = PE.id_planilla
  GROUP BY P.id;
END $$

-- ===================================================
-- PROCEDURE: get_planillas_por_anio
-- Descripción: Obtiene todas las planillas registradas
--              por año.
-- Entradas:
--    - IN anio INT: año de las planillas a buscar.
-- 
-- ===================================================
CREATE PROCEDURE get_planillas_por_anio(anio INT)
BEGIN
  SELECT * FROM Planillas 
  WHERE YEAR(fecha_nomina) = anio;
END $$

-- ===================================================
-- PROCEDURE: get_planillas_por_anio_con_cant_empleados
-- Descripción: Obtiene la información de las planillas
--              por año con la cantidad de empleados
--
-- ===================================================
CREATE PROCEDURE get_planillas_por_anio_con_cant_empleados(anio INT)
BEGIN
  SELECT P.id, P.fecha_nomina, P.carga_social, COUNT(PE.id) AS cant_empleados
  FROM Planillas P
  INNER JOIN PlanillasXEmpleados PE ON P.id = PE.id_planilla
  WHERE YEAR(P.fecha_nomina) = anio
  GROUP BY P.id;
END $$

-- ===================================================
-- PROCEDURE: registrar_planilla
-- Descripción: Registra una planilla.
-- Entradas:
--    - IN fecha_nomina DATE: fecha de la planilla.
-- 
-- Nota: El valor de la carga social se agarra de la tabla
--       de una vez.
-- ===================================================
CREATE PROCEDURE registrar_planilla(IN fecha_nomina DATE)
BEGIN
    DECLARE monto_carga_social DOUBLE(10,2);
    SELECT porcentaje_cargo INTO monto_carga_social FROM CargosSociales WHERE id = 1;
    INSERT INTO Planillas (fecha_nomina, carga_social)
      VALUES (fecha_nomina, monto_carga_social);
END $$

-- ===================================================
-- PROCEDURE: get_planilla_por_fecha
-- Descripción: Obtiene la planilla registrada por fecha.
-- Entradas:
--    - IN fecha_nomina DATE: fecha de la planilla a buscar.
-- 
-- ===================================================
CREATE PROCEDURE get_planilla_por_fecha(IN mes_nomina INT, IN anio_nomina INT)
BEGIN
    SELECT * FROM Planillas 
    WHERE MONTH(fecha_nomina) = mes_nomina AND YEAR(fecha_nomina) = anio_nomina;
END $$

-- ===================================================
-- PROCEDURE: eliminar_planilla_por_fecha
-- Descripción: Elimina una planilla registrada por fecha.
-- Entradas:
--    - IN fecha_nomina DATE: fecha de la planilla a eliminar.
--
-- ===================================================
CREATE PROCEDURE eliminar_planilla_por_fecha(IN mes_nomina INT, IN anio_nomina INT)
BEGIN
    DELETE FROM Planillas 
    WHERE MONTH(fecha_nomina) = mes_nomina AND YEAR(fecha_nomina) = anio_nomina;
END $$


DELIMITER ;

-- ===================================================
-- Tabla: PlanillasXEmpleados
-- Descripcion: Tabla para almacenar a los empleados
-- que trabajan en la planilla
-- ===================================================
CREATE TABLE PlanillasXEmpleados (
  id INT NOT NULL AUTO_INCREMENT,
  id_planilla INT NOT NULL,
  cedula_empleado VARCHAR(50) NOT NULL,
  CONSTRAINT pk_planXemp PRIMARY KEY (id),
  CONSTRAINT fk_planXemp_plan FOREIGN KEY (id_planilla)
    REFERENCES Planillas(id) ON DELETE CASCADE,
  CONSTRAINT fk_planXemp_emp FOREIGN KEY (cedula_empleado)
    REFERENCES Empleados(cedula) ON DELETE RESTRICT,
  CONSTRAINT uq_planXemp UNIQUE (id_planilla, cedula_empleado)
);

DELIMITER $$

-- ===================================================
-- PROCEDURE: get_planXemp
-- Descripción: Obtiene todos los empleados que trabajan
--              en la planilla.
-- Entradas:
--    - IN id_planilla INT: id de la planilla.
-- 
-- ===================================================
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

-- ===================================================
-- PROCEDURE: get_planXemp_por_fecha
-- Descripción: Obtiene todos los empleados que trabajan
--              en la planilla por fecha.
-- Entradas:
--    - IN fecha_nomina DATE: fecha de la planilla.
-- 
-- ===================================================
CREATE PROCEDURE get_planXemp_por_fecha(IN fecha_nomina DATE)
BEGIN
  SELECT Emp.cedula, Emp.nombre_completo, Emp.salario_mensual, Rol.nombre
    FROM Empleados Emp
    INNER JOIN Roles Rol ON Emp.id_rol = Rol.id
    INNER JOIN PlanillasXEmpleados PlaXEmp ON Emp.cedula = PlaXEmp.cedula_empleado
    INNER JOIN Planillas Pla ON PlaXEmp.id_planilla = Pla.id
    WHERE Pla.fecha_nomina = fecha_nomina;
END $$

-- ===================================================
-- PROCEDURE: get_cant_empleados_planilla
-- Descripción: Obtiene la cantidad de empleados que
--              trabajan en la planilla.
-- Entradas:
--    - IN id_planilla INT: id de la planilla.
-- 
-- ===================================================
CREATE PROCEDURE get_cant_empleados_planilla(IN id_planilla INT)
BEGIN
  SELECT COUNT(*) FROM PlanillasXEmpleados 
  WHERE id_planilla = id_planilla;
END $$

-- ===================================================
-- PROCEDURE: get_cant_empleados_por_fecha
-- Descripción: Obtiene la cantidad de empleados que
--              trabajan en la planilla por fecha.
-- Entradas:
--    - IN fecha_nomina DATE: fecha de la planilla.
-- 
-- ===================================================
CREATE PROCEDURE get_cant_empleados_por_fecha(IN fecha_nomina DATE)
BEGIN
  SELECT COUNT(*) FROM PlanillasXEmpleados 
  INNER JOIN Planillas Pla ON PlanillasXEmpleados.id_planilla = Pla.id
  WHERE Pla.fecha_nomina = fecha_nomina;
END $$

-- ===================================================
-- PROCEDURE: create_planXemp
-- Descripción: Crea una relacion entre una planilla y
--              un empleado.
-- Entradas:
--    - IN id_planilla INT: id de la planilla.
--    - IN cedula_empleado VARCHAR(50): cedula del empleado.
-- 
-- ===================================================
CREATE PROCEDURE create_planXemp(IN id_planilla INT, IN cedula_empleado VARCHAR(50))
BEGIN
  INSERT INTO PlanillasXEmpleados (id_planilla, cedula_empleado)
    VALUES (id_planilla, cedula_empleado);
END $$

-- ===================================================
-- PROCEDURE: create_planXemp_fecha
-- Descripción: Crea una relacion entre una planilla y
--              un empleado por fecha.
-- Entradas:
--    - IN fecha_nomina DATE: fecha de la planilla.
--    - IN cedula_empleado VARCHAR(50): cedula del empleado.
-- 
-- ===================================================
CREATE PROCEDURE create_planXemp_fecha(IN pFecha_nomina DATE, IN cedula_empleado VARCHAR(50))
BEGIN
  DECLARE id_planilla INT;
  SELECT id INTO id_planilla FROM Planillas WHERE fecha_nomina = pFecha_nomina;
  INSERT INTO PlanillasXEmpleados (id_planilla, cedula_empleado)
    VALUES (id_planilla, cedula_empleado);
END $$

-- ===================================================
-- PROCEDURE: remove_planXemp_fecha
-- Descripción: Elimina una relacion entre una planilla y
--              un empleado por fecha.
-- Entradas:
--    - IN fecha_nomina DATE: fecha de la planilla.
--    - IN cedula_empleado VARCHAR(50): cedula del empleado.
-- 
-- ===================================================
CREATE PROCEDURE remove_planXemp_fecha(IN p_fecha DATE, IN p_cedula VARCHAR(50))
BEGIN
  DECLARE id_planilla INT;
  SELECT id INTO id_planilla FROM Planillas WHERE fecha_nomina = p_fecha;
  DELETE FROM PlanillasXEmpleados 
  WHERE id_planilla = id_planilla AND cedula_empleado = p_cedula;
END $$

-- ===================================================
-- PROCEDURE: remove_planXemp
-- Descripción: Elimina una relacion entre una planilla y
--              un empleado.
-- Entradas:
--    - IN id_planilla INT: id de la planilla.
--    - IN cedula_empleado VARCHAR(50): cedula del empleado.
-- 
-- ===================================================
CREATE PROCEDURE remove_planXemp(IN id_planilla INT, IN cedula_empleado VARCHAR(50))
BEGIN
  DELETE FROM PlanillasXEmpleados 
  WHERE id_planilla = id_planilla AND cedula_empleado = cedula_empleado;
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
  id_area int NOT NULL,
  cedula_comercio VARCHAR(50) NOT NULL,
  subtotal DOUBLE(15,2) NOT NULL,
  impuesto DOUBLE(15,2) NOT NULL,
  total DOUBLE(15,2) NOT NULL,
  CONSTRAINT pk_facturas PRIMARY KEY (id),
  CONSTRAINT fk_fact_comercio FOREIGN KEY (cedula_comercio)
    REFERENCES Comercios(cedula) ON DELETE CASCADE,
  CONSTRAINT fk_fact_area FOREIGN KEY (id_area)
    REFERENCES Areas(id) ON DELETE CASCADE
);

DELIMITER $$

-- ===================================================
-- PROCEDURE: create_factura
-- Descripción: Crea una factura.
-- Entradas:
--    - IN nombre_cliente VARCHAR(50): nombre del cliente.
--    - IN fecha_facturacion DATE: fecha de la factura.
--    - IN id_area INT: id de la area.
--    - IN cedula_comercio VARCHAR(50): cedula del comercio.
--    - IN subtotal DOUBLE(15,2): subtotal de la factura.
--    - IN impuesto DOUBLE(15,2): impuesto de la factura.
--    - IN total DOUBLE(15,2): total de la factura.
-- 
-- ===================================================
CREATE PROCEDURE create_factura(
    IN nombre_cliente VARCHAR(50), IN fecha_facturacion DATE, 
    IN id_area INT, IN cedula_comercio VARCHAR(50), 
    IN subtotal DOUBLE(15,2), IN impuesto DOUBLE(15,2), 
    IN total DOUBLE(15,2)
)
BEGIN
  INSERT INTO Facturas (nombre_cliente, fecha_facturacion, id_area, 
	cedula_comercio, subtotal, impuesto, total)
    VALUES (nombre_cliente, fecha_facturacion, id_area, 
    cedula_comercio, subtotal, impuesto, total);

  UPDATE Comercios SET num_sig_factura = num_sig_factura + 1 
  WHERE cedula = cedula_comercio;
  SET @id_factura = LAST_INSERT_ID();
  SELECT @id_factura;
END $$

-- ===================================================
-- PROCEDURE: get_factura_por_id
-- Descripción: Obtiene una factura por su id.
-- Entradas:
--    - IN id INT: id de la factura.
-- 
-- ===================================================
CREATE PROCEDURE get_factura_por_id(IN id INT)
BEGIN
  SELECT Com.nombre, Com.cedula, Com.telefono,
         Fac.nombre_cliente, Fac.fecha_facturacion,
         Area.nombre, Fac.id_area, Fac.subtotal, Fac.impuesto, Fac.total
  FROM Facturas Fac
  INNER JOIN Comercios Com ON Fac.cedula_comercio = Com.cedula
  INNER JOIN Areas Area ON Fac.id_area = Area.id
  WHERE Fac.id = id;
END $$

-- ===================================================
-- PROCEDURE: get_facturas_por_anio
-- Descripción: Obtiene las facturas de un comercio
--              por año.
-- Entradas:
--    - IN anio INT: año de las facturas.
-- 
-- ===================================================
CREATE PROCEDURE get_facturas_por_anio(IN anio INT)
BEGIN
  SELECT Com.nombre, Com.cedula, Com.telefono,
         Fac.id, Fac.nombre_cliente, Fac.fecha_facturacion,
         Area.nombre, Fac.subtotal, Fac.impuesto, Fac.total
  FROM Facturas Fac
  INNER JOIN Comercios Com ON Fac.cedula_comercio = Com.cedula
  INNER JOIN Areas Area ON Fac.id_area = Area.id
  WHERE YEAR(Fac.fecha_facturacion) = anio;
END $$

-- ===================================================
-- PROCEDURE: get_facturas
-- Descripción: Obtiene todas las facturas de un comercio.
-- Entradas:
--    - IN p_ced_comercio VARCHAR(50): cedula del comercio.
-- 
-- ===================================================
CREATE PROCEDURE get_facturas(IN p_ced_comercio VARCHAR(50))
BEGIN
  SELECT Com.nombre, Com.cedula, Com.telefono,
         Fac.id, Fac.nombre_cliente, Fac.fecha_facturacion,
         Area.nombre, Fac.subtotal, Fac.impuesto, Fac.total
    FROM Facturas Fac
    INNER JOIN Comercios Com ON Fac.cedula_comercio = Com.cedula
    INNER JOIN Areas Area ON Fac.id_area = Area.id
    WHERE Fac.cedula_comercio = p_ced_comercio;    
END$$

-- ===================================================
-- get_cantidad_facturas
-- Descripción: Obtiene la cantidad de facturas de un comercio.
-- Entradas:
--    - IN p_ced_comercio VARCHAR(50): cedula del comercio.
--
-- ===================================================
CREATE PROCEDURE get_cantidad_facturas(IN p_ced_comercio VARCHAR(50))
BEGIN
  SELECT COUNT(*) FROM Facturas
  WHERE cedula_comercio = p_ced_comercio;
END$$

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
    REFERENCES Facturas(id) ON DELETE RESTRICT,
  CONSTRAINT fk_factXdetprod_prod FOREIGN KEY (id_producto)
    REFERENCES Productos(id) ON DELETE RESTRICT,
  CONSTRAINT uq_factXdetprod UNIQUE (id_producto, id_factura)
);

DELIMITER $$

-- ===================================================
-- PROCEDURE: get_cantidad_detalles_factura
-- Descripción: Obtiene la cantidad de detalles de una
-- factura.
-- Entradas:
--    - IN id_factura INT: id de la factura.
-- 
-- ===================================================
CREATE PROCEDURE get_cantidad_detalles_factura(IN pIdFactura INT)
BEGIN
  SELECT COUNT(*) FROM FacturasXDetalleProducto
  WHERE id_factura = pIdFactura;
END $$

-- ===================================================
-- PROCEDURE: get_factXdetprod
-- Descripción: Obtiene los detalles de una factura.
-- Entradas:
--    - IN id_factura INT: id de la factura.
-- 
-- ===================================================
CREATE PROCEDURE get_factXdetprod(IN id_factura INT)
BEGIN
  SELECT Prod.id, Prod.nombre, Prod.costo,
         Prod.impuesto, FacXdet.cantidad
  FROM FacturasXDetalleProducto FacXdet
  INNER JOIN Productos Prod ON FacXdet.id_producto = Prod.id
  WHERE FacXdet.id_factura = id_factura;
END $$

-- ===================================================
-- PROCEDURE: create_factXdetprod
-- Descripción: Crea un detalle de una factura.
-- Entradas:
--    - IN id_producto VARCHAR(50): id del producto.
--    - IN id_factura INT: id de la factura.
--    - IN cantidad INT: cantidad del producto.
-- 
-- ===================================================
CREATE PROCEDURE create_factXdetprod(
  IN id_producto VARCHAR(50), 
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
