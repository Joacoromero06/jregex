#include "jregex.h"

int main()
{
  char* re = "a\\*b";
  //char* s = "abbcd0eee";

  // compilar la regex, creadondo una Maquina de Estado 
  ExecQueue eq = compile_regex(re);
  mostrar_eq(eq, 1);
  free_eq(eq);

  // ejecutar la Maquina de Estados y testear si s pertenece al Leng
  //bool b = check(eq, s);
}
