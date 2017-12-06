// FWAES.cpp: implementation of the CFWAES class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "stdafx.h"
#include "Crypt.h"
#include "FWAES.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static uchar8_t S_box[16][16] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static uchar8_t Inv_S_box[16][16] = {
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

static uchar8_t Rcon[4][10] = {
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFWAES::CFWAES()
{

}

CFWAES::~CFWAES()
{

}

void CFWAES::SetState(uchar8_t state[][4], const uchar8_t *pText)
{
	for ( uint32_t r=0; r<4; r++ ) {
		for ( uint32_t c=0; c<4; c++ ) {
			state[r][c] = pText[r+4*c];
		}
	}
}

inline
void CFWAES::AddRoundKey(uchar8_t state[][4], const uchar8_t key[][4])
{
	for ( uint32_t c=0; c<4; c++ ) {
		for ( uint32_t r=0; r<4; r++ ) {
			state[r][c] = state[r][c] ^ key[r][c];
		}
	}
}

void CFWAES::KeySchedule(const uint32_t nRound, uchar8_t key[][4])
{
	uchar8_t roundkey[4][4];
	uchar8_t word[4];
	int r;
	// RotWord
	for ( uint32_t r=0; r<3; r++ ) {
		word[r] = key[r+1][3];
	}

	word[3] = key[0][3];

	// SubBytes
	for ( r=0; r<4; r++ ) {
		word[r] = S_box[word[r]>>4][word[r]&0x0f];
	}

	// Wi = Wi-4 ^ sub ^ Rcon
	// Wi = Wi-4 ^ Wi-1
	for ( r=0; r<4; r++ ) {
		roundkey[r][0] = key[r][0] ^ word[r] ^ Rcon[r][nRound-1]; // nRound : from 1 to 10
		roundkey[r][1] = key[r][1] ^ roundkey[r][0];
		roundkey[r][2] = key[r][2] ^ roundkey[r][1];
		roundkey[r][3] = key[r][3] ^ roundkey[r][2];
	}

	memcpy(key, roundkey, 16);
}

void CFWAES::SubBytes(uchar8_t state[][4])
{
	for ( uint32_t r=0; r<4; r++ ) {
		for ( uint32_t c=0; c<4; c++ ) {
			state[r][c] = S_box[state[r][c]>>4][state[r][c]&0x0f];
		}
	}
}

void CFWAES::ShiftRows(uchar8_t state[][4])
{
	// row 0 - nothing to do
	// row 1 - shift 1
	// row 2 - shift 2
	// row 3 - shift 3

	uchar8_t row[4];

	for ( uint32_t r=1; r<=3; r++ ) {
		for ( uint32_t c=0; c<4; c++ ) {
			row[c] = state[r][(c+r)%4];
		}
		memcpy(state[r], row, 4);
	}
}

void CFWAES::MixColumns(uchar8_t state[][4])
{
	uchar8_t st[4][4];

	for ( uint32_t c=0; c<4; c++ ) {
		
		st[0][c] = mul(state[0][c], 0x02) ^
				   mul(state[1][c], 0x03) ^
				   mul(state[2][c], 0x01) ^
				   mul(state[3][c], 0x01);

		st[1][c] = mul(state[0][c], 0x01) ^
				   mul(state[1][c], 0x02) ^
				   mul(state[2][c], 0x03) ^
				   mul(state[3][c], 0x01);

		st[2][c] = mul(state[0][c], 0x01) ^
				   mul(state[1][c], 0x01) ^
				   mul(state[2][c], 0x02) ^
				   mul(state[3][c], 0x03);

		st[3][c] = mul(state[0][c], 0x03) ^
				   mul(state[1][c], 0x01) ^
				   mul(state[2][c], 0x01) ^
				   mul(state[3][c], 0x02);
	}

	memcpy(state, st, 16);
}

inline
uchar8_t CFWAES::mul(uchar8_t a, uchar8_t b)
{
	uchar8_t p = 0;
    uchar8_t counter;
    uint32_t hi_bit_set;
	
    for ( counter=0; counter<8; counter++ ) {

		if ( (b & 1) == 1 )
			p ^= a;

		hi_bit_set = (a & 0x80);
		a <<= 1;

		if ( hi_bit_set == 0x80 )
			a ^= 0x1b; /* x^8 + x^4 + x^3 + x + 1 */

		b >>= 1;
    }

	return p;
}

void CFWAES::Encode(const uchar8_t *pKey, const uchar8_t *pPlainText, uchar8_t *pCipherText, const uint32_t nRound/*=10*/)
{
	uchar8_t input_state[4][4];
	uchar8_t key_state[4][4];

	SetState(input_state, pPlainText);
	SetState(key_state, pKey);

	AddRoundKey(input_state, key_state);

	for ( uint32_t i=1; i<=nRound; i++ ) {
		KeySchedule(i, key_state);
		SubBytes(input_state);
		ShiftRows(input_state);
		if ( i != nRound )
			MixColumns(input_state);
		AddRoundKey(input_state, key_state);
	}

	for ( uint32_t r=0; r<4; r++ ) {
		for ( uint32_t c=0; c<4; c++ ) {
			pCipherText[r*4+c] = input_state[c][r];
		}
	}
}

void CFWAES::InvSubBytes(uchar8_t state[][4])
{
	for ( uint32_t r=0; r<4; r++ ) {
		for ( uint32_t c=0; c<4; c++ ) {
			state[r][c] = Inv_S_box[state[r][c]>>4][state[r][c]&0x0f];
		}
	}
}

void CFWAES::InvShiftRows(uchar8_t state[][4])
{
	uchar8_t row[4];

	for ( uint32_t r=1; r<=3; r++ ) {
		for ( uint32_t c=0; c<4; c++ ) {
			row[c] = state[r][(c+4-r)%4];
		}
		memcpy(state[r], row, 4);
	}
}

void CFWAES::InvMixColumns(uchar8_t state[][4])
{
	uchar8_t st[4][4];

	for ( uint32_t c=0; c<4; c++ ) {
		
		st[0][c] = mul(state[0][c], 0x0e) ^
				   mul(state[1][c], 0x0b) ^
				   mul(state[2][c], 0x0d) ^
				   mul(state[3][c], 0x09);

		st[1][c] = mul(state[0][c], 0x09) ^
				   mul(state[1][c], 0x0e) ^
				   mul(state[2][c], 0x0b) ^
				   mul(state[3][c], 0x0d);

		st[2][c] = mul(state[0][c], 0x0d) ^
				   mul(state[1][c], 0x09) ^
				   mul(state[2][c], 0x0e) ^
				   mul(state[3][c], 0x0b);

		st[3][c] = mul(state[0][c], 0x0b) ^
				   mul(state[1][c], 0x0d) ^
				   mul(state[2][c], 0x09) ^
				   mul(state[3][c], 0x0e);
	}

	memcpy(state, st, 16);
}

void CFWAES::InvKeySchedule(const uint32_t nRound, const uchar8_t inkey[][4], uchar8_t outkey[][4])
{
	uchar8_t roundkey[4][4];
	uchar8_t word[4];
	int r;

	// RotWord
	for ( uint32_t r=0; r<3; r++ ) {
		word[r] = inkey[r+1][3];
	}

	word[3] = inkey[0][3];

	// SubBytes
	for ( r=0; r<4; r++ ) {
		word[r] = S_box[word[r]>>4][word[r]&0x0f];
	}

	// Wi = Wi-4 ^ sub ^ Rcon
	// Wi = Wi-4 ^ Wi-1
	for ( r=0; r<4; r++ ) {
		roundkey[r][0] = inkey[r][0] ^ word[r] ^ Rcon[r][nRound-1]; // nRound : from 1 to 10
		roundkey[r][1] = inkey[r][1] ^ roundkey[r][0];
		roundkey[r][2] = inkey[r][2] ^ roundkey[r][1];
		roundkey[r][3] = inkey[r][3] ^ roundkey[r][2];
	}

	memcpy(outkey, roundkey, 16);
}

void CFWAES::Decode(const uchar8_t *pKey, const uchar8_t *pCipherText, uchar8_t *pPlainText, const uint32_t nRound/*=10*/)
{
	uchar8_t input_state[4][4];
	uchar8_t key_state[11][4][4];
	int i;

	SetState(input_state, pCipherText);
	SetState(key_state[0], pKey);

	for ( int i=1; i<=(int)nRound; i++ ) {
		InvKeySchedule(i, key_state[i-1], key_state[i]);
	}

	AddRoundKey(input_state, key_state[nRound]);

	for ( i=nRound-1; i>=0; i-- ) {
		InvShiftRows(input_state);
		InvSubBytes(input_state);
		AddRoundKey(input_state, key_state[i]);
		if ( i != 0 )
			InvMixColumns(input_state);
	}

	for ( uint32_t r=0; r<4; r++ ) {
		for ( uint32_t c=0; c<4; c++ ) {
			pPlainText[r*4+c] = input_state[c][r];
		}
	}
}

void CFWAES::Cipher(mode_t mode, const uchar8_t *pKey, const uint32_t nPlainTextLen, const uchar8_t *pPlainText,
		uint32_t *nCipherTextLen, uchar8_t *pCipherText, const uint32_t nRound/*=10*/)
{
	uint32_t nQuotient = nPlainTextLen/16;
	uint32_t nRemainder = nPlainTextLen%16;
	uint32_t nCnt = nQuotient;

	uint32_t x, y;
	
	if ( nRemainder ) {
		nCnt++;
	}

	uchar8_t *pPlainText2 = new uchar8_t[nCnt*16+1];
	memset(pPlainText2, NULL, nCnt*16+1);

	memcpy(pPlainText2, pPlainText, nPlainTextLen);

	uchar8_t IV[4][4];
	memset(IV, NULL, 16);

	for ( uint32_t i=0; i<nCnt; i++ ) {

		if ( mode == CBC ) {
			for ( x=0; x<4; x++ )
				for ( y=0; y<4; y++ )
					(pPlainText2+(i*16))[x*4+y] ^= IV[y][x];
		}

		Encode(pKey, pPlainText2+(i*16), pCipherText+(i*16), nRound);

		if ( mode == CBC ) {
			for ( x=0; x<4; x++ )
				for ( y=0; y<4; y++ )
					IV[y][x] = (pCipherText+(i*16))[x*4+y];
		}
	}

	*nCipherTextLen = nCnt*16;

	delete [] pPlainText2;
}

void CFWAES::Decipher(mode_t mode, const uchar8_t *pKey, const uint32_t nCipherTextLen, const uchar8_t *pCipherText,
		uint32_t *nPlainTextLen, uchar8_t *pPlainText, const uint32_t nRound/*=10*/)
{
	uint32_t nQuotient = nCipherTextLen/16;
	uint32_t nRemainder = nCipherTextLen%16;
	uint32_t nCnt = nQuotient;

	uint32_t x, y;

	if ( nRemainder ) {
		nCnt++;
	}

	uchar8_t IV[4][4];
	memset(IV, NULL, 16);

	for ( uint32_t i=0; i<nCnt; i++ ) {

		Decode(pKey, pCipherText+(i*16), pPlainText+(i*16), nRound);

		if ( mode == CBC ) {
			for ( x=0; x<4; x++ )
				for ( y=0; y<4; y++ ) {
					(pPlainText+(i*16))[x*4+y] ^= IV[y][x];
					IV[y][x] = (pCipherText+(i*16))[x*4+y];
				}
		}
	}

	*nPlainTextLen = nCnt*16;
/*
	for ( i=nCnt*16-1; i>=(nCnt-1)*16; i-- ) {
		if ( pPlainText[i] == NULL )
			(*nPlainTextLen)--;
		else
			break;

		if ( i == 0 )
			break;
	}
*/
}