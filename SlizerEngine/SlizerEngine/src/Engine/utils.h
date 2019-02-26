#define SCR_WIDTH 800
#define SCR_HEIGHT 600


#define NON_COPYABLE_CLASS(C) private: \
C( const C& ) = delete; \
C& operator=( const C& ) = delete

#ifdef _DEBUG
#define DEBUG_EXP(exp)              exp
#ifdef _WIN32
#define ASSERT_INVALID()            __debugbreak()
#else
#define ASSERT_INVALID()            { *(int*)0 = 0; }
#endif
#define ASSERT(exp, ...)            if (!(exp)) ASSERT_INVALID()
#define ASSERT_OR_ALERT(exp, ...)   ([&](){ if (!!(exp)) return (exp); ALERT("ALERT", __VA_ARGS__); exit(-1); })()

#else

#define OUTPUT_MSG(...)             void(0)
#define LOG_ERROR(tag, ...)         void(0)
#define LOG_WARNING(tag, ...)       void(0)
#define LOG_INFO(tag, ...)          void(0)
#define ASSERT_INVALID()            void(0)
#define ALERT(...)                  void(0)
#define DEBUG_EXP(exp)
#define ASSERT(exp, ...)            void(0)
#define ASSERT_OR_ALERT(exp,...)    void(0)

#endif
