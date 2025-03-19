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
	/* Add the functionality for adding contacts here */
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
}

Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
