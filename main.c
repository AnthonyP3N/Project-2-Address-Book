#include <stdio.h>
//#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>

#include "address_book.h"
#include "address_book_fops.h"
#include "address_book_menu.h"

int main(void)
{
    AddressBook address_book;
    Status ret;

    /* Load the file from .csv file if any */
    ret = load_file(&address_book);

    if (ret == e_success)
    {
        /* Show all the available menu */
        ret = menu(&address_book);
    }

    /*  Always ask to save before exiting */
    if (address_book.count > 0 || address_book.count == 0)
    {
        save_prompt(&address_book);
    }

    printf("Program exited successfully.\n");
    return 0;
}
