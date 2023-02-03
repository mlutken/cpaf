#ifndef UTILS_H
#define UTILS_H

#include <QObject>

#define EVBASE_EXPORT

namespace evbase {
#ifdef _WIN32
#define __STRING(expr) expr
#endif

    // note: assertFail() is explicitly marked with the [[noreturn]]
    // attribute for two reasons:
    // 1. To avoid false positives from the Clang Static Analyzer.
    // 2. Allow the compiler to optimize based on the knowledge that
    //    the function never returns.
    extern "C" // Clang needs this in order to use __attribute__((noreturn))
#ifdef _WIN32
    __declspec(noreturn) // Visual Studio version of the noreturn attribute
#endif
    EVBASE_EXPORT void assertFail(const char *file, int line, const char *condition, const char *message = nullptr)
#ifndef WIN32
        __attribute__((noreturn)) // GCC and Clang version of the noreturn attribute
#endif
        ;

    /** \def EV_ASSERT(condition)
     *  \brief Assertion macro.
     *
     * \param condition Boolean expression to be evaluated.
     *                  If the expression is false then a message is printed
     *                  to the global logfile (evbase::logfile) and the
     *                  program is aborted.
     */
#ifndef NDEBUG
#define EV_ASSERT(condition)                                            \
    do { if (!(condition))                                              \
             evbase::assertFail(__FILE__, __LINE__, __STRING(#condition)); \
    } while (false)
#define EV_ASSERT_MSG(condition, message)                               \
    do { if (!(condition))                                              \
            evbase::assertFail(__FILE__, __LINE__, __STRING(#condition), message); \
    } while (false)
#else
#define EV_ASSERT(condition) do {} while (false)
#define EV_ASSERT_MSG(condition, message) do {} while (false)
#endif // NDEBUG
} // end namespace


// Macro which connects a signal to a slot and which causes the
// application to abort if the connection fails.
#define EV_CONNECT(source, signal, receiver, slot)               \
    if (!QObject::connect(source, signal, receiver, slot))       \
        EV_ASSERT_MSG(false, Q_FUNC_INFO);


#endif // UTILS_H

