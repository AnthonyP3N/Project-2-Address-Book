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
            printf("No existing address book found. Creating a new one.\n");
            return e_fail;
        }
    
        address_book->count = 0;
        address_book->list = NULL;
    
        char line[1024];  
        while (fgets(line, sizeof(line), fp))
        {
            ContactInfo new_contact = {0};
            char *token;
    
            //  Read Serial Number
            token = strtok(line, ",");
            if (!token)
            {
                printf("Error: Corrupted data, skipping line.\n");
                continue;
            }
            new_contact.si_no = atoi(token);
    
            //  Read Name
            token = strtok(NULL, ",");
            if (!token)
            {
                printf("Error: Corrupted data, skipping line.\n");
                continue;
            }
            strncpy(new_contact.name[0], token, NAME_LEN);
            new_contact.name[0][NAME_LEN - 1] = '\0';
    
            //  Read Phone Numbers (split by `|`)
            token = strtok(NULL, ",");
            if (token)
            {
                char *phone_token = strtok(token, "|");
                while (phone_token)
                {
                    strncpy(new_contact.phone_numbers[new_contact.phone_count], phone_token, NUMBER_LEN);
                    new_contact.phone_numbers[new_contact.phone_count][NUMBER_LEN - 1] = '\0';
                    new_contact.phone_count++;
    
                    phone_token = strtok(NULL, "|");
                }
            }
    
            //  Read Email Addresses (split by `;`)
            token = strtok(NULL, "\n");  
            if (token)
            {
                char *email_token = strtok(token, ";");
                while (email_token)
                {
                    strncpy(new_contact.email_addresses[new_contact.email_count], email_token, EMAIL_ID_LEN);
                    new_contact.email_addresses[new_contact.email_count][EMAIL_ID_LEN - 1] = '\0';
                    new_contact.email_count++;
    
                    email_token = strtok(NULL, ";");
                }
            }
    
            //  Ensure valid data before adding to the list
            if (strlen(new_contact.name[0]) == 0)
            {
                printf("Warning: Skipping invalid contact entry.\n");
                continue;
            }
    
            //  Expand the list dynamically
            ContactInfo *temp = realloc(address_book->list, (address_book->count + 1) * sizeof(ContactInfo));
            if (!temp)
            {
                printf("Error: Memory allocation failed while loading file.\n");
                fclose(fp);
                return e_fail;
            }
            address_book->list = temp;
    
            //  Store the new contact
            address_book->list[address_book->count] = new_contact;
            address_book->count++;
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
            fprintf(fp, "%d,", i + 1);
    
            //  Write Name
            fprintf(fp, "%s,", contact->name[0]);
    
            //  Write all phone numbers (use `|` as separator)
            if (contact->phone_count > 0)
            {
                for (int j = 0; j < contact->phone_count; j++)
                {
                    fprintf(fp, "%s", contact->phone_numbers[j]);
                    if (j < contact->phone_count - 1)
                    {
                        fprintf(fp, "|");
                    }
                }
            }
            fprintf(fp, ",");
    
            //  Write all email addresses (use `;` as separator)
            if (contact->email_count > 0)
            {
                for (int j = 0; j < contact->email_count; j++)
                {
                    fprintf(fp, "%s", contact->email_addresses[j]);
                    if (j < contact->email_count - 1)
                    {
                        fprintf(fp, ";");
                    }
                }
            }
    
            fprintf(fp, "\n");  //  Move to next line for next contact
        }
    
        fclose(fp);
        printf("Contacts successfully saved to %s\n", DEFAULT_FILE);
        return e_success;
    }
    