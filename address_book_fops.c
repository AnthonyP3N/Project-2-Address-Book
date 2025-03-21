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
    FILE *fp = fopen(DEFAULT_FILE, "r");
    if (!fp)
    {
        printf("No existing address book found. Creating a new one...\n");

        fp = fopen(DEFAULT_FILE, "w");
        if (!fp)
        {
            printf("Error: Could not create file %s\n", DEFAULT_FILE);
            return e_fail;
        }
        fclose(fp);

        address_book->count = 0;
        address_book->list = NULL;
        return e_success;
    }

    address_book->count = 0;
    address_book->list = NULL;

    char line[256];
    ContactInfo new_contact = {0};  //  Temporary storage for one contact

    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = '\0';  //  Remove newline character

        if (strncmp(line, "S:", 2) == 0)  //  Serial Number
        {
            new_contact.si_no = atoi(line + 2);
        }
        else if (strncmp(line, "N:", 2) == 0)  //  Name
        {
            strncpy(new_contact.name[0], line + 2, NAME_LEN);
            new_contact.name[0][NAME_LEN - 1] = '\0';
        }
        else if (strncmp(line, "P:", 2) == 0)  //  Phone Numbers
        {
            if (new_contact.phone_count < PHONE_NUMBER_COUNT)
            {
                strncpy(new_contact.phone_numbers[new_contact.phone_count], line + 2, NUMBER_LEN);
                new_contact.phone_numbers[new_contact.phone_count][NUMBER_LEN - 1] = '\0';
                new_contact.phone_count++;
            }
        }
        else if (strncmp(line, "E:", 2) == 0)  //  Email Addresses
        {
            if (new_contact.email_count < EMAIL_ID_COUNT)
            {
                strncpy(new_contact.email_addresses[new_contact.email_count], line + 2, EMAIL_ID_LEN);
                new_contact.email_addresses[new_contact.email_count][EMAIL_ID_LEN - 1] = '\0';
                new_contact.email_count++;
            }
        }
        else if (strcmp(line, "END") == 0)  //  Marks End of One Contact
        {
            //  Store contact in address book
            ContactInfo *temp = realloc(address_book->list, (address_book->count + 1) * sizeof(ContactInfo));
            if (!temp)
            {
                printf("Error: Memory allocation failed while loading file.\n");
                fclose(fp);
                return e_fail;
            }
            address_book->list = temp;
            address_book->list[address_book->count] = new_contact;
            address_book->count++;

            //  Reset `new_contact` for the next entry
            memset(&new_contact, 0, sizeof(ContactInfo));
        }
    }

    fclose(fp);
    printf("Contacts successfully loaded from %s\n", DEFAULT_FILE);
    return e_success;
}

    
    
    
    
    
    
    
    
    
    
Status save_file(AddressBook *address_book)
{
	/*
	 * Write contacts back to file.
	 * Re write the complete file currently
	 */ 
    if (!address_book || !address_book->list)
    {
        printf("Error: No contacts to save.\n");
        return e_fail;
    }

    FILE *fp = fopen(DEFAULT_FILE, "w");
    if (!fp)
    {
        printf("Error: Unable to open file for saving.\n");
        return e_fail;
    }

    for (int i = 0; i < address_book->count; i++)
    {
        ContactInfo *contact = &address_book->list[i];

        //  Write Serial Number
        fprintf(fp, "S:%d\n", contact->si_no);

        //  Write Name
        fprintf(fp, "N:%s\n", contact->name[0]);

        //  Write Phone Numbers (One Per Line)
        for (int j = 0; j < contact->phone_count; j++)
        {
            fprintf(fp, "P:%s\n", contact->phone_numbers[j]);
        }

        //  Write Email Addresses (One Per Line)
        for (int j = 0; j < contact->email_count; j++)
        {
            fprintf(fp, "E:%s\n", contact->email_addresses[j]);
        }

        fprintf(fp, "END\n");  //  Marks end of one contact
    }

    fclose(fp);
    printf("Contacts successfully saved to %s\n", DEFAULT_FILE);
    return e_success;
}
