#pragma once
#include <stdbool.h>

extern void _ccn_assert_handler(const char *file, const char *function, unsigned int line, 
                                const char *assertion_string, bool assertion_result, const char *format, ...);

#ifndef NDEBUG
#define ccn_assert(assertion, ...) (_ccn_assert_handler(__FILE__, __func__, __LINE__, #assertion, assertion,  __VA_ARGS__))
#define ccn_contract_in(contract) (_ccn_assert_handler(__FILE__, __func__, __LINE__, #contract, contract, "Contract violation _in_"))
#else
#define ccn_assert(assertion, ...) ((void)0)
#define ccn_contract_in(contract) ((void)0)
#endif

