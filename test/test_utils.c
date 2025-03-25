#include "utils.h"
#include "unity.h"
#include "defs.h"
#include "CException.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {
    remove("test/files/open_source_file.asm");
}

void test_strip(void) {
    // both sides
    char s[32] = "  string    ";
    strip(s);
    TEST_ASSERT_EQUAL_STRING("string", s);

    // right side only
    strcpy(s, "string   ");
    TEST_ASSERT_EQUAL_STRING("string   ", s);

    strip(s);
    TEST_ASSERT_EQUAL_STRING("string", s);

    // left side only
    strcpy(s, "     string");
    TEST_ASSERT_EQUAL_STRING("     string", s);

    strip(s);
    TEST_ASSERT_EQUAL_STRING("string", s);
}

void test_is_empty_string(void) {
    // not empty, should return 0
    char not_empty[] = " "; 
    TEST_ASSERT_EQUAL(0, is_empty_string(not_empty));

    // empty, should return 1
    char empty[] = "";
    TEST_ASSERT_EQUAL(1, is_empty_string(empty));
}

void test_explode(void) {
    // should return 0 items
    char  **exploded_0_items = explode("", ";");
    const char **exploded_0_items_expected[] = {NULL};
    TEST_ASSERT_EQUAL_STRING_ARRAY(exploded_0_items_expected, exploded_0_items, 1);
    free_exploded(exploded_0_items);

    // should return 1 items
    char **exploded_1_items = explode("123", " ");
    const char *exploded_1_items_expected[] = {"123", NULL};
    TEST_ASSERT_EQUAL_STRING_ARRAY(exploded_1_items_expected, exploded_1_items, 1);
    free_exploded(exploded_1_items);

    // should return 3 items
    // 0 = 1, 1 = 2, 2 = 3
    char **exploded_3_items = explode("1 2 3", " ");
    const char *exploded_3_items_expected[] = {"1", "2", "3", NULL };
    TEST_ASSERT_EQUAL_STRING_ARRAY(exploded_3_items_expected, exploded_3_items, 3);
    free_exploded(exploded_3_items);
}

void test_arrlen(void) {
    // should return 0
    char  **exploded_0_items = explode("", ";");
    TEST_ASSERT_EQUAL(0, arrlen(exploded_0_items));
    free_exploded(exploded_0_items);

    // should return 3
    char **exploded_3_items = explode("1 2 3", " ");
    TEST_ASSERT_EQUAL(3, arrlen(exploded_3_items));
    free_exploded(exploded_3_items);
}

void test_in_arr(void) {
    // not in array, should return 0
    const char *not_arr[] = {"ronaldo", "brilha", "corinthians", NULL};
    TEST_ASSERT_EQUAL(0, in_arr("kek", not_arr));

    // in array, sould return 1
    // sapientiam autem non vincit malitia
    const char *arr[] = {"sapientiam", "autem", "non", "vincit", "malitia", NULL}; 
    TEST_ASSERT_EQUAL(1, in_arr("vincit", arr));
}

void test_re_match(void) {
    // re_match uses REG_EXTENDED
    // match pmatch[0].rm_so should be >= 0
    regmatch_t pmatch[1];
    re_match("[[:digit:]]+", "some str1ng to match", pmatch);
    TEST_ASSERT_GREATER_OR_EQUAL(0, pmatch[0].rm_so);

    // no match, pmatch[0].rm_so should be -1
    re_match("praise kek", "hello world", pmatch);
    TEST_ASSERT_EQUAL(-1, pmatch[0].rm_so);
}

void test_strtoupper(void) {
    // should succeed
    const char expected[] = "TEST GIVE B4D";
    char s[] = "tEsT giVe b4d";
    strtoupper(s);
    TEST_ASSERT_EQUAL_STRING(expected, s);
}

void test_get_file_name(void) {
    const char file_path[] = "some/buried/folder/Foo.vm";
    char *s = get_file_name(file_path);

    const char expected[] = "Foo";
    TEST_ASSERT_EQUAL_STRING(expected, s);
}

void test_open_source_file(void) {
    // should fail
    CEXCEPTION_T e;
    Try {
        open_source_file("test/files/file_doesnt_exist.vm");
    } Catch(e) {
        // exit throws error code
        TEST_ASSERT_EQUAL(1, e);
    }

    // should succeed
    FILE *file = open_source_file("test/files/open_source_file.vm");
    TEST_ASSERT_NOT_NULL(file);
    fclose(file);
}

void test_open_output_file(void) {
    // should fail
    CEXCEPTION_T e;
    FILE *file = open_source_file("test/files/open_source_file.vm");
    Try {
        // try to write to root, will fail
        const char file_path[] = "/write_to_root.vm";
        open_output_file(file_path, file);
    } Catch(e) {
        // do not fclose(file), open_output_file will close on error
        TEST_ASSERT_EQUAL(1, e);
    }

    // should succeed
    file = open_source_file("test/files/open_source_file.vm");
    const char file_path[] = "test/files/open_source_file.vm";
    FILE *output_file = open_output_file(file_path, file);
    TEST_ASSERT_NOT_NULL(output_file);
    fclose(file);
    fclose(output_file);
}

