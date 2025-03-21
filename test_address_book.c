#include "unity.h"
#include "address_book.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "address_book_fops.h"

#define TEST_FILE "test_address_book.csv"

AddressBook test_address_book;

/* Test setup function - Runs before every test */
void setUp(void)
{
    test_address_book.count = 0;
    test_address_book.list = NULL;
}

/* Test teardown function - Runs after every test */
void tearDown(void)
{
    if (test_address_book.list)
    {
        free(test_address_book.list);
        test_address_book.list = NULL;
    }
}

/* Test Case: Save and Load Single Contact */
void test_save_load_single_contact(void)
{
    ContactInfo contact = {0};
    contact.si_no = 1;
    strcpy(contact.name[0], "John Doe");
    strcpy(contact.phone_numbers[0], "1234567890");
    strcpy(contact.email_addresses[0], "john@example.com");
    contact.phone_count = 1;
    contact.email_count = 1;

    // Add contact to test address book
    test_address_book.count = 1;
    test_address_book.list = malloc(sizeof(ContactInfo));
    test_address_book.list[0] = contact;

    // Save and reload
    save_file(&test_address_book);
    test_address_book.count = 0;
    free(test_address_book.list);
    test_address_book.list = NULL;
    load_file(&test_address_book);

    // Assertions
    TEST_ASSERT_EQUAL_INT(1, test_address_book.count);
    TEST_ASSERT_EQUAL_INT(1, test_address_book.list[0].si_no);
    TEST_ASSERT_EQUAL_STRING("John Doe", test_address_book.list[0].name[0]);
    TEST_ASSERT_EQUAL_STRING("1234567890", test_address_book.list[0].phone_numbers[0]);
    TEST_ASSERT_EQUAL_STRING("john@example.com", test_address_book.list[0].email_addresses[0]);
}

/* Test Case: Save and Load Multiple Contacts */
void test_save_load_multiple_contacts(void)
{
    ContactInfo contacts[2] = {0};

    // Contact 1
    contacts[0].si_no = 1;
    strcpy(contacts[0].name[0], "Ant");
    strcpy(contacts[0].phone_numbers[0], "1111111111");
    strcpy(contacts[0].email_addresses[0], "ant@example.com");
    contacts[0].phone_count = 1;
    contacts[0].email_count = 1;

    // Contact 2
    contacts[1].si_no = 2;
    strcpy(contacts[1].name[0], "Tim");
    strcpy(contacts[1].phone_numbers[0], "2222222222");
    strcpy(contacts[1].email_addresses[0], "tim@example.com");
    contacts[1].phone_count = 1;
    contacts[1].email_count = 1;

    // Add contacts to test address book
    test_address_book.count = 2;
    test_address_book.list = malloc(2 * sizeof(ContactInfo));
    memcpy(test_address_book.list, contacts, 2 * sizeof(ContactInfo));

    // Save and reload
    save_file(&test_address_book);
    test_address_book.count = 0;
    free(test_address_book.list);
    test_address_book.list = NULL;
    load_file(&test_address_book);

    // Assertions
    TEST_ASSERT_EQUAL_INT(2, test_address_book.count);

    TEST_ASSERT_EQUAL_INT(1, test_address_book.list[0].si_no);
    TEST_ASSERT_EQUAL_STRING("Ant", test_address_book.list[0].name[0]);
    TEST_ASSERT_EQUAL_STRING("1111111111", test_address_book.list[0].phone_numbers[0]);
    TEST_ASSERT_EQUAL_STRING("ant@example.com", test_address_book.list[0].email_addresses[0]);

    TEST_ASSERT_EQUAL_INT(2, test_address_book.list[1].si_no);
    TEST_ASSERT_EQUAL_STRING("Tim", test_address_book.list[1].name[0]);
    TEST_ASSERT_EQUAL_STRING("2222222222", test_address_book.list[1].phone_numbers[0]);
    TEST_ASSERT_EQUAL_STRING("tim@example.com", test_address_book.list[1].email_addresses[0]);
}

/* Test Case: Ensure Emails Persist on Save and Load */
void test_email_persistence(void)
{
    ContactInfo contact = {0};
    contact.si_no = 1;
    strcpy(contact.name[0], "Charlie");
    strcpy(contact.phone_numbers[0], "3333333333");
    strcpy(contact.email_addresses[0], "charlie@example.com");
    strcpy(contact.email_addresses[1], "charlie.work@example.com");
    contact.phone_count = 1;
    contact.email_count = 2;

    // Add contact to test address book
    test_address_book.count = 1;
    test_address_book.list = malloc(sizeof(ContactInfo));
    test_address_book.list[0] = contact;

    // Save and reload
    save_file(&test_address_book);
    test_address_book.count = 0;
    free(test_address_book.list);
    test_address_book.list = NULL;
    load_file(&test_address_book);

    // Assertions
    TEST_ASSERT_EQUAL_INT(1, test_address_book.count);
    TEST_ASSERT_EQUAL_INT(2, test_address_book.list[0].email_count);
    TEST_ASSERT_EQUAL_STRING("charlie@example.com", test_address_book.list[0].email_addresses[0]);
    TEST_ASSERT_EQUAL_STRING("charlie.work@example.com", test_address_book.list[0].email_addresses[1]);
}

/* Test Case: Save and Load Empty Contact List */
void test_save_load_empty(void)
{
    test_address_book.count = 0;
    test_address_book.list = NULL;

    // Save and reload
    save_file(&test_address_book);
    load_file(&test_address_book);

    // Assertions
    TEST_ASSERT_EQUAL_INT(0, test_address_book.count);
    TEST_ASSERT_NULL(test_address_book.list);
}

/* Main function for running Unity tests */
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_save_load_single_contact);
    RUN_TEST(test_save_load_multiple_contacts);
    RUN_TEST(test_email_persistence);
    RUN_TEST(test_save_load_empty);

    return UNITY_END();
}
