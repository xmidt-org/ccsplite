/**
 * Copyright 2016 Comcast Cable Communications Management, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef __CCSPLITE_H__
#define __CCSPLITE_H__

#include <stdbool.h>
#include <stdint.h>

/**
 *  Used to initialize the library before use.  Due to cURL, this must be
 *  called 1 time before any multi-threaded use of the library occurs.
 *
 *  @note Due to cURL, this is not thread safe.
 *  @note You do not need to call this function if you are single threaded.
 */
void ccsplite_init( void );


/**
 *  Used to clean up any memory before you exit.  This is mostly of interest
 *  when using Valgrind for leak detection.
 *
 *  @note Due to cURL, once this is called, the library cannot be initialized
 *        again and all calls to ccsplite will fail.
 */
void ccsplite_destroy( void );


/**
 *  Provides the caller with the value of the specified TR-181 object.
 *
 *  @note An error will result if the wrong type function is called for
 *        the parameter type.
 *
 *  @param obj_name [in]  the TR-181 object to retrieve
 *  @param timeout  [in]  the number of seconds to timeout before failing
 *  @param value    [out] the value of the object
 *
 *  @return 0 if successful, error otherwise
 */
int ccsplite_get_bool(const char *obj_name, int32_t timeout, bool *value);
int ccsplite_get_int32(const char *obj_name, int32_t timeout, int32_t *value);
int ccsplite_get_uint32(const char *obj_name, int32_t timeout, uint32_t *value);
int ccsplite_get_string(const char *obj_name, int32_t timeout, char **value);

#endif
