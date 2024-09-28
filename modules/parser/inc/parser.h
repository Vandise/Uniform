#ifndef __UNIFORM_PARSERH
#define __UNIFORM_PARSERH 1

#include "uniform/logger/shared.h"
#include "uniform/error/shared.h"
#include "uniform/tokens/shared.h"
#include "uniform/symboltable/shared.h"
#include "uniform/core/shared.h"

typedef struct UniformParserStruct {
  unsigned int token_index;
  UniformSymbolTable* symbol_table;
  UniformTokenArray* token_arr;
} UniformParser;

struct UniformParserModuleStruct {
  UniformParser* (*init)(UniformSymbolTable*, UniformTokenArray*);

  UniformToken* (*get_token)(UniformParser*);
  UniformToken* (*peek)(UniformParser*, int);
  void (*next)(UniformParser*);
  int (*token_in_list)(UNIFORM_TOKEN_CODE, UNIFORM_TOKEN_CODE[]);

  void (*close)(UniformParser*);
};

extern struct UniformParserModuleStruct UniformParserModule;

#endif