#ifndef __UNIFORM_EXPRESSIONSH
#define __UNIFORM_EXPRESSIONSH 1

struct UniformParserExpressionStruct {
  void (*process)(UniformParser*);
};

extern struct UniformParserModuleStruct UniformParserModule;
extern struct UniformParserExpressionStruct UniformParserExpression;

#endif