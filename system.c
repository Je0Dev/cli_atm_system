#include "atm.h"

// Define global variables for the ATM system
Customer customers[MAX_CUSTOMERS];  // Array to store all customers
Customer *current_customer = NULL;  // Pointer to the currently logged-in customer
Account *current_account = NULL;    // Pointer to the currently active account

/**
 * Initialize the ATM system with default settings.
 * 
 * This function sets up the initial state of the system by:
 * - Seeding the random number generator for account number generation
 * - Initializing all customer slots as inactive
 * - Setting all pointers to NULL and counters to zero
 * - Clearing all name and PIN fields
 */
void initialize_system() {
    // Seed random number generator for unique account number generation
    srand((unsigned int)time(NULL));
    
    // Initialize all customer slots in the array
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        customers[i].is_active = false;      // Mark as inactive
        customers[i].accounts_head = NULL;   // No accounts initially
        customers[i].accounts_last = NULL;   // No accounts initially
        customers[i].num_accounts = 0;       // Zero accounts
        strcpy(customers[i].name, "");       // Clear name field
        strcpy(customers[i].pin, "");        // Clear PIN field
    }
    
    // Initialize current session pointers
    current_customer = NULL;
    current_account = NULL;
}

/**
 * Populate the system with default hardcoded customer and account data.
 * 
 * This function creates sample customers with predefined names, PINs, and accounts
 * for demonstration and testing purposes. Each customer gets one initial account
 * with a starting balance.
 */
void populate_default_data() {
    printf("\\nCreating default customer and account data...\\n");
    
    // Create customer 1: Giannis Papadogiannis
    strcpy(customers[0].name, "Giannis Papadogiannis");
    strcpy(customers[0].pin, "1235");
    customers[0].is_active = true;
    customers[0].num_accounts = 0;

    // Create customer 2: Eirini Famfarotou
    strcpy(customers[1].name, "Eirini Famfarotou");
    strcpy(customers[1].pin, "5228");
    customers[1].is_active = true;
    customers[1].num_accounts = 0;

    // Create customer 3: Nikos Kostakis
    strcpy(customers[2].name, "Nikos Kostakis");
    strcpy(customers[2].pin, "1661");
    customers[2].is_active = true;
    customers[2].num_accounts = 0;

    // Create customer 4: Eleni Michailidou
    strcpy(customers[3].name, "Eleni Michailidou");
    strcpy(customers[3].pin, "9229");
    customers[3].is_active = true;
    customers[3].num_accounts = 0;

    // Create customer 5: Kostas Antoniou
    strcpy(customers[4].name, "Kostas Antoniou");
    strcpy(customers[4].pin, "3333");
    customers[4].is_active = true;
    customers[4].num_accounts = 0;

    // Create initial accounts for each active customer
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        if (customers[i].is_active) {
            // Generate a unique account number and create initial account
            Account *initial_account = create_account_node(
                generate_unique_account_number(),  // Unique account number
                1000.0 + (i * 250),               // Starting balance with variation
                &customers[i]                     // Owner of the account
            );
            
            // Add the account to the customer if creation was successful
            if (initial_account) {
                add_account_to_customer(&customers[i], initial_account);
            } else {
                fprintf(stderr, "Failed to create initial account for %s.\\n", customers[i].name);
            }
        }
    }
    printf("Default data creation is complete!\\n");
}

/**
 * Clean up the system and free all allocated memory.
 * 
 * This function resets the system state by:
 * - Freeing all accounts for each customer
 * - Marking all customers as inactive
 * - Resetting all counters and clearing string fields
 * - Nullifying current session pointers
 */
void cleanup_system() {
    // Free all accounts for each customer
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        free_customer_accounts(&customers[i]);
        customers[i].is_active = false;      // Mark as inactive
        customers[i].num_accounts = 0;       // Reset account counter
        strcpy(customers[i].name, "");       // Clear name
        strcpy(customers[i].pin, "");        // Clear PIN
    }
    
    // Reset current session state
    current_customer = NULL;
    current_account = NULL;
}

/**
 * Save all customer and account data to persistent files.
 * 
 * This function writes the current system state to two files:
 * - CUSTOMERS_FILE: Contains customer information (name, PIN)
 * - ACCOUNTS_FILE: Contains account information (number, balance, owner)
 * 
 * Data is saved in CSV format for easy reading and backup.
 */
void save_data_to_files() {
    // Open customers file for writing in binary mode
    FILE *fp_customers = fopen(CUSTOMERS_FILE, "wb");
    if (!fp_customers) {
        perror("Error opening customers file for writing");
        return;
    }

    // Open accounts file for writing in binary mode
    FILE *fp_accounts = fopen(ACCOUNTS_FILE, "wb");
    if (!fp_accounts) {
        perror("Error opening accounts file for writing");
        fclose(fp_customers);
        return;
    }

    // Write customer data to file
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        if (customers[i].is_active) {
            // Format: customer_index,name,pin
            fprintf(fp_customers, "%d,%s,%s\\n", i, customers[i].name, customers[i].pin);

            // Write all accounts for this customer
            Account *current_acc = customers[i].accounts_head;
            while (current_acc) {
                // Format: account_number,customer_index,balance
                fprintf(fp_accounts, "%d,%d,%.2f\\n", 
                       current_acc->account_number, i, current_acc->balance);
                current_acc = current_acc->next;
            }
        }
    }

    // Close both files
    fclose(fp_customers);
    fclose(fp_accounts);
}

/**
 * Load customer and account data from persistent files.
 * 
 * This function reads data from the saved files and reconstructs the system state.
 * If files don't exist or are corrupted, it creates default data.
 * 
 * The function handles data validation and reports any inconsistencies found.
 */
void load_data_from_files() {
    // Try to open customers file for reading
    FILE *fp_customers_file = fopen(CUSTOMERS_FILE, "rb");
    int loaded_customers_count = 0;

    if (!fp_customers_file) {
        // If file doesn't exist, create default data
        printf("Customer data file not found. Creating default data.\\n");
        populate_default_data();
        return;
    }

    // Clear existing data before loading new data
    cleanup_system();
    initialize_system();

    int cust_idx_from_file;                    // Customer index from file
    char name_from_file[MAX_NAME_LENGTH];      // Name read from file
    char pin_from_file[MAX_PIN_STORAGE_LENGTH]; // PIN read from file

    // Read customer data line by line
    while (fscanf(fp_customers_file, "%d,%[^,],%s\\n", &cust_idx_from_file, name_from_file, pin_from_file) == 3) {
        // Validate customer index is within bounds
        if (cust_idx_from_file >= 0 && cust_idx_from_file < MAX_CUSTOMERS) {
            // Only process if this slot isn't already active
            if (!customers[cust_idx_from_file].is_active) {
                strcpy(customers[cust_idx_from_file].name, name_from_file);
                strcpy(customers[cust_idx_from_file].pin, pin_from_file);
                customers[cust_idx_from_file].is_active = true;
                loaded_customers_count++;
            }
        }
    }
    fclose(fp_customers_file);

    // Try to open accounts file for reading
    FILE *fp_accounts_file = fopen(ACCOUNTS_FILE, "rb");
    int loaded_accounts_count = 0;

    if (fp_accounts_file) {
        int acc_num_from_file, cust_idx_from_file_acc;  // Account and customer indices from file
        double balance_from_file;                       // Balance read from file
        int invalid_accounts_count = 0;                 // Counter for invalid accounts

        // Read account data line by line
        while (fscanf(fp_accounts_file, "%d,%d,%lf\\n", &acc_num_from_file, &cust_idx_from_file_acc, &balance_from_file) == 3) {
            // Validate customer index and check if customer is active
            if (cust_idx_from_file_acc >= 0 && cust_idx_from_file_acc < MAX_CUSTOMERS && customers[cust_idx_from_file_acc].is_active) {
                // Check if account number is unique
                if (find_account_globally(acc_num_from_file, NULL) == NULL) {
                    // Create and add the account
                    Account *new_acc = create_account_node(acc_num_from_file, balance_from_file, &customers[cust_idx_from_file_acc]);
                    if (new_acc) {
                        add_account_to_customer(&customers[cust_idx_from_file_acc], new_acc);
                        loaded_accounts_count++;
                    }
                }
            } else {
                // Account references invalid or inactive customer
                invalid_accounts_count++;
            }
        }
        fclose(fp_accounts_file);

        // Report any invalid accounts found
        if (invalid_accounts_count > 0) {
            printf("Warning: %d accounts were ignored due to invalid data.\\n", invalid_accounts_count);
        }
    }

    // Display loading summary
    printf("Data loading complete!\\n");
    printf("Loaded:\\n");
    printf("- %d active customers\\n", loaded_customers_count);
    printf("- %d accounts\\n", loaded_accounts_count);
}