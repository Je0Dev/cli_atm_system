#include "atm.h"

/**
 * Create a new account node with the specified parameters.
 * 
 * This function allocates memory for a new Account structure and initializes
 * it with the provided account number, initial balance, and owner reference.
 * 
 * @param account_number The unique account number for the new account
 * @param initial_balance The starting balance for the new account
 * @param owner_customer Pointer to the customer who owns this account
 * @return Account* Pointer to the newly created account, or NULL if allocation failed
 */
Account* create_account_node(int account_number, double initial_balance, Customer *owner_customer) {
    // Allocate memory for new account
    Account* new_account = (Account*)malloc(sizeof(Account));
    if (new_account) {
        // Initialize account fields
        new_account->account_number = account_number;
        new_account->balance = initial_balance;
        new_account->owner = owner_customer;
        new_account->next = NULL;
    }
    return new_account;
}

/**
 * Add a new account to a customer's account list.
 * 
 * This function adds a new account to the end of a customer's linked list
 * of accounts, updating the head, last, and account count as necessary.
 * 
 * @param customer_ptr Pointer to the customer receiving the new account
 * @param new_account_node Pointer to the account node to add
 */
void add_account_to_customer(Customer *customer_ptr, Account *new_account_node) {
    // Validate input parameters
    if (!customer_ptr || !new_account_node) return;

    // If customer has no accounts yet, set both head and last to new account
    if (customer_ptr->accounts_head == NULL) {
        customer_ptr->accounts_head = new_account_node;
        customer_ptr->accounts_last = new_account_node;
    } else {
        // Add new account to the end of the list
        customer_ptr->accounts_last->next = new_account_node;
        customer_ptr->accounts_last = new_account_node;
    }
    // Increment account count
    customer_ptr->num_accounts++;
}

/**
 * Remove an account from a customer's account list.
 * 
 * This function searches for and removes an account with the specified number
 * from a customer's linked list, properly handling memory deallocation and
 * list pointer updates.
 * 
 * @param customer_ptr Pointer to the customer whose account is being removed
 * @param account_number_to_delete The account number to remove
 * @return bool true if account was found and removed, false otherwise
 */
bool remove_account_from_customer(Customer *customer_ptr, int account_number_to_delete) {
    // Validate input parameters
    if (!customer_ptr || !customer_ptr->accounts_head) return false;

    Account *current = customer_ptr->accounts_head;  // Current node being examined
    Account *prev = NULL;                             // Previous node in the list

    // Search for the account to delete
    while (current != NULL && current->account_number != account_number_to_delete) {
        prev = current;
        current = current->next;
    }

    // If account not found, return false
    if (current == NULL) return false;

    // Handle removal based on position in list
    if (prev == NULL) {
        // Account to delete is the head of the list
        customer_ptr->accounts_head = current->next;
    } else {
        // Account to delete is in the middle or end
        prev->next = current->next;
    }

    // Update last pointer if necessary
    if (current == customer_ptr->accounts_last) {
        customer_ptr->accounts_last = prev;
    }

    // Free memory and update account count
    free(current);
    customer_ptr->num_accounts--;
    return true;
}

/**
 * Find an account by number within a specific customer's account list.
 * 
 * This function searches through a customer's linked list of accounts
 * to find one with the specified account number.
 * 
 * @param customer_ptr Pointer to the customer whose accounts to search
 * @param account_number_to_find The account number to search for
 * @return Account* Pointer to the found account, or NULL if not found
 */
Account* find_account_by_number(const Customer *customer_ptr, int account_number_to_find) {
    // Validate input parameter
    if (!customer_ptr) return NULL;
    
    Account* current = customer_ptr->accounts_head;
    // Search through the linked list
    while (current != NULL) {
        if (current->account_number == account_number_to_find) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * Find an account globally across all customers in the system.
 * 
 * This function searches through all active customers to find an account
 * with the specified number, optionally returning a pointer to the owner.
 * 
 * @param account_number The account number to search for
 * @param found_customer_ptr Pointer to store the owner customer (can be NULL)
 * @return Account* Pointer to the found account, or NULL if not found
 */
Account* find_account_globally(int account_number, Customer** found_customer_ptr) {
    // Initialize output parameter if provided
    if (found_customer_ptr) {
        *found_customer_ptr = NULL;
    }
    
    // Search through all customer slots
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        if (customers[i].is_active) {
            // Search within this customer's accounts
            Account* acc = find_account_by_number(&customers[i], account_number);
            if (acc) {
                // Account found, set owner if requested
                if (found_customer_ptr) {
                    *found_customer_ptr = &customers[i];
                }
                return acc;
            }
        }
    }
    return NULL;
}

/**
 * Display all accounts for a specific customer.
 * 
 * This function prints a formatted list of all accounts belonging to
 * the specified customer, showing account number and balance.
 * 
 * @param customer_ptr Pointer to the customer whose accounts to display
 */
void display_customer_accounts(const Customer *customer_ptr) {
    if (customer_ptr) {
        Account *acc = customer_ptr->accounts_head;
        // Iterate through the linked list and display each account
        while (acc) {
            printf("  Account Number: %d, Balance: %.2f\n", acc->account_number, acc->balance);
            acc = acc->next;
        }
    }
}

/**
 * Generate a unique account number for a new account.
 * 
 * This function generates random 4-digit account numbers until it finds
 * one that is not already in use by any customer in the system.
 * 
 * @return int A unique account number
 */
int generate_unique_account_number() {
    int new_acc_num;
    // Keep generating numbers until a unique one is found
    do {
        new_acc_num = 1000 + rand() % 9000;  // Generate 4-digit number (1000-9999)
    } while (find_account_globally(new_acc_num, NULL) != NULL);
    return new_acc_num;
}

/**
 * Create a new account for a customer with validation.
 * 
 * This function checks if a customer can create a new account (based on
 * maximum allowed accounts), then creates and adds a new account with
 * a zero initial balance.
 * 
 * @param customer Pointer to the customer requesting a new account
 * @return bool true if account creation successful, false otherwise
 */
bool create_new_account_wrapper(Customer *customer) {
    // Check if customer has reached maximum account limit
    if (customer->num_accounts >= MAX_ACCOUNTS_ALLOWED_PER_CUSTOMER) {
        printf("Maximum number of accounts reached.\\n");
        return false;
    }

    // Create new account with unique number and zero balance
    Account *new_account = create_account_node(generate_unique_account_number(), 0.0, customer);
    if (new_account) {
        add_account_to_customer(customer, new_account);
        printf("New account created successfully with account number: %d\n", new_account->account_number);
        return true;
    }
    return false;
}

/**
 * Delete an account from a customer with validation.
 * 
 * This function allows a customer to delete one of their accounts,
 * with validation to ensure they don't delete their only account.
 * 
 * @param customer_ptr Pointer to the customer requesting account deletion
 * @return bool true if account deletion successful, false otherwise
 */
bool delete_account_wrapper(Customer *customer_ptr) {
    // Prevent deletion of the only account
    if (customer_ptr->num_accounts <= 1) {
        printf("You cannot delete your only account.\n");
        return false;
    }

    int account_to_delete;
    // Get account number to delete from user
    if (!get_int_input(&account_to_delete, "Enter account number to delete: ")) {
        printf("Invalid input for account number.\n");
        return false;
    }

    // Attempt to remove the account
    if (remove_account_from_customer(customer_ptr, account_to_delete)) {
        printf("Account deleted successfully.\\n");
        // If the deleted account was the current one, switch to another
        if (current_account->account_number == account_to_delete) {
            current_account = customer_ptr->accounts_head;
        }
        return true;
    } else {
        printf("Account not found or could not be deleted.\n");
        return false;
    }
}

/**
 * Switch the current active account for a customer.
 * 
 * This function allows a customer to change which of their accounts
 * is currently active for transactions.
 * 
 * @param customer_ptr Pointer to the customer requesting account switch
 * @return bool true if account switch successful, false otherwise
 */
bool switch_account_wrapper(Customer *customer_ptr) {
    int target_account_number;
    // Get target account number from user
    if (!get_int_input(&target_account_number, "Enter account number to switch to: ")) {
        printf("Invalid input for account number.\n");
        return false;
    }

    // Find the target account
    Account *target_account = find_account_by_number(customer_ptr, target_account_number);
    if (target_account) {
        current_account = target_account;
        printf("Switched to account %d.\n", target_account->account_number);
        return true;
    } else {
        printf("Account not found.\n");
        return false;
    }
}

/**
 * Display the balance of a specific account.
 * 
 * This function prints the current balance for the specified account
 * in a formatted manner.
 * 
 * @param account Pointer to the account whose balance to display
 */
void display_balance(const Account *account) {
    if (account) {
        printf("Current balance for account %d: %.2f\n", account->account_number, account->balance);
    }
}

/**
 * Withdraw money from an account with validation.
 * 
 * This function allows withdrawal from an account after validating
 * that sufficient funds are available and the amount is positive.
 * 
 * @param account Pointer to the account to withdraw from
 * @return bool true if withdrawal successful, false otherwise
 */
bool withdraw_money_wrapper(Account *account) {
    double amount;
    // Get withdrawal amount from user
    if (get_double_input(&amount, "Enter amount to withdraw: ")) {
        // Validate withdrawal amount and sufficient funds
        if (amount > 0 && amount <= account->balance) {
            account->balance -= amount;
            printf("Withdrawal successful. New balance: %.2f\n", account->balance);
            return true;
        } else {
            printf("Invalid amount or insufficient funds.\n");
            return false;
        }
    }
    return false;
}

/**
 * Deposit money into an account with validation.
 * 
 * This function allows depositing money into an account after validating
 * that the deposit amount is positive.
 * 
 * @param account Pointer to the account to deposit into
 * @return bool true if deposit successful, false otherwise
 */
bool deposit_money_wrapper(Account *account) {
    double amount;
    // Get deposit amount from user
    if (get_double_input(&amount, "Enter amount to deposit: ")) {
        // Validate deposit amount
        if (amount > 0) {
            account->balance += amount;
            printf("Deposit successful. New balance: %.2f\n", account->balance);
            return true;
        } else {
            printf("Invalid amount.\n");
            return false;
        }
    }
    return false;
}

/**
 * Free all accounts belonging to a customer.
 * 
 * This function deallocates memory for all accounts in a customer's
 * linked list and resets the customer's account pointers and count.
 * 
 * @param customer_ptr Pointer to the customer whose accounts to free
 */
void free_customer_accounts(Customer *customer_ptr) {
    Account *current = customer_ptr->accounts_head;
    // Iterate through and free each account
    while (current != NULL) {
        Account *temp = current;
        current = current->next;
        free(temp);
    }
    // Reset customer's account pointers and count
    customer_ptr->accounts_head = NULL;
    customer_ptr->accounts_last = NULL;
}