#ifndef JREGEX_H
#define JREGEX_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "jmemory.h"



typedef struct state_node_t State;

// Representacion en runtime de AFD, con diciplina FIFO
// Concepto de Maquina de Estados, cada State representa una regex
// Relacion con AST, la MS tiene una estructura de arbol polimorfico
// Relacion con el grafo de un AFD, la MS se utiliza para simular el recorrido en un AFD
typedef struct { // begin-end para implementar facil DEQUE
  State* begin;
  State* end;
} ExecQueue;
State* popl(ExecQueue*);
State* popr(ExecQueue*);
void pushl(ExecQueue*, State*);
void pushr(ExecQueue*, State*);

void mostrar_eq(ExecQueue q, int ind);
void free_eq(ExecQueue q);

typedef struct state_node_t {
  char tipo;        // 'L' 'G' '.'
  char quantifier;  // '1' '?' '*' 
  union {
    char car;       // caracter
    ExecQueue eq;   //execution queue
  };
  struct state_node_t* next;
} State;
// para crear states tipo 'G' no se utiliza el parametro c
State* create_state(char t, char q, char c);

typedef struct {
  ExecQueue** data; 
  size_t tope;
  size_t capacity;
} ExecQueueStack; // Stack de referencias
                  
ExecQueueStack build_eqs();
void free_eqs(ExecQueueStack s);
ExecQueue* top_eqs(ExecQueueStack s);
ExecQueue* pop_eqs(ExecQueueStack* s);
void push_eqs(ExecQueueStack* s, ExecQueue* q);
bool esvacia_eqs(ExecQueueStack s);

ExecQueue compile_regex(char* re);


bool check(ExecQueue q);

typedef struct sm_result_t { // State Machine run result
  bool b;   // regex SM acepta una cadena
  size_t c; // cuantos caracteres recorrio
} Result;

#endif
