#ifndef __UNIFORM_ERRORUTILH
#define __UNIFORM_ERRORUTILH 1

#include <stdio.h>
#include <stdarg.h>

typedef enum {
  UNIFORM_UNDEFINED_ERROR,
  UNIFORM_FILE_NOT_FOUND,
  UNIFORM_SYNTAX_ERROR,
  UNIFORM_MACRO_SYNTAX_ERROR,
  UNIFORM_INVALID_MACRO_ERROR,
  UNIFORM_MISSING_CORE_LIBRARY_ERROR
} UNIFORM_ERROR_CODE;

typedef struct UniformErrorUtilModuleStruct {
  int print_errors;

  void (*trace_error)(
    UNIFORM_ERROR_CODE code,
    const char* filename,
    unsigned int line_number,
    unsigned int buffer_offset,
    ...
  );

} UniformErrorUtilModule;

extern UniformErrorUtilModule UniformErrorUtil;

#endif