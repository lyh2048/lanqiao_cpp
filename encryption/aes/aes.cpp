#ifndef _CRYPTO_UTIL_H_
#define _CRYPTO_UTIL_H_

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string>

#include <aes.h>
#include <hex.h>
#include <files.h>
#include <default.h>
#include <filters.h>
#include <osrng.h>


using namespace CryptoPP;


enum AESKeyLength {
	AES_KEY_LENGTH_16 = 16,
	AES_KEY_LENGTH_24 = 24,
	AES_KEY_LENGTH_32 = 32
};


class CCryptoUtil {
public:
	static int encryptByAES(const std::string &inData, const std::string &strKey, std::string &outData, std::string &errMsg);
	static int decryptByAES(const std::string &inData, const std::string &strKey, std::string &outData, std::string &errMsg);
};


int CCryptoUtil::encryptByAES(const std::string& inData, const std::string& strKey, std::string& outData, std::string& errMsg)
{
	outData = "";
	errMsg = "";

	// 判断待加密的字符串或者密钥是否为空
	if (inData.empty() || strKey.empty())
	{
		errMsg = "indata or key is empty!";
		return -1;
	}

	unsigned int iKeyLen = strKey.length();

	// 判断密钥长度是否符合要求
	if (iKeyLen != AES_KEY_LENGTH_16 && iKeyLen != AES_KEY_LENGTH_24 && iKeyLen != AES_KEY_LENGTH_32)
	{
		errMsg = "aes key invalid!";
		return -2;
	}

	byte iv[AES::BLOCKSIZE];
	int iResult = 0;
	try
	{
		// CBC 模式加密
		CBC_Mode<AES>::Encryption e;
		e.SetKeyWithIV((byte*) strKey.c_str(), iKeyLen, iv);
		StringSource ss(inData, true, new StreamTransformationFilter(e, new StringSink(outData)));
	}
	catch (const CryptoPP::Exception& e)
	{
		errMsg = "Encryptor throw exception!";
		iResult = -3;
	}
	return iResult;
}


int CCryptoUtil::decryptByAES(const std::string& inData, const std::string& strKey, std::string& outData, std::string& errMsg)
{
	outData = "";
	errMsg = "";

	// 判断待解密的字符串或者密钥是否为空
	if (inData.empty() || strKey.empty())
	{
		errMsg = "indata or key is empty!";
		return -1;
	}

	unsigned int iKeyLen = strKey.length();

	// 判断密钥长度是否符合要求
	if (iKeyLen != AES_KEY_LENGTH_16 && iKeyLen != AES_KEY_LENGTH_24 && iKeyLen != AES_KEY_LENGTH_32)
	{
		errMsg = "aes key invalid!";
		return -2;
	}

	byte iv[AES::BLOCKSIZE];
	int iResult = 0;
	try
	{
		// CBC 模式解密
		CBC_Mode<AES>::Decryption d;
		d.SetKeyWithIV((byte*)strKey.c_str(), iKeyLen, iv);
		StringSource ss(inData, true, new StreamTransformationFilter(d, new StringSink(outData)));
	}
	catch (const CryptoPP::Exception& e)
	{
		errMsg = "Decryptor throw exception!";
		iResult = -3;
	}
	return iResult;
}


int main()
{
	// 待加密的字符串
	std::string strCipher;
	// 用来加解密的密钥
	std::string strKey;

	std::cout << "Please enter a string" << std::endl;
	std::cin >> strCipher;
	std::cout << "Please enter a key, yout just can write 16, 24 or 32 words as a key" << std::endl;
	std::cin >> strKey;

	std::string strResult;
	std::string strErrMsg;
	int iResult = CCryptoUtil::encryptByAES(strCipher, strKey, strResult, strErrMsg);
	if (iResult)
	{
		std::cout << "CCryptoUtil::encryptByAES failed, errMsg: " << strErrMsg << std::endl;
		return -1;
	}

	std::string strPlainText;
	iResult = CCryptoUtil::decryptByAES(strResult, strKey, strPlainText, strErrMsg);
	if (iResult)
	{
		std::cout << "CCryptoUtil::decryptByAES failed, errMsg: " << strErrMsg << std::endl;
		return -2;
	}
	std::cout << "PlainText: " << strPlainText << std::endl;
	return 0;
}


#endif