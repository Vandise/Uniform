#ifndef __UNIFORM_EXPRESSIONSH
#define __UNIFORM_EXPRESSIONSH 1

#include "uniform/logger/shared.h"

typedef struct UniformParserStruct UniformParser;

struct UniformParserExpressionStruct {
  UniformASTExpressionNode* (*process)(UniformParser*, UniformASTExpressionNode*, UniformSymbolTableNode*);
};

extern struct UniformParserExpressionStruct UniformParserExpression;

#endif