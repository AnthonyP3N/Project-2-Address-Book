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
