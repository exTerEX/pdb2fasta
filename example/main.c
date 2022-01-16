#include "pdb2fasta/pdb2fasta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE_LENGTH 50

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("pdb2fasta /path/to/pdb > /path/to/fasta\n");
    return argc;
  }

  // Read file to stream file
  FILE *file = fopen(argv[1], "r");

  // Check if file exists
  if (file == NULL)
  {
    fprintf(stderr, "ERROR! no such file %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  int start_index = 0, end_index = strlen(argv[1]);
  for (unsigned i = 0; i < strlen(argv[1]); i++)
  {
    if (argv[1][i] == '/' || argv[1][i] == '\\')
    {
      start_index = i + 1;
      end_index = strlen(argv[1]);
    }
    if (argv[1][i] == '.' && end_index == strlen(argv[1]))
    {
      end_index = i;
    }
  }

  char name[end_index - start_index + 1];
  for (unsigned i = start_index; i < end_index; i++)
  {
    name[i - start_index] = argv[1][i];
  }
  name[end_index - start_index] = '\0';

  if (strlen(name) >= BASE_LENGTH)
  {
    name[BASE_LENGTH] = '\0';
  }

  char *content = malloc(sizeof(*content) * 1);

  content = pdb2fasta(file, name, content);

  // Write converted output to file.
  FILE *out = fopen(argv[2], "w");
  fprintf(out, "%s", content);
  fclose(out);

  free(content);

  fclose(file);

  return (EXIT_SUCCESS);
}
