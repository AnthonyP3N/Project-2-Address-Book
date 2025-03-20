#include <stdio.h>
//#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_menu.h"
#include "address_book_fops.h"

//#include "abk_fileops.h"
//#include "abk_log.h"
//#include "abk_menus.h"
//#include "abk.h"

int get_option(int *type, const char *msg)
{
	/*
	 * Mutilfuction user intractions like
	 * Just an enter key detection
	 * Read an number
	 * Read a charcter
	 */ 

	/* Fill the code to add above functionality */
	printf("%s", msg);
    if (scanf("%d", type) != 1) {
        while (getchar() != '\n'); // Clear invalid input
        return -1; // Return an error value if input fails
    }
    while (getchar() != '\n'); // Clear newline from buffer
    return *type;
}

Status save_prompt(AddressBook *address_book)
{
    char option;

    while (1)  // Loop only when necessary
    {
        printf("\nWould you like to save your changes before exiting? (Y/N): ");
        scanf(" %c", &option);
        while (getchar() != '\n'); // Clear buffer

        if (option == 'Y' || option == 'y')
        {
            save_file(address_book);
            printf("Exiting. Data saved in %s\n", DEFAULT_FILE);
            free(address_book->list);
            exit(0);  // Terminate program
        }
        else if (option == 'N' || option == 'n')
        {
            printf("Exiting without saving.\n");
            free(address_book->list);
            exit(0);  // Terminate program
        }
        else
        {
            printf("Invalid option. Please enter 'Y' or 'N'.\n");
        }
    }

    return e_success;
}




Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
    if (address_book == NULL || address_book->list == NULL || address_book->count == 0)
    {
        printf("No contacts available.\n");
        return e_no_match;
    }

    int page = 0;
    int total_pages = (address_book->count + WINDOW_SIZE - 1) / WINDOW_SIZE;
    char option;

    do
    {
        printf("==============================================================================================================\n");
        printf("%-3s %-7s %-3s %-30s %-3s %-20s %-3s %-30s %-3s\n", 
               ":", "S.No", ":", "Name", ":", "Phone Numbers", ":", "Email IDs", ":");
        printf("==============================================================================================================\n");

        for (int i = page * WINDOW_SIZE; i < (page + 1) * WINDOW_SIZE && i < address_book->count; i++)
        {
            ContactInfo *contact = &address_book->list[i];

            if (!contact) continue;

            int max_rows = (contact->phone_count > contact->email_count) ? contact->phone_count : contact->email_count;

            //  Print first row with name, first phone, and first email
            printf("%-3s %-7d %-3s %-30s %-3s %-20s %-3s %-30s %-3s\n",
                   ":", contact->si_no, ":", contact->name[0], ":",
                   (contact->phone_count > 0) ? contact->phone_numbers[0] : "", ":",
                   (contact->email_count > 0) ? contact->email_addresses[0] : "", ":");

            //  Print remaining rows with phones and emails correctly aligned
            for (int j = 1; j < max_rows; j++)
            {
                printf("%-3s %-7s %-3s %-30s %-3s %-20s %-3s %-30s %-3s\n",
                       ":", "", ":", "", ":",
                       (j < contact->phone_count) ? contact->phone_numbers[j] : "", ":",
                       (j < contact->email_count) ? contact->email_addresses[j] : "", ":");
            }

            //  Add a separator between contacts (but not at the end)
            if (i < (page + 1) * WINDOW_SIZE - 1 && i < address_book->count - 1) 
            {
                printf("--------------------------------------------------------------------------------------------------------------\n");
            }
        }

        printf("\n-------------------------------------------------------------------------------\n");
        printf("Page %d of %d. Use 'N' for next, 'P' for previous, 'Q' to quit: ", page + 1, total_pages);
        option = getchar();
        while (getchar() != '\n');  // Clear input buffer

        if ((option == 'N' || option == 'n') && page < total_pages - 1)
        {
            page++;
        }
        else if ((option == 'P' || option == 'p') && page > 0)
        {
            page--;
        }
    } while (option != 'Q' && option != 'q');

    return e_success;
}




void menu_header(const char *str)
{
	fflush(stdout);

	printf("#######  Address Book  #######\n");
	if (str != NULL)
	{
		printf("#######  %s\n", str);
	}
}
void main_menu(void)
{
    printf("#######  Address Book  #######\n");
    printf("0. Exit\n");
    printf("1. Add Contact\n");
    printf("2. Search Contact\n");
    printf("3. Edit Contact\n");
    printf("4. Delete Contact\n");
    printf("5. List Contacts\n");
    printf("6. Save\n");
    printf("\nPlease select an option: ");
}

Status menu(AddressBook *address_book)
{
    int option;

    do
    {
        main_menu();
        get_option(&option, "");

        if ((address_book->count == 0) && (option != e_add_contact))
        {
            printf("No contacts found! Please add contacts first.\n");
            continue;
        }

        switch (option)
        {
            case e_add_contact:
                add_contacts(address_book);
                break;
            case e_search_contact:
                search_contact(address_book);
                break;
            case e_edit_contact:
                edit_contact(address_book);
                break;
            case e_delete_contact:
                delete_contact(address_book);
                break;
            case e_list_contacts:
                list_contacts(address_book, "List of Contacts", NULL, NULL, e_list);
                break;
            case e_save:
                save_file(address_book);
                break;
            case e_exit:
                save_prompt(address_book);
                return e_exit;  
            default:
                printf("Invalid option! Please try again.\n");
                break;
        }
    } while (option != e_exit);

    return e_success;
}




Status add_contacts(AddressBook *address_book)
{
    if (!address_book) return e_fail;

    ContactInfo new_contact = {0};  //  Initialize contact
    new_contact.si_no = address_book->count + 1;
    int option, phone_count = 0, email_count = 0;

    do
    {
        printf("\n-----------------------------------------------\n");
        printf("|          ADD NEW CONTACT MENU              |\n");
        printf("-----------------------------------------------\n");
        printf("0. Back                                    \n");
        printf("1. Name:    %-30s |\n", (strlen(new_contact.name[0]) > 0) ? new_contact.name[0] : " ");

        // Display Phone Numbers
        if (phone_count == 0)
        {
            printf("2. Phone No 1:                              \n");
        }
        else
        {
            for (int i = 0; i < phone_count; i++)
            {
                printf("2.%d. Phone No: %-30s \n", i + 1, new_contact.phone_numbers[i]);
            }
        }

        // Display Emails
        if (email_count == 0)
        {
            printf("3. Email ID 1:                              \n");
        }
        else
        {
            for (int i = 0; i < email_count; i++)
            {
                printf("3.%d. Email ID: %-30s \n", i + 1, new_contact.email_addresses[i]);
            }
        }

        printf("-----------------------------------------------\n");
        printf("Please select an option: ");

        get_option(&option, "");

        switch (option)
        {
            case 1:
                printf("Enter Name: ");
                fgets(new_contact.name[0], NAME_LEN, stdin);
                new_contact.name[0][strcspn(new_contact.name[0], "\n")] = '\0';
                break;

            case 2:
                if (phone_count < PHONE_NUMBER_COUNT)
                {
                    printf("Enter Phone Number %d: ", phone_count + 1);
                    fgets(new_contact.phone_numbers[phone_count], NUMBER_LEN, stdin);
                    new_contact.phone_numbers[phone_count][strcspn(new_contact.phone_numbers[phone_count], "\n")] = '\0';
                    phone_count++;
                    new_contact.phone_count = phone_count;  // Ensure phone_count is stored
                }
                else
                {
                    printf("Maximum phone numbers reached!\n");
                }
                break;

            case 3:
                if (email_count < EMAIL_ID_COUNT)
                {
                    printf("Enter Email ID %d: ", email_count + 1);
                    fgets(new_contact.email_addresses[email_count], EMAIL_ID_LEN, stdin);
                    new_contact.email_addresses[email_count][strcspn(new_contact.email_addresses[email_count], "\n")] = '\0';
                    email_count++;
                    new_contact.email_count = email_count;  // Ensure email_count is stored
                }
                else
                {
                    printf("Maximum email IDs reached!\n");
                }
                break;

            case 0:
                printf("\nReturning to main menu...\n");
                break;

            default:
                printf("\nInvalid option! Please try again.\n");
                break;
        }
    } while (option != 0);

    // Ensure the contact is properly stored
    if (strlen(new_contact.name[0]) > 0 && phone_count > 0)
    {
        address_book->list = realloc(address_book->list, (address_book->count + 1) * sizeof(ContactInfo));
        if (!address_book->list)
        {
            printf("Memory allocation failed.\n");
            return e_fail;
        }

        address_book->list[address_book->count] = new_contact;
        address_book->count++;

        printf("Contact added successfully!\n");
        return e_success;
    }

    return e_fail;
}



Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
		int i, match_count = 0;

    printf("\n###### Address Book ######\n");
    printf("###### Search Results:\n");
    printf("\nS.No : Name              : Phone No      : Email ID\n");
    printf("-------------------------------------------------------------\n");

    for (i = 0; i < loop_count; i++) {
        ContactInfo contact = address_book->list[i];
        bool_t is_match = 0;

        switch (field) {
            case 1: 
	#ifdef _WIN32
                if (_stricmp(contact.name[0], str) == 0)
	#else
                if (strcasecmp(contact.name[0], str) == 0)
	#endif
                    is_match = 1;
                break;

            case 2:
	#ifdef _WIN32
                if (_stricmp(contact.phone_numbers[0], str) == 0)
	#else
                if (strcasecmp(contact.phone_numbers[0], str) == 0)
	#endif
                    is_match = 1;
                break;

            case 3:
	#ifdef _WIN32
                if (_stricmp(contact.email_addresses[0], str) == 0)
	#else
                if (strcasecmp(contact.email_addresses[0], str) == 0)
	#endif
                    is_match = 1;
                break;
        }

        	if (is_match) {
            printf("%-5d : %-17s : %-13s : %s\n",
                   contact.si_no, contact.name[0], contact.phone_numbers[0], contact.email_addresses[0]);
            match_count++;
        	}
    	}

    	if (match_count == 0) {
        printf("\nNo matching contacts found.\n");
    	}

    	return (match_count > 0) ? e_success : e_no_match;
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
	if (address_book->count == 0) {
        printf("\nNo contacts available.\n");
        return e_no_match;
    }

	char search_key[NAME_LEN];
		int i, match_count = 0;
	
		// Display search options
		printf("\nSearch Contact by:\n");
        printf("0. Back\n");
		printf("1. Name\n");
		printf("2. Phone Number\n");
		printf("3. Email Address\n");
		printf("Please select an option: ");
	
		int option;
		scanf("%d", &option);
		getchar();  // Clear input buffer 
	
		// Prompt user for input based on search type
		switch (option)
		{
			case 1: // Search by Name
				printf("\nEnter the name: ");
				fgets(search_key, NAME_LEN, stdin);
				search_key[strcspn(search_key, "\n")] = 0;  // Remove newline
				break;
	
			case 2: // Search by Phone #
				printf("\nEnter the phone number: ");
				fgets(search_key, NUMBER_LEN, stdin);
				search_key[strcspn(search_key, "\n")] = 0;
				break;
	
			case 3: // Search by Email Address
				printf("\nEnter the email address: ");
				fgets(search_key, EMAIL_ID_LEN, stdin);
				search_key[strcspn(search_key, "\n")] = 0;
				break;

            case 0:
                printf("\nReturning to main menu...\n");
                break;
	
			default:
				printf("\nInvalid option. Please try again.\n");
				return e_fail;
		}
	
		// Display header for search results
		printf("\n###### Address Book ######\n");
		printf("###### Search Result:\n");
		printf("\nS.No : Name              : Phone No      : Email ID\n");
		printf("-------------------------------------------------------------\n");
	
		// Loop through the contacts to find a match
		for (i = 0; i < address_book->count; i++)
		{
			ContactInfo contact = address_book->list[i];
			bool_t is_match = 0;
	
			switch (option)
			{
				case 1: // Match name
					if (strcasecmp(contact.name[0], search_key) == 0)
					{
						is_match = 1;
					}
					break;
	
				case 2: // Match phone number
					if (strcasecmp(contact.phone_numbers[0], search_key) == 0)
					{
						is_match = 1;
					}
					break;
	
				case 3: // Match email
					if (strcasecmp(contact.email_addresses[0], search_key) == 0)
					{
						is_match = 1;
					}
					break;
			}
            if (option == 0){
                printf("\nReturning to main menu...\n");
                return e_success;
            }
		   
			if (is_match)
			{
				// Display contact with Serial No
				printf("%-5d : %-17s : %-13s : %s\n", contact.si_no, contact.name[0], contact.phone_numbers[0], contact.email_addresses[0]);
				match_count++;
			}
		}
	
		if (match_count == 0)
		{
			printf("\nNo matching contact found.\n");
		}
	
		printf("-------------------------------------------------------------\n");
		printf("Press: [q] | Cancel:\n");
		
		getchar();

		return e_success;
}

Status edit_contact(AddressBook *address_book)
{
    if (address_book->count == 0)
    {
        printf("There are no contacts available to edit.\n");
        return e_no_match;
    }

    char search_key[NAME_LEN];
    int option, i, found = -1;

    printf("\nEdit Contact\n");
    printf("Enter Name to Search (or 0 to go back): ");
    fgets(search_key, NAME_LEN, stdin);
    search_key[strcspn(search_key, "\n")] = '\0';

    if (strcmp(search_key, "0") == 0) {
        printf("\nReturning to main menu...\n");
        return e_success;
    }

    for (i = 0; i < address_book->count; i++)
    {
        if (strcasecmp(address_book->list[i].name[0], search_key) == 0)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("\nNo matching contact found.\n");
        return e_no_match;
    }

    ContactInfo *contact = &address_book->list[found];

    do
    {
        printf("\nEditing Contact: %s\n", contact->name[0]);
        printf("1. Edit Name\n");
        printf("2. Edit Phone Number(s)\n");
        printf("3. Edit/Add Email ID(s)\n");
        printf("0. Back\n");
        printf("Choose an option: ");

        get_option(&option, "");

        switch (option)
        {
            case 1:
                printf("Enter new Name: ");
                fgets(contact->name[0], NAME_LEN, stdin);
                contact->name[0][strcspn(contact->name[0], "\n")] = '\0';
                break;

            case 2: // Edit phone numbers
                if (contact->phone_count == 0)
                {
                    printf("No phone numbers available. Add a new one.\n");
                }
                else
                {
                    printf("Existing Phone Numbers:\n");
                    for (int j = 0; j < contact->phone_count; j++)
                    {
                        printf("%d. %s\n", j + 1, contact->phone_numbers[j]);
                    }
                }
                printf("\nChoose a number index to edit (or 0 to add a new number or any other key to return menu): ");
                int num_index;
                get_option(&num_index, "");

                if (num_index == 0 && contact->phone_count < PHONE_NUMBER_COUNT)
                {
                    printf("Enter new Phone Number: ");
                    fgets(contact->phone_numbers[contact->phone_count], NUMBER_LEN, stdin);
                    contact->phone_numbers[contact->phone_count][strcspn(contact->phone_numbers[contact->phone_count], "\n")] = '\0';
                    contact->phone_count++;  //  Ensure phone_count updates
                }
                else if (num_index > 0 && num_index <= contact->phone_count)
                {
                    printf("Enter new Phone Number: ");
                    fgets(contact->phone_numbers[num_index - 1], NUMBER_LEN, stdin);
                    contact->phone_numbers[num_index - 1][strcspn(contact->phone_numbers[num_index - 1], "\n")] = '\0';
                }
                else
                {
                    printf("Invalid choice.\n");
                }
                break;

            case 3: // Edit/Add email addresses
                if (contact->email_count == 0)
                {
                    printf("No email IDs available. Add a new one.\n");
                }
                else
                {
                    printf("Existing Email IDs:\n");
                    for (int j = 0; j < contact->email_count; j++)
                    {
                        printf("%d. %s\n", j + 1, contact->email_addresses[j]);
                    }
                }
                printf("\nChoose an email index to edit (or 0 to add a new email or any other key to return men): ");
                int email_index;
                get_option(&email_index, "");

                if (email_index == 0 && contact->email_count < EMAIL_ID_COUNT)
                {
                    printf("Enter new Email ID: ");
                    fgets(contact->email_addresses[contact->email_count], EMAIL_ID_LEN, stdin);
                    contact->email_addresses[contact->email_count][strcspn(contact->email_addresses[contact->email_count], "\n")] = '\0';
                    contact->email_count++;  //  Ensure email_count updates
                }
                else if (email_index > 0 && email_index <= contact->email_count)
                {
                    printf("Enter new Email ID: ");
                    fgets(contact->email_addresses[email_index - 1], EMAIL_ID_LEN, stdin);
                    contact->email_addresses[email_index - 1][strcspn(contact->email_addresses[email_index - 1], "\n")] = '\0';
                }
                else
                {
                    printf("Invalid choice.\n");
                }
                break;

            case 4:
                printf("\nReturning to main menu...\n");
                break;

            default:
                printf("\nInvalid option! Please try again.\n");
                break;
        }
    } while (option != 0);

    return e_success;
}


Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
        if (address_book->count == 0)
        {
            printf("There are no contacts available to delete.\n");
            return e_no_match;
        }
    
        char search_key[EMAIL_ID_LEN];
        int option, i, found = -1;
    
        printf("\nDelete Contact\n");
        printf("0. Back\n");
        printf("1. Name\n");
        printf("2. Phone Number\n");
        printf("3. Email Address\n");
        printf("Choose an option: ");
        get_option(&option, "");
    
        if (option == 0)
        {
            printf("\nReturning to main menu...\n");
            return e_success;
        }
    
        printf("Enter search term: ");
        fgets(search_key, sizeof(search_key), stdin);
        search_key[strcspn(search_key, "\n")] = 0;
    
        for (i = 0; i < address_book->count; i++)
        {
            ContactInfo *contact = &address_book->list[i];
    
            if ((option == 1 && strcmp(contact->name[0], search_key) == 0) ||
                (option == 2 && strcmp(contact->phone_numbers[0], search_key) == 0) ||
                (option == 3 && strcmp(contact->email_addresses[0], search_key) == 0))
            {
                found = i;
                break;
            }
        }
    
        if (found == -1)
        {
            printf("\nNo matching contact found.\n");
            return e_no_match;
        }
    
        printf("\nAre you sure you want to delete this contact? (Y/N): ");
        char confirm;
        scanf(" %c", &confirm);
        confirm = tolower(confirm);
    
        if (confirm == 'y')
        {
            for (int j = found; j < address_book->count - 1; j++)
            {
                address_book->list[j] = address_book->list[j + 1];
            }
            address_book->count--;
            printf("\nContact deleted successfully!\n");
            save_file(address_book);
        }
        else
        {
            printf("\nDeletion canceled.\n");
        }
    
        return e_success;
    }
    