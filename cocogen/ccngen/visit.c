#include "palm/dbug.h"

#include "ccngen/equation.h"
#include "ccngen/visit.h"

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
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

