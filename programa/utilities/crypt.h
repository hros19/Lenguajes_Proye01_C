#ifndef CRYPT_H
#define CRYPT_H

#include <string.h>
#include <stdio.h>

void Encriptar(const char *clave, char *clave_encriptada) {
    int i;
    for (i = 0; i < strlen(clave); i++) {
        clave_encriptada[i] = clave[i] + 0xAED;
    }
}

void Desencriptar(const char *clave_encriptada, char *clave_desencriptada) {
    int i;
    for (i = 0; i < strlen(clave_encriptada); i++) {
        clave_desencriptada[i] = clave_encriptada[i] - 0xAED;
    }
}

#endif // CRYPT_H