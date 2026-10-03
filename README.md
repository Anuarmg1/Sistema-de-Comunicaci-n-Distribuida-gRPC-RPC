# Proyecto RPC y gRPC - Registro Distribuido de Alumnos

Este proyecto contiene dos implementaciones distribuidas para administrar alumnos en un archivo:

- **Actividad 1:** Sun RPC en C usando XDR, `rpcgen`, cliente y servidor.
- **Actividad 2:** gRPC en Python usando Protocol Buffers, cliente y servidor con streaming.

## Estructura

```text
proyecto_rpc_grpc/
├── actividad1_sunrpc/
│   ├── registro_alumnos.x
│   ├── registro_alumnos_server.c
│   ├── registro_alumnos_client.c
│   └── Makefile
├── actividad2_grpc/
│   ├── alumnos.proto
│   ├── server.py
│   ├── client.py
│   ├── requirements.txt
│   ├── generar_stubs.sh
│   └── alumnos.json
└── docs/
    ├── DOCUMENTACION.md
    └── GUIA_VIDEO.md
```

---

# Actividad 1: Sun RPC en C

## Requisitos

En Linux/Ubuntu:

```bash
sudo apt update
sudo apt install rpcbind rpcsvc-proto libtirpc-dev build-essential
```

## Compilar

```bash
cd actividad1_sunrpc
make
```

## Ejecutar servidor

Terminal 1:

```bash
sudo service rpcbind start
./registro_alumnos_server
```

## Ejecutar clientes

Terminal 2:

```bash
./registro_alumnos_client localhost
```

Para probar distribución real, cambia `localhost` por la IP del servidor:

```bash
./registro_alumnos_client 192.168.1.50
```

## Operaciones disponibles

1. Registrar alumno
2. Buscar alumno por ID, nombre o curso
3. Actualizar alumno por ID
4. Eliminar alumno por ID

El servidor guarda los registros en:

```text
alumnos_rpc.txt
```

---

# Actividad 2: gRPC en Python

## Crear entorno virtual

```bash
cd actividad2_grpc
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

En Windows PowerShell:

```powershell
python -m venv venv
.\venv\Scripts\Activate.ps1
pip install -r requirements.txt
```

## Generar archivos gRPC desde el .proto

```bash
./generar_stubs.sh
```

O manualmente:

```bash
python -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. alumnos.proto
```

Esto genera:

```text
alumnos_pb2.py
alumnos_pb2_grpc.py
```

## Ejecutar servidor

Terminal 1:

```bash
python server.py
```

## Ejecutar clientes

Terminal 2:

```bash
python client.py localhost:50051
```

Terminal 3:

```bash
python client.py localhost:50051
```

En otra computadora de la misma red:

```bash
python client.py IP_DEL_SERVIDOR:50051
```

Ejemplo:

```bash
python client.py 192.168.1.50:50051
```

## Operaciones disponibles

1. Registrar alumno
2. Consultar alumno por ID
3. Actualizar alumno
4. Eliminar alumno
5. Buscar por criterios usando server streaming
6. Listar todos los alumnos usando server streaming

El servidor gRPC guarda los registros en:

```text
alumnos.json
```

---

# Prueba de múltiples clientes

Para demostrar distribución:

1. Ejecuta el servidor.
2. Abre dos o tres terminales con clientes diferentes.
3. En un cliente registra alumnos.
4. En otro cliente consulta o lista los alumnos.
5. En otro cliente actualiza o elimina.
6. Muestra que todos acceden al mismo archivo del servidor.

---

# Limpieza

Sun RPC:

```bash
cd actividad1_sunrpc
make clean
```

gRPC:

```bash
cd actividad2_grpc
rm -f alumnos_pb2.py alumnos_pb2_grpc.py
```
