/*
 * Pruebas para una cola de punteros.
 */

#include "cola.h"
#include "acutest.h"

static void test_crear_cola_destruir_vacia(void)
{
    cola_t *cola = cola_crear();
    TEST_ASSERT(cola != NULL);
    TEST_CHECK(cola_esta_vacia(cola));
    cola_desencolar(cola);
    cola_destruir(cola);
}

static void test_encolar(void)
{
    int n = 12;
    int *primerPuntero = &n;
    char *palabra = "Hola";
    int *primero;

    cola_t *cola = cola_crear();
    TEST_ASSERT(cola != NULL);
    TEST_CHECK(cola_esta_vacia(cola));
    TEST_CHECK(cola_encolar(cola, primerPuntero));
    primero = cola_ver_primero(cola);
    TEST_ASSERT(primero == primerPuntero);
    TEST_CHECK(cola_encolar(cola, palabra));
    primero = cola_ver_primero(cola);
    TEST_ASSERT(primero == primerPuntero);
    cola_destruir(cola);
}

static void test_encolar_desencolar_destruir(void)
{
    int v[] = {0, 15};
    int *elemento_a_encolar = &v[0];

    cola_t *cola = cola_crear();
    TEST_ASSERT(cola != NULL);
    TEST_CHECK(cola_esta_vacia(cola));
    TEST_CHECK(cola_encolar(cola, elemento_a_encolar));
    int *valor = cola_desencolar(cola);
    TEST_ASSERT(valor == elemento_a_encolar);
    cola_destruir(cola);
}

static void test_ver_primero(void)
{
    int v[] = {5, 10};
    int *elemento_a_encolar = &v[1];
    int *segundo_elemento_a_encolar = &v[0];
    int *primero;
    cola_t *cola = cola_crear();

    TEST_ASSERT(cola != NULL);
    TEST_CHECK(cola_esta_vacia(cola));
    TEST_CHECK(cola_ver_primero(cola) == NULL);

    TEST_CHECK(cola_encolar(cola, elemento_a_encolar));
    TEST_CHECK(!cola_esta_vacia(cola));
    primero = cola_ver_primero(cola);
    TEST_CHECK(primero == elemento_a_encolar);

    TEST_CHECK(cola_encolar(cola, segundo_elemento_a_encolar));
    TEST_CHECK(!cola_esta_vacia(cola));
    primero = cola_ver_primero(cola);
    TEST_CHECK(primero == elemento_a_encolar);

    cola_destruir(cola);
}

static void test_desencolar(void)
{
    int v[] = {10, 20, 1};
    cola_t *cola = cola_crear();

    TEST_ASSERT(cola != NULL);
    TEST_CHECK(cola_encolar(cola, &v[0]));
    TEST_CHECK(cola_encolar(cola, &v[1]));
    TEST_CHECK(cola_encolar(cola, &v[2]));

    TEST_CHECK(cola_desencolar(cola) == &v[0]);
    TEST_CHECK(cola_desencolar(cola) == &v[1]);
    TEST_CHECK(cola_desencolar(cola) == &v[2]);
    TEST_CHECK(cola_esta_vacia(cola));

    cola_destruir(cola);
}

static void test_varios_elementos(void)
{
    int v[3] = {100, 50, 20};
    cola_t *cola = cola_crear();

    TEST_ASSERT(cola != NULL);

    TEST_ASSERT(cola != NULL);
    TEST_CHECK(cola_encolar(cola, &v[0]));
    TEST_CHECK(cola_encolar(cola, &v[1]));
    TEST_CHECK(!cola_esta_vacia(cola));
    TEST_CHECK(cola_ver_primero(cola) == &v[0]);

    TEST_CHECK(cola_encolar(cola, &v[2]));
    TEST_CHECK(cola_desencolar(cola) == &v[0]);
    TEST_CHECK(cola_ver_primero(cola) == &v[1]);

    TEST_CHECK(cola_desencolar(cola) == &v[1]);
    TEST_CHECK(cola_ver_primero(cola) == &v[2]);

    TEST_CHECK(cola_desencolar(cola) == &v[2]);
    TEST_CHECK(cola_esta_vacia(cola));

    cola_destruir(cola);
}

TEST_LIST = {
    {"Crear y destruir la cola", test_crear_cola_destruir_vacia},
    {"Cola encolar", test_encolar},
    {"Cola encolar, desencolar y destruir", test_encolar_desencolar_destruir},
    {"cola ver primero", test_ver_primero},
    {"cola desencolar", test_desencolar},
    {"cola varios elementos", test_varios_elementos},
    {NULL, NULL}};
