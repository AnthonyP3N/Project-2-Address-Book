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
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
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
}
