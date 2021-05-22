#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ti_token.h>
#include <ti_parse.h>

int main(int argc, const char **argv) {
  printf("TIMAC(Today I Made A Compiler) version 0.01, by segfaultdev\n");
  const char *input_path = NULL, *output_path = NULL;
  FILE *input, *output;

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-o"))
      output_path = argv[++i];
    else
      input_path = argv[i];
  }

  if (!input_path)
    printf("error: missing input file\n"), exit(1);
  if (!output_path)
    printf("error: missing output file\n"), exit(1);

  if (!(input = fopen(input_path, "r")))
    printf("error: cannot open input file\n"), exit(1);
  if (!(output = fopen(output_path, "w")))
    printf("error: cannot open output file\n"), exit(1);

  fseek(input, 0, SEEK_END);
  size_t input_size = ftell(input);
  fseek(input, 0, SEEK_SET);

  char *input_buffer = malloc(input_size + 1);
  fread(input_buffer, 1, input_size, input);
  fclose(input);

  ti_program_t program;
  ti_tokenize(&program, input_buffer);
  ti_parse(&program);

  free(input_buffer);

  fclose(output);
  return 0;
}
