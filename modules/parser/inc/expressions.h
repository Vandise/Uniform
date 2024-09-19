#ifndef __UNIFORM_EXPRESSIONSH
#define __UNIFORM_EXPRESSIONSH 1

typedef struct UniformParserStruct UniformParser;

struct UniformParserExpressionStruct {
  void (*process)(UniformParser*);
};

extern struct UniformParserExpressionStruct UniformParserExpression;

#endif