#include "pdb2fasta/pdb2fasta.h"
#include <argp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE_LENGTH 50

const char *argp_program_version = "pdb2fasta 0.1.0";
const char *argp_program_bug_address = "developer@sagen.io";
static char doc[] = "Converts PDB to fasta files.";
static struct argp_option options[] = {
  {"input", 'i', "FILE", 0, "Path to input (PDB) file."},
  {"output", 'o', "FILE", OPTION_ARG_OPTIONAL, "Path to output (FASTA) file."},
  {"name", 'n', 0, OPTION_ARG_OPTIONAL, "Name identifier in FASTA file."},
  {0}};

struct arguments {
  char *input_file;
  char *output_file;
  char *file_name;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;

  switch (key)
  {
  case 'i': arguments->input_file = arg; break;
  case 'o': arguments->output_file = arg; break;
  case 'n': arguments->file_name = arg; break;
  case ARGP_KEY_ARG: return 0;
  default: return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = {options, parse_opt, 0, doc, 0, 0, 0};

int main(int argc, char **argv)
{
  struct arguments arguments;

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  FILE *file = fopen(arguments.input_file, "r");

  if (file == NULL)
  {
    fprintf(stderr, "ERROR: No file named \'%s\'\n", arguments.input_file);
    return EXIT_FAILURE;
  }

  char *content = malloc(sizeof(*content) * 1);

  if (!(arguments.file_name == NULL))
  {
    int start_index = 0, end_index = strlen(arguments.input_file);
    for (unsigned i = 0; i < strlen(arguments.input_file); i++)
    {
      if (arguments.input_file[i] == '/' || arguments.input_file[i] == '\\')
      {
        start_index = i + 1;
        end_index = strlen(arguments.input_file);
      }
      if (arguments.input_file[i] == '.' &&
          end_index == strlen(arguments.input_file))
      {
        end_index = i;
      }
    }

    char file_name[end_index - start_index + 1];
    for (unsigned i = start_index; i < end_index; i++)
    {
      file_name[i - start_index] = arguments.input_file[i];
    }
    file_name[end_index - start_index] = '\0';

    if (strlen(file_name) >= BASE_LENGTH)
    {
      file_name[BASE_LENGTH] = '\0';
    }

    content = pdb2fasta(file, file_name, content);
  }
  else
  {
    // TODO: Correct/warn if name is longer then 50 chars
    content = pdb2fasta(file, arguments.file_name, content);
  }

  if (!(arguments.output_file == NULL))
  {
    printf("%s", content);
  }
  else
  {
    FILE *out = fopen(arguments.output_file, "w");
    fprintf(out, "%s", content);
    fclose(out);
  }

  free(content);

  fclose(file);

  return (EXIT_SUCCESS);
}
