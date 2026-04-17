#include "client.h"
//#include <readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

    
	logger = iniciar_logger();

    log_trace(logger, "soy un mensaje de log de trace");
    log_debug(logger, "soy un mensaje de log de debug");
    log_info(logger, "soy un mensaje de log de info");
    log_warning(logger, "soy un mensaje de log de warning");
    log_error(logger, "soy un mensaje de log de error");

    
	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */
    
	config = iniciar_config();
    
        // Usando el config creado previamente, leemos los valores del config y los 
        // dejamos en las variables 'ip', 'puerto' y 'valor'
    
        valor = config_get_string_value(config, "CLAVE");
        ip = config_get_string_value(config, "IP");
        puerto = config_get_string_value(config, "PUERTO");
        
        
        // Loggeamos el valor de config
    log_info(logger, valor);
    log_info(logger, ip);
    log_info(logger, puerto);





	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto); //el hecho de crear SOCKET "CONEXION" es literalmente "crear el enchufe macho, de X aparato"
                                            //y el "crear SOCKET ESCUCHA" es "crear el enchufe HEMBRA (o el tomacorriente) de la pared"


	// Enviamos al servidor el valor de CLAVE como mensaje

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0Client.log","client.exe", true, LOG_LEVEL_INFO);

    if (nuevo_logger == NULL) {
        printf("¡No se pudo crear el logger!"); 
        abort();
    }
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");

    if (nuevo_config == NULL) {
            printf("¡No se pudo crear el config!"); 
            abort();
    }
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* lineaDeEntrada;

    while (1)
    {
        lineaDeEntrada = readline("> ");

        // Controlar si el usuario envió EOF (ej. Ctrl+D)
        if (lineaDeEntrada == NULL) {
            break;
        }

        // Controlar si se ingresó un string vacío para salir
        if (strcmp(lineaDeEntrada, "") == 0) {
            free(lineaDeEntrada); // Liberar el string vacío antes de romper el ciclo
            break;
        }

        // Procesar y loguear la línea
        log_info(logger, lineaDeEntrada);

        // Liberar la memoria de la iteración actual antes de la siguiente lectura
        free(lineaDeEntrada);
    }


	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
    /* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
    con las funciones de las commons y del TP mencionadas en el enunciado */
    log_destroy(logger);
    config_destroy(config);
}
