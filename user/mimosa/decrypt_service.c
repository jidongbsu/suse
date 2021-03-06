#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "cacheCryptoMain.h"
#include "config.h"
#include "aes.h"
#include "bignum.h"
#include "rsa.h"


/* this is the AES master key, in this project, it is supposed to be derived from the debug registers. */
unsigned char mkt[16] = { \
0x52,0x47,0x99,0x32, \
0x4f,0x20,0x6d,0xf0, \
0x1f,0x5b,0x30,0x31, \
0x0c,0xe3,0x50,0x1a \
};


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

#define KEY_LEN 128



typedef struct _key_rsa{
    unsigned char N[MAX_MPI_IN_BYTE];
    unsigned char E[MAX_MPI_IN_BYTE];
    unsigned char D[MAX_MPI_IN_BYTE];
    unsigned char P[KEY_LEN];
    unsigned char Q[KEY_LEN];
    unsigned char DP[KEY_LEN];
    unsigned char DQ[KEY_LEN];
    unsigned char QP[KEY_LEN];
}key_rsa;




#define PT_LEN  24
#define RSA_PT "\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x41\x42" \
"\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E" 


static int myrand( void *rng_state, unsigned char *output, size_t len )
{
    size_t i;

    if( rng_state != NULL )
        rng_state  = NULL;

    for( i = 0; i < len; ++i )
        output[i] = rand();

    return( 0 );
}



// Originally intended



static int doAll(unsigned char *ciphertext, key_rsa *cipherKey){

  printf("Inside: RSA Value of N :%s\n", cipherKey->N);
  
  int j;

  size_t len;
  rsa_context rsa;
  unsigned char rsa_decrypted[PT_LEN];
  unsigned char rsa_hash[PT_LEN];
  unsigned char rsa_sig_out[PT_LEN];
  unsigned char rsa_hash_result[PT_LEN];

  // Decrypted key storing buffer
  unsigned char decrypted_RSA_D[sizeof(RSA_D)];
  unsigned char decrypted_RSA_P[sizeof(RSA_P)];
  unsigned char decrypted_RSA_Q[sizeof(RSA_Q)];
  unsigned char decrypted_RSA_DP[sizeof(RSA_DP)];
  unsigned char decrypted_RSA_DQ[sizeof(RSA_DQ)];
  unsigned char decrypted_RSA_QP[sizeof(RSA_QP)];

  rsa_init( &rsa, RSA_PKCS_V15, 0 );
  rsa.len = KEY_LEN;

  //reading public key from the key_rsa structure
  
  /*
  mpi_read_string( &rsa.N , 16, cipherKey->N);
  mpi_read_string( &rsa.E , 16, cipherKey->E);

  if( rsa_check_pubkey(&rsa) != 0 ) {
    printf( "Public key error! \n" );
    exit(0);
  }else{
    printf( "Public key reading successful! \n" );
  }
  */


  // decrypt rsa private key's with AES
  aes_context aes;
  aes_setkey_dec(&aes,mkt,AES_KEY_SIZE_BITS); 

  

  printf("Inside doAll: sizeof(cipherKey->D)/AES_BLOCK_SIZE: %d\n", sizeof(cipherKey->D)/AES_BLOCK_SIZE);
  printf("Inside doAll: size of cipherKey->D is : %d\n", sizeof(cipherKey->D));

  for(j=0;j<sizeof(cipherKey->D)/AES_BLOCK_SIZE;++j){
    aes_crypt_ecb(&aes,AES_DECRYPT, cipherKey->D + AES_BLOCK_SIZE*j,decrypted_RSA_D+AES_BLOCK_SIZE*j);
    printf("J is %d\n", j );
    printf("decrypt D is : %s\n",decrypted_RSA_D );
  }
 //  printf("Inside doAll: Decrypted RSA_D value: %s\n", decrypted_RSA_D);

 //  for(j=0;j<sizeof(cipherKey->P)/AES_BLOCK_SIZE;++j){
 //    aes_crypt_ecb(&aes,AES_DECRYPT, cipherKey->P + AES_BLOCK_SIZE*j,decrypted_RSA_P+AES_BLOCK_SIZE*j);
 //  }
 //  printf("Inside doAll: Decrypted RSA_P value: %s\n", decrypted_RSA_P);
  
 //  for(j=0;j<sizeof(cipherKey->Q)/AES_BLOCK_SIZE;++j){
 //    aes_crypt_ecb(&aes,AES_DECRYPT, cipherKey->Q + AES_BLOCK_SIZE*j,decrypted_RSA_Q+AES_BLOCK_SIZE*j);
 //  }
 // printf("Inside doAll: Decrypted RSA_Q value: %s\n", decrypted_RSA_Q);

 //  for(j=0;j<sizeof(cipherKey->DP)/AES_BLOCK_SIZE;++j){
 //    aes_crypt_ecb(&aes,AES_DECRYPT, cipherKey->DP + AES_BLOCK_SIZE*j,decrypted_RSA_DP+AES_BLOCK_SIZE*j);
 //  }
 //  printf("Inside doAll: Decrypted RSA_DP value: %s\n", decrypted_RSA_DP);

 //  for(j=0;j<sizeof(cipherKey->DQ)/AES_BLOCK_SIZE;++j){
 //    aes_crypt_ecb(&aes,AES_DECRYPT, cipherKey->DQ + AES_BLOCK_SIZE*j,decrypted_RSA_DQ+AES_BLOCK_SIZE*j);
 //  }
 //  printf("Inside doAll: Decrypted RSA_DQ value: %s\n", decrypted_RSA_DQ);

 //  for(j=0;j<sizeof(cipherKey->QP)/AES_BLOCK_SIZE;++j){
 //    aes_crypt_ecb(&aes,AES_DECRYPT, cipherKey->QP + AES_BLOCK_SIZE*j,decrypted_RSA_QP+AES_BLOCK_SIZE*j);
 //  }
 //  printf("Inside doAll: Decrypted RSA_QP value: %s\n", decrypted_RSA_QP);
  


 //  // reading 
 //  mpi_read_string( &rsa.N , 16, cipherKey->N);
 //  mpi_read_string( &rsa.E , 16, cipherKey->E);
 //  mpi_read_string( &rsa.D , 16, decrypted_RSA_D);
 //  mpi_read_string( &rsa.P , 16, decrypted_RSA_P);
 //  mpi_read_string( &rsa.Q , 16, decrypted_RSA_Q);
 //  mpi_read_string( &rsa.DP, 16, decrypted_RSA_DP);
 //  mpi_read_string( &rsa.DQ, 16, decrypted_RSA_DQ );
 //  mpi_read_string( &rsa.QP, 16, decrypted_RSA_QP );

 //  // Checking the public and private keys
 //  if( rsa_check_pubkey(  &rsa ) != 0 ||rsa_check_privkey( &rsa ) != 0 ) {
 //    printf( "Public/Private key error! \n" );
 //    exit(0);
 //  }else{
 //    printf("Key reading success\n");
 //  }


 //  //memcpy( rsa_plaintext, RSA_PT, PT_LEN );

 //  //printf("plain text is: %s\n", rsa_plaintext);
  

  

 //  if( rsa_pkcs1_decrypt( &rsa, &myrand, NULL, RSA_PRIVATE, &len,
 //                       ciphertext, rsa_decrypted,
 //                       sizeof(rsa_decrypted) ) != 0 ) {
 //    printf( "Decryption failed! \n" );
 //    exit(0);
 //  }else{
 //    printf("Decrypted plaintext %s\n",rsa_decrypted );
 //  }

   rsa_free(&rsa);


  return 0;
  
}








void main(){

    int result,j;
    result = -1;
    unsigned char rsa_plaintext[PT_LEN];
    unsigned char rsa_ciphertext[KEY_LEN];
    
    unsigned char encrypted_RSA_D[sizeof(RSA_KEY_NO_LABEL)];
    unsigned char encrypted_RSA_P[sizeof(RSA_KEY_NO_LABEL)];
    unsigned char encrypted_RSA_Q[sizeof(RSA_KEY_NO_LABEL)];
    unsigned char encrypted_RSA_DP[sizeof(RSA_KEY_NO_LABEL)];
    unsigned char encrypted_RSA_DQ[sizeof(RSA_KEY_NO_LABEL)];
    unsigned char encrypted_RSA_QP[sizeof(RSA_KEY_NO_LABEL)];

    unsigned char decrypt_private_key[sizeof(RSA_KEY_NO_LABEL)];
    unsigned char decrypt_private_key_P[sizeof(RSA_KEY_NO_LABEL)];

    rsa_context rsa;
    key_rsa test;


    rsa_init( &rsa, RSA_PKCS_V15, 0 );
    rsa.len = KEY_LEN;

    // setting RSA public key
    mpi_read_string( &rsa.N , 16, RSA_N  );
    mpi_read_string( &rsa.E , 16, RSA_E  );

    if( rsa_check_pubkey(&rsa) != 0) {
    printf( "Public key error! \n" );
    exit(0);
  }

  memcpy( rsa_plaintext, RSA_PT, PT_LEN );
  printf("plain text is: %s\n", rsa_plaintext);

  if( rsa_pkcs1_encrypt( &rsa, &myrand, NULL, RSA_PUBLIC, PT_LEN, rsa_plaintext, rsa_ciphertext ) != 0 ) {
      printf( "Encryption failed! \n" );
      exit(0);
  }else {
    printf("RSA Encryption Successful\n");
  }


 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


  //calling AES to encrypt private keys
  aes_context aes;


  // following function will generate all the AES round keys for encryption
  aes_setkey_enc(&aes,mkt,AES_KEY_SIZE_BITS);

  for(j=0;j<sizeof(RSA_D)/AES_BLOCK_SIZE;++j){
    aes_crypt_ecb(&aes,AES_ENCRYPT, RSA_D + AES_BLOCK_SIZE*j,encrypted_RSA_D+AES_BLOCK_SIZE*j);  
  }
  //printf("Original RSA_D value: %s\n", RSA_D );
  //printf("Encrypted RSA_D value: %s\n", encrypted_RSA_D);
  //printf("private key --> RSA_D encrypted \n");

  for(j=0;j<sizeof(RSA_P)/AES_BLOCK_SIZE;++j){
    aes_crypt_ecb(&aes,AES_ENCRYPT, RSA_P + AES_BLOCK_SIZE*j,encrypted_RSA_P+AES_BLOCK_SIZE*j);  
  }
  printf("private key --> RSA_P encrypted \n");

  for(j=0;j<sizeof(RSA_Q)/AES_BLOCK_SIZE;++j){
    aes_crypt_ecb(&aes,AES_ENCRYPT, RSA_Q + AES_BLOCK_SIZE*j,encrypted_RSA_Q+AES_BLOCK_SIZE*j);  
  }
  printf("private key --> RSA_Q encrypted \n");

  for(j=0;j<sizeof(RSA_DP)/AES_BLOCK_SIZE;++j){
    aes_crypt_ecb(&aes,AES_ENCRYPT, RSA_DP + AES_BLOCK_SIZE*j,encrypted_RSA_DP+AES_BLOCK_SIZE*j);  
  }
  printf("private key --> RSA_DP encrypted \n");

  for(j=0;j<sizeof(RSA_DQ)/AES_BLOCK_SIZE;++j){
    aes_crypt_ecb(&aes,AES_ENCRYPT, RSA_DQ + AES_BLOCK_SIZE*j,encrypted_RSA_DQ+AES_BLOCK_SIZE*j);  
  }
  printf("private key --> RSA_DQ encrypted \n");

  for(j=0;j<sizeof(RSA_QP)/AES_BLOCK_SIZE;++j){
    aes_crypt_ecb(&aes,AES_ENCRYPT, RSA_QP + AES_BLOCK_SIZE*j,encrypted_RSA_QP+AES_BLOCK_SIZE*j);  
  }
  printf("private key --> RSA_QP encrypted \n");


  // adding this encrypted keys into _key_rsa structure
  memcpy(test.N, RSA_N, sizeof(RSA_N));
  memcpy(test.E, RSA_E, sizeof(RSA_E));
  memcpy(test.D, encrypted_RSA_D, sizeof(encrypted_RSA_D));
  memcpy(test.P, encrypted_RSA_P, sizeof(encrypted_RSA_P));
  memcpy(test.Q, encrypted_RSA_Q, sizeof(encrypted_RSA_Q));
  memcpy(test.DP, encrypted_RSA_DP, sizeof(encrypted_RSA_DP));
  memcpy(test.DQ, encrypted_RSA_DQ, sizeof(encrypted_RSA_DQ));
  memcpy(test.QP, encrypted_RSA_QP, sizeof(encrypted_RSA_QP));
  //printf("RSA Value of N :%s\n", test.N);


  


// this should be inside doALL function
/*
  aes_setkey_dec(&aes,mkt,AES_KEY_SIZE_BITS);
  for(j=0;j<sizeof(RSA_D)/AES_BLOCK_SIZE;++j){
    aes_crypt_ecb(&aes,AES_DECRYPT, encrypted_RSA_D + AES_BLOCK_SIZE*j,decrypt_private_key+AES_BLOCK_SIZE*j);    
  }
  printf("Decrypted RSA_D value: %s\n", decrypt_private_key);
  printf("sizeof(RSA_D)/AES_BLOCK_SIZE: %d\n", sizeof(RSA_D)/AES_BLOCK_SIZE);


*/



  aes_setkey_dec(&aes,mkt,AES_KEY_SIZE_BITS);

  printf("sizeof(sizeof(test.D)/AES_BLOCK_SIZE: %d\n", sizeof(test.D)/AES_BLOCK_SIZE);
  printf("sizeof(sizeof(RSA_D)/AES_BLOCK_SIZE: %d\n", sizeof(RSA_D)/AES_BLOCK_SIZE);


  for(j=0;j<sizeof(test.D)/AES_BLOCK_SIZE;++j){
    aes_crypt_ecb(&aes,AES_DECRYPT, test.D + AES_BLOCK_SIZE*j,decrypt_private_key+AES_BLOCK_SIZE*j);
  }
  printf("Decrypted RSA_D value: %s\n", decrypt_private_key);

  for(j=0;j<sizeof(test.P)/AES_BLOCK_SIZE;++j){
    aes_crypt_ecb(&aes,AES_DECRYPT, test.P + AES_BLOCK_SIZE*j,decrypt_private_key_P+AES_BLOCK_SIZE*j);
  }
  printf("Decrypted RSA_P value: %s\n", decrypt_private_key_P);
  



    // calling doAll Function with my custom stracture
    result = doAll(rsa_ciphertext, &test);


  



  
}
