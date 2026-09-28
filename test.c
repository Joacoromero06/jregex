#include "jregex.h"

void memory_pop_push_test();
int main()
{
  memory_pop_push_test();
}

void memory_pop_push_test()
{

  ExecQueue eq = {.begin = NULL, .end =NULL};
  
  for (int c='A'; c <= 'F'; c++)
  {
    if (c == 'C')
    {
      State* s = create_state('G', '*', 0);
      pushl(&eq, s);
      for (int cin = 'G'; cin <= 'K'; ++cin)
        pushl(&s->eq, create_state('L', '1', cin));
    }
    else pushl(&eq, create_state('L', '1', c));
  }

  mostrar_eq(eq, 1);

  State* pop = NULL;
  printf("Mostrar poppeds\n\n");
  for (int i = 0; i < 6; ++i)
  {
    pop = i%2==0? popl(&eq) : popr(&eq);
    if (pop != NULL)
    {
      printf("{tipo: '%c' - quantifier: '%c' - car: '%c'}\n", pop->tipo, pop->quantifier, pop->car);
      if (pop->tipo == 'G') free_eq(pop->eq);
      free(pop);
      pop = NULL;
    }
  }

  free_eq(eq);
}
