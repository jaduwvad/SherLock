/*
 * name :
 *   FWAES.h
 *
 * description :
 *   Ciphering using AES algorithm.
 *
 * history :
 *   2005.10.13 created by firewolf.
 *
 */

/* Copyright (C) Samsung SDS 2005. All Rights Reserved. */


// FWAES.h: interface for the CFWAES class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FWAES_H__55D5E831_2799_46B5_8712_C27DC76440A4__INCLUDED_)
#define AFX_FWAES_H__55D5E831_2799_46B5_8712_C27DC76440A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFWAES  
{
public:
	CFWAES();
	virtual ~CFWAES();

public:
	void SetState(uchar8_t state[][4], const uchar8_t *pText);
	void AddRoundKey(uchar8_t state[][4], const uchar8_t key[][4]);
	void KeySchedule(const uint32_t nRound, uchar8_t key[][4]);
	void InvKeySchedule(const uint32_t nRound, const uchar8_t inkey[][4], uchar8_t outkey[][4]);
	void SubBytes(uchar8_t state[][4]);
	void ShiftRows(uchar8_t state[][4]);
	void MixColumns(uchar8_t state[][4]);
	void InvSubBytes(uchar8_t state[][4]);
	void InvShiftRows(uchar8_t state[][4]);
	void InvMixColumns(uchar8_t state[][4]);
	uchar8_t mul(uchar8_t a, uchar8_t b);
	void Encode(const uchar8_t *pKey, const uchar8_t *pPlainText, uchar8_t *pCipherText, const uint32_t nRound=10);
	void Decode(const uchar8_t *pKey, const uchar8_t *pCipherText, uchar8_t *pPlainText, const uint32_t nRound=10);

	void Cipher(mode_t mode, const uchar8_t *pKey, const uint32_t nPlainTextLen, const uchar8_t *pPlainText,
		uint32_t *nCipherTextLen, uchar8_t *pCipherText, const uint32_t nRound=10);
	void Decipher(mode_t mode, const uchar8_t *pKey, const uint32_t nCipherTextLen, const uchar8_t *pCipherText,
		uint32_t *nPlainTextLen, uchar8_t *pPlainText, const uint32_t nRound=10);
};

#endif // !defined(AFX_FWAES_H__55D5E831_2799_46B5_8712_C27DC76440A4__INCLUDED_)
