/**
 *  Copyright 2010-2016 Comcast Cable Communications Management, LLC
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "../src/ccsplite.h"

void test_bool()
{
    int rv;
    bool b;

    /* Make sure the value isn't modified on failure. */
    b = true;
    rv = ccsplite_get_bool( "Device.NonJson", 1, &b );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_TRUE( b );

    /* Make sure the value isn't modified on failure. */
    b = false;
    rv = ccsplite_get_bool( "Device.NonJson", 1, &b );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_FALSE( b );

    /* Make sure we can handle a DOM mismatch. */
    rv = ccsplite_get_bool( "Device.WrongDom", 1, &b );
    CU_ASSERT_NOT_EQUAL( 0, rv );

    /* Handle a wrong type. */
    rv = ccsplite_get_bool( "Device.IntInt512", 1, &b );
    CU_ASSERT_NOT_EQUAL( 0, rv );

    rv = ccsplite_get_bool( "Device.StringString", 1, &b );
    CU_ASSERT_NOT_EQUAL( 0, rv );

    /* Normal cases. */
    rv = ccsplite_get_bool( "Device.BoolStringTrue", 1, &b );
    CU_ASSERT_EQUAL( 0, rv );
    CU_ASSERT_TRUE( b );

    rv = ccsplite_get_bool( "Device.BoolStringFalse", 1, &b );
    CU_ASSERT_EQUAL( 0, rv );
    CU_ASSERT_FALSE( b );

    rv = ccsplite_get_bool( "Device.BoolFalse", 1, &b );
    CU_ASSERT_EQUAL( 0, rv );
    CU_ASSERT_FALSE( b );

    rv = ccsplite_get_bool( "Device.BoolTrue", 1, &b );
    CU_ASSERT_EQUAL( 0, rv );
    CU_ASSERT_TRUE( b );

    /* Timeout case. */
    rv = ccsplite_get_bool( "Device.Delay2", 1, &b );
    CU_ASSERT_NOT_EQUAL( 0, rv );
}

void test_int32_t()
{
    int rv;
    int32_t i32;

    /* Make sure the value isn't modified on failure. */
    i32 = 999;
    rv = ccsplite_get_int32( "Device.NonJson", 1, &i32 );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_EQUAL( i32, 999 );

    /* Make sure we can handle a DOM mismatch. */
    i32 = 999;
    rv = ccsplite_get_int32( "Device.WrongDom", 1, &i32 );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_EQUAL( i32, 999 );

    /* Handle a wrong type. */
    rv = ccsplite_get_int32( "Device.BoolFalse", 1, &i32 );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_EQUAL( i32, 999 );

    rv = ccsplite_get_int32( "Device.StringString", 1, &i32 );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_EQUAL( i32, 999 );

    /* Normal cases. */
    rv = ccsplite_get_int32( "Device.IntString512", 1, &i32 );
    CU_ASSERT_EQUAL( 0, rv )
    CU_ASSERT_EQUAL( i32, 512 );

    rv = ccsplite_get_int32( "Device.IntStringMinus512", 1, &i32 );
    CU_ASSERT_EQUAL( 0, rv )
    CU_ASSERT_EQUAL( i32, -512 );

    rv = ccsplite_get_int32( "Device.IntInt512", 1, &i32 );
    CU_ASSERT_EQUAL( 0, rv )
    CU_ASSERT_EQUAL( i32, 512 );

    rv = ccsplite_get_int32( "Device.IntIntMinus512", 1, &i32 );
    CU_ASSERT_EQUAL( 0, rv )
    CU_ASSERT_EQUAL( i32, -512 );

    /* Timeout case. */
    rv = ccsplite_get_int32( "Device.Delay2", 1, &i32 );
    CU_ASSERT_NOT_EQUAL( 0, rv )
}

void test_uint32_t()
{
    int rv;
    uint32_t u32;

    /* Make sure the value isn't modified on failure. */
    u32 = 999;
    rv = ccsplite_get_uint32( "Device.NonJson", 1, &u32 );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_EQUAL( u32, 999 );

    /* Make sure we can handle a DOM mismatch. */
    u32 = 999;
    rv = ccsplite_get_uint32( "Device.WrongDom", 1, &u32 );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_EQUAL( u32, 999 );

    /* Handle a wrong type. */
    u32 = 999;
    rv = ccsplite_get_uint32( "Device.BoolFalse", 1, &u32 );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_EQUAL( u32, 999 );

    rv = ccsplite_get_uint32( "Device.StringString", 1, &u32 );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_EQUAL( u32, 999 );

    /* Normal cases. */
    rv = ccsplite_get_uint32( "Device.IntString512", 1, &u32 );
    CU_ASSERT_EQUAL( 0, rv )
    CU_ASSERT_EQUAL( u32, 512 );

    rv = ccsplite_get_uint32( "Device.IntStringMinus512", 1, &u32 );
    CU_ASSERT_EQUAL( 0, rv )
    CU_ASSERT_EQUAL( u32, 0xfffffe00 );

    rv = ccsplite_get_uint32( "Device.IntInt512", 1, &u32 );
    CU_ASSERT_EQUAL( 0, rv )
    CU_ASSERT_EQUAL( u32, 512 );

    rv = ccsplite_get_uint32( "Device.IntIntMinus512", 1, &u32 );
    CU_ASSERT_EQUAL( 0, rv )
    CU_ASSERT_EQUAL( u32, 0xfffffe00 );

    /* Timeout case. */
    rv = ccsplite_get_uint32( "Device.Delay2", 1, &u32 );
    CU_ASSERT_NOT_EQUAL( 0, rv )
}

void test_string()
{
    int rv;
    char *val;
    const char const *dont_modify = "don't modify";

    /* Make sure the value isn't modified on failure. */
    val = (char*) dont_modify;
    rv = ccsplite_get_string( "Device.NonJson", 1, &val );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_EQUAL( dont_modify, val );

    /* Make sure we can handle a DOM mismatch. */
    rv = ccsplite_get_string( "Device.WrongDom", 1, &val );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_EQUAL( dont_modify, val );

    /* Handle a wrong type. */
    rv = ccsplite_get_string( "Device.BoolFalse", 1, &val );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_EQUAL( dont_modify, val );

    rv = ccsplite_get_string( "Device.IntInt512", 1, &val );
    CU_ASSERT_NOT_EQUAL( 0, rv );
    CU_ASSERT_EQUAL( dont_modify, val );

    /* Normal cases. */
    val = (char*) dont_modify;
    rv = ccsplite_get_string( "Device.StringString", 1, &val );
    CU_ASSERT_EQUAL( 0, rv );
    CU_ASSERT_NOT_EQUAL( dont_modify, val );
    CU_ASSERT_STRING_EQUAL( "I am a string.", val );
    free( val );

    val = (char*) dont_modify;
    rv = ccsplite_get_string( "Device.IntString512", 1, &val );
    CU_ASSERT_EQUAL( 0, rv );
    CU_ASSERT_NOT_EQUAL( dont_modify, val );
    CU_ASSERT_STRING_EQUAL( "512", val );
    free( val );

    val = (char*) dont_modify;
    rv = ccsplite_get_string( "Device.LongString", 3, &val );
    CU_ASSERT_EQUAL( 0, rv );
    CU_ASSERT_NOT_EQUAL( dont_modify, val );
    CU_ASSERT_STRING_EQUAL( "broken text", val );
    free( val );

    /* Timeout case. */
    rv = ccsplite_get_string( "Device.Delay2", 1, &val );
    CU_ASSERT_NOT_EQUAL( 0, rv );
}

void add_suites( CU_pSuite *suite )
{
    *suite = CU_add_suite( "ccsplite encoding tests", NULL, NULL );

    CU_add_test( *suite, "Test booleans", test_bool );
    CU_add_test( *suite, "Test int32_t", test_int32_t );
    CU_add_test( *suite, "Test uint32_t", test_uint32_t );
    CU_add_test( *suite, "Test string", test_string );

    ccsplite_destroy();
}

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
int main( void )
{
    unsigned rv = 1;
    CU_pSuite suite = NULL;

    if( CUE_SUCCESS == CU_initialize_registry() ) {
        add_suites( &suite );

        if( NULL != suite ) {
            CU_basic_set_mode( CU_BRM_VERBOSE );
            CU_basic_run_tests();
            printf( "\n" );
            CU_basic_show_failures( CU_get_failure_list() );
            printf( "\n\n" );
            rv = CU_get_number_of_tests_failed();
        }

        CU_cleanup_registry();
    }

    if( 0 != rv ) {
        return 1;
    }
    return 0;
}
