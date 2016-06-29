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
#include <inttypes.h>
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
struct payload {
    char *buf;
    size_t len;
};

/*----------------------------------------------------------------------------*/
/*                            File Scoped Variables                           */
/*----------------------------------------------------------------------------*/
static int __ccsplite_init = 0;

/*----------------------------------------------------------------------------*/
/*                             Function Prototypes                            */
/*----------------------------------------------------------------------------*/
static int __get_json( const char *obj_name, int32_t timeout, cJSON **tree );
static size_t __write_fn( void *p, size_t size, size_t nmemb, void *stream );
static cJSON* __find_value_field( cJSON *tree );

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/

/* See ccsplite.h for details. */
void ccsplite_init( void )
{
    if( 0 == __ccsplite_init ) {
        curl_global_init( CURL_GLOBAL_NOTHING );
        __ccsplite_init = 1;
    }
}


/* See ccsplite.h for details. */
void ccsplite_destroy( void )
{
    if( 1 == __ccsplite_init ) {
        curl_global_cleanup();
        __ccsplite_init = 2;
    }
}


/* See ccsplite.h for details. */
int ccsplite_get_bool( const char *obj_name, int32_t timeout, bool *value )
{
    int rv;
    cJSON *json;

    json = NULL;
    rv = __get_json( obj_name, timeout, &json );
    if( 0 == rv ) {
        cJSON *json_val;

        json_val = __find_value_field( json );

        rv = -1;
        if( NULL != json_val ) {
            switch( json_val->type ) {
                case cJSON_False:
                    *value = false;
                    rv = 0;
                    break;
                case cJSON_True:
                    *value = true;
                    rv = 0;
                    break;
                case cJSON_String:
                    if( 0 == strcasecmp("true", json_val->valuestring) ) {
                        *value = true;
                        rv = 0;
                    } else if( 0 == strcasecmp("false", json_val->valuestring) ) {
                        *value = false;
                        rv = 0;
                    }
                    break;
                default:
                    break;
            }
        }

        cJSON_Delete( json );
    }

    return rv;
}


/* See ccsplite.h for details. */
int ccsplite_get_int32( const char *obj_name, int32_t timeout, int32_t *value )
{
    int rv;
    cJSON *json;

    json = NULL;
    rv = __get_json( obj_name, timeout, &json );
    if( 0 == rv ) {
        cJSON *json_val;

        json_val = __find_value_field( json );

        rv = -1;
        if( NULL != json_val ) {
            switch( json_val->type ) {
                case cJSON_Number:
                    *value = (int32_t) json_val->valueint;
                    rv = 0;
                    break;
                case cJSON_String:
                {
                    int32_t tmp;
                    int _rv;

                    _rv = sscanf( json_val->valuestring, "%" SCNd32, &tmp );
                    if( 1 == _rv ) {
                        *value = tmp;
                        rv = 0;
                    }
                    break;
                }
                default:
                    break;
            }
        }

        cJSON_Delete( json );
    }

    return rv;
}


/* See ccsplite.h for details. */
int ccsplite_get_uint32( const char *obj_name, int32_t timeout, uint32_t *value )
{
    int rv;
    cJSON *json;

    json = NULL;
    rv = __get_json( obj_name, timeout, &json );
    if( 0 == rv ) {
        cJSON *json_val;

        json_val = __find_value_field( json );

        rv = -1;
        if( NULL != json_val ) {
            switch( json_val->type ) {
                case cJSON_Number:
                    *value = (uint32_t) json_val->valueint;
                    rv = 0;
                    break;
                case cJSON_String:
                {
                    uint32_t tmp;
                    int _rv;

                    _rv = sscanf( json_val->valuestring, "%" SCNu32, &tmp );
                    if( 1 == _rv ) {
                        *value = tmp;
                        rv = 0;
                    }
                    break;
                }
                default:
                    break;
            }
        }

        cJSON_Delete( json );
    }

    return rv;
}


/* See ccsplite.h for details. */
int ccsplite_get_string( const char *obj_name, int32_t timeout, char **value )
{
    int rv;
    cJSON *json;

    json = NULL;
    rv = __get_json( obj_name, timeout, &json );
    if( 0 == rv ) {
        cJSON *json_val;

        json_val = __find_value_field( json );

        rv = -1;
        if( NULL != json_val ) {
            if( cJSON_String == json_val->type ) {
                *value = strdup( json_val->valuestring );
                rv = 0;
            }
        }

        cJSON_Delete( json );
    }

    return rv;
}


/*----------------------------------------------------------------------------*/
/*                             Internal functions                             */
/*----------------------------------------------------------------------------*/

/**
 */
static int __get_json( const char *obj_name, int32_t timeout, cJSON **tree )
{
    const char const *url_fmt = "http://localhost:62000/config/%s";
    size_t len;
    char *url;
    int rv;

    rv = -1;

    /* Do the heavy lifting in here. */
    len = snprintf( NULL, 0, url_fmt, obj_name );
    url = (char*) malloc( sizeof(char) * (len + 1) );
    if( NULL != url ) {
        CURL *curl;

        sprintf( url, url_fmt, obj_name );

        /* If the library has not been initialized, do it now for cURL. */
        ccsplite_init();

        if( 1 == __ccsplite_init ) {
            curl = curl_easy_init();
            if( curl ) {
                CURLcode res;
                struct payload payload;

                payload.buf = NULL;
                payload.len = 0;

                /* Do the right thing here. */
                curl_easy_setopt( curl, CURLOPT_URL, url );
                curl_easy_setopt( curl, CURLOPT_TIMEOUT, (long) timeout );
                curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, __write_fn );
                curl_easy_setopt( curl, CURLOPT_WRITEDATA, &payload );
                res = curl_easy_perform( curl );

                if( CURLE_OK == res ) {
                    cJSON *tmp;

                    cJSON_Hooks hooks = {
                        .malloc_fn = malloc,
                        .free_fn = free,
                    };

                    cJSON_InitHooks( &hooks );

                    tmp = cJSON_Parse( payload.buf );
                    if( NULL != tmp ) {
                        *tree = tmp;
                        rv = 0;
                    }
                }

                if( NULL != payload.buf ) {
                    free( payload.buf );
                }

                curl_easy_cleanup( curl );
            }
        }

        free( url );
    }

    return rv;
}


/**
 */
static size_t __write_fn( void *p, size_t size, size_t nmemb, void *stream )
{
    struct payload *payload;
    size_t new_len;
    char *buf;
    size_t rv;

    rv = (size * nmemb) + 1; /* Error case */

    payload = (struct payload*) stream;

    new_len = payload->len + (nmemb * size);
    buf = (char*) malloc( sizeof(char) * (new_len + 1) );
    if( NULL != buf ) {
        if( NULL != payload->buf ) {
            memcpy( buf, payload->buf, payload->len );
            free( payload->buf );
            payload->buf = buf;
            buf += (payload->len * sizeof(char));
        } else {
            payload->buf = buf;
        }

        rv = size * nmemb;
        memcpy( buf, p, rv );
        payload->len = new_len;
    }

    return rv;
}


/**
 */
static cJSON* __find_value_field( cJSON *tree )
{
    cJSON *p;

    p = cJSON_GetObjectItem( tree, "parameters" );
    if( NULL != p ) {
        p = cJSON_GetArrayItem( p, 0 );
        if( NULL != p ) {
            p = cJSON_GetObjectItem( p, "value" );
        }
    }

    return p;
}
