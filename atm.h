#ifndef ATM_H
#define ATM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

// ========================================
// MACROS AND CONSTANTS
// ========================================

// System configuration constants
#define MAX_CUSTOMERS 5                              // Maximum number of customers in the system
#define ACTUAL_PIN_DIGITS 4                          // Number of digits in a valid PIN
#define MAX_PIN_STORAGE_LENGTH (ACTUAL_PIN_DIGITS + 1) // Buffer size for PIN storage (includes null terminator)
#define MAX_NAME_LENGTH 50                           // Maximum length for customer names
#define ACCOUNT_NUMBER_DIGITS 4                      // Number of digits in account numbers
#define MAX_ACCOUNTS_ALLOWED_PER_CUSTOMER 5          // Maximum accounts per customer
#define CUSTOMERS_FILE "customers.dat"               // Filename for customer data persistence
#define ACCOUNTS_FILE "accounts.dat"                 // Filename for account data persistence

// Error handling macro for file corruption
#define HANDLE_CORRUPTED_FILE_AND_RESET(fp) \
    do { \
        fprintf(stderr, "Error reading file! Creating default data!\n"); \
        if (fp) {fclose(fp);} \
        populate_default_data(); \
        return; \
    } while(0)

// Macro to convert macro values to strings for string literals
#define MACRO_TO_STRING_HELPER(x) #x
#define MACRO_TO_STRING(x) MACRO_TO_STRING_HELPER(x)

// ========================================
// FORWARD DECLARATION
// ========================================

// Forward declaration to resolve circular dependency between Customer and Account
struct customer;

// ========================================
// DATA STRUCTURES
// ========================================

/**
 * Account structure representing a bank account.
 * 
 * This structure contains all information related to a single bank account,
 * including its number, balance, owner reference, and linkage to other accounts.
 */
typedef struct account {
    int account_number;           // Unique identifier for the account (4-digit number)
    double balance;               // Current balance in the account
    struct customer *owner;       // Pointer to the customer who owns this account
    struct account *next;         // Pointer to the next account in the linked list
} Account;

/**
 * Customer structure representing a bank customer.
 * 
 * This structure contains customer information including authentication
 * details, account management, and session state.
 */
typedef struct customer {
    char pin[MAX_PIN_STORAGE_LENGTH];  // Customer's 4-digit PIN for authentication
    char name[MAX_NAME_LENGTH];        // Customer's full name
    Account *accounts_head;            // Head of the linked list of customer's accounts
    Account *accounts_last;            // Pointer to the last account for efficient appending
    int num_accounts;                  // Count of active accounts for this customer
    bool is_active;                    // Flag indicating if this customer slot is in use
} Customer;

// ========================================
// GLOBAL VARIABLES
// ========================================

// External declarations for global variables used throughout the system
extern Customer customers[MAX_CUSTOMERS];  // Array storing all customers in the system
extern Customer *current_customer;         // Pointer to the currently logged-in customer
extern Account *current_account;           // Pointer to the currently active account

// ========================================
// FUNCTION PROTOTYPES
// ========================================

// --- SYSTEM MANAGEMENT FUNCTIONS ---

/**
 * Initialize the ATM system with default settings.
 * Sets up random number generation, clears customer data, and resets pointers.
 */
void initialize_system();

/**
 * Populate the system with default hardcoded customer and account data.
 * Creates sample customers with predefined names, PINs, and initial accounts.
 */
void populate_default_data();

/**
 * Clean up the system and free all allocated memory.
 * Resets all customer and account data to initial state.
 */
void cleanup_system();

/**
 * Save all customer and account data to persistent files.
 * Writes data to CUSTOMERS_FILE and ACCOUNTS_FILE in CSV format.
 */
void save_data_to_files();

/**
 * Load customer and account data from persistent files.
 * Reads data from files and reconstructs the system state.
 * Creates default data if files are missing or corrupted.
 */
void load_data_from_files();

// --- USER INTERFACE FUNCTIONS ---

/**
 * Display the login screen and handle user authentication.
 * Supports both regular customer login and administrator login.
 * 
 * @return bool true if login successful, false otherwise
 */
bool login_screen();

/**
 * Display the main menu for authenticated customers.
 * Provides access to all banking operations and account management.
 */
void main_menu();

/**
 * Display the administrator menu with special privileges.
 * Allows viewing all customers and switching to any account.
 */
void admin_menu();

/**
 * Allow administrator to switch to a specific customer account.
 * 
 * @return bool true if switch successful, false otherwise
 */
bool admin_switch_to_account();

/**
 * Display all customers and their associated accounts.
 * Used primarily by administrators for system overview.
 */
void display_all_customers_accounts();

/**
 * Clear the terminal screen for a clean interface.
 * Provides cross-platform screen clearing functionality.
 */
void clear_screen();

// --- CUSTOMER MANAGEMENT FUNCTIONS ---

/**
 * Validate the format of a PIN string.
 * Checks for correct length and numeric characters only.
 * 
 * @param pin_str Pointer to the PIN string to validate
 * @return bool true if PIN format is valid, false otherwise
 */
bool is_valid_pin_format(const char *pin_str);

/**
 * Validate the format of a customer name string.
 * Checks for appropriate length and character content.
 * 
 * @param name_str Pointer to the name string to validate
 * @return bool true if name format is valid, false otherwise
 */
bool is_valid_name_format(const char *name_str);

/**
 * Handle PIN change request for a customer.
 * Requires current PIN verification before allowing change.
 * 
 * @param customer_ptr Pointer to the customer whose PIN is being changed
 * @return bool true if PIN change successful, false otherwise
 */
bool change_pin_wrapper(Customer *customer_ptr);

// --- ACCOUNT MANAGEMENT FUNCTIONS ---

/**
 * Create a new account node with the specified parameters.
 * Allocates memory and initializes account fields.
 * 
 * @param account_number The unique account number for the new account
 * @param initial_balance The starting balance for the new account
 * @param owner_customer Pointer to the customer who owns this account
 * @return Account* Pointer to the newly created account, or NULL if allocation failed
 */
Account* create_account_node(int account_number, double initial_balance, Customer *owner_customer);

/**
 * Add a new account to a customer's account list.
 * Updates the customer's linked list and account count.
 * 
 * @param customer_ptr Pointer to the customer receiving the new account
 * @param new_account_node Pointer to the account node to add
 */
void add_account_to_customer(Customer *customer_ptr, Account *new_account_node);

/**
 * Remove an account from a customer's account list.
 * Handles memory deallocation and list pointer updates.
 * 
 * @param customer_ptr Pointer to the customer whose account is being removed
 * @param account_number_to_delete The account number to remove
 * @return bool true if account was found and removed, false otherwise
 */
bool remove_account_from_customer(Customer *customer_ptr, int account_number_to_delete);

/**
 * Find an account by number within a specific customer's account list.
 * 
 * @param customer_ptr Pointer to the customer whose accounts to search
 * @param account_number_to_find The account number to search for
 * @return Account* Pointer to the found account, or NULL if not found
 */
Account* find_account_by_number(const Customer *customer_ptr, int account_number_to_find);

/**
 * Find an account globally across all customers in the system.
 * 
 * @param account_number The account number to search for
 * @param found_customer_ptr Pointer to store the owner customer (can be NULL)
 * @return Account* Pointer to the found account, or NULL if not found
 */
Account* find_account_globally(int account_number, Customer** found_customer_ptr);

/**
 * Display all accounts for a specific customer.
 * Prints formatted account information to standard output.
 * 
 * @param customer_ptr Pointer to the customer whose accounts to display
 */
void display_customer_accounts(const Customer *customer_ptr);

/**
 * Generate a unique account number for a new account.
 * Ensures the generated number is not already in use.
 * 
 * @return int A unique account number
 */
int generate_unique_account_number();

/**
 * Create a new account for a customer with validation.
 * Checks account limits before creating and adding new account.
 * 
 * @param customer Pointer to the customer requesting a new account
 * @return bool true if account creation successful, false otherwise
 */
bool create_new_account_wrapper(Customer *customer);

/**
 * Delete an account from a customer with validation.
 * Prevents deletion of the only account and handles current account switching.
 * 
 * @param customer_ptr Pointer to the customer requesting account deletion
 * @return bool true if account deletion successful, false otherwise
 */
bool delete_account_wrapper(Customer *customer_ptr);

/**
 * Switch the current active account for a customer.
 * Changes which account is used for transactions.
 * 
 * @param customer_ptr Pointer to the customer requesting account switch
 * @return bool true if account switch successful, false otherwise
 */
bool switch_account_wrapper(Customer *customer_ptr);

/**
 * Display the balance of a specific account.
 * Prints formatted balance information to standard output.
 * 
 * @param account Pointer to the account whose balance to display
 */
void display_balance(const Account *account);

/**
 * Withdraw money from an account with validation.
 * Checks for sufficient funds and positive amounts.
 * 
 * @param account Pointer to the account to withdraw from
 * @return bool true if withdrawal successful, false otherwise
 */
bool withdraw_money_wrapper(Account *account);

/**
 * Deposit money into an account with validation.
 * Checks for positive deposit amounts.
 * 
 * @param account Pointer to the account to deposit into
 * @return bool true if deposit successful, false otherwise
 */
bool deposit_money_wrapper(Account *account);

/**
 * Free all accounts belonging to a customer.
 * Deallocates memory for all accounts and resets customer pointers.
 * 
 * @param customer_ptr Pointer to the customer whose accounts to free
 */
void free_customer_accounts(Customer *customer_ptr);

// --- UTILITY FUNCTIONS ---

/**
 * Get a double-precision floating-point number from user input.
 * Includes input validation and buffer clearing.
 * 
 * @param value Pointer to store the input value
 * @param prompt The message to display to the user
 * @return bool true if input was successfully read and parsed, false otherwise
 */
bool get_double_input(double *value, const char *prompt);

/**
 * Get an integer from user input.
 * Includes input validation and buffer clearing.
 * 
 * @param value Pointer to store the input value
 * @param prompt The message to display to the user
 * @return bool true if input was successfully read and parsed, false otherwise
 */
bool get_int_input(int *value, const char *prompt);

/**
 * Clear the input buffer to prevent input issues.
 * Reads and discards characters until newline or EOF.
 */
void clear_input_buffer();

#endif // ATM_H
