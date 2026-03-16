#include "atm.h"

/**
 * Validate the format of a PIN string.
 * 
 * This function checks if a PIN string meets the required format:
 * - Must not be NULL
 * - Must be exactly 4 digits long (defined by ACTUAL_PIN_DIGITS)
 * - Must contain only numeric characters (0-9)
 * 
 * @param pin_str Pointer to the PIN string to validate
 * @return bool true if PIN format is valid, false otherwise
 */
bool is_valid_pin_format(const char *pin_str) {
    // Check if pointer is NULL
    if (pin_str == NULL) return false;
    
    // Get string length and verify it matches expected PIN length
    int len = strlen(pin_str);
    if (len != ACTUAL_PIN_DIGITS) {
        return false;
    }
    
    // Check each character is a digit
    for (int i = 0; i < len; i++) {
        if (!isdigit(pin_str[i])) {
            return false;
        }
    }
    return true;
}

/**
 * Validate the format of a customer name string.
 * 
 * This function checks if a name string meets the required format:
 * - Must not be NULL
 * - Must have a length between 1 and MAX_NAME_LENGTH-1 characters
 * - Must contain only alphabetic characters or spaces
 * 
 * @param name_str Pointer to the name string to validate
 * @return bool true if name format is valid, false otherwise
 */
bool is_valid_name_format(const char *name_str) {
    // Check if pointer is NULL or string is empty
    if (name_str == NULL || strlen(name_str) == 0 || strlen(name_str) >= MAX_NAME_LENGTH) {
        return false;
    }
    
    // Check each character is alphabetic or space
    for (int i = 0; name_str[i] != '\0'; i++) {
        if (!(isalpha(name_str[i]) || isspace(name_str[i]))) {
            return false;
        }
    }
    return true;
}

/**
 * Handle PIN change request for a customer.
 * 
 * This function provides a secure way for customers to change their PIN
 * by requiring them to enter their current PIN for verification before
 * allowing the change to a new PIN.
 * 
 * @param customer_ptr Pointer to the customer whose PIN is being changed
 * @return bool true if PIN change successful, false otherwise
 */
bool change_pin_wrapper(Customer *customer_ptr) {
    // Validate customer pointer
    if (!customer_ptr) return false;

    // Temporary variables to store PIN inputs
    char old_pin[MAX_PIN_STORAGE_LENGTH];
    char new_pin1[MAX_PIN_STORAGE_LENGTH];
    char new_pin2[MAX_PIN_STORAGE_LENGTH];

    // Prompt for current PIN
    printf("Enter your current PIN: ");
    scanf("%s", old_pin);
    clear_input_buffer();

    // Verify current PIN matches stored PIN
    if (strcmp(old_pin, customer_ptr->pin) != 0) {
        printf("Incorrect PIN.\\n");
        return false;
    }

    // Prompt for new PIN
    printf("Enter new PIN: ");
    scanf("%s", new_pin1);
    clear_input_buffer();

    // Validate new PIN format
    if (!is_valid_pin_format(new_pin1)) {
        printf("Invalid PIN format. PIN must be %d digits.\\n", ACTUAL_PIN_DIGITS);
        return false;
    }

    // Prompt to confirm new PIN
    printf("Confirm new PIN: ");
    scanf("%s", new_pin2);
    clear_input_buffer();

    // Verify both PIN entries match
    if (strcmp(new_pin1, new_pin2) == 0) {
        // Update customer's PIN
        strcpy(customer_ptr->pin, new_pin1);
        printf("PIN changed successfully.\\n");
        return true;
    } else {
        printf("PINs do not match.\\n");
        return false;
    }
}