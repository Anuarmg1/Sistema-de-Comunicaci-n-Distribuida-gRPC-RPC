import grpc
import sys

import alumnos_pb2
import alumnos_pb2_grpc


def pedir_entero(mensaje):
    while True:
        try:
            return int(input(mensaje))
        except ValueError:
            print("Ingresa un número válido.")


def imprimir_alumno(alumno):
    print("-----------------------------")
    print(f"ID: {alumno.id}")
    print(f"Nombre: {alumno.nombre}")
    print(f"Apellido: {alumno.apellido}")
    print(f"Edad: {alumno.edad}")
    print(f"Curso: {alumno.curso}")
    print(f"Dirección: {alumno.direccion}")
    print("-----------------------------")


def registrar(stub):
    print("\n--- Registrar alumno ---")

    nombre = input("Nombre: ")
    apellido = input("Apellido: ")
    edad = pedir_entero("Edad: ")
    curso = input("Curso: ")
    direccion = input("Dirección: ")

    request = alumnos_pb2.AlumnoRequest(
        nombre=nombre,
        apellido=apellido,
        edad=edad,
        curso=curso,
        direccion=direccion
    )

    respuesta = stub.RegistrarAlumno(request)

    print(f"Éxito: {respuesta.exito}")
    print(f"Mensaje: {respuesta.mensaje}")
    print(f"ID generado: {respuesta.id_generado}")


def consultar(stub):
    print("\n--- Consultar alumno ---")

    id_alumno = pedir_entero("ID del alumno: ")

    request = alumnos_pb2.ConsultaRequest(id=id_alumno)
    respuesta = stub.ConsultarAlumno(request)

    print(f"Mensaje: {respuesta.mensaje}")

    if respuesta.encontrado:
        imprimir_alumno(respuesta)


def actualizar(stub):
    print("\n--- Actualizar alumno ---")

    id_alumno = pedir_entero("ID del alumno a actualizar: ")

    nombre = input("Nuevo nombre: ")
    apellido = input("Nuevo apellido: ")
    edad = pedir_entero("Nueva edad: ")
    curso = input("Nuevo curso: ")
    direccion = input("Nueva dirección: ")

    request = alumnos_pb2.AlumnoRequest(
        id=id_alumno,
        nombre=nombre,
        apellido=apellido,
        edad=edad,
        curso=curso,
        direccion=direccion
    )

    respuesta = stub.ActualizarAlumno(request)

    print(f"Éxito: {respuesta.exito}")
    print(f"Mensaje: {respuesta.mensaje}")


def eliminar(stub):
    print("\n--- Eliminar alumno ---")

    id_alumno = pedir_entero("ID del alumno a eliminar: ")

    request = alumnos_pb2.ConsultaRequest(id=id_alumno)
    respuesta = stub.EliminarAlumno(request)

    print(f"Éxito: {respuesta.exito}")
    print(f"Mensaje: {respuesta.mensaje}")


def buscar_por_criterios(stub):
    print("\n--- Buscar por criterios streaming ---")
    print("Puedes dejar campos vacíos si no quieres usarlos.")

    nombre = input("Nombre: ")
    apellido = input("Apellido: ")
    curso = input("Curso: ")

    request = alumnos_pb2.BusquedaRequest(
        nombre=nombre,
        apellido=apellido,
        curso=curso
    )

    print("\nResultados recibidos del servidor:")

    respuestas = stub.BuscarPorCriterios(request)

    for alumno in respuestas:
        if alumno.encontrado:
            imprimir_alumno(alumno)
        else:
            print(alumno.mensaje)


def listar_alumnos(stub):
    print("\n--- Listar todos los alumnos streaming ---")

    respuestas = stub.ListarAlumnos(alumnos_pb2.Vacio())

    for alumno in respuestas:
        if alumno.encontrado:
            imprimir_alumno(alumno)
        else:
            print(alumno.mensaje)


def menu(stub):
    while True:
        print("\n==============================")
        print(" SISTEMA gRPC REGISTRO ALUMNOS")
        print("==============================")
        print("1. Registrar alumno")
        print("2. Consultar alumno por ID")
        print("3. Actualizar alumno")
        print("4. Eliminar alumno")
        print("5. Buscar por criterios streaming")
        print("6. Listar todos streaming")
        print("0. Salir")

        opcion = input("Opción: ")

        if opcion == "1":
            registrar(stub)
        elif opcion == "2":
            consultar(stub)
        elif opcion == "3":
            actualizar(stub)
        elif opcion == "4":
            eliminar(stub)
        elif opcion == "5":
            buscar_por_criterios(stub)
        elif opcion == "6":
            listar_alumnos(stub)
        elif opcion == "0":
            print("Saliendo...")
            break
        else:
            print("Opción no válida.")


def main():
    if len(sys.argv) > 1:
        servidor = sys.argv[1]
    else:
        servidor = "localhost:50051"

    canal = grpc.insecure_channel(servidor)
    stub = alumnos_pb2_grpc.RegistroAlumnosStub(canal)

    print(f"Conectado al servidor gRPC: {servidor}")
    menu(stub)


if __name__ == "__main__":
    main()
