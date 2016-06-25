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
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON/cJSON.h>
#include <curl/curl.h>

#include "ccsplite.h"

/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                            File Scoped Variables                           */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                             Function Prototypes                            */
/*----------------------------------------------------------------------------*/
static int __ccsplite_get_json( const char *obj_name, int32_t timeout,
                                cJSON **obj );

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
int ccsplite_get_bool( const char *obj_name, int32_t timeout, bool *value )
{
    int rv;
    cJSON *json;

    json = NULL;
    rv = __ccsplite_get_json( obj_name, timeout, &json );
    if( 0 == rv ) {
        if( cJSON_Number == json->type ) {
            *value = false;
            if( 0 != json->valueint ) {
                *value = true;
            }
        }

        cJSON_Delete( json );
    }

    return rv;
}

int ccsplite_get_int32( const char *obj_name, int32_t timeout, int32_t *value )
{
    int rv;
    cJSON *json;

    json = NULL;
    rv = __ccsplite_get_json( obj_name, timeout, &json );
    if( 0 == rv ) {
        if( cJSON_Number == json->type ) {
            *value = (int32_t) json->valueint;
        }

        cJSON_Delete( json );
    }

    return rv;
}

int ccsplite_get_uint32( const char *obj_name, int32_t timeout, uint32_t *value )
{
    (void) obj_name;
    (void) timeout;
    (void) value;
    return -1;
}

int ccsplite_get_int64( const char *obj_name, int32_t timeout, int64_t *value )
{
    (void) obj_name;
    (void) timeout;
    (void) value;
    return -1;
}

int ccsplite_get_uint64( const char *obj_name, int32_t timeout, uint64_t *value )
{
    (void) obj_name;
    (void) timeout;
    (void) value;
    return -1;
}

int ccsplite_get_string( const char *obj_name, int32_t timeout, char **value )
{
    int rv;
    cJSON *json;

    json = NULL;
    rv = __ccsplite_get_json( obj_name, timeout, &json );
    if( 0 == rv ) {
        if( cJSON_String == json->type ) {
            *value = strdup( json->valuestring );
        }

        cJSON_Delete( json );
    }

    return rv;
}



/*----------------------------------------------------------------------------*/
/*                             Internal functions                             */
/*----------------------------------------------------------------------------*/
static int __ccsplite_get_json( const char *obj_name, int32_t timeout,
                                cJSON **obj )
{
    const char const *url_fmt = "http://localhost:62000/config?name=%s";
    size_t len;
    char *url;

    /* Do the heavy lifting in here. */
    len = snprintf( NULL, 0, url_fmt, obj_name );
    url = (char*) malloc( sizeof(char) * (len + 1) );
    if( NULL != url ) {
        CURL *curl;

        sprintf( url, url_fmt, obj_name );

        curl = curl_easy_init();
        if( curl ) {
            CURLcode res;
            /* Do the right thing here. */
            curl_easy_setopt( curl, CURLOPT_URL, "http://localhost:62000" );
            curl_easy_setopt( curl, CURLOPT_TIMEOUT, (long) timeout );
            res = curl_easy_perform( curl );

            if( CURLE_OK == res ) {
                printf( "done.\n" );

                /* Do something with the object */
                (void) obj;
            }

            curl_easy_cleanup( curl );
            return 0;
        }
    }

    return -1;
}
