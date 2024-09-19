#include "uniform/parser/shared.h"

// ============================
//          Forwards
// ============================

static void process(UniformParser* parser);

// ============================
//        Implementation
// ============================

static void process(UniformParser* parser) {
  
}

// ============================
//            Module
// ============================

struct UniformParserExpressionStruct UniformParserExpression = {
  .process = process
};