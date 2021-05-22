#ifndef __TI_PARSE_H__
#define __TI_PARSE_H__

#include <ti_token.h>

typedef struct ti_context ti_context;

struct ti_context {

};


void ti_parse_stat(ti_program_t *program, ti_context *context);
void ti_parse_func(ti_program_t *program);

void ti_parse(ti_program_t *program);

#endif
