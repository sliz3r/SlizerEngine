#define SE_ERROR -1
#define SE_CONTINUE 0

#ifdef _DEBUG
#define OUTPUT_MSG(...)             Framework::Debug::Internal::OutputConsolePrint(__VA_ARGS__)
#define LOG_ERROR(tag, ...)         Framework::Editor::ConsoleLog(RGBAColor::Red, true, "error", tag, __VA_ARGS__)
#define LOG_WARNING(tag, ...)       Framework::Editor::ConsoleLog(RGBAColor::Yellow, true, "warning", tag, __VA_ARGS__)
#define LOG_INFO(tag, ...)          Framework::Editor::ConsoleLog(RGBAColor::White, true, "info", tag, __VA_ARGS__)
#define ALERT(tag, ...)             Framework::Debug::Internal::ShowAlertPopup(tag, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
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
