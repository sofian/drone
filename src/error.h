#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED

#define LEVEL_NODEBUG -1
#define LEVEL_ERROR   0
#define LEVEL_WARNING 1
#define LEVEL_NOTICE  2

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL LEVEL_ERROR
#endif

// This macro is absolutely NOT SAFE! NEVER USE IT ALONE!!!
// Use ASSERT_ERROR, ASSERT_WARNING and ASSERT_NOTICE instead, below.
#define TRIGGER_ASSERT(expr, func) \
((expr) ? static_cast<void>(0) : func(__STRING(expr)) );

#define ASSERT_ERROR(expr)   TRIGGER_ASSERT(expr, ERROR)
#define ASSERT_WARNING(expr) TRIGGER_ASSERT(expr, WARNING)
#define ASSERT_NOTICE(expr)  TRIGGER_ASSERT(expr, NOTICE)

inline void ERROR(const char* msg, ...);
inline void WARNING(const char* msg, ...);
inline void NOTICE(const char* msg, ...);

void message(const char* msg, ...);
void print(const char* msg, ...);

void errormsg(const char* msg, ...);
void warningmsg(const char* msg, ...);
void noticemsg(const char* msg, ...);

void ERROR(const char* msg, ...)
{
#if DEBUG_LEVEL >= LEVEL_ERROR
  errormsg(msg);
#endif
}

void WARNING(const char* msg, ...)
{
#if DEBUG_LEVEL >= LEVEL_WARNING
  warningmsg(msg);
#endif
}

void NOTICE(const char* msg, ...)
{
#if DEBUG_LEVEL >= LEVEL_NOTICE
  noticemsg(msg);
#endif
}

#endif
