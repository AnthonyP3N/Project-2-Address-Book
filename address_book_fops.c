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

    if (ret == 0){
        address_book->fp = fopen(DEFAULT_FILE, "r"); 
    }
    else{
        address_book->fp = fopen(DEFAULT_FILE, "w");
    }

    if (address_book->fp == NULL){
        return e_fail;
    }

    address_book->count = 0;
    address_book->list = NULL;

    // Read the contacts from the file
    char line[256];
    while (fgets(line, sizeof(line), address_book->fp)){
        ContactInfo contact;
        if (sscanf(line, "%31[^,],%31[^,],%31[^\n]",
                   contact.name[0],
                   contact.phone_numbers[0],
                   contact.email_addresses[0]) == 3)
        {
            contact.si_no = address_book->count + 1;
            address_book->list = realloc(address_book->list, (address_book->count + 1) * sizeof(ContactInfo));
            if (!address_book->list)
            {
                fclose(address_book->fp);
                return e_fail;
            }
            address_book->list[address_book->count] = contact;
            address_book->count++;
        }
    }

    fclose(address_book->fp);
    return e_success;
}


Status save_file(AddressBook *address_book)
{
	/*
	 * Write contacts back to file.
	 * Re write the complete file currently
	 */ 

    address_book->fp = fopen(DEFAULT_FILE, "w");
    	if (address_book->fp == NULL){
        	return e_fail;
    	}

    	for (int i = 0; i < address_book->count; i++){
        	ContactInfo *contact = &address_book->list[i];

        	fprintf(address_book->fp, "%s,%s,%s\n",
                contact->name[0],
                contact->phone_numbers[0],
                contact->email_addresses[0]);
    }

    fclose(address_book->fp);
    return e_success;
}

	
