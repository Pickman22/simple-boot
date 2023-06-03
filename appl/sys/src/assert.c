#include "dbg.h"

typedef struct {
  const char* file;
  int line;
  const char* func;
  const char* failed_expr;
} assert_info_t;

volatile assert_info_t g_assert_info = {0};

void __assert_func(const char *file,
                   int line,
                   const char *func,
                   const char *failedexpr)
{
  DBG_ERR("%s:%s - %s: %s\n\r", file, line, func, failedexpr);
  g_assert_info = (assert_info_t) {
    .file = file,
    .line = line,
    .func = func,
    .failed_expr = failedexpr,
  };
  __asm("bkpt 1");
  while(1);
}
