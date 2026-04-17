#include"utils.h"


t_log* logger;

int iniciar_servidor(void)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	//assert(!"no implementado!");
    int err;
	struct addrinfo hints, *server_info, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(NULL, PUERTO, &hints, &server_info);

	// Creamos el socket de escucha del servidor
        // TODO

    int socket_servidor = socket(server_info->ai_family,
                         server_info->ai_socktype,
                         server_info->ai_protocol);

        //aca calculo que tendria que poner "int fd_escucha = socket(etc etc etc...)"
        //PD: fd_escucha no me gusta para nada, cambiarlo
   
    // ANTES de asociar con BIND, uso setsockport: REUSEPORT para que el SO me permita "reutilizar" puertos
    err = setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));
   
	// Asociamos el socket a un puerto CON BIND()
    err = bind(socket_servidor, server_info->ai_addr, server_info->ai_addrlen);

	// Escuchamos las conexiones entrantes CON LISTEN() (creo)
    err = listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(server_info);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	//assert(!"no implementado!");
	log_trace(logger, "Esperando cliente...\n");
    
	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL); //Una vez que el cliente fue aceptado, accept() retorna un nuevo socket (file descriptor) que representa la conexión BIDIRECCIONAL entre ambos procesos.

                                                                //Esto quiere decir nuestro server_fd no va a ser el que participe de dicha comunicación, solamente tiene la responsabilidad de quedarse escuchando nuevas conexiones y aceptarlas.
    
	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
