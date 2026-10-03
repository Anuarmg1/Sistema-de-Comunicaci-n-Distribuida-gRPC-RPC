from concurrent import futures
import grpc
import json
import os
import threading

import alumnos_pb2
import alumnos_pb2_grpc

ARCHIVO = "alumnos.json"
lock = threading.Lock()


def cargar_alumnos():
    if not os.path.exists(ARCHIVO):
        return []

    with open(ARCHIVO, "r", encoding="utf-8") as archivo:
        try:
            return json.load(archivo)
        except json.JSONDecodeError:
            return []


def guardar_alumnos(alumnos):
    with open(ARCHIVO, "w", encoding="utf-8") as archivo:
        json.dump(alumnos, archivo, indent=4, ensure_ascii=False)


def generar_id(alumnos):
    if not alumnos:
        return 1
    return max(alumno["id"] for alumno in alumnos) + 1


class RegistroAlumnosServicer(alumnos_pb2_grpc.RegistroAlumnosServicer):

    def RegistrarAlumno(self, request, context):
        with lock:
            alumnos = cargar_alumnos()
            nuevo_id = generar_id(alumnos)

            alumno = {
                "id": nuevo_id,
                "nombre": request.nombre,
                "apellido": request.apellido,
                "edad": request.edad,
                "curso": request.curso,
                "direccion": request.direccion
            }

            alumnos.append(alumno)
            guardar_alumnos(alumnos)

        return alumnos_pb2.Respuesta(
            exito=True,
            mensaje="Alumno registrado correctamente",
            id_generado=nuevo_id
        )

    def ConsultarAlumno(self, request, context):
        alumnos = cargar_alumnos()

        for alumno in alumnos:
            if alumno["id"] == request.id:
                return alumnos_pb2.AlumnoResponse(
                    id=alumno["id"],
                    nombre=alumno["nombre"],
                    apellido=alumno["apellido"],
                    edad=alumno["edad"],
                    curso=alumno["curso"],
                    direccion=alumno["direccion"],
                    encontrado=True,
                    mensaje="Alumno encontrado"
                )

        return alumnos_pb2.AlumnoResponse(
            encontrado=False,
            mensaje="Alumno no encontrado"
        )

    def ActualizarAlumno(self, request, context):
        with lock:
            alumnos = cargar_alumnos()

            for alumno in alumnos:
                if alumno["id"] == request.id:
                    alumno["nombre"] = request.nombre
                    alumno["apellido"] = request.apellido
                    alumno["edad"] = request.edad
                    alumno["curso"] = request.curso
                    alumno["direccion"] = request.direccion

                    guardar_alumnos(alumnos)

                    return alumnos_pb2.Respuesta(
                        exito=True,
                        mensaje="Alumno actualizado correctamente",
                        id_generado=request.id
                    )

        return alumnos_pb2.Respuesta(
            exito=False,
            mensaje="No se encontró el alumno",
            id_generado=0
        )

    def EliminarAlumno(self, request, context):
        with lock:
            alumnos = cargar_alumnos()

            nuevos_alumnos = [
                alumno for alumno in alumnos
                if alumno["id"] != request.id
            ]

            if len(nuevos_alumnos) == len(alumnos):
                return alumnos_pb2.Respuesta(
                    exito=False,
                    mensaje="No se encontró el alumno",
                    id_generado=0
                )

            guardar_alumnos(nuevos_alumnos)

        return alumnos_pb2.Respuesta(
            exito=True,
            mensaje="Alumno eliminado correctamente",
            id_generado=request.id
        )

    def BuscarPorCriterios(self, request, context):
        alumnos = cargar_alumnos()

        nombre = request.nombre.lower().strip()
        apellido = request.apellido.lower().strip()
        curso = request.curso.lower().strip()

        encontrados = 0

        for alumno in alumnos:
            coincide = True

            if nombre and nombre not in alumno["nombre"].lower():
                coincide = False

            if apellido and apellido not in alumno["apellido"].lower():
                coincide = False

            if curso and curso not in alumno["curso"].lower():
                coincide = False

            if coincide:
                encontrados += 1
                yield alumnos_pb2.AlumnoResponse(
                    id=alumno["id"],
                    nombre=alumno["nombre"],
                    apellido=alumno["apellido"],
                    edad=alumno["edad"],
                    curso=alumno["curso"],
                    direccion=alumno["direccion"],
                    encontrado=True,
                    mensaje="Resultado encontrado"
                )

        if encontrados == 0:
            yield alumnos_pb2.AlumnoResponse(
                encontrado=False,
                mensaje="No se encontraron alumnos con esos criterios"
            )

    def ListarAlumnos(self, request, context):
        alumnos = cargar_alumnos()

        if not alumnos:
            yield alumnos_pb2.AlumnoResponse(
                encontrado=False,
                mensaje="No hay alumnos registrados"
            )
            return

        for alumno in alumnos:
            yield alumnos_pb2.AlumnoResponse(
                id=alumno["id"],
                nombre=alumno["nombre"],
                apellido=alumno["apellido"],
                edad=alumno["edad"],
                curso=alumno["curso"],
                direccion=alumno["direccion"],
                encontrado=True,
                mensaje="Alumno listado"
            )


def iniciar_servidor():
    servidor = grpc.server(futures.ThreadPoolExecutor(max_workers=10))

    alumnos_pb2_grpc.add_RegistroAlumnosServicer_to_server(
        RegistroAlumnosServicer(),
        servidor
    )

    servidor.add_insecure_port("[::]:50051")
    servidor.start()

    print("Servidor gRPC iniciado en el puerto 50051")
    print("Esperando clientes...")

    servidor.wait_for_termination()


if __name__ == "__main__":
    iniciar_servidor()
