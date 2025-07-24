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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "sigscan.h"

int a2v(char c) {
    if ((c >= '0') && (c <= '9'))
        return c - '0';
    if ((c >= 'a') && (c <= 'f'))
        return c - 'a' + 10;
    if ((c >= 'A') && (c <= 'F'))
        return c - 'A' + 10;
    else
        return 0;
}

uint8_t unhexlify_byte(char a, char b) {
    uint8_t rv = a2v(a) << 4;
    rv |= a2v(b);
    return rv;
}

void free_processed_signature(PROCESSED_SIGNATURE sig) {
    free(sig.signature);
    free(sig.mask);
}

PROCESSED_SIGNATURE process_signature(SIGNATURE sig) {
    PROCESSED_SIGNATURE proc_sig;

    char* whitespace_removed = (char*)calloc(strlen(sig.signature), 1);

    // remove spaces
    for (int i = 0, c = 0; i < strlen(sig.signature); i++) {
        if (sig.signature[i] == ' ')
            continue;
        else {
            whitespace_removed[c] = sig.signature[i];
            c++; // its not tho
        }
    }

    proc_sig.signature = (uint8_t*)calloc(strlen(sig.signature), 1);
    proc_sig.mask = (char*)calloc(strlen(sig.signature), 1);

    for (int i = 0, c = 0; i < strlen(whitespace_removed); i += 2, c++) {
        if (whitespace_removed[i] == '?') {
            proc_sig.mask[c] = 'x';
            proc_sig.signature[c] = '\xff';
        } else {
            proc_sig.mask[c] = 'o';
            proc_sig.signature[c] = unhexlify_byte(whitespace_removed[i], whitespace_removed[i + 1]);
        }
    }

    free(whitespace_removed);
    return proc_sig;
}

size_t find_processed_signature(size_t start, size_t end, PROCESSED_SIGNATURE sig) {
    uint8_t* reader = (uint8_t*)start;
    for (int i = 0; i < end - start; i++) {
        for (int c = 0;; c++) {
            if ((size_t)reader + i + c >= end)
                break;
            if (c == strlen(sig.mask))
                return (size_t)reader + i;
            if (sig.mask[c] == 'x')
                continue;
            if (sig.signature[c] != reader[i + c]) {
                break;
            }
        }
    }
    return 0;
}

size_t find_signature(size_t start, size_t end, SIGNATURE sig) {
    PROCESSED_SIGNATURE proc_sig = process_signature(sig);
    size_t rv = find_processed_signature(start, end, proc_sig);
    free_processed_signature(proc_sig);
    return rv;
}
