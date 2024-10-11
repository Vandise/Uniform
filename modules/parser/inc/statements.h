#ifndef __UNIFORM_STATEMENTSH
#define __UNIFORM_STATEMENTSH 1

#include "uniform/logger/shared.h"

typedef struct UniformParserStruct UniformParser;

struct UniformParserStatementStruct {
  UniformASTNode* (*process)(UniformParser*, UniformSymbolTableNode*);
};

extern struct UniformParserStatementStruct UniformParserStatement;

#endif