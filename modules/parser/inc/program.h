#ifndef __UNIFORM_PROGRAMPARSERH
#define __UNIFORM_PROGRAMPARSERH 1

#include "uniform/logger/shared.h"

typedef struct UniformParserStruct UniformParser;

struct UniformProgramParserStruct {
  UniformAST* (*process)(UniformParser*);
};

extern struct UniformProgramParserStruct UniformProgramParser;

#endif