#ifndef __TI_TOK_H__

#define TOKEN_ERROR -1
#define TOKEN_INT    0
#define TOKEN_IDENT  1

typedef struct ti_token_t ti_token_t;
typedef struct ti_program_t ti_program_t;

struct ti_token_t {
  int type, value;
  char *text;

  int line, column;
};

struct ti_program_t {
  ti_token_t *tokens;
  int token_cnt;

  char **lines;
  int line_cnt;
};

int ti_token_type(const char *token, int length);
void ti_tokenize(const char *code, ti_program_t *program);

#endif
