#include "registro_alumno.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCHIVO "alumnos.txt"
#define TEMP "alumnos_temp.txt"

bool_t *
registrar_alumno_1_svc(alumno *argp, struct svc_req *rqstp)
{
    static bool_t resultado;
    FILE *fp = fopen(ARCHIVO, "a");

    if (fp == NULL) {
        resultado = FALSE;
        return &resultado;
    }

    fprintf(fp, "%d|%s|%s|%d|%s|%s\n",
            argp->id,
            argp->nombre,
            argp->apellido,
            argp->edad,
            argp->curso,
            argp->direccion);

    fclose(fp);

    resultado = TRUE;
    return &resultado;
}

resultado_busqueda *
buscar_alumno_1_svc(busqueda *argp, struct svc_req *rqstp)
{
    static resultado_busqueda resultado;
    static alumno alumno_encontrado;

    static char nombre[100];
    static char apellido[100];
    static char curso[100];
    static char direccion[100];
    static char mensaje[200];

    FILE *fp = fopen(ARCHIVO, "r");

    resultado.encontrado = FALSE;

    alumno_encontrado.id = 0;
    alumno_encontrado.nombre = "";
    alumno_encontrado.apellido = "";
    alumno_encontrado.edad = 0;
    alumno_encontrado.curso = "";
    alumno_encontrado.direccion = "";

    strcpy(mensaje, "Alumno no encontrado");

    if (fp == NULL) {
        strcpy(mensaje, "No se pudo abrir el archivo de alumnos");
        resultado.datos = alumno_encontrado;
        resultado.mensaje = mensaje;
        return &resultado;
    }

    char linea[600];

    while (fgets(linea, sizeof(linea), fp)) {
        int id;
        int edad;

        char copia[600];
        strcpy(copia, linea);

        char *token = strtok(copia, "|");
        if (token == NULL) continue;
        id = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(nombre, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(apellido, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        edad = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(curso, token);

        token = strtok(NULL, "\n");
        if (token == NULL) continue;
        strcpy(direccion, token);

        int coincide = 0;

        if (argp->id != 0 && id == argp->id) {
            coincide = 1;
        }

        if (strlen(argp->nombre) > 0 && strcmp(nombre, argp->nombre) == 0) {
            coincide = 1;
        }

        if (strlen(argp->curso) > 0 && strcmp(curso, argp->curso) == 0) {
            coincide = 1;
        }

        if (coincide) {
            alumno_encontrado.id = id;
            alumno_encontrado.nombre = nombre;
            alumno_encontrado.apellido = apellido;
            alumno_encontrado.edad = edad;
            alumno_encontrado.curso = curso;
            alumno_encontrado.direccion = direccion;

            resultado.encontrado = TRUE;
            resultado.datos = alumno_encontrado;

            strcpy(mensaje, "Alumno encontrado");
            resultado.mensaje = mensaje;

            fclose(fp);
            return &resultado;
        }
    }

    fclose(fp);

    resultado.datos = alumno_encontrado;
    resultado.mensaje = mensaje;

    return &resultado;
}

bool_t *
actualizar_alumno_1_svc(alumno *argp, struct svc_req *rqstp)
{
    static bool_t resultado;
    resultado = FALSE;

    FILE *fp = fopen(ARCHIVO, "r");
    FILE *temp = fopen(TEMP, "w");

    if (fp == NULL || temp == NULL) {
        if (fp != NULL) fclose(fp);
        if (temp != NULL) fclose(temp);
        return &resultado;
    }

    char linea[600];

    while (fgets(linea, sizeof(linea), fp)) {
        int id;
        int edad;
        char nombre[100];
        char apellido[100];
        char curso[100];
        char direccion[100];

        char copia[600];
        strcpy(copia, linea);

        char *token = strtok(copia, "|");
        if (token == NULL) continue;
        id = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(nombre, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(apellido, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        edad = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(curso, token);

        token = strtok(NULL, "\n");
        if (token == NULL) continue;
        strcpy(direccion, token);

        if (id == argp->id) {
            fprintf(temp, "%d|%s|%s|%d|%s|%s\n",
                    argp->id,
                    argp->nombre,
                    argp->apellido,
                    argp->edad,
                    argp->curso,
                    argp->direccion);
            resultado = TRUE;
        } else {
            fprintf(temp, "%d|%s|%s|%d|%s|%s\n",
                    id,
                    nombre,
                    apellido,
                    edad,
                    curso,
                    direccion);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(ARCHIVO);
    rename(TEMP, ARCHIVO);

    return &resultado;
}

bool_t *
eliminar_alumno_1_svc(int *argp, struct svc_req *rqstp)
{
    static bool_t resultado;
    resultado = FALSE;

    FILE *fp = fopen(ARCHIVO, "r");
    FILE *temp = fopen(TEMP, "w");

    if (fp == NULL || temp == NULL) {
        if (fp != NULL) fclose(fp);
        if (temp != NULL) fclose(temp);
        return &resultado;
    }

    char linea[600];

    while (fgets(linea, sizeof(linea), fp)) {
        int id;

        char copia[600];
        strcpy(copia, linea);

        char *token = strtok(copia, "|");
        if (token == NULL) continue;

        id = atoi(token);

        if (id == *argp) {
            resultado = TRUE;
        } else {
            fputs(linea, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(ARCHIVO);
    rename(TEMP, ARCHIVO);

    return &resultado;
}
