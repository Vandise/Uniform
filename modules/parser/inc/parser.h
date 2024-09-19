#ifndef __UNIFORM_PARSERH
#define __UNIFORM_PARSERH 1

#include "uniform/error/shared.h"
#include "uniform/scanner/tokens.h"
#include "uniform/symboltable/shared.h"

typedef struct UniformParserStruct {
  unsigned int token_index;

  UniformTokenArray* token_arr;
} UniformParser;

struct UniformParserModuleStruct {
  UniformParser* (*init)(UniformTokenArray*);

  UniformToken* (*get_token)(UniformParser*);
  UniformToken* (*peek)(UniformParser*, int);
  void (*next)(UniformParser*);

  void (*close)(UniformParser*);
} UniformParserModule;

extern struct UniformParserModuleStruct UniformParserModule;

#endif