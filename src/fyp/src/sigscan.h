/**
 * @brief Brief description of the function.
 *
 * Detailed explanation of what the function does, its purpose,
 * and any algorithms or complex logic.
 *
 * @param arg1 Description of the first argument.
 * @param arg2 Description of the second argument.
 * @return Description of the return value.
 */
#ifndef FYP_MODULES_SIGNATURES_H
#define FYP_MODULES_SIGNATURES_H

#include <cstdint>
#include <stddef.h>

int a2v(char c);
char v2a(int c);
char* hexlify(char* bstr);

typedef struct _SIGNATURE {
    char* function_name;
    char* signature;
} SIGNATURE;

typedef struct _PROCESSED_SIGNATURE {
    uint8_t* signature;
    char* mask;
} PROCESSED_SIGNATURE;

void free_processed_signature(PROCESSED_SIGNATURE sig);
PROCESSED_SIGNATURE process_signature(SIGNATURE sig);

size_t find_processed_signature(size_t start, size_t end, PROCESSED_SIGNATURE sig);
size_t find_signature(size_t start, size_t end, SIGNATURE sig);

#endif // FYP_MODULES_SIGNATURES
