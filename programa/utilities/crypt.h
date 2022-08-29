/*****Datos administrativos*******************************
* Nombre del archivo: crypt
* Tipo de archivo: archivo de encabezado (.h)
* Proyecto: Sistema de producción agrícola
* Autor: programmingalgorithms.com
* source: www.programmingalgorithms.com/algorithm/xor-encryption/c
* Empresa: Instituto Tecnológico de Costa Rica
*****Descripción******************************************
* Archivo para la encriptación de datos.
*****Versión**********************************************
* ## | Fecha y hora | Autor
* 15/08/2022
**********************************************************/

// NOTA: NO FUE DISEÑADA POR ESTUDIANTES, REVISAR ENCABEZADO.

#ifndef CRYPT_H
#define CRYPT_H

#include <string.h>
#include <stdio.h>

static const char key[] = "hans";

/**
 * @brief Encripta una cadena de texto.
 * @param text La cadena de texto a encriptar.
 * @return La cadena de texto encriptada.
 */
char* Encriptar(char* text) {
    char* encrypted = (char*) malloc(sizeof(char) * (strlen(text) + 1));
    int i;
    for (i = 0; i < strlen(text); i++) {
        encrypted[i] = text[i] ^ key[i % strlen(key)];
    }
    encrypted[i] = '\0';
    return encrypted;
}

/**
 * @brief Desencripta una cadena de texto.
 * 
 * @param text La cadena de texto a desencriptar.
 * @return char* La cadena de texto desencriptada.
 */
char* Desencriptar(char* text) {
    char* decrypted = (char*) malloc(sizeof(char) * (strlen(text) + 1));
    int i;
    for (i = 0; i < strlen(text); i++) {
        decrypted[i] = text[i] ^ key[i % strlen(key)];
    }
    decrypted[i] = '\0';
    return decrypted;
}

#endif // CRYPT_H