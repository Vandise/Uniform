#include "uniform/preprocessor/shared.h"

// ============================
//          Forwards
// ============================
static void emit_token(UniformPreprocessor* preprocessor, UniformScanner* scanner);
// ============================
//        Implementation
// ============================

static void emit_token(UniformPreprocessor* preprocessor, UniformScanner* scanner) {
  UniformLogger.log_info("Preprocessor::emit_token(token: %d, string: %s)", scanner->current_token.code, scanner->current_token.token_string);

  switch(scanner->current_token.code) {
    case UNDEFINED_TOKEN:
    case T_ERROR:
    case T_MACRO:
      break;
    case T_END_OF_FILE:
    case T_NEWLINE:
      EMIT_TOKEN("\n")
      break;
    case T_STRING:
      EMIT_TOKEN(scanner->current_token.token_string)
      break;
    case T_MODULE:
    case T_END:
    case T_STRUCT:
    case T_FUNC:
    case T_CASE:
    case T_RETURN:
    case T_IDENTIFIER:
    case T_CONSTANT:
    case T_NUMERIC:
      EMIT_TOKEN(scanner->current_token.token_string)
      EMIT_TOKEN(" ")
      break;
    case T_DOT:
      EMIT_TOKEN(".")
      break;
    case T_EQUAL:
      EMIT_TOKEN("=")
      break;
    case T_OPEN_CURLY_BRACE:
      EMIT_TOKEN("{")
      break;
    case T_CLOSE_CURLY_BRACE:
      EMIT_TOKEN("}")
      break;
    case T_OPEN_BRACKET:
      EMIT_TOKEN("[")
      break;
    case T_CLOSE_BRACKET:
      EMIT_TOKEN("]")
      break;
    case T_OPEN_PAREN:
      EMIT_TOKEN("(")
      break;
    case T_CLOSE_PAREN:
      EMIT_TOKEN(")")
      break;
    case T_PLUS:
      EMIT_TOKEN("+")
      break;
    case T_MINUS:
      EMIT_TOKEN("-")
      break;
    case T_STAR:
      EMIT_TOKEN("*")
      break;
    case T_SLASH:
      EMIT_TOKEN("/")
      break;
    case T_PIN:
      EMIT_TOKEN("^")
      break;
    case T_COLON:
      EMIT_TOKEN(":")
      break;
    case T_SEMICOLON:
      EMIT_TOKEN(";")
      break;
    case T_COMMA:
      EMIT_TOKEN(",")
      break;
    case T_QUESTION:
      EMIT_TOKEN("?")
      break;
    case T_BANG:
      EMIT_TOKEN("!")
      break;
    case T_PIPE:
      EMIT_TOKEN("|")
      break;
    case T_GREATER_THAN:
      EMIT_TOKEN(">")
      break;
    case T_LESS_THAN:
      EMIT_TOKEN("<")
      break;
    case T_TILDE:
      EMIT_TOKEN("~")
      break;
    case T_PERCENT:
      EMIT_TOKEN("%")
      break;
    case T_PIPE_OPERATOR:
      EMIT_TOKEN("|>")
      break;
    case T_LAMBDA:
      EMIT_TOKEN("->")
      break;
    default:
      break;
  }
}

// ============================
//            Module
// ============================

struct UniformTokenEmitterModuleStruct UniformTokenEmitterModule = {
  .emit_token = emit_token
};