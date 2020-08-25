#ifndef POLARSSL_CONFIG_H
#define POLARSSL_CONFIG_H

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

//#define POLARSSL_HAVE_LONGLONG

#define POLARSSL_HAVE_ASM

#define POLARSSL_HAVE_SSE2

#define POLARSSL_GENPRIME

//#define POLARSSL_FS_IO


//#define POLARSSL_NO_DEFAULT_ENTROPY_SOURCES
//#define POLARSSL_NO_PLATFORM_ENTROPY


//#define POLARSSL_PKCS1_V21

//#define POLARSSL_RSA_NO_CRT


//#define POLARSSL_SELF_TEST
#define POLARSSL_BIGNUM_C
#define POLARSSL_RSA_C
#define POLARSSL_SHA1_C

//#define POLARSSL_SELF_TEST
//#define POLARSSL_RSA_NO_CRT

#define POLARSSL_AES_C
//#define POLARSSL_AES_ROM_TABLES

#endif /* config.h */
