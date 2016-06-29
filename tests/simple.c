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

#include "../src/ccsplite.h"

void test_bool()
{
    int rv;
    bool b;

    rv = ccsplite_get_bool( "Device.Invalid.Param", 10000, &b );
    CU_ASSERT_NOT_EQUAL( 0, rv );

    rv = ccsplite_get_bool( "Device.BoolStringTrue", 10000, &b );
    CU_ASSERT_EQUAL( 0, rv );
    CU_ASSERT_TRUE( b );

    rv = ccsplite_get_bool( "Device.BoolStringFalse", 10000, &b );
    CU_ASSERT_EQUAL( 0, rv );
    CU_ASSERT_FALSE( b );

/*
    rv = ccsplite_get_bool( "Device.BoolFalse", 10000, &b );
    CU_ASSERT_EQUAL( 0, rv );
    CU_ASSERT_FALSE( b );
*/

/*
    rv = ccsplite_get_bool( "Device.BoolTrue", 10000, &b );
    CU_ASSERT_EQUAL( 0, rv );
    CU_ASSERT_TRUE( b );
*/
}

void test_int32_t()
{
/*
    int rv;
    int32_t i32;
    rv = ccsplite_get_int32( "Device.IntString512", 10000, &i32 );
    CU_ASSERT_EQUAL( 0, rv )
    CU_ASSERT_EQUAL( i32, 512 );
*/
}

void add_suites( CU_pSuite *suite )
{
    *suite = CU_add_suite( "ccsplite encoding tests", NULL, NULL );
    CU_add_test( *suite, "Test booleans", test_bool );
    CU_add_test( *suite, "Test int32_t", test_int32_t );
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
