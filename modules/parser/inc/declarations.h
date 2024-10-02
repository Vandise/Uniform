#ifndef __UNIFORM_DECLARATIONH
#define __UNIFORM_DECLARATIONH 1

#include "uniform/logger/shared.h"

typedef struct UniformParserStruct UniformParser;

struct UniformParserDeclarationStruct {
  UniformASTNode* (*process)(UniformParser*, UniformASTModuleNode*);
};

extern struct UniformParserDeclarationStruct UniformParserDeclaration;

#endif