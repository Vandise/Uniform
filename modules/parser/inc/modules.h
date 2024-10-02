#ifndef __UNIFORM_MODULESH
#define __UNIFORM_MODULESH 1

#include "uniform/logger/shared.h"

typedef struct UniformParserStruct UniformParser;

struct UniformModuleParserStruct {
  UniformASTNode* (*process)(UniformParser*, UniformASTModuleNode*);
};

extern struct UniformModuleParserStruct UniformModuleParser;

#endif