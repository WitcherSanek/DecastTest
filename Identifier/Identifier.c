// Identifier.c : This file contains implementation of identifier. 
// Main idea: transform identifier into array of numbers, and transform it back after increment.
// 'B'..'Z'=25 symbols, '1'..'9'=9 symbols, so we can transform every piece of identifier into numbers from 1 to 225 (because 225 = 25 * 9).
//

#include <stdint.h>

#include "Identifier.h"

#define FIRST_LETTER_FOR_FIRST_SYMBOL  'B'
#define LAST_LETTER_FOR_FIRST_SYMBOL   'Z'
#define FIRST_LETTER_FOR_SECOND_SYMBOL '1'
#define LAST_LETTER_FOR_SECOND_SYMBOL  '9' 

#define NUMBER_OF_FIRST_SYMBOLS (LAST_LETTER_FOR_FIRST_SYMBOL - FIRST_LETTER_FOR_FIRST_SYMBOL + 1)
#define NUMBER_OF_SECOND_SYMBOLS (LAST_LETTER_FOR_SECOND_SYMBOL - FIRST_LETTER_FOR_SECOND_SYMBOL + 1)
#define MAX_IDENTIFIER_VALUE (NUMBER_OF_FIRST_SYMBOLS * NUMBER_OF_SECOND_SYMBOLS)

/// <summary>
/// Transforms piece of identifier to number. "B1"->1, "B2"->2... "C1"->10,..."Z9"->225
/// </summary>
/// <param name="identifier"> Pointer to the beginning of valid identifier piece</param>
/// <returns> 1..225 for valid identifier pieces </returns>
static uint8_t IdentifierPieceToNumber(char* identifier)
{
    return NUMBER_OF_SECOND_SYMBOLS * (identifier[0] - FIRST_LETTER_FOR_FIRST_SYMBOL) 
                                    + (identifier[1] - FIRST_LETTER_FOR_SECOND_SYMBOL + 1);
}

/// <summary>
/// Transforms number to piece of identifier and appends it to pointed identifier
/// </summary>
/// <param name="number"> Number to be transformed, 1..225</param>
/// <param name="buf"> Pointed identifier </param>
static void AppendNumberToIdentifier(uint8_t number, char* buf)
{
    number--;
    buf[0] = (number / NUMBER_OF_SECOND_SYMBOLS) + FIRST_LETTER_FOR_FIRST_SYMBOL;
    buf[1] = (number % NUMBER_OF_SECOND_SYMBOLS) + FIRST_LETTER_FOR_SECOND_SYMBOL;
}

/// <summary>
/// Transforms whole identifier into array of numbers
/// </summary>
/// <param name="identifier"> Pointer to the beginning of valid identifier </param>
/// <param name="numberArray"> Pointer to array of numbers which will contain transformed identifier</param>
static void IdentifierToNumberArray(char* identifier, uint8_t* numberArray)
{
    int offset = 0;
    while (identifier[offset * IDENTIFIER_LENGTH] != 0)
    {
        numberArray[offset] = IdentifierPieceToNumber(&identifier[offset * IDENTIFIER_LENGTH]);
        offset++;
    }
}

/// <summary>
/// Transforms valid array of numbers to complete identifier
/// </summary>
/// <param name="numberArray"> Pointer to array of numbers </param>
/// <param name="identifier"> Pointer to output buffer </param>
static void NumberArrayToIdentifier(uint8_t* numberArray, char* identifier)
{
    int offset = 0;
    while (numberArray[offset] != 0 && offset < MAX_IDENTIFIER_PIECES)
    {
        if (offset > 0)
            identifier[offset * IDENTIFIER_LENGTH - 1] = '-';
        AppendNumberToIdentifier(numberArray[offset], &identifier[offset * IDENTIFIER_LENGTH]);
        offset++;
    }
    identifier[offset * IDENTIFIER_LENGTH - 1] = 0;
}

/// <summary>
/// Tries to 'increment' array of numbers
/// </summary>
/// <param name="numberArray"> Pointer to array of numbers to be incremented </param>
/// <returns> Error if overflow occurs </returns>
static status_e NumberArrayIncrement(uint8_t* numberArray)
{
    int carry = 1;
    for (int i = MAX_IDENTIFIER_PIECES - 1; i >= 0; i--)
    {
        // skipping non-existent LSB bytes
        if (numberArray[i] == 0)
            continue;

        // check whether increment is completed
        if (carry == 0)
            break;

        numberArray[i] += carry;
        carry = 0;

        // checking whether carry is present
        if (numberArray[i] > MAX_IDENTIFIER_VALUE)
        {
            numberArray[i] -= MAX_IDENTIFIER_VALUE;
            carry = 1;
        }
    }

    //if new MSB identifier piece must be created
    if (carry > 0)
    {
        // checking whether there is free piece in identifier
        if (numberArray[MAX_IDENTIFIER_PIECES - 1] != 0)
            return STATUS_OVERFLOW;
        for (int i = MAX_IDENTIFIER_PIECES - 2; i >= 0; i--)
            numberArray[i + 1] = numberArray[i];
        numberArray[0] = carry;
    }

    return STATUS_SUCCESS;
}

/// <summary>
/// Checks whether identifier is valid
/// </summary>
/// <param name="identifier">Null-terminated identifier</param>
/// <returns> Error if identifier is invalid </returns>
static status_e ValidateIdentifier(char* identifier)
{
    int offset = 0;
    while (identifier[offset] != 0)
    {
        // validating first character of every identifier piece
        if (identifier[offset] < FIRST_LETTER_FOR_FIRST_SYMBOL
            || identifier[offset] > LAST_LETTER_FOR_FIRST_SYMBOL)
            return STATUS_WRONG_SEQUENCE;

        // validating second character of every identifier piece
        offset++;
        if (identifier[offset] < FIRST_LETTER_FOR_SECOND_SYMBOL
            || identifier[offset] > LAST_LETTER_FOR_SECOND_SYMBOL)
            return STATUS_WRONG_SEQUENCE;

        // validating '-' character of every identifier piece
        offset++;
        if (identifier[offset] != '-' && identifier[offset] != 0)
            return STATUS_WRONG_SEQUENCE;

        if (identifier[offset] == '-')
            offset++;

    }

    // detecting redundant '-' at the end
    if (offset % 3 != 2)
        return STATUS_WRONG_SEQUENCE;

    // validating length of identifier
    if (offset >= MAX_IDENTIFIER_PIECES * IDENTIFIER_LENGTH)
        return STATUS_OVERFLOW;

    return STATUS_SUCCESS;
}

status_e IncrementIdentifier(char* identifier, char* outputIdentifier)
{
    status_e operationStatus = ValidateIdentifier(identifier);
    if (operationStatus != STATUS_SUCCESS)
        return operationStatus;

    uint8_t integerRepresentation[MAX_IDENTIFIER_PIECES] = { 0 };

    IdentifierToNumberArray(identifier, integerRepresentation);

    operationStatus = NumberArrayIncrement(integerRepresentation);
    if (operationStatus != STATUS_SUCCESS)
        return operationStatus;

    NumberArrayToIdentifier(integerRepresentation, outputIdentifier);

    return STATUS_SUCCESS;
}