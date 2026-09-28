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
  State* nav = q->begin, *ant = NULL;
  assert(nav != NULL && "intento de popear elemento de dequeue nula");
  while (nav->next != NULL) // hasta que nav sea el ultimo (el pop)
  {
    ant = nav;
    nav = nav->next;
  }
  if (ant == NULL) // el ultimo es el unico (queue queda vacia)
  {
    q->begin = q->end = NULL;
  }
  else // ant apunta al ultimo, lo desreferencio, ant es end
  {
    ant->next = NULL;
    q->end = ant; 
  }

  return nav; // transfiero owner ship
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

ExecQueue compile_regex(char* re)
{
  ExecQueue q = {.begin = NULL, .end = NULL}; // queue root
  ExecQueue* cq = NULL;
  ExecQueueStack s = build_eqs();
  push_eqs(&s, &q); // la primera referencia a eq que compilaremos es root

  State* curr;
  while (*re != '\0')
  {
    curr = NULL; // inicializar referencia nula
    cq = top_eqs(s); // current queue para compilar
    switch (*re)
    {
    case '?':
      curr = popr(cq);
      assert(curr != NULL && "queue no vacia en parsing '?'");
      // Sea cual sea el tipo 'L' 'G' '.'
      // Sea cual sea el quantifier '?' '*' '1'
      curr->quantifier = '?';
      pushr(cq, curr);
      break;
    case '*':
      curr = popr(cq);
      assert(curr != NULL && "queue no vacia en parsing '*'");
      curr->quantifier = '*';
      pushr(cq, curr);
      break;
    case '.':
      curr = create_state('.', '1', '.');
      assert(curr != NULL && "error create_state en parsing '.'");
      pushr(cq, curr);
      break;
    case '+':
      curr = popr(cq);
      assert(curr != NULL && "queue no vacia al parsear +");
      // agrego el mismo estado y uno con quantificador *
      // reg+ = regreg*
      pushr(cq, curr); // exactamente la misma instancia, entrego owner
      curr = create_state(curr->tipo, '*', curr->car); 
      pushr(cq, curr); // entrego ownership
      break; 
    case '(':
      curr = create_state('G', '1', '\0');  
      pushr(cq, curr);
      push_eqs(&s, &curr->eq); // compilar la inner regex del group
      break;
    case ')':
      assert(top_eqs(s) != NULL && ") debe cerrar algun grupo");
      pop_eqs(&s);//  la compilacion inner termino, compilo para la externa
      // la referencia a la inner se pierde, queda en referenciada por su outer
      break;
    case '\\':
      assert(*(re+1) != '\0' && "error en compilacion regex, para utilizar '\\' debe utilizar el escape: '\\\\'");
      re++;
      curr = create_state('L', '1', *re);
      pushr(cq, curr);
      break;
    default:
      curr = create_state('L', '1', *re);
      pushr(cq, curr);
      break;
    }
    re++;    
  }

  free_eqs(s);
  return q;
}

ExecQueueStack build_eqs()
{
  ExecQueueStack s = {.data = NULL, .capacity = 0, .tope = 0};
  return s;
}
void free_eqs(ExecQueueStack s)
{
  if (s.data != NULL) free(s.data); // libero el array de punteros, no toco lo que apuntan
  s.capacity = 0; s.tope = 0;
}
bool esvacia_eqs(ExecQueueStack s)
{
  if (s.data == NULL) assert(s.capacity == 0 && "inconsistencia de EQS data nul size distinto de cero");
  if (s.capacity == 0) assert(s.data == NULL && "inconsistencia de EQS size 0 pero data no NULL");

  return s.capacity == 0;
}
ExecQueue* top_eqs(ExecQueueStack s)
{
  assert(!esvacia_eqs(s) && "no se puede topear de una pila vacia de ExecQueue");
  return s.data[s.tope-1];
}
ExecQueue* pop_eqs(ExecQueueStack *s)
{
  assert(!esvacia_eqs(*s) && "no se puede popear de una pila vacia de ExecQueue");
  return s->data[--s->tope];
}
void push_eqs(ExecQueueStack* s, ExecQueue* q)
{
#define GROW_CAPACITY(cap) ( cap== 0? 8: cap*2 )
  if (s->capacity <= s->tope)
  {
    s->capacity = GROW_CAPACITY(s->capacity);
    s->data = reallocarray(s->data, s->capacity, sizeof(ExecQueue*));
  }
  s->data[s->tope++] = q;
}
