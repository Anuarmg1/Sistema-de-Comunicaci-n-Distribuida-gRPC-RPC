const MAX_STR = 100;
const MAX_MSG = 200;

struct alumno {
    int id;
    string nombre<MAX_STR>;
    string apellido<MAX_STR>;
    int edad;
    string curso<MAX_STR>;
    string direccion<MAX_STR>;
};

struct busqueda {
    int id;
    string nombre<MAX_STR>;
    string curso<MAX_STR>;
};

struct resultado_busqueda {
    bool encontrado;
    alumno datos;
    string mensaje<MAX_MSG>;
};

program REGISTROALUMNOS {
    version REGISTROALUMNOS_V1 {
        bool registrar_alumno(alumno) = 1;
        resultado_busqueda buscar_alumno(busqueda) = 2;
        bool actualizar_alumno(alumno) = 3;
        bool eliminar_alumno(int) = 4;
    } = 1;
} = 0x20000001;
