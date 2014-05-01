#include "prog4.h"

int* superargc = NULL;
char*** superargv = NULL;

int main( int argc, char *argv[])
{
  superargv = &argv;
  superargc = &argc;

  pagereplace ();

  return 0;
}
