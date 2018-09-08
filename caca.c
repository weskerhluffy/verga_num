/*
 ============================================================================
 Name        : defensa_cacastillo.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

// XXX: http://codeforces.com/contest/954/problem/G
#if 1
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdint.h>
#include <ctype.h>

#ifdef CACA_COMUN_LOG
#include <execinfo.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#include <stdarg.h>
#endif

#if 1
#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX LONG_MAX
#endif

#ifndef ULONG_LONG_MAX
#define ULONG_LONG_MAX ULONG_MAX
#endif

#define CACA_COMUN_TAM_MAX_LINEA (16*200000)
#define CACA_LOG_MAX_TAM_CADENA 2000

#define CACA_COMUN_BUF_STATICO (char[1000] ) { '\0' }

#define BITCH_VECTOR_NUM_BITS (sizeof(bitch_vector) * 8)

#define CACA_COMUN_ASSERT_DUROTE 0
#define CACA_COMUN_ASSERT_SUAVECITO 1
#define CACA_COMUN_ASSERT_NIMADRES 2

#define CACA_COMUN_VALOR_INVALIDO ((tipo_dato)UINT_MAX)
#define CACA_COMUN_IDX_INVALIDO ((natural)CACA_COMUN_VALOR_INVALIDO)
//#define CACA_COMUN_LOG
#define CACA_COMUN_FUNC_STATICA static inline

typedef char byteme;
typedef unsigned int natural;
typedef byteme tipo_dato;
typedef long long entero_largo;
typedef unsigned long long entero_largo_sin_signo;
typedef long long bitch_vector;

typedef enum BOOLEANOS {
	falso = 0, verdadero
} bool;

/*
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_SUAVECITO
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_NIMADRES
 */
#define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_DUROTE

#define assert_timeout_dummy(condition) 0;

#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_DUROTE
#define assert_timeout(condition) assert(condition);
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_SUAVECITO
#define assert_timeout(condition) if(!(condition)){while(1){printf("");};abort();}
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_NIMADRES
#define assert_timeout(condition) 0
#endif

#ifdef CACA_COMUN_LOG
#define caca_log_debug(formato, args...) \
		do \
		{ \
			size_t profundidad = 0; \
			void *array[CACA_LOG_MAX_TAM_CADENA]; \
 			profundidad = backtrace(array, CACA_LOG_MAX_TAM_CADENA); \
			caca_log_debug_func(formato,__FILE__, __func__, __LINE__,profundidad,##args); \
		} \
		while(0);
#else
#define caca_log_debug(formato, args...) 0;
#endif

#define caca_comun_max(x,y) ((x) < (y) ? (y) : (x))
#define caca_comun_min(x,y) ((x) < (y) ? (x) : (y))

#define caca_comun_calloc_local(tipo) (&(tipo){0})

void caca_log_debug_func(const char *format, ...);

void caca_comun_current_utc_time(struct timespec *ts) {

#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
#else
#ifdef CACA_COMUN_LOG
	clock_gettime(CLOCK_REALTIME, ts);
#endif
#endif

}

void caca_comun_timestamp(char *stime) {
	time_t ltime;
	struct tm result;
	long ms;
#ifndef ONLINE_JUDGE
	struct timespec spec;
#endif
	char parte_milisecundos[50];

	ltime = time(NULL);

#ifndef ONLINE_JUDGE
	localtime_r(&ltime, &result);
	asctime_r(&result, stime);
#endif

	*(stime + strlen(stime) - 1) = ' ';

#ifndef ONLINE_JUDGE
	caca_comun_current_utc_time(&spec);
	ms = round(spec.tv_nsec / 1.0e3);
#endif
	sprintf(parte_milisecundos, "%ld", ms);
	strcat(stime, parte_milisecundos);
}
#ifdef CACA_COMUN_LOG
void caca_log_debug_func(const char *format, ...) {

	va_list arg;
	va_list arg2;
	const char *PEDAZO_TIMESTAMP_HEADER = "tiempo: %s; ";
	const char *HEADER =
	"archivo: %s; funcion: %s; linea %d; nivel: %zd caca 8====D ";
	char formato[CACA_LOG_MAX_TAM_CADENA + sizeof(HEADER)
	+ sizeof(PEDAZO_TIMESTAMP_HEADER)] = {'\0'};
	char pedazo_timestamp[sizeof(PEDAZO_TIMESTAMP_HEADER) + 100] = {'\0'};
	char cadena_timestamp[100] = {'\0'};

	caca_comun_timestamp(cadena_timestamp);
	sprintf(pedazo_timestamp, PEDAZO_TIMESTAMP_HEADER, cadena_timestamp);

	strcpy(formato, pedazo_timestamp);
	strcat(formato, HEADER);
	strcat(formato, format);
	strcat(formato, "\n");

	va_start(arg, format);
	va_copy(arg2, arg);
	vprintf(formato, arg2);
	va_end(arg2);
	va_end(arg);
	setbuf(stdout, NULL);
}
#endif

#ifdef CACA_COMUN_LOG
CACA_COMUN_FUNC_STATICA char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, natural tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%1d", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

CACA_COMUN_FUNC_STATICA char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2u",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	int i, j;
	natural inicio_buffer_act = 0;
	for (int i = 0; i < filas_tam; i++) {
		caca_comun_arreglo_a_cadena(matrix + i * columas_tam, columas_tam,
				buffer + inicio_buffer_act);
		inicio_buffer_act += strlen(buffer + inicio_buffer_act);
		buffer[inicio_buffer_act++] = '\n';
		/*		caca_log_debug("pero q mierda inicio buffer act %u %s",inicio_buffer_act,buffer);*/
	}
	return buffer;
}

CACA_COMUN_FUNC_STATICA char *caca_comun_arreglo_a_cadena_entero_largo_sin_signo(
		entero_largo_sin_signo *arreglo, entero_largo_sin_signo tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%llu", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
#else
CACA_COMUN_FUNC_STATICA char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo,
		natural tam_arreglo, char *buffer) {
	return NULL;
}
CACA_COMUN_FUNC_STATICA char *caca_comun_arreglo_a_cadena_natural(
		natural *arreglo, natural tam_arreglo, char *buffer) {
	return NULL;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	return NULL;
}

CACA_COMUN_FUNC_STATICA char *caca_comun_arreglo_a_cadena_entero_largo_sin_signo(
		entero_largo_sin_signo *arreglo, entero_largo_sin_signo tam_arreglo,
		char *buffer) {
	return NULL;
}
#endif

#define caca_comun_arreglo_a_cadena_entero_largo_sin_signo_buf_local(a,a_tam) caca_comun_arreglo_a_cadena_entero_largo_sin_signo(a,a_tam,CACA_COMUN_BUF_STATICO)
#define caca_comun_arreglo_a_cadena_buf_local(a,a_tam) caca_comun_arreglo_a_cadena(a,a_tam,CACA_COMUN_BUF_STATICO)

CACA_COMUN_FUNC_STATICA void caca_comun_strreplace(char s[], char chr,
		char repl_chr) {
	int i = 0;
	while (s[i] != '\0') {
		if (s[i] == chr) {
			s[i] = repl_chr;
		}
		i++;
	}
}

CACA_COMUN_FUNC_STATICA int caca_comun_lee_matrix_long_stdin(tipo_dato *matrix,
		int *num_filas, int *num_columnas, int num_max_filas,
		int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	tipo_dato numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(CACA_COMUN_TAM_MAX_LINEA, sizeof(char));

	while (indice_filas < num_max_filas
			&& fgets(linea, CACA_COMUN_TAM_MAX_LINEA, stdin)) {
		indice_columnas = 0;
		cadena_numero_actual = linea;
		caca_comun_strreplace(linea, '\n', '\0');
		if (!strlen(linea)) {
			caca_log_debug("weird, linea vacia");
			continue;
		}
		for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
				cadena_numero_actual) {
			numero = strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
			if (cadena_numero_actual == siguiente_cadena_numero) {
				break;
			}
			*(matrix + indice_filas * num_max_columnas + indice_columnas) =
					numero;
			indice_columnas++;
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug("las filas son %d, con clos %d", indice_filas,
				indice_columnas);
	}

	*num_filas = indice_filas;
	free(linea);
	return 0;
}

CACA_COMUN_FUNC_STATICA natural caca_comun_cuenta_bitchs(tipo_dato num) {
	natural bitch_cnt = 0;
	tipo_dato num_mod = 0;
	num_mod = num;
	while (num_mod) {
		num_mod &= ~(num_mod & (-num_mod));
		bitch_cnt++;
	}
	return bitch_cnt;
}

CACA_COMUN_FUNC_STATICA char caca_comun_letra_a_valor_minuscula(char letra) {
	return letra - 'a';
}

CACA_COMUN_FUNC_STATICA natural caca_comun_max_natural(natural *nums,
		natural nums_tam) {
	natural max = 0;

	for (int i = 0; i < nums_tam; i++) {
		natural num_act = nums[i];
		if (num_act > max) {
			max = num_act;
		}
	}

	return max;
}

// XXX: https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
CACA_COMUN_FUNC_STATICA char *caca_comun_trimea(char *cad, natural cad_tam) {
	char c = cad[0];
	char tmp = '\0';
	natural i = 0;
	natural j = 0;

	caca_log_debug("entrada %s cad_tam %u", cad, cad_tam);
	while (j < cad_tam && cad[j] != '\0') {
		caca_log_debug("en j %u car %c", j, cad[j]);
		while (j < cad_tam && !isalpha(cad[j])) {
			caca_log_debug("brincando j %u car %c", j, cad[j]);
			j++;
		}
		caca_log_debug("aora j %u car %c", j, cad[j]);
		if (j == cad_tam) {
			caca_log_debug("q ped");
			break;
		}
		tmp = cad[i];
		cad[i] = cad[j];
		cad[j] = tmp;
		i++;
		j++;
	}
	caca_log_debug("mierda '%c'", cad[j]);

	i = 0;
	while (isalpha(cad[i++]))
		;
	caca_log_debug("salida %s", cad);
	cad[i - 1] = '\0';

	return cad;
}

#endif

#define CACA_COMUN_LIMPIA_MEM(m,s) (memset(m,0,s))
#define CACA_COMUN_LIMPIA_MEM_STATICA(m) (memset(m,0,sizeof(*(m))))

#endif

#if 1

entero_largo numero_largote_anade_digito(entero_largo l, byteme d) {
	return l * 10 + d;
}

CACA_COMUN_FUNC_STATICA entero_largo numero_largote_divis_divis(
		byteme *dividendo, natural dividendo_tam, entero_largo divis,
		byteme *cociente, natural cociente_tam) {
	entero_largo resto = 0;
	int i = dividendo_tam - 1;

	caca_log_debug("dividen %s divis %llu",
			caca_comun_arreglo_a_cadena_buf_local(dividendo,dividendo_tam),
			divis);

	assert_timeout(!cociente || cociente_tam >= dividendo_tam);

	if (cociente) {
		CACA_COMUN_LIMPIA_MEM(cociente, cociente_tam);
	}

	while (i >= 0) {
		entero_largo c = 0;
		while (resto < divis && i >= 0) {
			resto = numero_largote_anade_digito(resto, dividendo[i]);
			i--;
		}
		if (resto >= divis) {
			c = resto / divis;
			resto = resto % divis;
			if (cociente) {
				cociente[i + 1] = c;
			}
		}
	}

	if (cociente) {
		caca_log_debug("coci %s resto %llu",
				caca_comun_arreglo_a_cadena_buf_local(dividendo,dividendo_tam),
				resto);
	} else {
		caca_log_debug("resto nomas %llu", resto);
	}

	return resto;
}

CACA_COMUN_FUNC_STATICA entero_largo numero_largote_mcd(entero_largo a,
		entero_largo b) {
	entero_largo r = 0;
	if (!a) {
		r = 0;
	} else {
		if (!b) {
			r = a;
		} else {
			r = numero_largote_mcd(b, a % b);
		}
	}
	return r;
}

CACA_COMUN_FUNC_STATICA entero_largo numero_largote_mcd_entero_largo(byteme *a,
		natural a_tam, entero_largo b) {
	return numero_largote_mcd(b,
			numero_largote_divis_divis(a, a_tam, b, NULL, 0));
}

CACA_COMUN_FUNC_STATICA void numero_largote_multiplica(byteme *a, natural a_tam,
		entero_largo b, byteme *r, natural r_tam) {
	entero_largo carrie = 0;
	natural i = 0;
	assert_timeout(r_tam >= a_tam);
	caca_log_debug("dividienti %s entre %llu",
			caca_comun_arreglo_a_cadena_buf_local(a, a_tam), b);

	for (i = 0; i < a_tam; i++) {
		entero_largo r_tmp = a[i] * b + carrie;
		caca_log_debug("a[%u] es %u mult %llu carrie %llu res %llu", i, a[i], b,
				carrie, r_tmp);
		r[i] = r_tmp % 10;
		carrie = r_tmp / 10;
	}
	if (r_tam > a_tam) {
		caca_log_debug("procesnado carrie %llu", carrie);
		while (carrie && i < r_tam) {
			entero_largo r_tmp = carrie;
			r[i] = r_tmp % 10;
			carrie = r_tmp / 10;
		}
	}
	caca_log_debug("coc %s rest %llu",
			caca_comun_arreglo_a_cadena_buf_local(r,r_tam), carrie);
	assert_timeout(!carrie);
}
CACA_COMUN_FUNC_STATICA natural numero_largote_cuenta_digitos(byteme *a,
		natural a_tam) {
	int i = a_tam - 1;
	while (i >= 0 && !a[i]) {
		i--;
	}
	return i + 1;
}

CACA_COMUN_FUNC_STATICA char *numero_largote_a_cadena(byteme *a, natural a_tam,
		char *buffer, natural buffer_tam) {
	int i = 0;
	natural j = 0;
	assert_timeout_dummy(a_tam<=buffer_tam);
	CACA_COMUN_LIMPIA_MEM(buffer, buffer_tam);
	i = a_tam - 1;
	while (i >= 0 && !a[i]) {
		i++;
	}
	for (j = 0; i >= 0; i--, j++) {
		buffer[j] = a[i] + '0';
	}
	buffer[j] = '\0';
	return buffer;
}

#endif

CACA_COMUN_FUNC_STATICA byteme obten_ultimo_digito(byteme *a, natural a_tam,
		entero_largo b) {
	entero_largo d = 0;
	entero_largo r = 0;
	byteme res = CACA_COMUN_VALOR_INVALIDO;
	assert_timeout(b);
	d = numero_largote_mcd_entero_largo(a, a_tam, b);
	r = b / d;
	assert_timeout(r);
	if (r < 10) {
		res = r;
	}
	return res;
}

#define MAX_CACA ((int)1E6)
byteme a[MAX_CACA] = { 0 };
byteme res_tmp[MAX_CACA + 1] = { 0 };
byteme res[MAX_CACA + 1] = { 0 };
char buffer[MAX_CACA] = { '\0' };

CACA_COMUN_FUNC_STATICA void caca_main() {
	natural p = 0;
	natural x = 0;
	entero_largo b = 0;
	byteme r = 0;
	bool exito = falso;

	scanf("%u %u\n", &p, &x);

	memset(a, 9, p);
	b = 10 * x - 1;
	r = obten_ultimo_digito(a, p, b);
	caca_log_debug("dig es %hu", r);

	if (r != CACA_COMUN_VALOR_INVALIDO) {
		entero_largo y = r;
		entero_largo resto = 0;
		numero_largote_multiplica(a, p, y, res_tmp, p + 1);
		resto = numero_largote_divis_divis(res_tmp, p + 1, b, res, p + 1);
		assert_timeout(!resto);

		if (numero_largote_cuenta_digitos(res, p) < p) {
			memcpy(res_tmp, res, p);
			numero_largote_multiplica(res_tmp, p, x, res, p);
		}
		if (numero_largote_cuenta_digitos(res, p) == p) {
			exito = verdadero;
		}
	}

	if (exito) {
		caca_log_debug("el num %s",
				caca_comun_arreglo_a_cadena_buf_local(res,p));
		printf("%s\n", numero_largote_a_cadena(res, p, buffer, MAX_CACA));
	} else {
		caca_log_debug("no se pudo");
		printf("Impossible\n");
	}
}

int main() {
	caca_main();
	return EXIT_SUCCESS;
}
