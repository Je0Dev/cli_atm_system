#include "atm.h"

/**
 * Main entry point for the ATM System application.
 * 
 * This function initializes the system, handles the main program loop,
 * manages user login, and orchestrates the overall flow of the application.
 * 
 * @return int EXIT_SUCCESS on normal termination, EXIT_FAILURE on error
 */
int main() {
    // Display welcome message to the user
    printf("\n--Welcome to the ATM System--\n");
    
    // Initialize the ATM system with default settings
    initialize_system();
    printf("The system has been initialized.\n");

    // Load existing customer and account data from files
    load_data_from_files();
    printf("Loading/creating initial data is complete.\n");

    char continue_program_char;  // User input for continuing the program
    bool continue_main_loop = true;  // Control variable for the main loop

    // Main program loop - continues until user chooses to exit
    do {
        // Clear screen for clean interface (commented out for debugging)
        //clear_screen();
        
        // Attempt user login
        if (login_screen()) {
            // If login successful, display the main menu
            main_menu();
        } else {
            // If login failed, inform the user
            printf("\nLogin failed!\n");
        }

        // Ask user if they want to continue with another transaction
        printf("\nWould you like to perform another transaction or try to log in again? (y/n): ");
        
        // Read user's choice to continue or exit
        if (scanf(" %c", &continue_program_char) != 1) {
            clear_input_buffer();
            fprintf(stderr, "You have entered an invalid option! The program is terminating!\n");
            continue_main_loop = false;
        }
        clear_input_buffer();

        // Convert input to uppercase and check if user wants to continue
        if (toupper(continue_program_char) != 'Y') {
            continue_main_loop = false;
        }

    } while (continue_main_loop);

    // Before exiting, save all data to files for persistence
    printf("\nSaving data...\n");
    save_data_to_files();
    
    // Clean up memory and close the system properly
    printf("\nClearing memory...\n");
    cleanup_system();
    printf("Data saved and memory cleared successfully!\n");
    
    // Display farewell message
    printf("\nThank you for using our ATM system! We hope to see you again soon!\n");

    return EXIT_SUCCESS;
}