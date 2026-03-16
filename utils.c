#include "atm.h"

/**
 * Get a double-precision floating-point number from user input.
 * 
 * This function prompts the user with a message and reads a double value
 * from standard input. It includes input validation and buffer clearing.
 * 
 * @param value Pointer to store the input value
 * @param prompt The message to display to the user
 * @return bool true if input was successfully read and parsed, false otherwise
 */
bool get_double_input(double *value, const char *prompt) {
    // Display prompt to user
    printf("%s", prompt);
    
    // Attempt to read double value from input
    if (scanf("%lf", value) == 1) {
        clear_input_buffer();  // Clear any remaining input
        return true;
    }
    
    // Input was invalid, clear buffer and return false
    clear_input_buffer();
    return false;
}

/**
 * Get an integer from user input.
 * 
 * This function prompts the user with a message and reads an integer value
 * from standard input. It includes input validation and buffer clearing.
 * 
 * @param value Pointer to store the input value
 * @param prompt The message to display to the user
 * @return bool true if input was successfully read and parsed, false otherwise
 */
bool get_int_input(int *value, const char *prompt) {
    // Display prompt to user
    printf("%s", prompt);
    
    // Attempt to read integer value from input
    if (scanf("%d", value) == 1) {
        clear_input_buffer();  // Clear any remaining input
        return true;
    }
    
    // Input was invalid, clear buffer and return false
    clear_input_buffer();
    return false;
}

/**
 * Clear the input buffer to prevent input issues.
 * 
 * This function reads and discards all characters from the input buffer
 * until it encounters a newline character or EOF. This is necessary after
 * using scanf to prevent leftover characters from affecting subsequent
 * input operations.
 */
void clear_input_buffer() {
    int c;
    // Read and discard characters until newline or EOF
    while ((c = getchar()) != '\n' && c != EOF);
}