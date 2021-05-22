#ifndef __TI_TOKEN_H__
#define __TI_TOKEN_H__

#define TOKEN_ERROR     -1

#define TOKEN_INT        0
#define TOKEN_IDENT      1

#define TOKEN_COMMA      2
#define TOKEN_COLON      3
#define TOKEN_SEMICOLON  4

#define TOKEN_FUNC       5
#define TOKEN_END        6

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

  // For token consuming and stuff
  int token_pos;
};

int ti_eof(ti_program_t *program);

int ti_token_type(const char *token);
void ti_tokenize(ti_program_t *program, const char *code);

int ti_peek(ti_program_t *program, int type);
ti_token_t ti_consume(ti_program_t *program);

#endif
