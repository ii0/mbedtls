/*
 *  Threading abstraction layer
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_THREADING_C)

#include "mbedtls/threading.h"

#if defined(MBEDTLS_THREADING_PTHREAD)
static void threading_mutex_init_pthread( mbedtls_threading_mutex_t *mutex )
{
    if( mutex == NULL )
        return;

    mutex->is_valid = pthread_mutex_init( &mutex->mutex, NULL ) == 0;
}

static void threading_mutex_free_pthread( mbedtls_threading_mutex_t *mutex )
{
    if( mutex == NULL )
        return;

    (void) pthread_mutex_destroy( &mutex->mutex );
}

static int threading_mutex_lock_pthread( mbedtls_threading_mutex_t *mutex )
{
    if( mutex == NULL || ! mutex->is_valid )
        return( MBEDTLS_ERR_THREADING_BAD_INPUT_DATA );

    if( pthread_mutex_lock( &mutex->mutex ) != 0 )
        return( MBEDTLS_ERR_THREADING_MUTEX_ERROR );

    return( 0 );
}

static int threading_mutex_unlock_pthread( mbedtls_threading_mutex_t *mutex )
{
    if( mutex == NULL || ! mutex->is_valid )
        return( MBEDTLS_ERR_THREADING_BAD_INPUT_DATA );

    if( pthread_mutex_unlock( &mutex->mutex ) != 0 )
        return( MBEDTLS_ERR_THREADING_MUTEX_ERROR );

    return( 0 );
}

void (*mbedtls_mutex_init)( mbedtls_threading_mutex_t * ) = threading_mutex_init_pthread;
void (*mbedtls_mutex_free)( mbedtls_threading_mutex_t * ) = threading_mutex_free_pthread;
int (*mbedtls_mutex_lock)( mbedtls_threading_mutex_t * ) = threading_mutex_lock_pthread;
int (*mbedtls_mutex_unlock)( mbedtls_threading_mutex_t * ) = threading_mutex_unlock_pthread;
#endif /* MBEDTLS_THREADING_PTHREAD */

#if defined(MBEDTLS_THREADING_ALT)
static int threading_mutex_fail( mbedtls_threading_mutex_t *mutex )
{
    ((void) mutex );
    return( MBEDTLS_ERR_THREADING_BAD_INPUT_DATA );
}
static void threading_mutex_dummy( mbedtls_threading_mutex_t *mutex )
{
    ((void) mutex );
    return;
}

void (*mbedtls_mutex_init)( mbedtls_threading_mutex_t * ) = threading_mutex_dummy;
void (*mbedtls_mutex_free)( mbedtls_threading_mutex_t * ) = threading_mutex_dummy;
int (*mbedtls_mutex_lock)( mbedtls_threading_mutex_t * ) = threading_mutex_fail;
int (*mbedtls_mutex_unlock)( mbedtls_threading_mutex_t * ) = threading_mutex_fail;

int mbedtls_threading_set_alt( int (*mutex_init)( mbedtls_threading_mutex_t * ),
                       int (*mutex_free)( mbedtls_threading_mutex_t * ),
                       int (*mutex_lock)( mbedtls_threading_mutex_t * ),
                       int (*mutex_unlock)( mbedtls_threading_mutex_t * ) )
{
    mbedtls_mutex_init = mutex_init;
    mbedtls_mutex_free = mutex_free;
    mbedtls_mutex_lock = mutex_lock;
    mbedtls_mutex_unlock = mutex_unlock;

    return( 0 );
}
#endif /* MBEDTLS_THREADING_ALT */

#endif /* MBEDTLS_THREADING_C */
