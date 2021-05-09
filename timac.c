#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <timac_x86.h>

typedef struct token_program_t token_program_t;

struct token_t {
  int type, value, line;
};

struct token_program_t {
  int index; // Current index of token

  token_t *tokens; // Dynamic array of tokens
  int token_count; // Number of tokens

  int *line_start; // Dynamic array of indeces to the first token of each line, ended by a -1
};

int is_valid(char c) {
  return isalnum(c) || c == '$';
}

int is_integer(char *token) {
  while (*token) {
    if (!isdigit(*token)) return 0;
    token++;
  }

  return 1;
}

int consume(token_program_t *program, token_t *token, int type) {
  if (program->index >= program->token_count) return 0;

  if (program->tokens[program->index].type == type) {
    *token = program->tokens[program->index];
    program->index++;

    return 1;
  }

  return 0;
}

void parse(FILE *out, token_program_t *program) {
  token_t token;

  while (program->index < program->token_count) {
    if (program->line_start[program->tokens[program->index].line - 1] == program->index)
      gen_line(out, program->tokens[program->index].line);

    if (consume(program, &token, TOKEN_SET)) {
      parse_set(out, program);
    } else if (consume(program, &token, TOKEN_PUSH)) {
      parse_push(out, program);
    } else if (consume(program, &token, TOKEN_POP)) {
      parse_pop(out, program);
    } else if (consume(program, &token, TOKEN_ADD)) {
      parse_add(out, program);
    } else if (consume(program, &token, TOKEN_SUB)) {
      parse_sub(out, program);
    } else if (consume(program, &token, TOKEN_GOTO)) {
      parse_push(out, program);
    } else if (consume(program, &token, TOKEN_PRINT)) {
      parse_print(out, program);
    } else if (consume(program, &token, TOKEN_EXIT)) {
      parse_exit(out, program);
    } else {
      printf("line %d: unexpected token\n", program->tokens[program->index].line);
      exit(1);
    }
  }
}

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

  token_program_t program;

  program.index = 0;

  program.tokens = NULL;
  program.token_count = 0;

  char buffer[100];
  int buffer_pos = 0;

  int line = 1;
  program.line_start = malloc(line * sizeof(int));
  program.line_start[0] = 0;

  gen_start(output);

  for (int i = 0; i < input_size; i++) {
    char c = input_buffer[i];

    if (c == '#') {
      while (input_buffer[i] != '\n') i++;

      program.line_start = realloc(program.line_start, (line + 1) * sizeof(int));
      program.line_start[line++] = program.token_count;
    } else if (is_valid(c)) {
      buffer[buffer_pos++] = c;
      buffer[buffer_pos] = '\0';
    } else {
      if (buffer_pos) {
        token_t token;
        token.line = line;

        if (is_integer(buffer)) {
          token.type = TOKEN_INTEGER;
          token.value = strtol(buffer, NULL, 0);
        } else if (strlen(buffer) == 1 && *buffer == '$') {
          token.type = TOKEN_STACK;
        } else {
          printf("line %d: invalid token: '%s'\n", line, buffer);
          exit(1);
        }

        program.tokens = realloc(program.tokens, (program.token_count + 1) * sizeof(token_t));
        program.tokens[program.token_count++] = token;

        buffer_pos = 0;
      }

      if (c == '\n') {
        program.line_start = realloc(program.line_start, (line + 1) * sizeof(int));
        program.line_start[line++] = program.token_count;
      }
    }
  }

  free(input_buffer);
  parse(output, &program);

  gen_end(output);

  fclose(output);
  return 0;
}
