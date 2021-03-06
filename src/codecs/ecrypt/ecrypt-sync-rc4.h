/* ecrypt-sync.h */

/* 
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 * 
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef RC4_SYNC
#define RC4_SYNC

#include "ecrypt-portable.h"

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/* 
 * The name of your cipher.
 */
#define RC4_NAME "RC4"                 /* [edit] */ 
#define RC4_PROFILE "bench"

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; RC4_KEYSIZE(i) <= RC4_MAXKEYSIZE; ++i)
 *   {
 *     keysize = RC4_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define RC4_MAXKEYSIZE 256                 /* [edit] */
#define RC4_KEYSIZE(i) (40 + (i)*8)        /* [edit] */

#define RC4_MAXIVSIZE 0                    /* [edit] */
#define RC4_IVSIZE(i) (0 + (i)*32)         /* [edit] */

/* ------------------------------------------------------------------------- */

/* Data structures */

/* 
 * RC4_ctx is the structure containing the representation of the
 * internal state of your cipher. 
 */

typedef struct
{
  u32 keylen;

  u32 i;
  u32 j;

  u32 s[256];
  u8 key[32];

} RC4_ctx;

/* ------------------------------------------------------------------------- */

/* Mandatory functions */

/*
 * Key and message independent initialization. This function will be
 * called once when the program starts (e.g., to build expanded S-box
 * tables).
 */
void RC4_init(void);

/*
 * Key setup. It is the user's responsibility to select the values of
 * keysize and ivsize from the set of supported values specified
 * above.
 */
void RC4_keysetup(
  RC4_ctx* ctx, 
  const u8* key, 
  u32 keysize,                /* Key size in bits. */ 
  u32 ivsize);                /* IV size in bits. */ 

/*
 * IV setup. After having called RC4_keysetup(), the user is
 * allowed to call RC4_ivsetup() different times in order to
 * encrypt/decrypt different messages with the same key but different
 * IV's.
 */
void RC4_ivsetup(
  RC4_ctx* ctx, 
  const u8* iv);

/*
 * Encryption/decryption of arbitrary length messages.
 *
 * For efficiency reasons, the API provides two types of
 * encrypt/decrypt functions. The RC4_encrypt_bytes() function
 * (declared here) encrypts byte strings of arbitrary length, while
 * the RC4_encrypt_blocks() function (defined later) only accepts
 * lengths which are multiples of RC4_BLOCKLENGTH.
 * 
 * The user is allowed to make multiple calls to
 * RC4_encrypt_blocks() to incrementally encrypt a long message,
 * but he is NOT allowed to make additional encryption calls once he
 * has called RC4_encrypt_bytes() (unless he starts a new message
 * of course). For example, this sequence of calls is acceptable:
 *
 * RC4_keysetup();
 *
 * RC4_ivsetup();
 * RC4_encrypt_blocks();
 * RC4_encrypt_blocks();
 * RC4_encrypt_bytes();
 *
 * RC4_ivsetup();
 * RC4_encrypt_blocks();
 * RC4_encrypt_blocks();
 *
 * RC4_ivsetup();
 * RC4_encrypt_bytes();
 * 
 * The following sequence is not:
 *
 * RC4_keysetup();
 * RC4_ivsetup();
 * RC4_encrypt_blocks();
 * RC4_encrypt_bytes();
 * RC4_encrypt_blocks();
 */

/*
 * By default RC4_encrypt_bytes() and RC4_decrypt_bytes() are
 * defined as macros which redirect the call to a single function
 * RC4_process_bytes(). If you want to provide separate encryption
 * and decryption functions, please undef
 * RC4_HAS_SINGLE_BYTE_FUNCTION.
 */
#define RC4_HAS_SINGLE_BYTE_FUNCTION       /* [edit] */
#ifdef RC4_HAS_SINGLE_BYTE_FUNCTION

#define RC4_encrypt_bytes(ctx, plaintext, ciphertext, msglen)   \
  RC4_process_bytes(0, ctx, plaintext, ciphertext, msglen)

#define RC4_decrypt_bytes(ctx, ciphertext, plaintext, msglen)   \
  RC4_process_bytes(1, ctx, ciphertext, plaintext, msglen)

void RC4_process_bytes(
  int action,                 /* 0 = encrypt; 1 = decrypt; */
  RC4_ctx* ctx, 
  const u8* input, 
  u8* output, 
  u32 msglen);                /* Message length in bytes. */ 

#else

void RC4_encrypt_bytes(
  RC4_ctx* ctx, 
  const u8* plaintext, 
  u8* ciphertext, 
  u32 msglen);                /* Message length in bytes. */ 

void RC4_decrypt_bytes(
  RC4_ctx* ctx, 
  const u8* ciphertext, 
  u8* plaintext, 
  u32 msglen);                /* Message length in bytes. */ 

#endif

/* ------------------------------------------------------------------------- */

/* Optional features */

/* 
 * For testing purposes it can sometimes be useful to have a function
 * which immediately generates keystream without having to provide it
 * with a zero plaintext. If your cipher cannot provide this function
 * (e.g., because it is not strictly a synchronous cipher), please
 * reset the RC4_GENERATES_KEYSTREAM flag.
 */

#define RC4_GENERATES_KEYSTREAM
#ifdef RC4_GENERATES_KEYSTREAM

void RC4_keystream_bytes(
  RC4_ctx* ctx,
  u8* keystream,
  u32 length);                /* Length of keystream in bytes. */

#endif

/* ------------------------------------------------------------------------- */

/* Optional optimizations */

/* 
 * By default, the functions in this section are implemented using
 * calls to functions declared above. However, you might want to
 * implement them differently for performance reasons.
 */

/*
 * All-in-one encryption/decryption of (short) packets.
 *
 * The default definitions of these functions can be found in
 * "ecrypt-sync.c". If you want to implement them differently, please
 * undef the RC4_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define RC4_USES_DEFAULT_ALL_IN_ONE        /* [edit] */

/*
 * Undef RC4_HAS_SINGLE_PACKET_FUNCTION if you want to provide
 * separate packet encryption and decryption functions.
 */
#define RC4_HAS_SINGLE_PACKET_FUNCTION     /* [edit] */
#ifdef RC4_HAS_SINGLE_PACKET_FUNCTION

#define RC4_encrypt_packet(                                        \
    ctx, iv, plaintext, ciphertext, mglen)                            \
  RC4_process_packet(0,                                            \
    ctx, iv, plaintext, ciphertext, mglen)

#define RC4_decrypt_packet(                                        \
    ctx, iv, ciphertext, plaintext, mglen)                            \
  RC4_process_packet(1,                                            \
    ctx, iv, ciphertext, plaintext, mglen)

void RC4_process_packet(
  int action,                 /* 0 = encrypt; 1 = decrypt; */
  RC4_ctx* ctx, 
  const u8* iv,
  const u8* input, 
  u8* output, 
  u32 msglen);

#else

void RC4_encrypt_packet(
  RC4_ctx* ctx, 
  const u8* iv,
  const u8* plaintext, 
  u8* ciphertext, 
  u32 msglen);

void RC4_decrypt_packet(
  RC4_ctx* ctx, 
  const u8* iv,
  const u8* ciphertext, 
  u8* plaintext, 
  u32 msglen);

#endif

/*
 * Encryption/decryption of blocks.
 * 
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * RC4_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define RC4_BLOCKLENGTH 16                 /* [edit] */

#define RC4_USES_DEFAULT_BLOCK_MACROS      /* [edit] */
#ifdef RC4_USES_DEFAULT_BLOCK_MACROS

#define RC4_encrypt_blocks(ctx, plaintext, ciphertext, blocks)  \
  RC4_encrypt_bytes(ctx, plaintext, ciphertext,                 \
    (blocks) * RC4_BLOCKLENGTH)

#define RC4_decrypt_blocks(ctx, ciphertext, plaintext, blocks)  \
  RC4_decrypt_bytes(ctx, ciphertext, plaintext,                 \
    (blocks) * RC4_BLOCKLENGTH)

#ifdef RC4_GENERATES_KEYSTREAM

#define RC4_keystream_blocks(ctx, keystream, blocks)            \
  RC4_keystream_bytes(ctx, keystream,                           \
    (blocks) * RC4_BLOCKLENGTH)

#endif

#else

/*
 * Undef RC4_HAS_SINGLE_BLOCK_FUNCTION if you want to provide
 * separate block encryption and decryption functions.
 */
#define RC4_HAS_SINGLE_BLOCK_FUNCTION      /* [edit] */
#ifdef RC4_HAS_SINGLE_BLOCK_FUNCTION

#define RC4_encrypt_blocks(ctx, plaintext, ciphertext, blocks)     \
  RC4_process_blocks(0, ctx, plaintext, ciphertext, blocks)

#define RC4_decrypt_blocks(ctx, ciphertext, plaintext, blocks)     \
  RC4_process_blocks(1, ctx, ciphertext, plaintext, blocks)

void RC4_process_blocks(
  int action,                 /* 0 = encrypt; 1 = decrypt; */
  RC4_ctx* ctx, 
  const u8* input, 
  u8* output, 
  u32 blocks);                /* Message length in blocks. */

#else

void RC4_encrypt_blocks(
  RC4_ctx* ctx, 
  const u8* plaintext, 
  u8* ciphertext, 
  u32 blocks);                /* Message length in blocks. */ 

void RC4_decrypt_blocks(
  RC4_ctx* ctx, 
  const u8* ciphertext, 
  u8* plaintext, 
  u32 blocks);                /* Message length in blocks. */ 

#endif

#ifdef RC4_GENERATES_KEYSTREAM

void RC4_keystream_blocks(
  RC4_ctx* ctx,
  u8* keystream,
  u32 blocks);                /* Keystream length in blocks. */ 

#endif

#endif

/*
 * If your cipher can be implemented in different ways, you can use
 * the RC4_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DRC4_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (RC4_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same RC4_BLOCKLENGTH, etc.). 
 */
#define RC4_MAXVARIANT 2                   /* [edit] */

#ifndef RC4_VARIANT
#define RC4_VARIANT 1
#endif

#if (RC4_VARIANT > RC4_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif
