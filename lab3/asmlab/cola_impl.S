#include "cola_asm.h"

.global cola_crear
.global cola_destruir
.global cola_encolar
.global cola_desencolar
.global cola_esta_vacia
.global cola_ver_primero

cola_crear:
  movl $SIZEOF_COLA, %esi
  movl $CALLOC_ARG, %edi
  call calloc
  ret

cola_esta_vacia:           /* Se permite un conditional move (cmovXX/setXX) */
  movq $0, %rax
  movq $1, %rcx
  cmpq OFFSET_HEAD(%rdi), %rax
  cmove %rcx, %rax
  ret

cola_ver_primero:          /* Se permite un salto condicional */
  call cola_esta_vacia
  cmpq $1, %rax
  je .salir_cola_ver_primero
  jmp .ver_primero

.salir_cola_ver_primero:
  movq $0, %rax
  ret

.ver_primero:
  movq OFFSET_HEAD(%rdi), %rcx
  movq OFFSET_DATA(%rcx), %rax
  ret

cola_encolar:              /* Se permite un conditional move */
  movq %rdi, %r12              # recibe la cola en %rdi y lo guarda en %r12
  movq %rsi, %r13              # recibe el valor en %rsi y lo guarda en %r13
  movl $CALLOC_ARG, %edi       # mueve a edi el primer argumento de calloc
  movl $SIZEOF_NODO, %esi      # mueve a esi el segundo argumento de calloc
  call calloc                  # devuelve en rax el puntero a la memoria reservada por el nodo
  movq %r13, OFFSET_DATA(%rax) # guardo el dato en el offset correspondiente
  movq %r12, %rdi              # libero %r12 y guardo en %rdi para llamar a cola_esta_vacia
  movq %rax, %r12              # uso %r12 para guardar el nodo
  call cola_esta_vacia
  cmpq $1, %rax                # esta vacia entonces encolo el primer elemento
  je .encolar_primero
  jmp .encolar

.encolar_primero:
  movq %r12, OFFSET_HEAD(%rdi) # cola->primero = nodo
  movq %r12, OFFSET_TAIL(%rdi) # cola->ultimo = nodo
  ret

.encolar:
  movq OFFSET_TAIL(%rdi), %r14 # r14 = cola->ultimo
  movq %r12, OFFSET_NEXT(%r14) # cola->ultimo->next = nodo
  movq %r12, OFFSET_TAIL(%rdi) # cola->ultimo = nodo
  movq $1, %rax
  ret

cola_desencolar:           /* Se permite dos saltos condicionales + un cmov */
  call cola_esta_vacia
  cmpq $0, %rax
  je .desencolar_elemento
  jmp .salir_desencolar

.salir_desencolar:
  ret

.desencolar_elemento:
  pushq %r12
  subq $16, %rsp
  movq %rdi, %r12              # guardo %rdi, la cola, en %r12 
  pushq %rbx
  subq $16, %rsp
  movq OFFSET_HEAD(%r12), %r13 # r13 = cola->head / cola->primero / nodo
  movq OFFSET_DATA(%r13), %rbx # rbx = cola->head->data / cola->primero->dato
  movq OFFSET_NEXT(%r13), %r8  # r8 = cola->head->next / cola->primero->next / segundo nodo
  movq %r8, OFFSET_HEAD(%r12)  # cola->head = r8 / cola->primero = r8 / lo guardo en cola->head
  cmpq OFFSET_HEAD(%r12), %rax # rax era 0 cuando entro a desencolar_elemento / chequea que %rdi, o sea el nuevo cola->head sea 0
  movq OFFSET_TAIL(%r12), %r14 # r14 = cola->tail
  cmove %rax, %r14             # cola->tail = 0
  movq %r13, %rdi              # paso el nodo a rdi para liberarlo
  call free                    # libero el nodo
  movq %rbx, %rax
  addq $16, %rsp
  popq %rbx
  movq %r12, %rdi
  addq $16, %rsp
  popq %r12
  ret

cola_destruir:             /* Sin restricciones */
  jmp .ciclo

.ciclo:    /* el ciclo se repite mientras la cola no este vacia */
  call cola_esta_vacia
  cmpq $1, %rax
  je .salir_cola_destruir
  call cola_desencolar
  jmp .ciclo

.salir_cola_destruir:
  call free
  ret