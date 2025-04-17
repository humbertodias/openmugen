#include "global.h"

FILE* pLogFile = NULL;

void PrintMessage(char* str, ...) {
    char    string[255];  // Temporary string
    va_list ap;           // Pointer to list of arguments

    // Initialize the argument list
    va_start(ap, str);

    // Fill the string with the arguments
    vsnprintf(string, sizeof(string), str, ap);

    // End the argument list
    va_end(ap);

#ifdef _XBOX
    pLogFile = fopen("d:\\log.txt", "a+");
#else
    pLogFile = fopen("log.txt", "a+");
#endif

    if (pLogFile != NULL) {
        // Write the message to the log file
        fprintf(pLogFile, "%s", string);
        fprintf(pLogFile, "\n");
        fclose(pLogFile);
    } else {
        // If the log file could not be opened
        printf("Failed to open log file.\n");
    }

    // Print the message to the console
    printf("%s", string);
    printf("\n");
}

void InitLogFile() {
#ifdef _XBOX
    pLogFile = fopen("d:\\log.txt", "w");
#else
    pLogFile = fopen("log.txt", "w");
#endif
    fclose(pLogFile);
}
