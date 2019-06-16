#ifndef _MACARON_BASE64_H_
#define _MACARON_BASE64_H_

/**
 * The MIT License (MIT)
 * Copyright (c) 2016 tomykaira
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string>

namespace macaron {

	class Base64 {
	public:

		static std::string Encode(const std::string data) {
			static constexpr char sEncodingTable[] = {
			  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			  'w', 'x', 'y', 'z', '0', '1', '2', '3',
			  '4', '5', '6', '7', '8', '9', '+', '/'
			};

			size_t in_len = data.size();
			size_t out_len = 4 * ((in_len + 2) / 3);
			std::string ret(out_len, '\0');
			size_t i;
			char* p = const_cast<char*>(ret.c_str());

			for (i = 0; i < in_len - 2; i += 3) {
				*p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
				*p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int)(data[i + 1] & 0xF0) >> 4)];
				*p++ = sEncodingTable[((data[i + 1] & 0xF) << 2) | ((int)(data[i + 2] & 0xC0) >> 6)];
				*p++ = sEncodingTable[data[i + 2] & 0x3F];
			}
			if (i < in_len) {
				*p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
				if (i == (in_len - 1)) {
					*p++ = sEncodingTable[((data[i] & 0x3) << 4)];
					*p++ = '=';
				}
				else {
					*p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int)(data[i + 1] & 0xF0) >> 4)];
					*p++ = sEncodingTable[((data[i + 1] & 0xF) << 2)];
				}
				*p++ = '=';
			}

			return ret;
		}

		static std::string Decode(const std::string & input, std::string & out) {
			static constexpr unsigned char kDecodingTable[] = {
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
			  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
			  64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
			  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
			  64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
			  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
			};

			size_t in_len = input.size();
			if (in_len % 4 != 0) return "Input data size is not a multiple of 4";

			size_t out_len = in_len / 4 * 3;
			if (input[in_len - 1] == '=') out_len--;
			if (input[in_len - 2] == '=') out_len--;

			out.resize(out_len);

			for (size_t i = 0, j = 0; i < in_len;) {
				uint32_t a = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
				uint32_t b = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
				uint32_t c = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
				uint32_t d = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];

				uint32_t triple = (a << 3 * 6) + (b << 2 * 6) + (c << 1 * 6) + (d << 0 * 6);

				if (j < out_len) out[j++] = (triple >> 2 * 8) & 0xFF;
				if (j < out_len) out[j++] = (triple >> 1 * 8) & 0xFF;
				if (j < out_len) out[j++] = (triple >> 0 * 8) & 0xFF;
			}

			return "";
		}

	};

}

#endif /* _MACARON_BASE64_H_ */


#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define AES_RPOL    0x011b // reduction polynomial (x^8 + x^4 + x^3 + x + 1)
#define AES_GEN     0x03   // gf(2^8) generator  (x + 1)
#define AES_SBOX_CC 0x63   // S-Box C constant

#define KEY_128 (128/8) //strong
#define KEY_192 (192/8) //stronger
#define KEY_256 (256/8) //heavy duty

#define aes_mul(a, b) ((a)&&(b)?g_aes_ilogt[(g_aes_logt[(a)]+g_aes_logt[(b)])%0xff]:0)
#define aes_inv(a)  ((a)?g_aes_ilogt[0xff-g_aes_logt[(a)]]:0)
#define min(a, b) ((a) < (b) ? (a) : (b))

#define BLOCK_SIZE (128/8) //size of block

typedef struct {
	unsigned char state[4][4];
	int kcol;
	size_t rounds;
	unsigned long keysched[0];
} aes_ctx_t;

typedef unsigned long long u64;
typedef unsigned char uchar;

class virtualAES
{
public:
	static void initialize();
	/*ECB Block cipher mode (max 16chars)*/
	static void encrypt(aes_ctx_t *ctx, unsigned char input[16], unsigned char output[16]);
	static void decrypt(aes_ctx_t *ctx, unsigned char input[16], unsigned char output[16]);

	/*CTR Block cipher mode (must be dividible by 16)*/
	static void encrypt_ctr(aes_ctx_t *ctx, uchar *input, uchar *output, size_t len, u64 nonce);
	static void decrypt_ctr(aes_ctx_t *ctx, uchar *input, uchar *output, size_t len, u64 nonce);

	static aes_ctx_t *allocatectx(unsigned char *key, size_t keyLen);
	static void rand_nonce(u64 *nonce);

	static void strtohex(unsigned char const* pucCharStr, char* pszHexStr, int iSize);
	static void hextostr(char const* pszHexStr, unsigned char* pucCharStr, int iSize);

private:
	static unsigned char g_aes_logt[256], g_aes_ilogt[256];
	static unsigned char g_aes_sbox[256], g_aes_isbox[256];

	static inline unsigned long aes_subword(unsigned long w);
	static inline unsigned long aes_rotword(unsigned long w);

	static void aes_keyexpansion(aes_ctx_t *ctx);
	static inline unsigned char aes_mul_manual(unsigned char a, unsigned char b); // use aes_mul instead

	static void aes_subbytes(aes_ctx_t *ctx);
	static void aes_shiftrows(aes_ctx_t *ctx);
	static void aes_mixcolumns(aes_ctx_t *ctx);
	static void aes_addroundkey(aes_ctx_t *ctx, int round);
	static void aes_invsubbytes(aes_ctx_t *ctx);
	static void aes_invshiftrows(aes_ctx_t *ctx);
	static void aes_invmixcolumns(aes_ctx_t *ctx);
};

