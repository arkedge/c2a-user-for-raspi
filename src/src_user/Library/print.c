/**
 * @file
 * @brief  HW 依存 Print, つまり ログ情報のフォーマット出力（デバッグ出力）
 */
// こいつもBootLoaderに配置するので，リプロ可能領域のものをincludeしないこと！！

// printはHW依存性が強いため，ヘッダーはcoreにおき，オーバーライドする
// ビルド対象にて選択する
#include <src_core/Library/print.h>

#include <stdio.h>
#include <stdarg.h>

#include "../Settings/sils_define.h"

// バッファサイズよりでかい文字列が来ると死ぬ
static char PRINT_buffer_[512];

#ifndef SILS_FW

#include <src_core/System/WatchdogTimer/watchdog_timer.h>

void Printf(const char* format, ...)
{
  char printf_buffer[512];   // バッファサイズよりでかい文字列が来ると死ぬ（が，これは実機環境でも同じか）
  va_list args;
  va_start(args, format);
  vsprintf(printf_buffer, format, args);

  printf("%s", printf_buffer);
  fflush(stdout);

  va_end(args);
}

#else

void Printf(const char* format, ...)
{
#ifdef SHOW_DEBUG_PRINT_ON_SILS
  va_list args;
  va_start(args, format);
  vsprintf(PRINT_buffer_, format, args);

  printf("%s", PRINT_buffer_);
  fflush(stdout);

  va_end(args);
#else
  // なにも表示しない
#endif
}

#endif
