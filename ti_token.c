#include <ti_token.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

int ti_eof(ti_program_t *program) {
  return (program->token_pos >= program->token_cnt);
}

int ti_token_type(const char *token) {
  if (!token) return TOKEN_ERROR;
  if (!(*token)) return TOKEN_ERROR;

  // Check if punctuation

  if (!strcmp(token, ",")) return TOKEN_COMMA;
  if (!strcmp(token, ":")) return TOKEN_COLON;
  if (!strcmp(token, ";")) return TOKEN_SEMICOLON;

  // Check if keyword

  if (!strcmp(token, "func")) return TOKEN_FUNC;
  if (!strcmp(token, "end"))  return TOKEN_END;

  // Check if integer of any base

  char *last_valid;
  strtol(token, &last_valid, 0);

  if (last_valid >= token + strlen(token))
    return TOKEN_INT;

  if (last_valid >= token + (strlen(token) - 1) && token[strlen(token) - 2] == '0' && token[strlen(token) - 1] == 'x')
    return TOKEN_INT;

  // Check if valid identifier

  if (!isalpha(*token) && *token != '_') return TOKEN_ERROR;

  for (int i = 1; token[i]; i++) {
    if (!isalnum(token[i]) && token[i] != '_')
      return TOKEN_ERROR;
  }

  return TOKEN_IDENT;
}

void ti_tokenize(ti_program_t *program, const char *code) {
  program->tokens = NULL;
  program->token_cnt = 0;

  program->token_pos = 0;

  program->lines = malloc(sizeof(char *));
  program->lines[0] = calloc(1, 1);
  program->line_cnt = 1;

  int line_len = 0;

  char *token_ptr = calloc(1, 1);
  int token_len = 0;

  int prev_type = TOKEN_ERROR;

  for (int i = 0; code[i]; i++) {
    if (code[i] == '\n') {
      program->lines = realloc(program->lines, (program->line_cnt + 1) * sizeof(char *));
      program->lines[program->line_cnt++] = calloc(1, 1);

      line_len = 0;
    } else {
      program->lines[program->line_cnt - 1] = realloc(program->lines[program->line_cnt - 1], line_len + 2);

      program->lines[program->line_cnt - 1][line_len++] = code[i];
      program->lines[program->line_cnt - 1][line_len] = '\0';
    }
  }

  for (int i = 0; code[i]; i++) {
    char *token_tmp = calloc(1, token_len + 2);
    strcpy(token_tmp, token_ptr);

    token_tmp[token_len + 0] = code[i];
    token_tmp[token_len + 1] = '\0';

    int curr_type = ti_token_type(token_tmp);

    if (curr_type == TOKEN_ERROR && prev_type != TOKEN_ERROR) {
      ti_token_t token;
      token.type = prev_type;
      token.line = 0;
      token.column = 0;
      token.text = token_ptr;

      program->tokens = realloc(program->tokens, (program->token_cnt + 1) * sizeof(ti_token_t));
      program->tokens[program->token_cnt++] = token;

      token_ptr = calloc(1, 1);
      token_len = 0;

      i--;
    } else if (!isspace(code[i])) {
      free(token_ptr);

      token_ptr = token_tmp;
      token_len++;
    }

    prev_type = curr_type;
  }

  for (int i = 0; i < program->token_cnt; i++) {
    printf("['%s', %d] ", program->tokens[i].text, program->tokens[i].type);
  }

  printf("\n");
}

int ti_peek(ti_program_t *program, int type) {
  if (program->token_pos >= program->token_cnt)
    return 0;

  return program->tokens[program->token_pos].type == type;
}

ti_token_t ti_consume(ti_program_t *program) {
  if (program->token_pos >= program->token_cnt)
    return (ti_token_t){TOKEN_ERROR, 0, NULL, 0, 0};

  return program->tokens[program->token_pos++];
}
