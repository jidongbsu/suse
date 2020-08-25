/**
 * \file rsa.h
 *
 * \brief The RSA public-key cryptosystem
 *
 *  Copyright (C) 2006-2010, Brainspark B.V.
 *
 *  This file is part of PolarSSL (http://www.polarssl.org)
 *  Lead Maintainer: Paul Bakker <polarssl_maintainer at polarssl.org>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef POLARSSL_RSA_H
#define POLARSSL_RSA_H

#include "bignum.h"

/*
 * RSA Error codes
 */
#define POLARSSL_ERR_RSA_BAD_INPUT_DATA                    -0x4080  /**< Bad input parameters to function. */
#define POLARSSL_ERR_RSA_INVALID_PADDING                   -0x4100  /**< Input data contains invalid padding and is rejected. */
#define POLARSSL_ERR_RSA_KEY_GEN_FAILED                    -0x4180  /**< Something failed during generation of a key. */
#define POLARSSL_ERR_RSA_KEY_CHECK_FAILED                  -0x4200  /**< Key failed to pass the libraries validity check. */
#define POLARSSL_ERR_RSA_PUBLIC_FAILED                     -0x4280  /**< The public key operation failed. */
#define POLARSSL_ERR_RSA_PRIVATE_FAILED                    -0x4300  /**< The private key operation failed. */
#define POLARSSL_ERR_RSA_VERIFY_FAILED                     -0x4380  /**< The PKCS#1 verification failed. */
#define POLARSSL_ERR_RSA_OUTPUT_TOO_LARGE                  -0x4400  /**< The output buffer for decryption is not large enough. */
#define POLARSSL_ERR_RSA_RNG_FAILED                        -0x4480  /**< The random generator failed to generate non-zeros. */

/*
 * PKCS#1 constants
 */
#define SIG_RSA_RAW     0
#define SIG_RSA_MD2     2
#define SIG_RSA_MD4     3
#define SIG_RSA_MD5     4
#define SIG_RSA_SHA1    5
#define SIG_RSA_SHA224 14
#define SIG_RSA_SHA256 11
#define SIG_RSA_SHA384 12
#define SIG_RSA_SHA512 13

#define RSA_PUBLIC      0
#define RSA_PRIVATE     1

#define RSA_PKCS_V15    0
#define RSA_PKCS_V21    1

#define RSA_SIGN        1
#define RSA_CRYPT       2

#define ASN1_STR_CONSTRUCTED_SEQUENCE   "\x30"
#define ASN1_STR_NULL                   "\x05"
#define ASN1_STR_OID                    "\x06"
#define ASN1_STR_OCTET_STRING           "\x04"

#define OID_DIGEST_ALG_MDX              "\x2A\x86\x48\x86\xF7\x0D\x02\x00"
#define OID_HASH_ALG_SHA1               "\x2b\x0e\x03\x02\x1a"
#define OID_HASH_ALG_SHA2X              "\x60\x86\x48\x01\x65\x03\x04\x02\x00"

#define OID_ISO_MEMBER_BODIES           "\x2a"
#define OID_ISO_IDENTIFIED_ORG          "\x2b"

/*
 * ISO Member bodies OID parts
 */
#define OID_COUNTRY_US                  "\x86\x48"
#define OID_RSA_DATA_SECURITY           "\x86\xf7\x0d"

/*
 * ISO Identified organization OID parts
 */
#define OID_OIW_SECSIG_SHA1             "\x0e\x03\x02\x1a"

/*
 * DigestInfo ::= SEQUENCE {
 *   digestAlgorithm DigestAlgorithmIdentifier,
 *   digest Digest }
 *
 * DigestAlgorithmIdentifier ::= AlgorithmIdentifier
 *
 * Digest ::= OCTET STRING
 */
#define ASN1_HASH_MDX                           \
(                                               \
    ASN1_STR_CONSTRUCTED_SEQUENCE "\x20"        \
      ASN1_STR_CONSTRUCTED_SEQUENCE "\x0C"      \
        ASN1_STR_OID "\x08"                     \
      OID_DIGEST_ALG_MDX                        \
    ASN1_STR_NULL "\x00"                        \
      ASN1_STR_OCTET_STRING "\x10"              \
)

#define ASN1_HASH_SHA1                          \
    ASN1_STR_CONSTRUCTED_SEQUENCE "\x21"        \
      ASN1_STR_CONSTRUCTED_SEQUENCE "\x09"      \
        ASN1_STR_OID "\x05"                     \
      OID_HASH_ALG_SHA1                         \
        ASN1_STR_NULL "\x00"                    \
      ASN1_STR_OCTET_STRING "\x14"

#define ASN1_HASH_SHA1_ALT                      \
    ASN1_STR_CONSTRUCTED_SEQUENCE "\x1F"        \
      ASN1_STR_CONSTRUCTED_SEQUENCE "\x07"      \
        ASN1_STR_OID "\x05"                     \
      OID_HASH_ALG_SHA1                         \
      ASN1_STR_OCTET_STRING "\x14"

#define ASN1_HASH_SHA2X                         \
    ASN1_STR_CONSTRUCTED_SEQUENCE "\x11"        \
      ASN1_STR_CONSTRUCTED_SEQUENCE "\x0d"      \
        ASN1_STR_OID "\x09"                     \
      OID_HASH_ALG_SHA2X                        \
        ASN1_STR_NULL "\x00"                    \
      ASN1_STR_OCTET_STRING "\x00"

/**
 * \brief          RSA context structure
 */
typedef struct
{
    int ver;                    /*!<  always 0          */
    size_t len;                 /*!<  size(N) in chars  */

    mpi N;                      /*!<  public modulus    */
    mpi E;                      /*!<  public exponent   */

    mpi D;                      /*!<  private exponent  */
    mpi P;                      /*!<  1st prime factor  */
    mpi Q;                      /*!<  2nd prime factor  */
    mpi DP;                     /*!<  D % (P - 1)       */
    mpi DQ;                     /*!<  D % (Q - 1)       */
    mpi QP;                     /*!<  1 / (Q % P)       */

    mpi RN;                     /*!<  cached R^2 mod N  */
    mpi RP;                     /*!<  cached R^2 mod P  */
    mpi RQ;                     /*!<  cached R^2 mod Q  */

    int padding;                /*!<  RSA_PKCS_V15 for 1.5 padding and
                                      RSA_PKCS_v21 for OAEP/PSS         */
    int hash_id;                /*!<  Hash identifier of md_type_t as
                                      specified in the md.h header file
                                      for the EME-OAEP and EMSA-PSS
                                      encoding                          */
}
rsa_context;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          Initialize an RSA context
 *
 *                 Note: Set padding to RSA_PKCS_V21 for the RSAES-OAEP
 *                 encryption scheme and the RSASSA-PSS signature scheme.
 *
 * \param ctx      RSA context to be initialized
 * \param padding  RSA_PKCS_V15 or RSA_PKCS_V21
 * \param hash_id  RSA_PKCS_V21 hash identifier
 *
 * \note           The hash_id parameter is actually ignored
 *                 when using RSA_PKCS_V15 padding.
 */
void rsa_init( rsa_context *ctx,
               int padding,
               int hash_id);

/**
 * \brief          Generate an RSA keypair
 *
 * \param ctx      RSA context that will hold the key
 * \param f_rng    RNG function
 * \param p_rng    RNG parameter
 * \param nbits    size of the public key in bits
 * \param exponent public exponent (e.g., 65537)
 *
 * \note           rsa_init() must be called beforehand to setup
 *                 the RSA context.
 *
 * \return         0 if successful, or an POLARSSL_ERR_RSA_XXX error code
 */
int rsa_gen_key( rsa_context *ctx,
                 int (*f_rng)(void *, unsigned char *, size_t),
                 void *p_rng,
                 unsigned int nbits, int exponent );

/**
 * \brief          Check a public RSA key
 *
 * \param ctx      RSA context to be checked
 *
 * \return         0 if successful, or an POLARSSL_ERR_RSA_XXX error code
 */
int rsa_check_pubkey( const rsa_context *ctx );

/**
 * \brief          Check a private RSA key
 *
 * \param ctx      RSA context to be checked
 *
 * \return         0 if successful, or an POLARSSL_ERR_RSA_XXX error code
 */
int rsa_check_privkey( const rsa_context *ctx );

/**
 * \brief          Do an RSA public key operation
 *
 * \param ctx      RSA context
 * \param input    input buffer
 * \param output   output buffer
 *
 * \return         0 if successful, or an POLARSSL_ERR_RSA_XXX error code
 *
 * \note           This function does NOT take care of message
 *                 padding. Also, be sure to set input[0] = 0 or assure that
 *                 input is smaller than N.
 *
 * \note           The input and output buffers must be large
 *                 enough (eg. 128 bytes if RSA-1024 is used).
 */
int rsa_public( rsa_context *ctx,
                const unsigned char *input,
                unsigned char *output );

/**
 * \brief          Do an RSA private key operation
 *
 * \param ctx      RSA context
 * \param input    input buffer
 * \param output   output buffer
 *
 * \return         0 if successful, or an POLARSSL_ERR_RSA_XXX error code
 *
 * \note           The input and output buffers must be large
 *                 enough (eg. 128 bytes if RSA-1024 is used).
 */
int rsa_private( rsa_context *ctx,
                 const unsigned char *input,
                 unsigned char *output );

/**
 * \brief          Add the message padding, then do an RSA operation
 *
 * \param ctx      RSA context
 * \param f_rng    RNG function (Needed for padding and PKCS#1 v2.1 encoding)
 * \param p_rng    RNG parameter
 * \param mode     RSA_PUBLIC or RSA_PRIVATE
 * \param ilen     contains the plaintext length
 * \param input    buffer holding the data to be encrypted
 * \param output   buffer that will hold the ciphertext
 *
 * \return         0 if successful, or an POLARSSL_ERR_RSA_XXX error code
 *
 * \note           The output buffer must be as large as the size
 *                 of ctx->N (eg. 128 bytes if RSA-1024 is used).
 */
int rsa_pkcs1_encrypt( rsa_context *ctx,
                       int (*f_rng)(void *, unsigned char *, size_t),
                       void *p_rng,
                       int mode, size_t ilen,
                       const unsigned char *input,
                       unsigned char *output );

/**
 * \brief          Do an RSA operation, then remove the message padding
 *
 * \param ctx      RSA context
 * \param mode     RSA_PUBLIC or RSA_PRIVATE
 * \param olen     will contain the plaintext length
 * \param input    buffer holding the encrypted data
 * \param output   buffer that will hold the plaintext
 * \param output_max_len    maximum length of the output buffer
 *
 * \return         0 if successful, or an POLARSSL_ERR_RSA_XXX error code
 *
 * \note           The output buffer must be as large as the size
 *                 of ctx->N (eg. 128 bytes if RSA-1024 is used) otherwise
 *                 an error is thrown.
 */
int rsa_pkcs1_decrypt( rsa_context *ctx,
                       int mode, size_t *olen,
                       const unsigned char *input,
                       unsigned char *output,
                       size_t output_max_len );

/**
 * \brief          Do a private RSA to sign a message digest
 *
 * \param ctx      RSA context
 * \param f_rng    RNG function (Needed for PKCS#1 v2.1 encoding)
 * \param p_rng    RNG parameter
 * \param mode     RSA_PUBLIC or RSA_PRIVATE
 * \param hash_id  SIG_RSA_RAW, SIG_RSA_MD{2,4,5} or SIG_RSA_SHA{1,224,256,384,512}
 * \param hashlen  message digest length (for SIG_RSA_RAW only)
 * \param hash     buffer holding the message digest
 * \param sig      buffer that will hold the ciphertext
 *
 * \return         0 if the signing operation was successful,
 *                 or an POLARSSL_ERR_RSA_XXX error code
 *
 * \note           The "sig" buffer must be as large as the size
 *                 of ctx->N (eg. 128 bytes if RSA-1024 is used).
 *
 * \note           In case of PKCS#1 v2.1 encoding keep in mind that
 *                 the hash_id in the RSA context is the one used for the
 *                 encoding. hash_id in the function call is the type of hash
 *                 that is encoded. According to RFC 3447 it is advised to
 *                 keep both hashes the same.
 */
int rsa_pkcs1_sign( rsa_context *ctx,
                    int (*f_rng)(void *, unsigned char *, size_t),
                    void *p_rng,
                    int mode,
                    int hash_id,
                    unsigned int hashlen,
                    const unsigned char *hash,
                    unsigned char *sig );

/**
 * \brief          Do a public RSA and check the message digest
 *
 * \param ctx      points to an RSA public key
 * \param mode     RSA_PUBLIC or RSA_PRIVATE
 * \param hash_id  SIG_RSA_RAW, SIG_RSA_MD{2,4,5} or SIG_RSA_SHA{1,224,256,384,512}
 * \param hashlen  message digest length (for SIG_RSA_RAW only)
 * \param hash     buffer holding the message digest
 * \param sig      buffer holding the ciphertext
 *
 * \return         0 if the verify operation was successful,
 *                 or an POLARSSL_ERR_RSA_XXX error code
 *
 * \note           The "sig" buffer must be as large as the size
 *                 of ctx->N (eg. 128 bytes if RSA-1024 is used).
 *
 * \note           In case of PKCS#1 v2.1 encoding keep in mind that
 *                 the hash_id in the RSA context is the one used for the
 *                 verification. hash_id in the function call is the type of hash
 *                 that is verified. According to RFC 3447 it is advised to
 *                 keep both hashes the same.
 */
int rsa_pkcs1_verify( rsa_context *ctx,
                      int mode,
                      int hash_id,
                      unsigned int hashlen,
                      const unsigned char *hash,
                      unsigned char *sig );

/**
 * \brief          Free the components of an RSA key
 *
 * \param ctx      RSA Context to free
 */
void rsa_free( rsa_context *ctx );

/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
int rsa_self_test( int verbose );

#ifdef __cplusplus
}
#endif




#define KEY_LEN 128
/*
#define RSA_N   "9292758453063D803DD603D5E777D788" \
                "8ED1D5BF35786190FA2F23EBC0848AEA" \
                "DDA92CA6C3D80B32C4D109BE0F36D6AE" \
                "7130B9CED7ACDF54CFC7555AC14EEBAB" \
                "93A89813FBF3C4F8066D2D800F7C38A8" \
                "1AE31942917403FF4946B0A83D3D3E05" \
                "EE57C6F5F5606FB5D4BC6CD34EE0801A" \
                "5E94BB77B07507233A0BC7BAC8F90F79"

#define RSA_E   "10001"

#define RSA_D   "24BF6185468786FDD303083D25E64EFC" \
                "66CA472BC44D253102F8B4A9D3BFA750" \
                "91386C0077937FE33FA3252D28855837" \
                "AE1B484A8A9A45F7EE8C0C634F99E8CD" \
                "DF79C5CE07EE72C7F123142198164234" \
                "CABB724CF78B8173B9F880FC86322407" \
                "AF1FEDFDDE2BEB674CA15F3E81A1521E" \
                "071513A1E85B5DFA031F21ECAE91A34D"

#define RSA_P   "C36D0EB7FCD285223CFB5AABA5BDA3D8" \
                "2C01CAD19EA484A87EA4377637E75500" \
                "FCB2005C5C7DD6EC4AC023CDA285D796" \
                "C3D9E75E1EFC42488BB4F1D13AC30A57"

#define RSA_Q   "C000DF51A7C77AE8D7C7370C1FF55B69" \
                "E211C2B9E5DB1ED0BF61D0D9899620F4" \
                "910E4168387E3C30AA1E00C339A79508" \
                "8452DD96A9A5EA5D9DCA68DA636032AF"

#define RSA_DP  "C1ACF567564274FB07A0BBAD5D26E298" \
                "3C94D22288ACD763FD8E5600ED4A702D" \
                "F84198A5F06C2E72236AE490C93F07F8" \
                "3CC559CD27BC2D1CA488811730BB5725"

#define RSA_DQ  "4959CBF6F8FEF750AEE6977C155579C7" \
                "D8AAEA56749EA28623272E4F7D0592AF" \
                "7C1F1313CAC9471B5C523BFE592F517B" \
                "407A1BD76C164B93DA2D32A383E58357"

#define RSA_QP  "9AE7FBC99546432DF71896FC239EADAE" \
                "F38D18D2B2F0E2DD275AA977E2BF4411" \
                "F5A3B2A5D33605AEBBCCBA7FEB9F2D2F" \
                "A74206CEC169D74BF5A8C50D6F48EA08"
*/
#define RSA_N "C185EA1D8C867C90E8D44667091229847C33C13FB230BC73474B88EF47C7DEDFA48667BBD531C08A3CFCBE4CA5BB923CD69C01A13FC522D35B975EFAA652A4A5D85214D22F7EC38483EA75B82FD941CD0D3E154B0B563B361C9D1D6D89988317FF703EE67D63CEABAEF9D50D3620DD008F1F645272C3489B6BE47047E1CBA09F"
#define RSA_E "10001"
#define RSA_D "2A70579B0B4A22DF147018D057A8E7F9E9D2DFBA3C357BDE3BF050A348B68825BB4ACE4CDEBFFE5D526A8AC0FC475A168569C8E1B7BE5F98C2293BE6F3BA83E888992FC769DA593FB202849C4EA7BD4F1E2ACF03E89CF34A62C0B62791230B7FA933DB5A90FE2F6A1D40FC47568DC03AB4F7BF7AB8F22E720A036A6B168F26E1"
#define RSA_P "F692FC113942CAA47CBD575E39F684AE24FAE1476BB08F08620DEB5F9203144CBC9F4E52101C30D43FDA8D2CF6382056EDB74C124C1290C26723BE22F6B955AD"
#define RSA_Q "C8EBC3750D49AB08B98BC33E78A0C3204535808F2316642834C4C25484539637D53E27D38C428CF335A3C71F415091C31DD60664E88B64E8C1AE4E4762BB20FB"
#define RSA_DP "869F075A4BFE086F37DF9444D0B6A408040EA9527A1C426B5C45A73D5D95AACE9EA90774CC70E7C3EA3CE39929B0C90F4CE8277351EF4CF3A6A620182CC6515D"
#define RSA_DQ "AB4B1D033105EA53F0E42FDB95B9F14114E00D2F7B097EF02C34C73C88E83E3C7187A12AA0FCA7F03D7DD8A853E5DDAF4C438886051FBA3319D48216281D437B"
#define RSA_QP "46B5101D6266A03595EEACED914AB8133E9B14E04800CCD20AAF4AF2B3B9EE147151661E2105DDD6DC41922C2C5FE7369FCF7E2C39BBCB364E4A484ADFAB90F6"

#define PT_LEN  24
#define RSA_PT  "\xAA\xBB\xCC\x03\x02\x01\x00\xFF\xFF\xFF\xFF\xFF" \
                "\x11\x22\x33\x0A\x0B\x0C\xCC\xDD\xDD\xDD\xDD\xDD"













#endif /* rsa.h */
