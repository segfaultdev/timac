#include <ti_token.h>
#include <ti_parse.h>
#include <stdlib.h>
#include <stdio.h>

void ti_parse_stat(ti_program_t *program, ti_context *context) {

}

void ti_parse_func(ti_program_t *program) {
  if (!ti_peek(program, TOKEN_IDENT)) {
    printf("error: invalid function name\n");
    exit(1);
  }

  ti_token_t ident = ti_consume(program);

  while (ti_peek(program, TOKEN_COMMA)) {
    ti_consume(program);

    if (!ti_peek(program, TOKEN_IDENT)) {
      printf("error: invalid argument name\n");
      exit(1);
    }

    ti_token_t arg = ti_consume(program);
  }

  if (!ti_peek(program, TOKEN_COLON)) {
    printf("error: invalid token on function declaration\n");
    exit(1);
  }

  while (!ti_eof(program)) {
    if (ti_peek(program, TOKEN_END)) {
      ti_consume(program);
      break;
    }

    ti_consume(program);
  }
}

void ti_parse(ti_program_t *program) {
  while (!ti_eof(program)) {
    if (ti_peek(program, TOKEN_FUNC)) {
      ti_consume(program);
      ti_parse_func(program);
    }
  }
}
