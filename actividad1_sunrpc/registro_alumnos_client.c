#include "registro_alumno.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void limpiar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void registrar(CLIENT *clnt)
{
    alumno a;
    bool_t *resultado;

    char nombre[100];
    char apellido[100];
    char curso[100];
    char direccion[100];

    printf("\n--- Registrar alumno ---\n");

    printf("ID: ");
    scanf("%d", &a.id);
    limpiar_buffer();

    printf("Nombre: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = 0;

    printf("Apellido: ");
    fgets(apellido, sizeof(apellido), stdin);
    apellido[strcspn(apellido, "\n")] = 0;

    printf("Edad: ");
    scanf("%d", &a.edad);
    limpiar_buffer();

    printf("Curso: ");
    fgets(curso, sizeof(curso), stdin);
    curso[strcspn(curso, "\n")] = 0;

    printf("Direccion: ");
    fgets(direccion, sizeof(direccion), stdin);
    direccion[strcspn(direccion, "\n")] = 0;

    a.nombre = nombre;
    a.apellido = apellido;
    a.curso = curso;
    a.direccion = direccion;

    resultado = registrar_alumno_1(&a, clnt);

    if (resultado == NULL) {
        clnt_perror(clnt, "Error en llamada RPC");
        return;
    }

    if (*resultado) {
        printf("Alumno registrado correctamente.\n");
    } else {
        printf("No se pudo registrar el alumno.\n");
    }
}

void buscar(CLIENT *clnt)
{
    busqueda b;
    resultado_busqueda *resultado;

    char nombre[100];
    char curso[100];

    printf("\n--- Buscar alumno ---\n");

    printf("ID a buscar, escribe 0 si no buscaras por ID: ");
    scanf("%d", &b.id);
    limpiar_buffer();

    printf("Nombre a buscar, deja vacio si no buscaras por nombre: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = 0;

    printf("Curso a buscar, deja vacio si no buscaras por curso: ");
    fgets(curso, sizeof(curso), stdin);
    curso[strcspn(curso, "\n")] = 0;

    b.nombre = nombre;
    b.curso = curso;

    resultado = buscar_alumno_1(&b, clnt);

    if (resultado == NULL) {
        clnt_perror(clnt, "Error en llamada RPC");
        return;
    }

    printf("\nMensaje: %s\n", resultado->mensaje);

    if (resultado->encontrado) {
        printf("ID: %d\n", resultado->datos.id);
        printf("Nombre: %s\n", resultado->datos.nombre);
        printf("Apellido: %s\n", resultado->datos.apellido);
        printf("Edad: %d\n", resultado->datos.edad);
        printf("Curso: %s\n", resultado->datos.curso);
        printf("Direccion: %s\n", resultado->datos.direccion);
    }
}

void actualizar(CLIENT *clnt)
{
    alumno a;
    bool_t *resultado;

    char nombre[100];
    char apellido[100];
    char curso[100];
    char direccion[100];

    printf("\n--- Actualizar alumno ---\n");

    printf("ID del alumno a actualizar: ");
    scanf("%d", &a.id);
    limpiar_buffer();

    printf("Nuevo nombre: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = 0;

    printf("Nuevo apellido: ");
    fgets(apellido, sizeof(apellido), stdin);
    apellido[strcspn(apellido, "\n")] = 0;

    printf("Nueva edad: ");
    scanf("%d", &a.edad);
    limpiar_buffer();

    printf("Nuevo curso: ");
    fgets(curso, sizeof(curso), stdin);
    curso[strcspn(curso, "\n")] = 0;

    printf("Nueva direccion: ");
    fgets(direccion, sizeof(direccion), stdin);
    direccion[strcspn(direccion, "\n")] = 0;

    a.nombre = nombre;
    a.apellido = apellido;
    a.curso = curso;
    a.direccion = direccion;

    resultado = actualizar_alumno_1(&a, clnt);

    if (resultado == NULL) {
        clnt_perror(clnt, "Error en llamada RPC");
        return;
    }

    if (*resultado) {
        printf("Alumno actualizado correctamente.\n");
    } else {
        printf("No se encontro el alumno.\n");
    }
}

void eliminar(CLIENT *clnt)
{
    int id;
    bool_t *resultado;

    printf("\n--- Eliminar alumno ---\n");

    printf("ID del alumno a eliminar: ");
    scanf("%d", &id);
    limpiar_buffer();

    resultado = eliminar_alumno_1(&id, clnt);

    if (resultado == NULL) {
        clnt_perror(clnt, "Error en llamada RPC");
        return;
    }

    if (*resultado) {
        printf("Alumno eliminado correctamente.\n");
    } else {
        printf("No se encontro el alumno.\n");
    }
}

int main(int argc, char *argv[])
{
    CLIENT *clnt;
    char *host;
    int opcion;

    if (argc < 2) {
        printf("Uso: %s servidor\n", argv[0]);
        printf("Ejemplo: %s localhost\n", argv[0]);
        exit(1);
    }

    host = argv[1];

    clnt = clnt_create(host, REGISTROALUMNOS, REGISTROALUMNOS_V1, "udp");

    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    do {
        printf("\n==============================\n");
        printf(" SISTEMA RPC REGISTRO ALUMNOS\n");
        printf("==============================\n");
        printf("1. Registrar alumno\n");
        printf("2. Buscar alumno\n");
        printf("3. Actualizar alumno\n");
        printf("4. Eliminar alumno\n");
        printf("0. Salir\n");
        printf("Opcion: ");

        scanf("%d", &opcion);
        limpiar_buffer();

        switch (opcion) {
            case 1:
                registrar(clnt);
                break;
            case 2:
                buscar(clnt);
                break;
            case 3:
                actualizar(clnt);
                break;
            case 4:
                eliminar(clnt);
                break;
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion no valida.\n");
        }

    } while (opcion != 0);

    clnt_destroy(clnt);

    return 0;
}
