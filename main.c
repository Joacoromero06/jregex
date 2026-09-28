#include "jregex.h"

int main()
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
  pop = popr(&eq);
  assert(false && "error en popr");
  for (int i = 0; i < 3; ++i)
  {
    pop = i%2==0? popl(&eq) : popr(&eq);
    if (pop != NULL)
    {
      printf("{tipo: '%c' - quantifier: '%c' - car: '%c'}\n", pop->tipo, pop->quantifier, pop->car);
      //free(pop);
    }
  }
  free_eq(eq);

  //char* re = "ab*c(d.e+)?";
  //char* s = "abbcd0eee";

  // compilar la regex, creadondo una Maquina de Estado 
  //ExecutionQueue* eq = compile(re);

  // ejecutar la Maquina de Estados y testear si s pertenece al Leng
  //bool b = check(eq, s);
}
