#ifndef ASUTIL_H_INCLUDED
#define ASUTIL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    #include "argos.h"
    #include "json.h"

#ifdef __cplusplus
}
#endif // __cplusplus

#include "version.h"
#include <limits.h>
#include <syslog.h>

#include <cstring>
#include <sstream>

using namespace std;

/*--------------------------------------------------------
	Estructura de dato usada para encapsular el valor
	del tag, dependiendo del tipo.
----------------------------------------------------------*/
union Tvalor{
	bool B;             //!< 1 bit
	char C;             //!< 1 byte
	short S;            //!< 2 byte
	int I;              //!< 4 byte
//#ifdef _x86_64_
//    long L;             //!< 8 byte en Arquitectura de 64 bits
//#else /* !_x86_64_ */
//    long long L;        //!< 8 byte en Arquitectura de 32 bits
//#endif /* _x86_64_ */
	__int64_t L;
	float F;            //!< 4 byte
	double D;           //!< 8 byte

};

/*--------------------------------------------------------
	Tipos de datos y longitudes soportados por los
	tag.
----------------------------------------------------------*/
enum Tdato{
	BIT_,           //!< 1 bit
	CARAC_,         //!< 1 byte
	CORTO_,         //!< 2 byte
	ENTERO_,        //!< 4 byte
	LARGO_,         //!< 8 byte
	REAL_,          //!< 4 byte
	DREAL_,         //!< 8 byte
	TERROR_ = -1
};

/*--------------------------------------------------------
	Estados soportados por las alarmas
----------------------------------------------------------*/
enum Ealarma{
	BAJO_,			//!< Bajo
	BBAJO_,			//!< Bajo Bajo
	ALTO_,			//!< Alto
	AALTO_,			//!< Alto Alto
	ACTIVA_,		//!< Activa
	DESACT_,		//!< Desactiva
	RECON_,			//!< Reconocida
	EERROR_ = -1
};

/*--------------------------------------------------------
	Tipos de registro soportados, usados para crear
	los datos historicos.
----------------------------------------------------------*/
enum Tlog{
	MUESTREO_,		//!< Por muestreo o polling
	EVENTO_,		//!< Por cambio de valor
	LERROR_ = -1
};

/*--------------------------------------------------------
	Tipos de funciones soportados, usados para interpolar
	los datos historicos.
----------------------------------------------------------*/
enum Tfunciones{
	AVG_,			//!< Promedio
	STD_,			//!< Desviacion Estandar
	VAR_,			//!< Varianza
	PZ_,			//!< Punto Z
	NO_,			//!< No aplica
	FERROR_ = -1
};

/*--------------------------------------------------------
	Tipos de comparaciones soportados, usadas para
	asignar los estado de las alarmas.
----------------------------------------------------------*/
enum Tcomp{
	IGUAL_QUE_,			//!< ==
	MAYOR_QUE_,			//!< >
	MENOR_QUE_,			//!< <
	MAYOR_IGUAL_QUE_,	//!< >=
	MENOR_IGUAL_QUE_,	//!< <=
	AERROR_ = -1
};

/*--------------------------------------------------------
	Valor asignado al Tag en caso de error
----------------------------------------------------------*/
#ifdef _x86_64_
const long _ERR_DATO_               =   ULLONG_MAX - 1;     //!< 0xFFFFFFFFFFFFFFFE
#else /* !_x86_64_ */
const long long _ERR_DATO_          =   ULLONG_MAX - 1;     //!< 0xFFFFFFFFFFFFFFFE
#endif /* _x86_64_ */

const size_t _MAX_TAGS_X_MSG_		=	50;		//!< Cantidad de Tags x Mensaje
const size_t _MAX_TAM_ARR_TAGS_ 	=	500;	//!< Tamano del Arreglo de Tags
const size_t _LONG_NOMB_TAG_		=	48;		//!< Longitud del Nombre del Tag
const size_t _MAX_ALARM_X_MSG_  	=	50;		//!< Cantidad de Alarmas x Mensaje
const size_t _MAX_TAM_ARR_ALARM_ 	=	500;	//!< Tamano del Arreglo de Alarmas
const size_t _LONG_NOMB_ALARM_	    =	48;		//!< Longitud del Nombre de Alarma
const size_t _LONG_NOMB_GALARM_	    =	50;		//!< Longitud del Nombre de Grupo de Alarmas
const size_t _LONG_NOMB_SGALARM_	=	50;		//!< Longitud del Nombre de Subgrupo de Alarmas
const size_t _LONG_NOMB_REG_		=	100;	//!< Longitud del Nombre de Registro

const size_t _MAX_CANT_FUNCIONES_	=	10;		//!< Cantidad de Funciones para historicos
const size_t _MAX_CANT_MUESTRAS_	=	1000;	//!< Cantidad de Muestras para las Funciones
const size_t _MAX_TRAZA_XML_REQ_	=	4096;	//!< Documento XML de Solicitud de Tendencia

//const size_t _MAX_TAM_NOMB_FUNC_	=	10;
extern const char *_FUNCIONES_HISTORICOS_[];	//!< Funciones para Historicos

/*--------------------------------------------------------
	Bit de Comparacion
----------------------------------------------------------*/
//const __int64_t __BIT2HEX[64] = { 01/20/2022 0x8000000000000000LL is too big for __int64_t
const __uint64_t __BIT2HEX[64] = {
	0x0000000000000001LL,
	0x0000000000000002LL,
	0x0000000000000004LL,
	0x0000000000000008LL,
	0x0000000000000010LL,
	0x0000000000000020LL,
	0x0000000000000040LL,
	0x0000000000000080LL,
	0x0000000000000100LL,
	0x0000000000000200LL,
	0x0000000000000400LL,
	0x0000000000000800LL,
	0x0000000000001000LL,
	0x0000000000002000LL,
	0x0000000000004000LL,
	0x0000000000008000LL,
	0x0000000000010000LL,
	0x0000000000020000LL,
	0x0000000000040000LL,
	0x0000000000080000LL,
	0x0000000000100000LL,
	0x0000000000200000LL,
	0x0000000000400000LL,
	0x0000000000800000LL,
	0x0000000001000000LL,
	0x0000000002000000LL,
	0x0000000004000000LL,
	0x0000000008000000LL,
	0x0000000010000000LL,
	0x0000000020000000LL,
	0x0000000040000000LL,
	0x0000000080000000LL,
	0x0000000100000000LL,
	0x0000000200000000LL,
	0x0000000400000000LL,
	0x0000000800000000LL,
	0x0000001000000000LL,
	0x0000002000000000LL,
	0x0000004000000000LL,
	0x0000008000000000LL,
	0x0000010000000000LL,
	0x0000020000000000LL,
	0x0000040000000000LL,
	0x0000080000000000LL,
	0x0000100000000000LL,
	0x0000200000000000LL,
	0x0000400000000000LL,
	0x0000800000000000LL,
	0x0001000000000000LL,
	0x0002000000000000LL,
	0x0004000000000000LL,
	0x0008000000000000LL,
	0x0010000000000000LL,
	0x0020000000000000LL,
	0x0040000000000000LL,
	0x0080000000000000LL,
	0x0100000000000000LL,
	0x0200000000000000LL,
	0x0400000000000000LL,
	0x0800000000000000LL,
	0x1000000000000000LL,
	0x2000000000000000LL,
	0x4000000000000000LL,
	0x8000000000000000LL
};

typedef __uint64_t		Cmens;

const Cmens		_TIPO_ALARMA_ 			= 0x00F0000000000000ULL;	//!< eth islandesa minuscula
const Cmens		_TIPO_TAG_ 				= 0x00FD000000000000ULL;	//!< y minuscula con acento agudo
//const Cmens		_TRAZA_SHM_ 			= 0x00E000000000E000ULL;	//!< alplha
//const Cmens		_TRAZA_BD_ 				= 0x00ED00000000ED00ULL;	//!< epsilon
const Cmens		_TRAZA_SHM_ 			= 0x6162616161616261ULL;	//!< para probar con telnet abaaaaba
const Cmens		_TRAZA_BD_ 				= 0x6261626262626162ULL;	//!< para probar con telnet babbbbab
const int		_ESCR_OK_				= 0x0000F500;				//!< OK escritura
const int		_ESCR_ERROR_			= 0x0000F800;				//!< Error en escritura

/*--------------------------------------------------------
	Estructura del mensaje para el registro de
	datos historicos en Base de Datos
----------------------------------------------------------*/
typedef struct _Tmsgssbd{
	__uint64_t	tipo;								//!< Alarma o Tag
	union{
		char	nombre_tag[_LONG_NOMB_TAG_];		//!< Nombre del Tag si es el caso
		char	nombre_alarma[_LONG_NOMB_ALARM_];	//!< Nombre de la Alarma si es el caso
	};
	size_t      posicion;							//!< Posicion del Indice en el Buffer Circular
	size_t      desde;								//!< Posicion de Inicio en el Buffer Circular
	size_t      hasta;								//!< Posicion de Fin en el Buffer Circular
}Tmsgssbd;


/*--------------------------------------------------------
	Estructura del mensaje para la escritura de
	Tags o Alarmas en el Dispositivo
----------------------------------------------------------*/
typedef struct _Tmsgsswr{
	__uint64_t  tipo;								//!< Alarma o Tag
	union{
		char	nombre_tag[_LONG_NOMB_TAG_];		//!< Nombre del Tag si es el caso
		char	nombre_alarma[_LONG_NOMB_ALARM_];	//!< Nombre de la Alarma si es el caso
	};
	union{
		Tvalor	valor;								//!< Valor del Tag si es el caso
		Ealarma	estado;								//!< Estado de la Alarma si es el caso
	};
	Tdato	tipo_dato;								//!< Tipo del Dato Destino
}Tmsgsswr;

/*--------------------------------------------------------
	Tamano de la palabra de acuerdo a la Arquitectura
----------------------------------------------------------*/
#ifdef _x86_64_
typedef signed long palabra_t;						//!< Tamano palabra en 64 bits
#else /* !_x86_64_ */
typedef signed int palabra_t;						//!< Tamano palabra en 32 bits
#endif /* _x86_64_ */


/*--------------------------------------------------------
	Conversion de un string a un Tipo de
	Dato valido
----------------------------------------------------------*/
#define TEXTO2DATO(DATO,TEXTO) \
	if(0 == strcmp("ENTERO_", TEXTO)) \
		DATO = ENTERO_; \
    else if(0 == strcmp("CARAC_", TEXTO)) \
		DATO = CARAC_; \
    else if(0 == strcmp("CORTO_", TEXTO)) \
		DATO = CORTO_; \
    else if(0 == strcmp("LARGO_", TEXTO)) \
		DATO = LARGO_; \
	else if(0 == strcmp("REAL_", TEXTO)) \
		DATO = REAL_; \
	else if(0 == strcmp("DREAL_", TEXTO)) \
		DATO = DREAL_; \
	else if(0 == strcmp("BIT_", TEXTO)) \
		DATO = BIT_; \
	else \
		DATO = TERROR_


/*--------------------------------------------------------
	Conversion de un string a un Estado de
	Alarma valido
----------------------------------------------------------*/
#define TEXTO2ESTADO(ESTADO,TEXTO) \
	if(0 == strcmp("BAJO_", TEXTO)) \
		ESTADO = BAJO_; \
	else if(0 == strcmp("BBAJO_", TEXTO)) \
		ESTADO = BBAJO_; \
	else if(0 == strcmp("ALTO_", TEXTO)) \
		ESTADO = ALTO_; \
	else if(0 == strcmp("AALTO_", TEXTO)) \
		ESTADO = AALTO_; \
	else if(0 == strcmp("ACTIVA_", TEXTO)) \
		ESTADO = ACTIVA_; \
	else if(0 == strcmp("DESACT_", TEXTO)) \
		ESTADO = DESACT_; \
	else if(0 == strcmp("RECON_", TEXTO)) \
		ESTADO = RECON_; \
	else \
		ESTADO = EERROR_


/*--------------------------------------------------------
	Conversion de un string a un Tipo de
	registro valido
----------------------------------------------------------*/
#define TEXTO2LOG(LOG,TEXTO) \
	if(0 == strcmp("MUESTREO_", TEXTO)) \
		LOG = MUESTREO_; \
	else if(0 == strcmp("EVENTO_", TEXTO)) \
		LOG = EVENTO_; \
	else \
		LOG = LERROR_


/*--------------------------------------------------------
	Conversion de un string a un Tipo de
	Funcion valido
----------------------------------------------------------*/
#define TEXTO2FUNC(FUNC,TEXTO) \
	if(0 == strcmp("AVG_", TEXTO)) \
		FUNC = AVG_; \
	else if(0 == strcmp("STD_", TEXTO)) \
		FUNC = STD_; \
	else if(0 == strcmp("VAR_", TEXTO)) \
		FUNC = VAR_; \
	else if(0 == strcmp("PZ_", TEXTO)) \
		FUNC = PZ_; \
	else if(0 == strcmp("NO_", TEXTO)) \
		FUNC = NO_; \
	else \
		FUNC = FERROR_


/*--------------------------------------------------------
	Conversion de un string a un Tipo de
	Comparacion valido
----------------------------------------------------------*/
#define TEXTO2COMP(COMP,TEXTO) \
	if(0 == strcmp("IGUAL_QUE_", TEXTO)) \
		COMP = IGUAL_QUE_; \
	else if(0 == strcmp("MAYOR_QUE_", TEXTO)) \
		COMP = MAYOR_QUE_; \
	else if(0 == strcmp("MENOR_QUE_", TEXTO)) \
		COMP = MENOR_QUE_; \
	else if(0 == strcmp("MAYOR_IGUAL_QUE_", TEXTO)) \
		COMP = MAYOR_IGUAL_QUE_; \
	else if(0 == strcmp("MENOR_IGUAL_QUE_", TEXTO)) \
		COMP = MENOR_IGUAL_QUE_; \
	else \
		COMP = AERROR_

#ifndef _DATADIR_
#define _DATADIR_			"/usr/share/"
#endif

extern char __ARCHIVO_CONF_TAGS[512];			//!< Archivo XML de Tags
extern char __ARCHIVO_CONF_ALARMAS[512];		//!< Archivo XML de Alarmas
extern char __ARCHIVO_CONF_DEVS[512];			//!< Archivo XML de Dispositivos
extern char __ARCHIVO_CONF_SERV_DATOS[512];		//!< Archivo XML de Servidor
extern char __ARCHIVO_CONF_CLIENTE_DATOS[512];	//!< Archivo XML de Clientes
extern char __ARCHIVO_CONF_HISTORICOS[512];		//!< Archivo XML de Historicos
//extern char *__FUNCIONES_HISTORICOS[];


/*--------------------------------------------------------
	Obtiene un numero primo mayor que el parametro
----------------------------------------------------------*/
void primo_mayor_que(unsigned int & );

/*--------------------------------------------------------
	Obtiene la cantidad de funciones disponibles
----------------------------------------------------------*/
//unsigned int numero_funciones_disponibles();

/*--------------------------------------------------------
	Convierte un numero con base dada en string
----------------------------------------------------------*/
template <class T>
string numero2string(T t, ios_base & (*f)(ios_base&))
{
  ostringstream oss;
  oss << f << t;
  return oss.str();
}

/*--------------------------------------------------------
	Registra en el LOG del sistema
----------------------------------------------------------*/
void escribir_log(const char *, int,const char *);

/*--------------------------------------------------------
	Imprime el numero de espacios necesarios para cuadrar
	la informaci??n imprimida en el espacio en pantalla
	deseado
----------------------------------------------------------*/
void mostrar_espaciado(ostream &, const char *,int);

//string dato2texto(Tdato);
//string log2texto(Tlog);
//string func2texto(Tfunciones);
//string valor2texto(Tvalor, Tdato);
//char * obtener_xml_servidor_tendencia(char *, unsigned int, const char *, unsigned int );







#endif // ASUTIL_H_INCLUDED
