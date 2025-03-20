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
	scanf("%d", type);
	fgets(msg, NAME_LEN, stdin);
	return type;
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
		 if (address_book->count == 0) {
			 printf("There are no contacts available yet. If you would like to add contacts, please quit and select add contacts in menu options.\n");
			 return e_no_match;
		 }
		 
		 int page = 0;
		 int total_pages = address_book->count + WINDOW_SIZE - 1/ WINDOW_SIZE; //count total number of pages needed
		 char option;
		 
		 do{
			 printf("==============================================================================================================");
			 printf("%-3s %-7s %-3s %-30s %-3s %-20s %-3s %-30s %-3s\n",":", "S.No",":", "Name", ":", "Phone No", ":", "Email ID", ":" );
			 printf("==============================================================================================================");
			 
			for (int i = page * WINDOW_SIZE; i < (page + 1) * WINDOW_SIZE && i < address_book->count; i++)
			{
				ContactInfo *contact = &address_book->list[i];
				printf("%-3s %-7s %-3s %-30s %-3s %-20s %-3s %-30s %-3s\n", 
					   ":",
					   contact->si_no,
					   ":", 
					   contact->name[0], 
					   ":",
					   contact->phone_numbers[0], 
					   ":",
					   contact->email_addresses[0],
						" :");
			}
			
			printf("-------------------------------------------------------------------------------\n");
			printf("Page %d of %d. Use 'N' for next, 'P' for previous, 'Q' to quit: ", page + 1, total_pages);
			option = getchar();
			while (getchar() != '\n'); // Clear input buffer
	
			if (option == 'N' || option == 'n')
			{
				if (page < total_pages - 1)
					page++;
			}
			else if (option == 'P' || option == 'p')
			{
				if (page > 0)
					page--;
			}
		} while (option != 'Q' && option != 'q');
	
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
	// getting new contact details
	if (!address_book)
	{
		return e_fail;
	}

	int answer;
	ContactInfo new_contact;

	while (answer == !0)
	{
		printf("####### Address Book ####### \n");
		printf("####### Add contact \n\n");
		printf("0. Back \n");
		printf("1. Name: ");
		printf("%s", new_contact.name, "\n");
		printf("2. Phone Number ID: ");
		printf("%s", new_contact.phone_numbers, "\n");
		printf("3. Email ID: ");
		printf("%s", new_contact.email_addresses, "\n\n");
		printf("Please select an option");

		switch(answer)
		{

			case 1:
			printf("Enter Name: ");
			fgets(new_contact.name[0], NAME_LEN, stdin);
			new_contact.name[0][strcspn(new_contact.name[0], "\n")] = '\0';
			break;

			case 2:
			printf("Enter Phone Number ID: ");
			fgets(new_contact.phone_numbers[0], NUMBER_LEN, stdin);
			new_contact.phone_numbers[0][strcspn(new_contact.phone_numbers[0], "\n")] = '\0';
			break;

			case 3:
			printf("Enter Email: ");
			fgets(new_contact.email_addresses[0], EMAIL_ID_LEN, stdin);
			new_contact.email_addresses[0][strcspn(new_contact.email_addresses[0], "\n")] = '\0';
			break;

			default:
			printf("\nPlease select an option");
			break;
		}		
	}
	
	new_contact.si_no = address_book -> count + 1;

	address_book -> list = realloc(address_book -> list, (address_book -> count + 1) * sizeof(ContactInfo));
	if (!address_book -> list)
	{
		printf("Memory allocation failed. \n");
		return e_fail;
	}

	address_book -> list[address_book -> count] = new_contact;
	address_book -> count++;

	printf("Contact added successfully.\n");
	return e_success;

	if (answer == 0)
	{
		main_menu();
	}
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

	if(address_book->count == 0){
		printf("There are no contacts available to edit.\n");
		return e_no_match;
	}

	char search_in[EMAIL_ID_LEN];
	int stype;
	int i;
	int found = 0; 

	printf("\nEdit Contact\n");
	printf("Search by: \n1.");
	printf("Name \n2.");
	printf("Phone Number \n3.");
	printf("Email \n");
	printf("Choose an option (1-3): ");
	scanf("%d", &stype);
	getchar();

	printf("Enter search term: ");
	fgets(search_in, sizeof(search_in), stdin);
	search_in[strcspn(search_in, "\n")] = 0;

	for(i=0; i< address_book->count; i++){
		ContactInfo *contact = &address_book->list[i];
		if((stype == 1 && strcmp(contact->name[0], search_in) == 0)||
			(stype == 2 && strcmp(contact->phone_numbers[0], search_in) == 0)||
			(stype == 3 && strcmp(contact->email_addresses[0], search_in) == 0)){
				found = 1;
				printf("\nContact found:\n");
				printf("Name: %s\n", contact->name[0]);
				printf("Phone: %s\n", contact->phone_numbers[0]);
				printf("Email: %s\n", contact->email_addresses[0]);

				printf("\n Which would you like to edit?\n1.");
				printf("Name \n2.");
				printf("Phone Number \n3.");
				printf("Email \n");
				printf("Choose an option (1-3): ");

				int edit_option;
				scanf("%d", &edit_option);
				getchar();

				if(edit_option == 1){
					printf("Enter a new name: ");
					fgets(contact->name[0], NAME_LEN, stdin);
					contact->name[0][strcspn(contact->name[0], "\n")] = 0;
				}else if(edit_option == 2){
					printf("Enter new phone number: ");
					fgets(contact->phone_numbers[0], NUMBER_LEN, stdin);
					contact->phone_numbers[0][strcspn(contact->phone_numbers[0], "\n")] = 0;
				}else if(edit_option == 3){
					printf("Enter a new email: ");
					fgets(contact->email_addresses[0], EMAIL_ID_LEN, stdin);
					contact->email_addresses[0][strcspn(contact->email_addresses[0], "\n")] = 0;
				}else{
					printf("Invalid choice. Edit failed.\n");
					return e_fail;
				}

				printf("\nContact updated successfully!\n");
				save_file(address_book);
				return e_success;
			}
		}
		
		if(!found){
			printf("\n No matching contact informaiton found.\n");
			return e_no_match;
		}

		return e_fail;
	}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */

	if(address_book->count == 0){
		printf("There are no contacts available to delete.\n");
		return e_no_match;
	}

	char search_in[EMAIL_ID_LEN];
	int stype;
	int i;
	int found = -1; 

	printf("\n Delete Contact\n");
	printf("Search by: \n1.");
	printf("Name \n2.");
	printf("Phone Number \n3.");
	printf("Email \n");
	printf("Choose an option (1-3): ");
	scanf("%d", &stype);
	getchar();

	printf("Enter search term: ");
	fgets(search_in, sizeof(search_in), stdin);
	search_in[strcspn(search_in, "\n")] = 0;

	for(i=0; i< address_book->count; i++){
		ContactInfo *contact = &address_book->list[i];
		if((stype == 1 && strcmp(contact->name[0], search_in) == 0)||
			(stype == 2 && strcmp(contact->phone_numbers[0], search_in) == 0)||
			(stype == 3 && strcmp(contact->email_addresses[0], search_in) == 0)){	
			
			found = i;
			printf("\nContact found:\n");
			printf("Name: %s\n",contact->name[0]);
			printf("Phone: %s\n",contact->phone_numbers[0]);
			printf("Email: %s\n",contact->email_addresses[0]);
			
			printf("\nAre you sure you want to delete this contact? (Y/N): ");
			char confirm;
			scanf(" %c", &confirm);
			confirm = tolower(confirm);

			if(confirm == 'y'){
				for(int j = found; j < address_book->count - 1; j++){
					address_book->list[j] = address_book -> list[j+1];
				}
				address_book->count--;
				printf("\nContact deleted successfully!\n");
				save_file(address_book);
				return e_success;
			}else{
				printf("\nDeletion canceled.\n");
				return e_fail;
			}
		}
	}
	printf("\nNo matching contact found.\n");
	return e_no_match;
}
