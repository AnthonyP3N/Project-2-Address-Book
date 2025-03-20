#include <stdio.h>
//#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_menu.h"

//#include "abk_fileops.h"
//#include "abk_log.h"
//#include "abk_menus.h"
//#include "abk.h"

int get_option(int type, const char *msg)
{
	/*
	 * Mutilfuction user intractions like
	 * Just an enter key detection
	 * Read an number
	 * Read a charcter
	 */ 

	/* Fill the code to add above functionality */
}

Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		main_menu();

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/* 
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */ 


	return e_success;
}

void menu_header(const char *str)
{
	fflush(stdout);

	system("clear");

	printf("#######  Address Book  #######\n");
	if (str != '\0')
	{
		printf("#######  %s\n", str);
	}
}

void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

Status menu(AddressBook *address_book)
{
	ContactInfo backup;
	Status ret;
	int option;

	do
	{
		main_menu();

		option = get_option(NUM, "");

		if ((address_book-> count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found!!. Would you like to add? Use Add Contacts");

			continue;
		}

		switch (option)
		{
			case e_add_contact:
				/* Add your implementation to call add_contacts function here */
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
				break;
				/* Add your implementation to call list_contacts function here */
			case e_save:
				save_file(address_book);
				break;
			case e_exit:
				break;
		}
	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook *address_book)
{
	/* Add the functionality for adding contacts here */
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
	{
		int i, match_count = 0;
	
		// Display header for search results
		printf("\n###### Address Book ######\n");
		printf("###### Search Results:\n");
		printf("\nS.No : Name              : Phone No      : Email ID\n");
		printf("-------------------------------------------------------------\n");
	
		// Loop through the specified number of contacts
		for (i = 0; i < loop_count; i++)
		{
			ContactInfo contact = address_book->list[i];
			bool_t is_match = 0;
	
			// Determine which field to compare against the search string
			switch (field)
			{
				case 1: // Search by Name
					if (strcasecmp(contact.name, str) == 0)
					{
						is_match = 1;
					}
					break;
	
				case 2: // Search by Phone Number
					if (strcasecmp(contact.phone_numbers, str) == 0)
					{
						is_match = 1;
					}
					break;
	
				case 3: // Search by Email Address
					if (strcasecmp(contact.email_addresses, str) == 0)
					{
						is_match = 1;
					}
					break;
	
				default:
					printf("\nInvalid field type specified for search.\n");
					return e_fail;
			}
	
			if (is_match)
			{
				// Display matching contact details
				printf("%-5d : %-17s : %-13s : %s\n", contact.si_no, contact.name, contact.phone_numbers, contact.email_addresses);
				match_count++;
			}
		}
	
		if (match_count == 0)
		{
			printf("\nNo matching contacts found.\n");
		}
	
		printf("-------------------------------------------------------------\n");
	
		// Provide any additional message as needed
		if (msg != NULL)
		{
			printf("%s\n", msg);
		}
	
		return (match_count > 0) ? e_success : e_no_match;
	}
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
	{char search_key[NAME_LEN];
		int i, match_count = 0;
	
		// Display search options
		printf("\nSearch Contact by:\n");
		printf("1. Name\n");
		printf("2. Phone Number\n");
		printf("3. Email Address\n");
		printf("Please select an option: ");
	
		int option;
		scanf("%d", &option);
		__fpurge(stdin);  // Clear input buffer 
	
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
					if (strcasecmp(contact.name, search_key) == 0)
					{
						is_match = 1;
					}
					break;
	
				case 2: // Match phone number
					if (strcasecmp(contact.phone_numbers, search_key) == 0)
					{
						is_match = 1;
					}
					break;
	
				case 3: // Match email
					if (strcasecmp(contact.email_addresses, search_key) == 0)
					{
						is_match = 1;
					}
					break;
			}
		   
			if (is_match)
			{
				// Display contact with Serial No
				printf("%-5d : %-17s : %-13s : %s\n", contact.si_no, contact.name, contact.phone_numbers, contact.email_addresses);
				match_count++;
			}
		}
	
		if (match_count == 0)
		{
			printf("\nNo matching contact found.\n");
		}
	
		printf("-------------------------------------------------------------\n");
		printf("Press: [q] | Cancel:\n");
	
		return e_success;
	}
}

Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
