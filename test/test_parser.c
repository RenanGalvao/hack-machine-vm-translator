#include "parser.h"
#include "mock_code.h"
#include "defs.h"
#include "utils.h"
#include "unity.h"
#include <stdio.h>

FILE *output_file = NULL;

void setUp(void) {
    const char output_file_path[] = "test/files/scan_output.asm";
    output_file = fopen(output_file_path, "w");
}

void tearDown(void) {
    fclose(output_file);
}

void test_scan(void) {
    // pop - fail
    const char pop_scan_fail_file_path[] = "test/files/pop_scan_fail.vm";
    FILE *pop_scan_fail_file = fopen(pop_scan_fail_file_path, "r");

    // can't pop to constant segment
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(pop_scan_fail_file, output_file));

    // invalid instruction - pop
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(pop_scan_fail_file, output_file));

    // invalid instruction - pop static 10 1
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(pop_scan_fail_file, output_file));

    // invalid instruction - pop static -10
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(pop_scan_fail_file, output_file));

    // invalid instruction using pointer segment - out of range
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(pop_scan_fail_file, output_file));

    // invalid instruction using temp segment - out of range
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(pop_scan_fail_file, output_file));
    fclose(pop_scan_fail_file);

    // pop - success
    const char pop_scan_success_file_path[] = "test/files/pop_scan_success.vm";
    FILE *pop_scan_success_file = fopen(pop_scan_success_file_path, "r");

    // pop segment 0 - all segments
    // see pop_scan_success.vm
    vm_setup_Expect(output_file);
    vm_end_Expect(output_file);
    for(int i = 0; i < 15; i++) {
        write_pop_instruction_ExpectAnyArgs();
    }
    TEST_ASSERT_EQUAL(0, scan(pop_scan_success_file, output_file));
    fclose(pop_scan_success_file);


    // push - fail
    const char push_scan_fail_file_path[] = "test/files/push_scan_fail.vm";
    FILE *push_scan_fail_file = fopen(push_scan_fail_file_path, "r");

    // invalid instruction - push
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(push_scan_fail_file, output_file));

    // invalid instruction - push static 10 1
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(push_scan_fail_file, output_file));

    // invalid instruction - push static -10
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(push_scan_fail_file, output_file));

    // invalid instruction using constant segment - max value exceeded
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(push_scan_fail_file, output_file));

    // invalid instruction using pointer segment - out of range
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(push_scan_fail_file, output_file));

    // invalid instruction using temp segment - out of range
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(push_scan_fail_file, output_file));
    fclose(push_scan_fail_file);

    // push - success
    const char push_scan_success_file_path[] = "test/files/push_scan_success.vm";
    FILE *push_scan_success_file = fopen(push_scan_success_file_path, "r");

    // push segment 0 - all segments
    // see push_scan_success.vm
    vm_setup_Expect(output_file);
    vm_end_Expect(output_file);
    for(int i = 0; i < 16; i++) {
        write_push_instruction_ExpectAnyArgs();
    }
    TEST_ASSERT_EQUAL(0, scan(push_scan_success_file, output_file));
    fclose(push_scan_success_file);


    // arithmetic - fail
    const char arithmetic_scan_fail_file_path[] = "test/files/arithmetic_scan_fail.vm";
    FILE *arithmetic_scan_fail_file = fopen(arithmetic_scan_fail_file_path, "r");

    // invalid instruction - ad
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(arithmetic_scan_fail_file, output_file));

    // invalid instruction - sb
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(arithmetic_scan_fail_file, output_file));

    // invalid instruction - e
    vm_setup_Expect(output_file);
    TEST_ASSERT_EQUAL(-1, scan(arithmetic_scan_fail_file, output_file));
    fclose(arithmetic_scan_fail_file);

    // arithmetic - success
    const char arithmetic_scan_success_file_path[] = "test/files/arithmetic_scan_success.vm";
    FILE *arithmetic_scan_success_file = fopen(arithmetic_scan_success_file_path, "r");

    // arithmetic segment 0 - all segments
    // see arithmetic_scan_success.vm
    vm_setup_Expect(output_file);
    vm_end_Expect(output_file);
    for(int i = 0; i < 9; i++) {
        write_arithmetic_instruction_ExpectAnyArgs();
    }
    TEST_ASSERT_EQUAL(0, scan(arithmetic_scan_success_file, output_file));
    fclose(arithmetic_scan_success_file);
}

