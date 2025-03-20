#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

Status load_file(AddressBook *address_book)
{
	/* 
	 * Check for file existance
	 */
	struct stat buffer; 
	int ret = stat(DEFAULT_FILE, &buffer);

	if (ret == 0)
	{
		/* 
		 * Do the neccessary step to open the file
		 * Do error handling
		 */ 
		address_book->fp = fopen(DEFAULT_FILE, "r"); 

	}
	else
	{
		/* Create a file for adding entries */
		address_book->fp = fopen(DEFAULT_FILE, "w");
	}

	if (address_book->fp == NULL){
		return e_fail;
	}

	return e_success;
}

Status save_file(AddressBook *address_book)
{
	/*
	 * Write contacts back to file.
	 * Re write the complete file currently
	 */ 
	address_book->fp = fopen(DEFAULT_FILE, "w");

	if (address_book->fp == NULL)
	{
		return e_fail;
	}
	/* 
	 * Add the logic to save the file
	 * Make sure to do error handling
	 */ 
	for(int i = 0; i < address_book->count; i++){
		ContactInfo *contact = &address_book->list[i];
	
		for(int j = 0; j < PHONE_NUMBER_COUNT; j++){
		fprintf(address_book->fp, "%s", contact->name[0]);
		}

		for(int k = 0; k < EMAIL_ID_COUNT; k++){
			fprintf(address_book->fp, "%s", contact->name[0]);
		}

		fprintf(address_book->fp, "\n");
	}
	

	fclose(address_book->fp);

	return e_success;
}
