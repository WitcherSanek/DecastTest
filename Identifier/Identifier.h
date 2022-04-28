#ifndef DECASTMETRONIC_H
#define DECASTMETRONIC_H

#define MAX_IDENTIFIER_PIECES 10
#define IDENTIFIER_LENGTH 3

typedef enum {
    STATUS_SUCCESS,
    STATUS_WRONG_SEQUENCE,
    STATUS_OVERFLOW
} status_e;

/// <summary>
/// 'Increments' valid identifier according to rules, returns error code otherwise
/// </summary>
/// <param name="identifier"> Pointer to null-terminated identifier </param>
/// <param name="outputIdentifier"> Pointer to output buffer sufficient for fitting 'incremented' identifier </param>
/// <returns> Error code if identifier is invalid or too big</returns>
status_e IncrementIdentifier(char* identifier, char* outputIdentifier);

#endif