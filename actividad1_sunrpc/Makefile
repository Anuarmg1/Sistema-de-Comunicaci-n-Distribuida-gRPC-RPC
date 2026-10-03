CC = gcc
CFLAGS = -Wall -g

SERVER = registro_alumnos_server
CLIENT = registro_alumnos_client

all: $(SERVER) $(CLIENT)

$(SERVER): registro_alumnos_server.o registro_alumno_svc.o registro_alumno_xdr.o
	$(CC) $(CFLAGS) -o $(SERVER) registro_alumnos_server.o registro_alumno_svc.o registro_alumno_xdr.o -ltirpc

$(CLIENT): registro_alumnos_client.o registro_alumno_clnt.o registro_alumno_xdr.o
	$(CC) $(CFLAGS) -o $(CLIENT) registro_alumnos_client.o registro_alumno_clnt.o registro_alumno_xdr.o -ltirpc

%.o: %.c
	$(CC) $(CFLAGS) -I/usr/include/tirpc -c $<

clean:
	rm -f *.o $(SERVER) $(CLIENT)
