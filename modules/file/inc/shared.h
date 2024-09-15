#ifndef __UNIFORM_FILEUTILH
#define __UNIFORM_FILEUTILH 1

#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define FILE_PATH_MAX 1024
#define FILE_PATH_DELIMITER "/" // todo: windows

typedef struct UniformFileUtilModuleStruct {
  char* (*get_file_path)(const char*);
} UniformFileUtilModule;

extern UniformFileUtilModule UniformFileUtil;

#endif