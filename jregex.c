#include "jregex.h"

// Transfiere el ownership de la referencia del primer Estado
State* popl(ExecQueue* q)
{
  assert(q != NULL && "error ExecQueue se usa como referencia de stack");
  State* s = q->begin;
  if (s == NULL)
  {
    // cola vacia
  }
  else if (s->next == NULL) // o q->begin == q->end
  {
    // cola con un elemento
    q->begin = q->end = NULL;
  }
  else {
    q->begin = q->begin->next;
  }
  return s;
}

// Obtiene la referencia del ultimo Estado
State* popr(ExecQueue* q)
{
  // Si como State Machine-Ast (estatico) 
  // No como ExecQueue-Grafo (dinamico)
  // .. se utiliza popr
  
  assert(q != NULL && "error ExecQueue se usa como referencia de stack");
  State* s = q->end, *nav = NULL;
  if (s == NULL) {}// queue vacia
  else if (q->begin->next == NULL) // unico elemento
    q->begin = q->end = NULL;
  else {
    // next no es NULL por la condicion anterior
    while (nav->next->next != NULL) nav = nav->next; 
    nav->next = NULL; // nav es el anterior a q->end que popeamos
  }
  return s;
}

// Agrega por cabeza la referencia de un Estado
void pushl(ExecQueue* q, State* s)
{
  assert(q != NULL && "error ExecQueue se usa como referencia de stack");
  // Se toma el ownership de s, s ya no debe ser usado o causara UB
  if (q->begin == NULL) { 
    assert(q->end == NULL && "error ExecQueue debe estar vacia en pushl");
    q->begin = q->end = s;
  } else {
    State* cab = q->begin;
    s->next = cab;
    q->begin = s;
  }
}
void pushr(ExecQueue* q, State* s)
{
  assert(q != NULL && "error ExecQueue se usa como referencia de stack");
  if (q->begin == NULL) {
    assert(q->end == NULL && "error ExecQueue debe estar vacia en pushl");
    q->begin = q->end = s;
  } else {   
    q->end->next = s;
    q->end = s;
  }
}

void print_indentacion(int n)
{
  for (int i=0; i < n; ++i)
    printf("\t");  
}
void mostrar_eq(ExecQueue q, int ind)
{
  State* s = q.begin;

  print_indentacion(ind-1);
  printf("[\n");
  if (s == NULL) 
  {
    // Execution Queue vacia
  } 
  while (s != NULL)
  {
    print_indentacion(ind);
    if (s->tipo == 'G')
    {
      printf("{tipo: '%c' - quantifier: '%c' - car: '_'}\n", s->tipo, s->quantifier);
      mostrar_eq(s->eq, ind+1);
    }
    else 
    {
      printf("{tipo: '%c' - quantifier: '%c' - car: '%c'}\n", s->tipo, s->quantifier, s->car);
    }
    // AVANCE
    s = s->next;
  }
  print_indentacion(ind-1);
  printf("]\n");

}


State* create_state(char t, char q, char c)
{
  State* s = alloc(State);
  s->tipo = t;
  s->quantifier = q; 
  s->next = NULL;
  assert(t == 'G' || t == 'L' || t == '.');
  assert(q == '1' || q == '?' || q == '*');
  if (t == 'G') {
    s->eq.begin = NULL; 
    s->eq.end = NULL;
  }
  else s->car = c;
  return s;
}

void free_eq(ExecQueue eq)
{
  State* s = eq.begin;
  State* curr = NULL;
  while (s != NULL)
  {
    // liberar eq interno si es necesario
    if (s->tipo == 'G') free_eq(s->eq);

    // liberar nodo State sin importar el tipo
    curr = s;
    s = s->next;
    free(curr);
  }
}
