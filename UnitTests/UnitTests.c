// UnitTests.c : This file contains implementation of unit-tests. 
//

#include <string.h>
#include <stdio.h>

#include "UnitTests.h"
#include "Identifier.h"

// macro for unit-testing. Tries to apply 'increment' on identifier, then checks whether output and status are valid. On error prints line of macro in source code
#define VERIFY(input, expectedStatus, expectedResponse)  \
{ \
    char response[MAX_IDENTIFIER_PIECES * IDENTIFIER_LENGTH] = { 0 }; \
    status_e status = IncrementIdentifier(input, response); \
    if (status != expectedStatus || (status == STATUS_SUCCESS && strcmp(expectedResponse, response) != 0)) \
    { \
        printf("Unit-test on line %d failed! \r\n", __LINE__); \
    } \
}

void UnitTests(void)
{
    // testing totally wrong input
    VERIFY("12345", STATUS_WRONG_SEQUENCE, NULL);

    // testing totally wrong input
    VERIFY("saghjdhg12378213ghusdfg231231231", STATUS_WRONG_SEQUENCE, NULL);

    // testing wrongly terminated identifier
    VERIFY("B1-", STATUS_WRONG_SEQUENCE, NULL);

    // testing wrongly terminated identifier
    VERIFY("B1-B2-Z", STATUS_WRONG_SEQUENCE, NULL);

    // testing wrongly terminated identifier
    VERIFY("B1-B2-B3+", STATUS_WRONG_SEQUENCE, NULL);

    // testing identifier with wrong character
    VERIFY("A1", STATUS_WRONG_SEQUENCE, NULL);

    // testing identifier with wrong character
    VERIFY("B0", STATUS_WRONG_SEQUENCE, NULL);

    // testing identifier with wrong character
    VERIFY("B1-b2", STATUS_WRONG_SEQUENCE, NULL);

    // testing too large identifier
    VERIFY("B1-B2-B3-B4-B5-B6-B7-B8-B9-C1-C2", STATUS_OVERFLOW, NULL);

    // testing increment
    VERIFY("B1", STATUS_SUCCESS, "B2");

    // testing increment
    VERIFY("Z4", STATUS_SUCCESS, "Z5");

    // testing increment
    VERIFY("B9", STATUS_SUCCESS, "C1");

    // testing increment
    VERIFY("Z9", STATUS_SUCCESS, "B1-B1");

    // testing increment
    VERIFY("B1-Z9", STATUS_SUCCESS, "B2-B1");

    // testing overflow
    VERIFY("Z9-Z9-Z9-Z9-Z9-Z9-Z9-Z9-Z9-Z9", STATUS_OVERFLOW, NULL);

    // testing edge situation
    VERIFY("Z8-Z9-Z9-Z9-Z9-Z9-Z9-Z9-Z9-Z9", STATUS_SUCCESS, "Z9-B1-B1-B1-B1-B1-B1-B1-B1-B1");
}