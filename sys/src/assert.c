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
  g_assert_info.file = file;
  g_assert_info.line = line;
  g_assert_info.func = func;
  g_assert_info.failed_expr = failedexpr;
  __asm("bkpt 1");
  while(1);
}
