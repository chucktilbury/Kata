#ifndef TRACE_H
#define TRACE_H

#ifdef TRACE
void message(int level, const char* file, const char* func, int line, const char* fmt, ...);
void set_msg_level(int level);
#define MSG(f, ...) message(1, __FILE__, __func__, __LINE__, (f), ##__VA_ARGS__)
#define LOG(v, f, ...) message((v), __FILE__, __func__, __LINE__, (f), ##__VA_ARGS__)
#define ENTER message(1, __FILE__, __func__, __LINE__, "enter")
#define LEAVE message(1, __FILE__, __func__, __LINE__, "leave")
#define LEVEL(v) set_msg_level(v)
#else
#define MSG(f, ...)
#define LOG(v, f, ...)
#define LEVEL(v)
#define ENTER
#define LEAVE
#endif

#endif
