#include <ti_token.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

int ti_token_type(const char *token, int length) {
  if (!token) return TOKEN_ERROR;
  if (!length) return TOKEN_ERROR;
  if (strlen(token) < length) return TOKEN_ERROR;

  // Check if integer of any base

  char *last_valid;
  strtol(token, &last_valid, 0);

  if (last_valid >= token + length)
    return TOKEN_INT;

  // Check if valid identifier

  if (!isalpha(*token) && *token != '_') return TOKEN_ERROR;

  for (int i = 1; i < length; i++) {
    if (!isalnum(token[i]) && token[i] != '_')
      return TOKEN_ERROR;
  }

  return TOKEN_IDENT;
}

void ti_tokenize(const char *code, ti_program_t *program) {
  for (int i = 0; )
}
