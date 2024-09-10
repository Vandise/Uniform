#include "uniform/scanner/shared.h"

// ============================
//          Forwards
// ============================

static UniformScanner* init(const char *file_name);
static int open_source_file(UniformScanner *scanner);
static int get_source_line(UniformScanner *scanner);
static void get_character(UniformScanner *scanner);
static void skip_blanks(UniformScanner* scanner);
static void get_special(UniformScanner *scanner);
static void get_word(UniformScanner* scanner, int is_constant);
static void get_string(UniformScanner* scanner);
static void get_numeric(UniformScanner* scanner);
static void accumulate_value(UniformScanner *scanner, double *valuep);

// ============================
//        Implementation
// ============================

static UniformScanner* init(const char *file_name) {
  errno = 0;

  UniformScanner* scanner = malloc(sizeof(UniformScanner));
  strcpy(scanner->source_name, file_name);
  strcpy(scanner->source_buffer, "\0");

  scanner->current_char = '\0';
  scanner->source_bufferp = NULL;
  scanner->line_number = 0;
  scanner->level = 0;
  scanner->buffer_offset = 0;
  scanner->errored = 0;

  if (open_source_file(scanner) == 0) {
    // todo: Fatal error with errors module.
    UniformLogger.log_fatal("Scanner::get_source_line(error: unable to open source file %s)", file_name);
    scanner->errored = 1;
    return scanner;
  }

  for (int ch = 0; ch < 256; ++ch) {
    scanner->char_table[ch] = UNDEFINED_CHAR_CODE;
  }

  return scanner;
}

static int open_source_file(UniformScanner *scanner) {
  UniformLogger.log_info("Scanner::open_source_file(file: %s)", scanner->source_name);

  scanner->source_file = fopen(scanner->source_name, "r");

  if (errno != 0) { return 0; }

  scanner->source_bufferp = scanner->source_buffer;

  return 1;
}

static int get_source_line(UniformScanner *scanner) {
  UniformLogger.log_info("Scanner::get_source_line");

  if (scanner != NULL && fgets(scanner->source_buffer, UNIFORM_SCANNER_MAX_SOURCE_LINE, scanner->source_file) != NULL) {
    scanner->line_number += 1;
    return 1;
  }
  return 0;
}

static void get_character(UniformScanner *scanner) {
  UniformLogger.log_debug("Scanner::get_character");

  if (*(scanner->source_bufferp) == '\0') {
    if (!get_source_line(scanner)) {
      UniformLogger.log_debug("Scanner::get_character(eof char: 1)");
      scanner->current_char = EOF_CHAR;
      return;
    }

    scanner->source_bufferp = scanner->source_buffer;
    scanner->buffer_offset = 0;
  }

  scanner->current_char = *(scanner->source_bufferp)++;

  UniformLogger.log_debug("Scanner::get_character(current char: %c)", scanner->current_char);

  switch(scanner->current_char) {
    // todo: skip comments
    default:
      scanner->buffer_offset += 1;
  }
}

static void skip_blanks(UniformScanner* scanner) {
  while(scanner->current_char == ' ') {
    get_character(scanner);
  }
}

static void get_token(UniformScanner* scanner) {
  UniformLogger.log_debug("Scanner::get_token(current char: %c)", scanner->current_char);

  if (scanner->current_char == '\0') {
    get_character(scanner);
  }

  skip_blanks(scanner);

  scanner->current_token.tokenp = scanner->current_token.token_string;

  switch(scanner->char_table[scanner->current_char]) {
    case LETTER_CHAR_CODE:
      get_word(scanner, 0);
      break;
    case UPPERCASE_LETTER_CHAR_CODE:
      get_word(scanner, 1);
      break;
    case NEWLINE_CHAR_CODE:
      scanner->current_token.code = T_NEWLINE;
      *(scanner->current_token.tokenp) = '\0';
      get_character(scanner);
      break;
    case CARRIAGE_RETURN_CHAR_CODE:
      scanner->current_token.code = T_NEWLINE;
      get_character(scanner);
      get_character(scanner);
      *(scanner->current_token.tokenp) = '\0';
      break;
    case DIGIT_CHAR_CODE:
      get_numeric(scanner);
      break;
    case QUOTE_CHAR_CODE:
      get_string(scanner);
      break;
    case EOF_CHAR_CODE:
      scanner->current_token.code = T_END_OF_FILE;
      break;
    default: get_special(scanner);
  }
}

static void get_word(UniformScanner* scanner, int is_constant) {
  UniformLogger.log_info("Scanner::get_word(is constant: %d)", is_constant);

  while(
    scanner->char_table[scanner->current_char] == LETTER_CHAR_CODE ||
    scanner->char_table[scanner->current_char] == UPPERCASE_LETTER_CHAR_CODE ||
    scanner->char_table[scanner->current_char] == UNDERSCORE_CHAR_CODE
  ) {
    *(scanner->current_token.tokenp)++ = scanner->current_char;
    get_character(scanner);
  }

  *(scanner->current_token.tokenp) = '\0';

  UniformLogger.log_info("Scanner::get_word(current word: %s)", scanner->current_token.token_string);

  if(!UniformTokenModule.string_is_reserved_word(scanner->current_token.token_string)) {
    if (is_constant) {
      scanner->current_token.code = T_CONSTANT;
    } else {
      scanner->current_token.code = T_IDENTIFIER;
    }
  } else {
    scanner->current_token.code = UniformTokenModule.get_token_code(scanner->current_token.token_string);
  }
}

static void get_string(UniformScanner* scanner) {
  UniformLogger.log_info("Scanner::get_string");

  char* literalp = scanner->current_token.literal.value.string;
  *(scanner->current_token.tokenp)++ = scanner->current_char == '"' ? '"' : '\'';

  get_character(scanner);

  while(scanner->current_char != EOF_CHAR_CODE) {
    if(scanner->current_char == '\'' || scanner->current_char == '"') {
      *(scanner->current_token.tokenp)++ = scanner->current_char;
      get_character(scanner);
      if(scanner->current_char != '\'' && scanner->current_char != '"') { break; }
    }

    *(scanner->current_token.tokenp)++ = scanner->current_char;
    *literalp++ = scanner->current_char;
    get_character(scanner);
  }

  UniformLogger.log_info(
    "Scanner::get_string(string: %s, size: %d)",
    scanner->current_token.literal.value.string,
    strlen(scanner->current_token.literal.value.string)
  );

  *(scanner->current_token.tokenp) = '\0';
  *literalp = '\0';

  scanner->current_token.code = T_STRING;
  scanner->current_token.literal.type = STRING_LIT;
  scanner->current_token.literal.size = strlen(scanner->current_token.literal.value.string);
}

static void get_numeric(UniformScanner* scanner) {
  UniformLogger.log_info("Scanner::get_numeric");

  // assume larger unsigned 8 bytes
  long int whole_count = 0;
  long int decimal_offset = 0;
  long int exponent = 0;
  double nvalue = 0.0;
  double evalue = 0.0;

  scanner->digit_count = 0;
  scanner->current_token.code = UNDEFINED_TOKEN;
  scanner->current_token.literal.type = I_64LIT;

  accumulate_value(scanner, &nvalue);

  if (scanner->errored == 1) {
    return;
  }

  whole_count = scanner->digit_count;

  if (scanner->current_char == '.') {
    get_character(scanner);

    scanner->current_token.literal.type = F_64LIT;
    *(scanner->current_token.tokenp)++ = '.';

    accumulate_value(scanner, &nvalue);

    if (scanner->errored == 1) {
      return;
    }

    decimal_offset = whole_count - scanner->digit_count;
  }

  exponent = evalue + decimal_offset;

  if (exponent != 0) {
    nvalue *= pow(10, exponent);
  }

  if (scanner->current_token.literal.type == I_64LIT) {
    if (nvalue >= I_32_LOWER && nvalue <= I_32_UPPER) {
      scanner->current_token.literal.type = I_32LIT;
      scanner->current_token.literal.value.i32 = nvalue;
      scanner->current_token.literal.size = sizeof(int);
    } else {
      scanner->current_token.literal.value.i64 = nvalue;
      scanner->current_token.literal.size = sizeof(long int);
    }
  } else {
    if (labs(exponent) <= FLT_DIG && nvalue >= FLT_MIN && nvalue <= FLT_MAX) {
      scanner->current_token.literal.type = F_32LIT;
      scanner->current_token.literal.value.f32 = nvalue;
      scanner->current_token.literal.size = sizeof(float);
    } else {
      scanner->current_token.literal.value.f64 = nvalue;
      scanner->current_token.literal.size = sizeof(double);
    }
  }

  *(scanner->current_token.tokenp) = '\0';
  scanner->current_token.code = T_NUMERIC;
}

static void accumulate_value(UniformScanner *scanner, double *valuep) {
  double value = *valuep;

  if (scanner->char_table[scanner->current_char] != DIGIT_CHAR_CODE) {
    scanner->current_token.code = T_ERROR;
    scanner->errored = 1;
    // invalid numeric
    return;
  }

  do {
    *(scanner->current_token.tokenp)++ = scanner->current_char;

    if (++scanner->digit_count <= DECIMAL_DIG) {
      value = 10 * value + (scanner->current_char - '0');
    } else {
      // todo: precision lost
      scanner->errored = 1;
    }

    get_character(scanner);

  } while(scanner->char_table[scanner->current_char] == DIGIT_CHAR_CODE);

  *valuep = value;
}

static void get_special(UniformScanner *scanner) {
  UniformLogger.log_info("Scanner::get_special(current character: %c)", scanner->current_char);

  *(scanner->current_token.tokenp) = scanner->current_char;

  switch(scanner->current_char) {
    case '@':   scanner->current_token.code = T_MACRO;  get_character(scanner);  break;
    case '.':   scanner->current_token.code = T_DOT;    get_character(scanner);  break;
    case '=':   scanner->current_token.code = T_EQUAL;  get_character(scanner);  break;
    case '{':   scanner->current_token.code = T_OPEN_CURLY_BRACE;   get_character(scanner);  break;
    case '}':   scanner->current_token.code = T_CLOSE_CURLY_BRACE;  get_character(scanner);  break;
    case '[':   scanner->current_token.code = T_OPEN_BRACKET;       get_character(scanner);  break;
    case ']':   scanner->current_token.code = T_CLOSE_BRACKET;      get_character(scanner);  break;
    case '+':   scanner->current_token.code = T_PLUS;      get_character(scanner);  break;
    case '-':
      scanner->current_token.code = T_MINUS;
      get_character(scanner);
      if (scanner->current_char == '>') {
        scanner->current_token.code = T_LAMBDA;
        get_character(scanner);
      }
      break;
    case '*':   scanner->current_token.code = T_STAR;      get_character(scanner);  break;
    case '/':   scanner->current_token.code = T_SLASH;     get_character(scanner);  break;
    case '^':   scanner->current_token.code = T_PIN;       get_character(scanner);  break;
    case '%':   scanner->current_token.code = T_PERCENT;   get_character(scanner);  break;
    case ':':   scanner->current_token.code = T_COLON;     get_character(scanner);  break;
    case ';':   scanner->current_token.code = T_SEMICOLON; get_character(scanner);  break;
    case ',':   scanner->current_token.code = T_COMMA;     get_character(scanner);  break;
    case '?':   scanner->current_token.code = T_QUESTION;  get_character(scanner);  break;
    case '!':   scanner->current_token.code = T_BANG;      get_character(scanner);  break;
    case '(':   scanner->current_token.code = T_OPEN_PAREN;  get_character(scanner);  break;
    case ')':   scanner->current_token.code = T_CLOSE_PAREN; get_character(scanner);  break;
    case '|':
      scanner->current_token.code = T_PIPE;
      get_character(scanner);
      if (scanner->current_char == '>') {
        scanner->current_token.code = T_PIPE_OPERATOR;
        get_character(scanner);
      }
      break;
    case '>':   scanner->current_token.code = T_GREATER_THAN;  get_character(scanner);  break;
    case '<':   scanner->current_token.code = T_LESS_THAN;     get_character(scanner);  break;
    case '~':   scanner->current_token.code = T_TILDE;         get_character(scanner);  break;
    default:
      UniformLogger.log_info("Scanner::get_special(status: errored)");
      scanner->errored = 1;
      scanner->current_token.code = T_ERROR;
  }

  *(scanner->current_token.tokenp) = '\0';
}

static void close(UniformScanner *scanner) {
  UniformLogger.log_info("Scanner::close(file: %s)", scanner->source_name);

  if (scanner->source_file != NULL) {
    fclose(scanner->source_file);
    scanner->source_file = NULL;
  }

  free(scanner);
}

// ============================
//            Module
// ============================

const struct UniformScannerModuleStruct UniformScannerModule = {
  .version = UNIFORM_SCANNER_VERSION,
  .init = init,
  .get_token = get_token,
  .close = close
};