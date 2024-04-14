#include "ccngen/equation.h"
#include "ccngen/visit.h"
#include "palm/dbug.h"

#ifdef __GNUC__
#define MAYBE_UNUSED __attribute__((unused))
#else
#define MAYBE_UNUSED
#endif

#if defined(__clang__)
#pragma clang diagnostic push
// Ignore unused parameters
#pragma clang diagnostic ignored "-Wunused-parameter"
// Ignore uninitialized variable (already handled by assert)
#pragma clang diagnostic ignored "-Wsometimes-uninitialized"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
// Ignore unused parameters
#pragma GCC diagnostic ignored "-Wunused-parameter"
#ifdef NDEBUG
// We do not initialize variables in unreachable cases, which are guarded
// by assertions in debug builds. We ignore this on release builds.
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

