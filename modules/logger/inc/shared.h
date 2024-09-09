#ifndef __UNIFORM_LOGGERH
#define __UNIFORM_LOGGERH 1

#include <stdio.h>
#include <stdarg.h>

#define UNIFORM_LOG_NONE  99
#define UNIFORM_LOG_DEVELOPMENT 6
#define UNIFORM_LOG_FATAL 5
#define UNIFORM_LOG_ERROR 4
#define UNIFORM_LOG_WARN  3
#define UNIFORM_LOG_INFO  2
#define UNIFORM_LOG_DEBUG 1
#define UNIFORM_LOG_ALL   0

typedef struct UniformLoggerModuleStruct {
  int log_level;

  void (*log_dev)(const char*,...);
  void (*log_fatal)(const char*,...);
  void (*log_error)(const char*,...);
  void (*log_warn)(const char*,...);
  void (*log_info)(const char*,...);
  void (*log_debug)(const char*,...);
} UniformLoggerModule;

extern UniformLoggerModule UniformLogger;

#endif