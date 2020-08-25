#include <stdio.h>
#include <string.h>
#include <openssl/crypto.h>
#include <openssl/dso.h>
#include <openssl/x509.h>
#include <openssl/objects.h>
#include <openssl/engine.h>
#include <openssl/rand.h>
#ifndef OPENSSL_NO_RSA
#include <openssl/rsa.h>
#endif
#include <openssl/bn.h>

#include <openssl/err.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

//#define SOFT_TEST

#ifdef SOFT_TEST
#include "rsa.h"
#endif

static char testKeyLabel[20] = {'T','E','S','T','K','E','Y',0};

//#define MAX_MPI_IN_BYTE (3072/8)
#define MAX_MPI_IN_BYTE (2048/8)
#define MAX_MOD	MAX_MPI_IN_BYTE
#define LABEL_SIZE  32
#define KEY_ID_LEN	(LABEL_SIZE)
typedef struct _Sign_Para{
	char label[LABEL_SIZE];
	unsigned char in[MAX_MPI_IN_BYTE];
	unsigned char out[MAX_MPI_IN_BYTE];
}Sign_Para,*pSign_Para;

typedef struct _GetPubPara{
	char label[LABEL_SIZE];
	unsigned char N[MAX_MPI_IN_BYTE];
	unsigned char E[MAX_MPI_IN_BYTE];
}GetPubPara,*pGetPubPara;



#define IsoToken_IOC_ID 'T'

#define IsoToken_IOC_SET_MASTER_KEY	_IOW(IsoToken_IOC_ID,0,unsigned char[MASTER_KEY_SIZE])
#define IsoToken_IOC_GET_PRIVATE_KEY_ID_NUM	_IOR(IsoToken_IOC_ID,1,int)
#define IsoToken_IOC_GET_PRIVATE_KEY_ID	_IOR(IsoToken_IOC_ID,1,unsigned char[KEY_ID_LEN])
#define IsoToken_IOC_SIGN	_IOWR(IsoToken_IOC_ID,2,unsigned char[MAX_MOD])

#define IsoToken_IOC_GET_PUBKEY	_IOWR(IsoToken_IOC_ID,3,unsigned char[MAX_MOD])











int RSA_padding_check_PKCS1_type_1(unsigned char *to, int tlen,
	     const unsigned char *from, int flen, int num);
int RSA_padding_check_PKCS1_OAEP(unsigned char *to, int tlen,
          const unsigned char *from, int flen, int num,
          const unsigned char *param, int plen);


int RSA_padding_check_SSLv23(unsigned char *to, int tlen,
         const unsigned char *from, int flen, int num);


int RSA_padding_check_none(unsigned char *to, int tlen,
          const unsigned char *from, int flen, int num);


/* BEGIN ERROR CODES */
/* The following lines are auto generated by the script mkerr.pl. Any changes
 * made after this point may be overwritten when the script is next run.
 */
static void ERR_load_Copker_strings(void);
static void ERR_unload_Copker_strings(void);
static void ERR_Copker_error(int function, int reason, char *file, int line);
#define Copkererr(f,r) ERR_Copker_error((f),(r),__FILE__,__LINE__)

/* Error codes for the Copker functions. */

/* Function codes. */
#define COPKER_F_SIGN				 105
#define COPKER_F_VERIFY			 106
#define COPKER_F_CTRL			 100
#define COPKER_F_FINISH			 101
#define COPKER_F_INIT			 102
#define COPKER_F_LOAD_PRIVKEY			 103
#define COPKER_F_LOAD_PUBKEY			 104

/* Reason codes. */
#define COPKER_R_ALREADY_LOADED			 100
#define COPKER_R_ASN1_OID_UNKNOWN_FOR_MD		 101
#define COPKER_R_COMMAND_NOT_IMPLEMENTED		 102
#define COPKER_R_DSO_FAILURE				 103
#define COPKER_R_FAILED_LOADING_PRIVATE_KEY		 104
#define COPKER_R_FAILED_LOADING_PUBLIC_KEY		 105
#define COPKER_R_NOT_LOADED				 106
#define COPKER_R_SIZE_TOO_LARGE_OR_TOO_SMALL		 107
#define COPKER_R_UNIT_FAILURE				 108
#define COPKER_R_UNKNOWN_ALGORITHM_TYPE		 109


/* BEGIN ERROR CODES */
#ifndef OPENSSL_NO_ERR

#define ERR_FUNC(func) ERR_PACK(0,func,0)
#define ERR_REASON(reason) ERR_PACK(0,0,reason)

static ERR_STRING_DATA Copker_str_functs[]=
	{
{ERR_FUNC(COPKER_F_SIGN),	"CCA_RSA_SIGN"},
{ERR_FUNC(COPKER_F_VERIFY),	"CCA_RSA_VERIFY"},
{ERR_FUNC(COPKER_F_CTRL),	"IBM_4758_CCA_CTRL"},
{ERR_FUNC(COPKER_F_FINISH),	"IBM_4758_CCA_FINISH"},
{ERR_FUNC(COPKER_F_INIT),	"IBM_4758_CCA_INIT"},
{ERR_FUNC(COPKER_F_LOAD_PRIVKEY),	"COPKER_LOAD_PRIVKEY"},
{ERR_FUNC(COPKER_F_LOAD_PUBKEY),	"IBM_4758_LOAD_PUBKEY"},
{0,NULL}
	};

static ERR_STRING_DATA Copker_str_reasons[]=
	{
{ERR_REASON(COPKER_R_ALREADY_LOADED)    ,"already loaded"},
{ERR_REASON(COPKER_R_ASN1_OID_UNKNOWN_FOR_MD),"asn1 oid unknown for md"},
{ERR_REASON(COPKER_R_COMMAND_NOT_IMPLEMENTED),"command not implemented"},
{ERR_REASON(COPKER_R_DSO_FAILURE)       ,"dso failure"},
{ERR_REASON(COPKER_R_FAILED_LOADING_PRIVATE_KEY),"failed loading private key"},
{ERR_REASON(COPKER_R_FAILED_LOADING_PUBLIC_KEY),"failed loading public key"},
{ERR_REASON(COPKER_R_NOT_LOADED)        ,"not loaded"},
{ERR_REASON(COPKER_R_SIZE_TOO_LARGE_OR_TOO_SMALL),"size too large or too small"},
{ERR_REASON(COPKER_R_UNIT_FAILURE)      ,"unit failure"},
{ERR_REASON(COPKER_R_UNKNOWN_ALGORITHM_TYPE),"unknown algorithm type"},
{0,NULL}
	};

#endif



static int Copker_lib_error_code=0;
static int Copker_error_init=1;

static void ERR_load_Copker_strings(void)
	{
	if (Copker_lib_error_code == 0)
		Copker_lib_error_code=ERR_get_next_error_library();

	if (Copker_error_init)
		{
		Copker_error_init=0;
#ifndef OPENSSL_NO_ERR
		ERR_load_strings(Copker_lib_error_code,Copker_str_functs);
		ERR_load_strings(Copker_lib_error_code,Copker_str_reasons);
#endif
		}
	}

static void ERR_unload_Copker_strings(void)
	{
	if (Copker_error_init == 0)
		{
#ifndef OPENSSL_NO_ERR
		ERR_unload_strings(Copker_lib_error_code,Copker_str_functs);
		ERR_unload_strings(Copker_lib_error_code,Copker_str_reasons);
#endif
		Copker_error_init=1;
		}
	}

static void ERR_Copker_error(int function, int reason, char *file, int line)
	{
	if (Copker_lib_error_code == 0)
		Copker_lib_error_code=ERR_get_next_error_library();
	ERR_PUT_error(Copker_lib_error_code,function,reason,file,line);
	}




static int copker_destroy(ENGINE *e);
static int copker_init(ENGINE *e);
static int copker_finish(ENGINE *e);
static int copker_ctrl(ENGINE *e, int cmd, long i, void *p, void (*f)(void));

/* rsa functions */
/*---------------*/
#ifndef OPENSSL_NO_RSA
static int cca_rsa_pub_enc(int flen, const unsigned char *from,
		unsigned char *to, RSA *rsa,int padding);
static int copker_rsa_priv_dec(int flen, const unsigned char *from,
		unsigned char *to, RSA *rsa,int padding);
static int cca_rsa_sign(int type, const unsigned char *m, unsigned int m_len,
		unsigned char *sigret, unsigned int *siglen, const RSA *rsa);
static int cca_rsa_verify(int dtype, const unsigned char *m, unsigned int m_len,
	const unsigned char *sigbuf, unsigned int siglen, const RSA *rsa);

/* utility functions */
/*-----------------------*/
static EVP_PKEY *copker_load_privkey(ENGINE*, const char*,
		UI_METHOD *ui_method, void *callback_data);
static EVP_PKEY *ibm_4758_load_pubkey(ENGINE*, const char*,
		UI_METHOD *ui_method, void *callback_data);

static int getModulusAndExponent(const unsigned char *token, long *exponentLength,
		unsigned char *exponent, long *modulusLength,
		long *modulusFieldLength, unsigned char *modulus);
#endif



#ifndef OPENSSL_NO_RSA
static void cca_ex_free(void *obj, void *item, CRYPTO_EX_DATA *ad,
		int idx,long argl, void *argp);
#endif

/* Function pointers for CCA verbs */
/*---------------------------------*/
/*
#ifndef OPENSSL_NO_RSA

static F_KEYRECORDREAD keyRecordRead;
static F_DIGITALSIGNATUREGENERATE digitalSignatureGenerate;
static F_DIGITALSIGNATUREVERIFY digitalSignatureVerify;
static F_PUBLICKEYEXTRACT publicKeyExtract;
static F_PKAENCRYPT pkaEncrypt;
static F_PKADECRYPT pkaDecrypt;
#endif
static F_RANDOMNUMBERGENERATE randomNumberGenerate;
*/

/* static variables */
/*------------------*/
#ifndef SOFT_TEST
#define COPKER_FILE_NAME_DEFAULT "/dev/template"
#else
#define COPKER_FILE_NAME_DEFAULT "/dev/urandom"
#endif 

static const char *COPKER_FILE_NAME = NULL;
static const char *get_COPKER_FILE_NAME(void)
	{
	if(COPKER_FILE_NAME)
		return COPKER_FILE_NAME;
	return COPKER_FILE_NAME_DEFAULT;
	}
static void free_COPKER_FILE_NAME(void)
	{
	if(COPKER_FILE_NAME)
		OPENSSL_free((void*)COPKER_FILE_NAME);
	COPKER_FILE_NAME = NULL;
	}
static long set_COPKER_FILE_NAME(const char *name)
	{
	free_COPKER_FILE_NAME();
	return (((COPKER_FILE_NAME = BUF_strdup(name)) != NULL) ? 1 : 0);
	}
/*
#ifndef OPENSSL_NO_RSA

static const char* n_keyRecordRead = CSNDKRR;
static const char* n_digitalSignatureGenerate = CSNDDSG;
static const char* n_digitalSignatureVerify = CSNDDSV;
static const char* n_publicKeyExtract = CSNDPKX;
static const char* n_pkaEncrypt = CSNDPKE;
static const char* n_pkaDecrypt = CSNDPKD;

#endif
static const char* n_randomNumberGenerate = CSNBRNG;
*/

#ifndef OPENSSL_NO_RSA
static int hndidx_label = -1;
#endif
//static DSO *dso = NULL;
//static int dso = 0;
static int fd = -1;
static int keyNum = 0;

/* openssl engine initialization structures */
/*------------------------------------------*/

#define COPKER_CMD_FILE_PATH		ENGINE_CMD_BASE
static const ENGINE_CMD_DEFN	copker_cmd_defns[] = {
	{COPKER_CMD_FILE_PATH,
		"FILE_PATH",
		"Specifies the path to the driver file COPKER uses",
		ENGINE_CMD_FLAG_STRING},
	{0, NULL, NULL, 0}
	};

#ifndef OPENSSL_NO_RSA
static RSA_METHOD ibm_4758_cca_rsa =
	{
	"Copker RSA method",
	cca_rsa_pub_enc,
	NULL,
	NULL,
	copker_rsa_priv_dec,
	NULL, /*rsa_mod_exp,*/
	NULL, /*mod_exp_mont,*/
	NULL, /* init */
	NULL, /* finish */
	RSA_FLAG_SIGN_VER,	  /* flags */
	NULL, /* app_data */
	cca_rsa_sign, /* rsa_sign */
	cca_rsa_verify, /* rsa_verify */
	NULL /* rsa_keygen */
	};
#endif

#ifdef SOFT_TEST
const RSA_METHOD *meth;
static int soft_copker_rsa_pub_enc(int flen, const unsigned char *from,
		unsigned char *to, RSA *rsa,int padding){
	return meth->rsa_pub_enc(flen,from,to,rsa,padding);
}
static int soft_copker_rsa_pri_enc(int flen, const unsigned char *from,
		unsigned char *to, RSA *rsa,int padding){
	return meth->rsa_priv_enc(flen,from,to,rsa,padding);
}
static int soft_copker_rsa_pub_dec(int flen, const unsigned char *from,
		unsigned char *to, RSA *rsa,int padding){
	return meth->rsa_pub_dec(flen,from,to,rsa,padding);
}
static int soft_copker_rsa_pri_dec(int flen, const unsigned char *from,
		unsigned char *to, RSA *rsa,int padding){
	return meth->rsa_priv_dec(flen,from,to,rsa,padding);
}

static int soft_copker_rsa_mod_exp(BIGNUM *r0, const BIGNUM *i, RSA *rsa, BN_CTX *ctx){
	return meth->rsa_mod_exp(r0,i,rsa,ctx);
}

static int soft_copker_BN_mod_exp_mont(BIGNUM *rr, const BIGNUM *a, const BIGNUM *p,
                      const BIGNUM *m, BN_CTX *ctx, BN_MONT_CTX *in_mont){
	return meth->bn_mod_exp(rr,a,p,m,ctx,in_mont);

}
static int soft_copker_init(RSA *rsa){
	return meth->init(rsa);
}
static int soft_copker_finish(RSA *rsa){
	return meth->finish(rsa);
}
static int polar_rsa_pri_dec(int flen, const unsigned char *from,
		unsigned char *to, RSA *rsa,int padding){
	BIGNUM *f, *ret;
	int j,num=0,r= -1;
	unsigned char *buf=NULL;
	BN_CTX *ctx=NULL;
	char *keyLabel;
	unsigned char temp[3072/8];
	int tempLen;
	if(fd == -1){
		return 0;
	}
	if((ctx = BN_CTX_new()) == NULL) goto err;
	BN_CTX_start(ctx);
	f   = BN_CTX_get(ctx);
	ret = BN_CTX_get(ctx);
	num = BN_num_bytes(rsa->n);
	buf = OPENSSL_malloc(num);
	if(!f || !ret || !buf)
		{
		//RSAerr(RSA_F_RSA_EAY_PRIVATE_DECRYPT,ERR_R_MALLOC_FAILURE);
		goto err;
		}

	/* This check was for equality but PGP does evil things
	 * and chops off the top '0' bytes */
	if (flen > num)
		{
		//RSAerr(RSA_F_RSA_EAY_PRIVATE_DECRYPT,RSA_R_DATA_GREATER_THAN_MOD_LEN);
		goto err;
		}
   	rsa_context rsac;

  	rsa_init(&rsac,RSA_PKCS_V15, 0);

	rsac.len = BN_num_bytes(rsa->n);

	tempLen = BN_bn2bin(rsa->n,temp);
	mpi_read_binary( &rsac.N, temp, tempLen );

	tempLen = BN_bn2bin(rsa->e,temp);
	mpi_read_binary( &rsac.E, temp, tempLen );

	tempLen = BN_bn2bin(rsa->d,temp);
	mpi_read_binary( &rsac.D, temp, tempLen );

	tempLen = BN_bn2bin(rsa->p,temp);
	mpi_read_binary( &rsac.P, temp, tempLen );

	tempLen = BN_bn2bin(rsa->q,temp);
	mpi_read_binary( &rsac.Q, temp, tempLen );

	tempLen = BN_bn2bin(rsa->dmp1,temp);
	mpi_read_binary( &rsac.DP, temp, tempLen );

	tempLen = BN_bn2bin(rsa->dmq1,temp);
	mpi_read_binary( &rsac.DQ, temp, tempLen );

	tempLen = BN_bn2bin(rsa->iqmp,temp);
	mpi_read_binary( &rsac.QP, temp, tempLen );

/*
	rsac.len = KEY_LEN;
	mpi_read_string( &rsac.N , 16, RSA_N  );
	mpi_read_string( &rsac.E , 16, RSA_E  );
	mpi_read_string( &rsac.D , 16, RSA_D  );
	mpi_read_string( &rsac.P , 16, RSA_P  );
	mpi_read_string( &rsac.Q , 16, RSA_Q  );
	mpi_read_string( &rsac.DP, 16, RSA_DP );
	mpi_read_string( &rsac.DQ, 16, RSA_DQ );
	mpi_read_string( &rsac.QP, 16, RSA_QP );
*/
	rsa_private( &rsac, from, buf);
	//rsa_pkcs1_decrypt( &rsac,RSA_PRIVATE, &r,from,buf,num);
	//r = soft_copker_rsa_pri_dec(flen, from,to, rsa,padding);
	//copker_rsa_private(from,buf,num,keyLabel);
	j = num - 1;

	switch (padding)
		{
	case RSA_PKCS1_PADDING:
		r=RSA_padding_check_PKCS1_type_2(to,num,buf+1,j,num);
		break;
#ifndef OPENSSL_NO_SHA
        case RSA_PKCS1_OAEP_PADDING:
	        r=RSA_padding_check_PKCS1_OAEP(to,num,buf,j,num,NULL,0);
                break;
#endif
 	case RSA_SSLV23_PADDING:
		r=RSA_padding_check_SSLv23(to,num,buf,j,num);
		break;
	case RSA_NO_PADDING:
		r=RSA_padding_check_none(to,num,buf,j,num);
		break;
	default:
		//RSAerr(RSA_F_RSA_EAY_PRIVATE_DECRYPT,RSA_R_UNKNOWN_PADDING_TYPE);
		goto err;
		}
	if (r < 0)
		///RSAerr(RSA_F_RSA_EAY_PRIVATE_DECRYPT,RSA_R_PADDING_CHECK_FAILED);

err:
	if (ctx != NULL)
		{
		BN_CTX_end(ctx);
		BN_CTX_free(ctx);
		}
	if (buf != NULL)
		{
		OPENSSL_cleanse(buf,num);
		OPENSSL_free(buf);
		}
	return(r);	
}
static RSA_METHOD soft_copker_rsa =
	{
	"Copker soft RSA method",
	NULL,//soft_copker_rsa_pub_enc,
	NULL,//soft_copker_rsa_pub_dec,
	NULL,//soft_copker_rsa_pri_enc,
	polar_rsa_pri_dec,//polar_rsa_pri_dec,//soft_copker_rsa_pri_dec,
	soft_copker_rsa_mod_exp,//NULL,//soft_copker_rsa_mod_exp, /*rsa_mod_exp,*/
	soft_copker_BN_mod_exp_mont,//NULL,//soft_copker_BN_mod_exp_mont, /*mod_exp_mont,*/
	NULL,//soft_copker_init, /* init */
	NULL,//soft_copker_finish, /* finish */
	RSA_FLAG_SIGN_VER,	  /*0 flags */
	NULL, /* app_data */
	0, /* rsa_sign */
	0, /* rsa_verify */
	NULL /* rsa_keygen */
	};

#endif

static const char *engine_COPKER_id = "TSX";
static const char *engine_COPKER_name = "Engine support for TSX";
#ifndef OPENSSL_NO_DYNAMIC_ENGINE 
/* Compatibility hack, the dynamic library uses this form in the path */
static const char *engine_COPKER_id_alt = "TSX";
#endif

/* engine implementation */
/*-----------------------*/
static int bind_helper(ENGINE *e)
	{
#ifdef SOFT_TEST
	meth  = RSA_PKCS1_SSLeay();
#endif
	if(!ENGINE_set_id(e, engine_COPKER_id) ||
			!ENGINE_set_name(e, engine_COPKER_name) ||
#ifndef OPENSSL_NO_RSA
#ifndef SOFT_TEST
			!ENGINE_set_RSA(e, &ibm_4758_cca_rsa) ||
#else			
			!ENGINE_set_RSA(e,  &soft_copker_rsa) ||
#endif
#endif
			!ENGINE_set_destroy_function(e, copker_destroy) ||
			!ENGINE_set_init_function(e, copker_init) ||
			!ENGINE_set_finish_function(e, copker_finish) ||
			!ENGINE_set_ctrl_function(e, copker_ctrl) ||
#ifndef OPENSSL_NO_RSA
			!ENGINE_set_load_privkey_function(e, copker_load_privkey) ||
			!ENGINE_set_load_pubkey_function(e, ibm_4758_load_pubkey) ||
#endif
			!ENGINE_set_cmd_defns(e, copker_cmd_defns))
		return 0;
	/* Ensure the error handling is set up */
	ERR_load_Copker_strings();
	return 1;
	}

#ifdef OPENSSL_NO_DYNAMIC_ENGINE
static ENGINE *engine_COPKER(void)
	{
	ENGINE *ret = ENGINE_new();
	if(!ret)
		return NULL;
	if(!bind_helper(ret))
		{
		ENGINE_free(ret);
		return NULL;
		}
	return ret;
	}

void ENGINE_load_Copker(void)
	{
	ENGINE *COPKER = engine_COPKER();
	if (!COPKER) return;
	ENGINE_add(COPKER);
	ENGINE_free(COPKER);
	ERR_clear_error();   
	}
#endif

static int copker_destroy(ENGINE *e)
	{
	ERR_unload_Copker_strings();
	free_COPKER_FILE_NAME();
	return 1;
	}

static int copker_init(ENGINE *e)
	{
	//goto err;
	if(fd != -1)
		{
		Copkererr(COPKER_F_INIT,COPKER_R_ALREADY_LOADED);
		goto err;
		}
	fd = open(get_COPKER_FILE_NAME(), O_RDWR, 0);
	if (fd < 0) {
		Copkererr(COPKER_F_INIT,COPKER_R_DSO_FAILURE);
		perror("open drive file");
		return 1;
	}
#ifndef SOFT_TEST
	ioctl(fd,IsoToken_IOC_GET_PRIVATE_KEY_ID_NUM,&keyNum);
#endif
#ifndef OPENSSL_NO_RSA
	hndidx_label = RSA_get_ex_new_index(0, "copker key label",
		NULL, NULL, cca_ex_free);

#endif
	return 1;
err:
	if(fd != -1)
		close(fd);
	fd = -1;
	return 0;
	}

static int copker_finish(ENGINE *e)
	{
	free_COPKER_FILE_NAME();
	fd = -1;
/*
#ifndef OPENSSL_NO_RSA
	keyRecordRead = (F_KEYRECORDREAD)0;
	randomNumberGenerate = (F_RANDOMNUMBERGENERATE)0;
	digitalSignatureGenerate = (F_DIGITALSIGNATUREGENERATE)0;
	digitalSignatureVerify = (F_DIGITALSIGNATUREVERIFY)0;
	publicKeyExtract = (F_PUBLICKEYEXTRACT)0;
	pkaEncrypt = (F_PKAENCRYPT)0;
	pkaDecrypt = (F_PKADECRYPT)0;
#endif
	randomNumberGenerate = (F_RANDOMNUMBERGENERATE)0;
*/
	return 1;
	}

static int copker_ctrl(ENGINE *e, int cmd, long i, void *p, void (*f)(void))
	{
	int initialised = ((fd == -1) ? 0 : 1);
	switch(cmd)
		{
	case COPKER_CMD_FILE_PATH:
		if(p == NULL)
			{
			Copkererr(COPKER_F_CTRL,
					ERR_R_PASSED_NULL_PARAMETER);
			return 0;
			}
		if(initialised)
			{
			Copkererr(COPKER_F_CTRL,
					COPKER_R_ALREADY_LOADED);
			return 0;
			}
		return set_COPKER_FILE_NAME((const char *)p);
	default:
		break;
		}
	Copkererr(COPKER_F_CTRL,
			COPKER_R_COMMAND_NOT_IMPLEMENTED);
	return 0;
	}

#ifndef OPENSSL_NO_RSA

#define MAX_CCA_PKA_TOKEN_SIZE 2500

static EVP_PKEY *copker_load_privkey(ENGINE* e, const char* key_id,
			UI_METHOD *ui_method, void *callback_data)
	{
	RSA *rtmp = NULL;
	EVP_PKEY *res = NULL;

	GetPubPara getP;
	//unsigned char keyLabel[20] = {'T','E','S','T','K','E','Y',0};
	unsigned char *keyLabel = NULL;
	char key_id_t[10] = {0};
	strcpy(key_id_t,"TESTKEY");
	
	

	key_id = key_id_t;

	if (strlen(key_id) > LABEL_SIZE)
		{
		Copkererr(COPKER_F_LOAD_PRIVKEY,
		COPKER_R_SIZE_TOO_LARGE_OR_TOO_SMALL);
		return NULL;
		}
	keyLabel = OPENSSL_malloc(LABEL_SIZE);
	if (!keyLabel)
		{
		Copkererr(COPKER_F_LOAD_PRIVKEY,
				ERR_R_MALLOC_FAILURE);
		goto err;
		}

	memset(keyLabel, 0, LABEL_SIZE);
	memcpy(keyLabel, key_id, strlen(key_id));

	strcpy(getP.label, key_id);
#ifndef SOFT_TEST
	if(ioctl(fd,IsoToken_IOC_GET_PUBKEY,&getP) == -1){
		Copkererr(COPKER_F_LOAD_PRIVKEY,
				ERR_R_MALLOC_FAILURE);
		goto err;
	}

	rtmp = RSA_new_method(e);
	rtmp->e = BN_bin2bn(getP.E, MAX_MPI_IN_BYTE, NULL);
	rtmp->n = BN_bin2bn(getP.N, MAX_MPI_IN_BYTE, NULL);
#else	
	BIO *bioS;
   	if ((bioS=BIO_new_file("/home/hello/apache/key.pem", "r")) == NULL)
	        return NULL;
	res = PEM_read_bio_PrivateKey(bioS,NULL,NULL,"123456");
	BIO_free(bioS);
	return res;
#endif
	RSA_set_ex_data(rtmp, hndidx_label, (char *)keyLabel);
	rtmp->flags |= RSA_FLAG_EXT_PKEY;

	res = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(res, rtmp);

	return res;
err:
	if (keyLabel)
		OPENSSL_free(keyLabel);
	return NULL;
	}

static EVP_PKEY *ibm_4758_load_pubkey(ENGINE* e, const char* key_id,
			UI_METHOD *ui_method, void *callback_data)
	{
/*
	RSA *rtmp = NULL;
	EVP_PKEY *res = NULL;
	unsigned char* keyToken = NULL;
	long keyTokenLength = MAX_CCA_PKA_TOKEN_SIZE;
	long returnCode;
	long reasonCode;
	long exitDataLength = 0;
	long ruleArrayLength = 0;
	unsigned char exitData[8];
	unsigned char ruleArray[8];
	unsigned char keyLabel[64];
	unsigned long keyLabelLength = strlen(key_id);
	unsigned char modulus[512];
	long modulusFieldLength = sizeof(modulus);
	long modulusLength = 0;
	unsigned char exponent[512];
	long exponentLength = sizeof(exponent);

	if (keyLabelLength > sizeof(keyLabel))
		{
		Copkererr(COPKER_F_IBM_4758_LOAD_PUBKEY,
			COPKER_R_SIZE_TOO_LARGE_OR_TOO_SMALL);
		return NULL;
		}

	memset(keyLabel,' ', sizeof(keyLabel));
	memcpy(keyLabel, key_id, keyLabelLength);

	keyToken = OPENSSL_malloc(MAX_CCA_PKA_TOKEN_SIZE + sizeof(long));
	if (!keyToken)
		{
		Copkererr(COPKER_F_IBM_4758_LOAD_PUBKEY,
				ERR_R_MALLOC_FAILURE);
		goto err;
		}

	keyRecordRead(&returnCode, &reasonCode, &exitDataLength, exitData,
		&ruleArrayLength, ruleArray, keyLabel, &keyTokenLength,
		keyToken+sizeof(long));

	if (returnCode)
		{
		Copkererr(COPKER_F_IBM_4758_LOAD_PUBKEY,
				ERR_R_MALLOC_FAILURE);
		goto err;
		}

	if (!getModulusAndExponent(keyToken+sizeof(long), &exponentLength,
			exponent, &modulusLength, &modulusFieldLength, modulus))
		{
		Copkererr(COPKER_F_IBM_4758_LOAD_PUBKEY,
			COPKER_R_FAILED_LOADING_PUBLIC_KEY);
		goto err;
		}

	(*(long*)keyToken) = keyTokenLength;
	rtmp = RSA_new_method(e);
	RSA_set_ex_data(rtmp, hndidx, (char *)keyToken);
	rtmp->e = BN_bin2bn(exponent, exponentLength, NULL);
	rtmp->n = BN_bin2bn(modulus, modulusFieldLength, NULL);
	rtmp->flags |= RSA_FLAG_EXT_PKEY;
	res = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(res, rtmp);

	return res;
err:
	if (keyToken)
		OPENSSL_free(keyToken);
*/
	return NULL;
	}

static int copker_rsa_private(const unsigned char *in,unsigned char *out,int len,char *keyLabel){// Sign_Para *para
	Sign_Para para;

	strcpy(para.label,keyLabel);
	memcpy(para.in,in,len);
#ifndef SOFT_TEST
	ioctl(fd,IsoToken_IOC_SIGN,para);
#endif	
	memcpy(out,para.out,len);

	return 1;
	


}
static int cca_rsa_pub_enc(int flen, const unsigned char *from,
			unsigned char *to, RSA *rsa,int padding)
	{
/*
	long returnCode;
	long reasonCode;
	long lflen = flen;
	long exitDataLength = 0;
	unsigned char exitData[8];
	long ruleArrayLength = 1;
	unsigned char ruleArray[8] = "PKCS-1.2";
	long dataStructureLength = 0;
	unsigned char dataStructure[8];
	long outputLength = RSA_size(rsa);
	long keyTokenLength;
	unsigned char* keyToken = (unsigned char*)RSA_get_ex_data(rsa, hndidx);

	keyTokenLength = *(long*)keyToken;
	keyToken+=sizeof(long);

	pkaEncrypt(&returnCode, &reasonCode, &exitDataLength, exitData,
		&ruleArrayLength, ruleArray, &lflen, (unsigned char*)from,
		&dataStructureLength, dataStructure, &keyTokenLength,
		keyToken, &outputLength, to);

	if (returnCode || reasonCode)
		return -(returnCode << 16 | reasonCode);
	return outputLength;
*/
	return 0;
	}

static int copker_rsa_priv_dec(int flen, const unsigned char *from,
			unsigned char *to, RSA *rsa,int padding)
	{
	int num=0,r= -1;
	//unsigned char buf[MAX_MPI_IN_BYTE];
	Sign_Para para;
	unsigned char *buf = 0;
	 //BIO *bio_out;
	// bio_out = BIO_new_file("/home/hello/debug.txt", "a");
	 //BIO_printf(bio_out, "Hello World\n");
	 

	//char *keyLabel = testKeyLabel;
	char keyLabel[20] = {'T','E','S','T','K','E','Y',0};
	int ilen;
	unsigned char *p;
	 unsigned char bt;
	if(fd == -1){	
		goto err;
	}
	strcpy(para.label,keyLabel);
	
	num = BN_num_bytes(rsa->n);
	ilen = num;
	memcpy(para.in,from,num);
	//if(!buf)
		//{
		//fprintf(fp,"malloc\n");fclose(fd);
		//RSAerr(RSA_F_RSA_EAY_PRIVATE_DECRYPT,ERR_R_MALLOC_FAILURE);
		//goto err;
		//}

	/* This check was for equality but PGP does evil things
	 * and chops off the top '0' bytes */
	if (flen > num)
		{
		//BIO_printf(bio_out, "flen > num\n");
		//RSAerr(RSA_F_RSA_EAY_PRIVATE_DECRYPT,RSA_R_DATA_GREATER_THAN_MOD_LEN);
		goto err;
		}
	//keyLabel = (char*)RSA_get_ex_data(rsa, hndidx_label);
	//keyLabel = "TESTKEY";
	//fprintf(fp,"label: %s\n",keyLabel);fclose(fd);
	//copker_rsa_private(&para.label,num);
	ioctl(fd,IsoToken_IOC_SIGN,&para);
	buf = para.out;
	p = para.out;

	switch (padding)
		{
	case RSA_PKCS1_PADDING:
           // BIO_printf(bio_out, "here 1\n");
            if( *p++ != 0 )
                goto err;

            bt = *p++;
            if( bt != 2  )
            {
                goto err;
            }

     
		while( *p != 0 && p < buf + ilen - 1 )
		    p++;

		if( *p != 0 || p >= buf + ilen - 1 )
		    goto err;

		p++;
		
            break;
	case RSA_NO_PADDING:
		//BIO_printf(bio_out, "here 3\n");
		memcpy( to, buf, num );
		break;
	default:
		//RSAerr(RSA_F_RSA_EAY_PRIVATE_DECRYPT,RSA_R_UNKNOWN_PADDING_TYPE);
		 //BIO_printf(bio_out, "here 2\n");
		goto err;
		}
	r =  ilen - (p - buf);
	memcpy( to, p, r );
	//BIO_printf(bio_out, "r: %d\n",r);
	if (r < 0)
		goto err;///RSAerr(RSA_F_RSA_EAY_PRIVATE_DECRYPT,RSA_R_PADDING_CHECK_FAILED);

err:
	//BIO_free(bio_out);
	return r;

/*
	long returnCode;
	long reasonCode;
	long lflen = flen;
	long exitDataLength = 0;
	unsigned char exitData[8];
	long ruleArrayLength = 1;
	unsigned char ruleArray[8] = "PKCS-1.2";
	long dataStructureLength = 0;
	unsigned char dataStructure[8];
	long outputLength = RSA_size(rsa);
	long keyTokenLength;
	unsigned char* keyToken = (unsigned char*)RSA_get_ex_data(rsa, hndidx);

	keyTokenLength = *(long*)keyToken;
	keyToken+=sizeof(long);

	pkaDecrypt(&returnCode, &reasonCode, &exitDataLength, exitData,
		&ruleArrayLength, ruleArray, &lflen, (unsigned char*)from,
		&dataStructureLength, dataStructure, &keyTokenLength,
		keyToken, &outputLength, to);

	return (returnCode | reasonCode) ? 0 : 1;
*/
	}

#define SSL_SIG_LEN 36

static int cca_rsa_verify(int type, const unsigned char *m, unsigned int m_len,
	const unsigned char *sigbuf, unsigned int siglen, const RSA *rsa)
	{
/*
	long returnCode;
	long reasonCode;
	long lsiglen = siglen;
	long exitDataLength = 0;
	unsigned char exitData[8];
	long ruleArrayLength = 1;
	unsigned char ruleArray[8] = "PKCS-1.1";
	long keyTokenLength;
	unsigned char* keyToken = (unsigned char*)RSA_get_ex_data(rsa, hndidx);
	long length = SSL_SIG_LEN;
	long keyLength ;
	unsigned char *hashBuffer = NULL;
	X509_SIG sig;
	ASN1_TYPE parameter;
	X509_ALGOR algorithm;
	ASN1_OCTET_STRING digest;

	keyTokenLength = *(long*)keyToken;
	keyToken+=sizeof(long);

	if (type == NID_md5 || type == NID_sha1)
		{
		sig.algor = &algorithm;
		algorithm.algorithm = OBJ_nid2obj(type);

		if (!algorithm.algorithm)
			{
			Copkererr(COPKER_F_CCA_RSA_VERIFY,
				COPKER_R_UNKNOWN_ALGORITHM_TYPE);
			return 0;
			}

		if (!algorithm.algorithm->length)
			{
			Copkererr(COPKER_F_CCA_RSA_VERIFY,
				COPKER_R_ASN1_OID_UNKNOWN_FOR_MD);
			return 0;
			}

		parameter.type = V_ASN1_NULL;
		parameter.value.ptr = NULL;
		algorithm.parameter = &parameter;

		sig.digest = &digest;
		sig.digest->data = (unsigned char*)m;
		sig.digest->length = m_len;

		length = i2d_X509_SIG(&sig, NULL);
		}

	keyLength = RSA_size(rsa);

	if (length - RSA_PKCS1_PADDING > keyLength)
		{
		Copkererr(COPKER_F_CCA_RSA_VERIFY,
			COPKER_R_SIZE_TOO_LARGE_OR_TOO_SMALL);
		return 0;
		}

	switch (type)
		{
		case NID_md5_sha1 :
			if (m_len != SSL_SIG_LEN)
				{
				Copkererr(COPKER_F_CCA_RSA_VERIFY,
				COPKER_R_SIZE_TOO_LARGE_OR_TOO_SMALL);
				return 0;
				}

			hashBuffer = (unsigned char *)m;
			length = m_len;
			break;
		case NID_md5 :
			{
			unsigned char *ptr;
			ptr = hashBuffer = OPENSSL_malloc(
					(unsigned int)keyLength+1);
			if (!hashBuffer)
				{
				Copkererr(COPKER_F_CCA_RSA_VERIFY,
						ERR_R_MALLOC_FAILURE);
				return 0;
				}

			i2d_X509_SIG(&sig, &ptr);
			}
			break;
		case NID_sha1 :
			{
			unsigned char *ptr;
			ptr = hashBuffer = OPENSSL_malloc(
					(unsigned int)keyLength+1);
			if (!hashBuffer)
				{
				Copkererr(COPKER_F_CCA_RSA_VERIFY,
						ERR_R_MALLOC_FAILURE);
				return 0;
				}
			i2d_X509_SIG(&sig, &ptr);
			}
			break;
		default:
			return 0;
		}

	digitalSignatureVerify(&returnCode, &reasonCode, &exitDataLength,
		exitData, &ruleArrayLength, ruleArray, &keyTokenLength,
		keyToken, &length, hashBuffer, &lsiglen,
						(unsigned char *)sigbuf);

	if (type == NID_sha1 || type == NID_md5)
		{
		OPENSSL_cleanse(hashBuffer, keyLength+1);
		OPENSSL_free(hashBuffer);
		}

	return ((returnCode || reasonCode) ? 0 : 1);
*/
	return 0;
	}

#define SSL_SIG_LEN 36

static int cca_rsa_sign(int type, const unsigned char *m, unsigned int m_len,
		unsigned char *sigret, unsigned int *siglen, const RSA *rsa)
	{
/*
	long returnCode;
	long reasonCode;
	long exitDataLength = 0;
	unsigned char exitData[8];
	long ruleArrayLength = 1;
	unsigned char ruleArray[8] = "PKCS-1.1";
	long outputLength=256;
	long outputBitLength;
	long keyTokenLength;
	unsigned char *hashBuffer = NULL;
	unsigned char* keyToken = (unsigned char*)RSA_get_ex_data(rsa, hndidx);
	long length = SSL_SIG_LEN;
	long keyLength ;
	X509_SIG sig;
	ASN1_TYPE parameter;
	X509_ALGOR algorithm;
	ASN1_OCTET_STRING digest;

	keyTokenLength = *(long*)keyToken;
	keyToken+=sizeof(long);

	if (type == NID_md5 || type == NID_sha1)
		{
		sig.algor = &algorithm;
		algorithm.algorithm = OBJ_nid2obj(type);

		if (!algorithm.algorithm)
			{
			Copkererr(COPKER_F_CCA_RSA_SIGN,
				COPKER_R_UNKNOWN_ALGORITHM_TYPE);
			return 0;
			}

		if (!algorithm.algorithm->length)
			{
			Copkererr(COPKER_F_CCA_RSA_SIGN,
				COPKER_R_ASN1_OID_UNKNOWN_FOR_MD);
			return 0;
			}

		parameter.type = V_ASN1_NULL;
		parameter.value.ptr = NULL;
		algorithm.parameter = &parameter;

		sig.digest = &digest;
		sig.digest->data = (unsigned char*)m;
		sig.digest->length = m_len;

		length = i2d_X509_SIG(&sig, NULL);
		}

	keyLength = RSA_size(rsa);

	if (length - RSA_PKCS1_PADDING > keyLength)
		{
		Copkererr(COPKER_F_CCA_RSA_SIGN,
			COPKER_R_SIZE_TOO_LARGE_OR_TOO_SMALL);
		return 0;
		}

	switch (type)
		{
		case NID_md5_sha1 :
			if (m_len != SSL_SIG_LEN)
				{
				Copkererr(COPKER_F_CCA_RSA_SIGN,
				COPKER_R_SIZE_TOO_LARGE_OR_TOO_SMALL);
				return 0;
				}
			hashBuffer = (unsigned char*)m;
			length = m_len;
			break;
		case NID_md5 :
			{
			unsigned char *ptr;
			ptr = hashBuffer = OPENSSL_malloc(
					(unsigned int)keyLength+1);
			if (!hashBuffer)
				{
				Copkererr(COPKER_F_CCA_RSA_SIGN,
						ERR_R_MALLOC_FAILURE);
				return 0;
				}
			i2d_X509_SIG(&sig, &ptr);
			}
			break;
		case NID_sha1 :
			{
			unsigned char *ptr;
			ptr = hashBuffer = OPENSSL_malloc(
					(unsigned int)keyLength+1);
			if (!hashBuffer)
				{
				Copkererr(COPKER_F_CCA_RSA_SIGN,
						ERR_R_MALLOC_FAILURE);
				return 0;
				}
			i2d_X509_SIG(&sig, &ptr);
			}
			break;
		default:
			return 0;
		}

	digitalSignatureGenerate(&returnCode, &reasonCode, &exitDataLength,
		exitData, &ruleArrayLength, ruleArray, &keyTokenLength,
		keyToken, &length, hashBuffer, &outputLength, &outputBitLength,
		sigret);

	if (type == NID_sha1 || type == NID_md5)
		{
		OPENSSL_cleanse(hashBuffer, keyLength+1);
		OPENSSL_free(hashBuffer);
		}

	*siglen = outputLength;

	return ((returnCode || reasonCode) ? 0 : 1);
*/
	return 0;
	}

static int getModulusAndExponent(const unsigned char*token, long *exponentLength,
		unsigned char *exponent, long *modulusLength, long *modulusFieldLength,
		unsigned char *modulus)
	{
/*
	unsigned long len;

	if (*token++ != (char)0x1E) // internal PKA token?
		return 0;

	if (*token++) // token version must be zero 
		return 0;

	len = *token++;
	len = len << 8;
	len |= (unsigned char)*token++;

	token += 4; // skip reserved bytes 

	if (*token++ == (char)0x04)
		{
		if (*token++) // token version must be zero 
			return 0;

		len = *token++;
		len = len << 8;
		len |= (unsigned char)*token++;

		token+=2; // skip reserved section 

		len = *token++;
		len = len << 8;
		len |= (unsigned char)*token++;

		*exponentLength = len;

		len = *token++;
		len = len << 8;
		len |= (unsigned char)*token++;

		*modulusLength = len;

		len = *token++;
		len = len << 8;
		len |= (unsigned char)*token++;

		*modulusFieldLength = len;

		memcpy(exponent, token, *exponentLength);
		token+= *exponentLength;

		memcpy(modulus, token, *modulusFieldLength);
		return 1;
		}
*/
	return 0;
	}

#endif /* OPENSSL_NO_RSA */



#ifndef OPENSSL_NO_RSA
static void cca_ex_free(void *obj, void *item, CRYPTO_EX_DATA *ad, int idx,
		long argl, void *argp)
	{
	if (item)
		OPENSSL_free(item);
	}
#endif

/* Goo to handle building as a dynamic engine */
#ifndef OPENSSL_NO_DYNAMIC_ENGINE 
static int bind_fn(ENGINE *e, const char *id)
	{
	if(id && (strcmp(id, engine_4758_cca_id) != 0) &&
			(strcmp(id, engine_4758_cca_id_alt) != 0))
		return 0;
	if(!bind_helper(e))
		return 0;
	return 1;
	}       
IMPLEMENT_DYNAMIC_CHECK_FN()
IMPLEMENT_DYNAMIC_BIND_FN(bind_fn)
#endif /* OPENSSL_NO_DYNAMIC_ENGINE */

