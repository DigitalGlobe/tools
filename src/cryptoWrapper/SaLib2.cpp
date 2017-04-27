// SaLib2.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <string>
#include "modes.h"
#include "aes.h"
#include "base64.h"
#include "filters.h"


using namespace CryptoPP;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

// This method encrypts the input string using the specified key and
// initialization vector. It uses AES 256-bit encryption. The return
// value is the encrypted string converted to base64 for easy transmission
// back to .Net.
extern "C" __declspec(dllexport) void APIENTRY SetStringWithBlock(LPTSTR key, int keyLen,
                                                                  LPTSTR iniVec, int iniVecLen,
                                                                  LPTSTR input, int inputLen,
                                                                  LPTSTR output, int outputLen,
                                                                  int blockLen)
{
  if (blockLen != 16 && blockLen != 32)
  {
    lstrcpynA((LPSTR)output, "<Error Block Length>", sizeof("<Error Block Length>"));
  }

  std::string localKey;
  std::string localIv;
  // First decode from base64 key and IV passed in. Decoded results stored in
  // localKey and localIv.
  StringSource((const byte*)key, keyLen, true, new Base64Decoder(new StringSink(localKey)));
  StringSource((const byte*)iniVec, iniVecLen, true, new Base64Decoder(new StringSink(localIv)));

  std::string plaintext((LPSTR)input, inputLen);
  
  std::string ciphertext;
  // We're using AES encryption - set it up with base64 decoded version of the key and initialization
  // vector passed in.
  CFB_Mode<AES>::Encryption
    Encryptor( (const byte*)localKey.c_str(), blockLen, (const byte*)localIv.c_str() );
  // Wrapping the StringSink with a Base64Encoder encodes the cipher text in base64
  StringSource( plaintext, true,
    new StreamTransformationFilter( Encryptor,
    new Base64Encoder(
    new StringSink( ciphertext )
    ) // Base64Encoder
    ) // StreamTransformationFilter
    ); // StringSource
  //  printf("ciphertext = %s\n", ciphertext.c_str());

#if 0
  // Do a test decryption

  // We're using AES decryption - set it up with base64 decoded version of the key and initialization
  // vector passed in.
  CFB_Mode<AES>::Decryption
    Decryptor( (const byte*)localKey.c_str(), blockLen, (const byte*)localIv.c_str() );

  std::string recoveredText;
  // Wrapping the StringSink with a Base64Decoder performs a base64 decode
  // on the ciphertext prior to feeding it into the transformation filter.
  // We do this because the ciphertext was base64 encoded during encryption.
  StringSource( ciphertext, true,
    new Base64Decoder(new StreamTransformationFilter( Decryptor,
    new StringSink( recoveredText )
    ) // StreamTransformationFilter
    ) // Base64Decoder
    ); // StringSource
  printf("Recovered text = %s\n", recoveredText.c_str());
#endif

  lstrcpynA((LPSTR)output, ciphertext.c_str(), outputLen);
}

// This method decrypts the input string using the specified key and
// initialization vector. It uses AES 256-bit decryption. The return
// value is the decrypted string converted to base64 for easy transmission
// back to .Net.
extern "C" __declspec(dllexport) void APIENTRY GetStringWithBlock(LPTSTR key, int keyLen,
                                                                  LPTSTR iniVec, int iniVecLen,
                                                                  LPTSTR input, int inputLen,
                                                                  LPTSTR output, int outputLen,
                                                                  int blockLen)
{
  if (blockLen != 16 && blockLen != 32)
  {
    lstrcpynA((LPSTR)output, "<Error Block Length>", sizeof("<Error Block Length>"));
  }

  std::string localKey;
  std::string localIv;
  // First decode from base64 key and IV passed in. Decoded results stored in
  // localKey and localIv.
  StringSource((const byte*)key, keyLen, true, new Base64Decoder(new StringSink(localKey)));
  StringSource((const byte*)iniVec, iniVecLen, true, new Base64Decoder(new StringSink(localIv)));

  // We're using AES decryption - set it up with base64 decoded version of the key and initialization
  // vector passed in.
  CFB_Mode<AES>::Decryption
    Decryptor( (const byte*)localKey.c_str(), blockLen, (const byte*)localIv.c_str() );

  std::string ciphertext((LPSTR)input, inputLen);
  std::string recoveredText;
  // Wrapping the StringSink with a Base64Decoder performs a base64 decode
  // on the ciphertext prior to feeding it into the transformation filter.
  // We do this because the ciphertext was base64 encoded during encryption.
  StringSource( ciphertext, true,
    new Base64Decoder(new StreamTransformationFilter( Decryptor,
    new StringSink( recoveredText )
    ) // StreamTransformationFilter
    ) // Base64Decoder
    ); // StringSource
  //  printf("Recovered text = %s\n", recoveredText.c_str());
  lstrcpynA((LPSTR)output, recoveredText.c_str(), outputLen);
}

// This method encrypts the input string using the specified key and
// initialization vector. It uses AES 128-bit encryption. The return
// value is the encrypted string converted to base64 for easy transmission
// back to .Net.
extern "C" __declspec(dllexport) void APIENTRY SetString(LPTSTR key, int keyLen,
                                                         LPTSTR iniVec, int iniVecLen,
                                                         LPTSTR input, int inputLen,
                                                         LPTSTR output, int outputLen)
{
  std::string localKey;
  std::string localIv;
  // First decode from base64 key and IV passed in. Decoded results stored in
  // localKey and localIv.
  StringSource((const byte*)key, keyLen, true, new Base64Decoder(new StringSink(localKey)));
  StringSource((const byte*)iniVec, iniVecLen, true, new Base64Decoder(new StringSink(localIv)));

  std::string plaintext((LPSTR)input, inputLen);
//   std::string plaintext = (LPSTR)input;
  std::string ciphertext;
  // We're using AES encryption - set it up with base64 decoded version of the key and initialization
  // vector passed in.
  CFB_Mode<AES>::Encryption
    Encryptor( (const byte*)localKey.c_str(), AES::DEFAULT_KEYLENGTH, (const byte*)localIv.c_str() );
  // Wrapping the StringSink with a Base64Encoder encodes the cipher text in base64
  StringSource( plaintext, true,
    new StreamTransformationFilter( Encryptor,
    new Base64Encoder(
    new StringSink( ciphertext )
    ) // Base64Encoder
    ) // StreamTransformationFilter
    ); // StringSource
//  printf("ciphertext = %s\n", ciphertext.c_str());

#if 0
  // Do a test decryption

  // We're using AES decryption - set it up with base64 decoded version of the key and initialization
  // vector passed in.
  CFB_Mode<AES>::Decryption
    Decryptor( (const byte*)localKey.c_str(), AES::DEFAULT_KEYLENGTH, (const byte*)localIv.c_str() );

  std::string recoveredText;
  // Wrapping the StringSink with a Base64Decoder performs a base64 decode
  // on the ciphertext prior to feeding it into the transformation filter.
  // We do this because the ciphertext was base64 encoded during encryption.
  StringSource( ciphertext, true,
    new Base64Decoder(new StreamTransformationFilter( Decryptor,
    new StringSink( recoveredText )
    ) // StreamTransformationFilter
    ) // Base64Decoder
    ); // StringSource
  printf("Recovered text = %s\n", recoveredText.c_str());
#endif

  lstrcpynA((LPSTR)output, ciphertext.c_str(), outputLen);
}

extern "C" __declspec(dllexport) void APIENTRY SetString_FirstCutWorked(LPTSTR key, int keyLen,
                        LPTSTR iniVec, int iniVecLen,
                        LPTSTR input, int inputLen,
                        LPTSTR output, int outputLen)
{
  // ...
  //   const char * key = "&h!k%w3yfop9bGaWd$e@";
  std::string localKey;
  std::string localIv;
//   byte localKey[AES::DEFAULT_KEYLENGTH];
//   byte localIv[AES::BLOCKSIZE];
  // First convert from base64 key and IV passed in to 
  StringSource((const byte*)key, keyLen, true, new Base64Decoder(new StringSink(localKey)));
  StringSource((const byte*)iniVec, iniVecLen, true, new Base64Decoder(new StringSink(localIv)));
  // initialize key and iv here
//   memset(key, 0, sizeof(key));
//   memset(iv, 0x01, sizeof(iv));
//   CFB_Mode<AES >::Encryption cfbEncryption((const byte*)localKey.c_str(), AES::DEFAULT_KEYLENGTH, (const byte*)localIv.c_str());

  AES::Encryption aesEncryption((const byte*)localKey.c_str(), AES::DEFAULT_KEYLENGTH);
  CFB_Mode_ExternalCipher::Encryption cfbEncryption(aesEncryption, (const byte*)localIv.c_str());


  //  You can also create a mode object that holds a reference to a block cipher object rather than an instance of it:

  //   AES::Encryption aesEncryption(key, AES::DEFAULT_KEYLENGTH);
  //   CFB_Mode_ExternalCipher::Encryption cfbEncryption(aesEncryption, iv);

  //   Mode objects implement the SymmetricCipher interface, documented at http://www.cryptopp.com/docs/ref/struct_symmetric_cipher_documentation.html. You can use it directly, for example:

#if 0 // WORKS
  byte plaintext[100]; // , ciphertext[100];
//   memset(ciphertext, 0, sizeof(ciphertext));
  // put data into plaintext here
  const char* origstring = "This is the original text";
  memcpy(plaintext, origstring, strlen(origstring));
  // encrypt
//   cfbEncryption.ProcessData(ciphertext, plaintext, 100);

  std::string ciphertext;
//   StreamTransformationFilter cfbEncryptor(cfbEncryption, new StringSink(ciphertext));
  StreamTransformationFilter cfbEncryptor(cfbEncryption, new Base64Encoder(new StringSink(ciphertext)));
  cfbEncryptor.Put(plaintext, 100);
  // input more plaintext here if needed
  cfbEncryptor.MessageEnd();
#endif

#if 1 // try this
  std::string plainText = "This is the new original text";
  std::string ciphertext;
  CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption
    Encryptor( (const byte*)localKey.c_str(), AES::DEFAULT_KEYLENGTH, (const byte*)localIv.c_str() );
  CryptoPP::StringSource( plainText, true,
    new CryptoPP::StreamTransformationFilter( Encryptor,
    new Base64Encoder(
    new CryptoPP::StringSink( ciphertext )
    ) // Base64Encoder
    ) // StreamTransformationFilter
    ); // StringSource
#endif
  printf("ciphertext = %s\n", ciphertext.c_str());

//   memset(plaintext, 0, sizeof(plaintext));
  // now decrypt
  CFB_Mode<AES>::Decryption
    Decryptor( (const byte*)localKey.c_str(), AES::DEFAULT_KEYLENGTH, (const byte*)localIv.c_str() );

  std::string recoveredText;
  StringSource( ciphertext, true,
    new Base64Decoder(new StreamTransformationFilter( Decryptor,
    new StringSink( recoveredText )
    ) // StreamTransformationFilter
    ) // Base64Decoder
    ); // StringSource
  printf("Recovered text = %s\n", recoveredText.c_str());
  *output = '\0';
  return;
#if 0
  CFB_Mode<AES >::Decryption cfbDecryption((const byte*)localKey.c_str(), 16, (const byte*)localIv.c_str());
  cfbDecryption.ProcessData(plaintext, (const byte*)ciphertext.c_str(), 100);

  std::string result;
  StringSource((const byte*)ciphertext.c_str(), sizeof(ciphertext), true, new Base64Encoder(new StringSink(result)));
//   std::basic_string<TCHAR> result;
//   StringSource((const byte*)ciphertext, sizeof(ciphertext), true,
//     new Base64Encoder(new StringSinkTemplate< std::basic_string<TCHAR> >(result)));
  printf("Ran through the DLL: %s\n", result.c_str());
  lstrcpynA((LPSTR)output, result.c_str(), outputLen);
//   lstrcpyn(output, result.c_str(), outputLen);
  //   For ECB and CBC mode, you must process data in multiples of the block size. Alternatively, you can wrap StreamTransformationFilter around the mode object and use it as a Filter object. StreamTransformationFilter will take care of buffering data into blocks for you when needed.

  //     std::string ciphertext;
  //   StreamTransformationFilter cfbEncryptor(cfbEncryption, new StringSink(ciphertext));
  //   cfbEncryptor.Put(plaintext, 100);
  //   // input more plaintext here if needed
  //   cfbEncryptor.MessageEnd();
  //   return ciphertext;
#endif
}

// This method decrypts the input string using the specified key and
// initialization vector. It uses AES 128-bit decryption. The return
// value is the decrypted string converted to base64 for easy transmission
// back to .Net.
extern "C" __declspec(dllexport) void APIENTRY GetString(LPTSTR key, int keyLen,
// __declspec(dllexport) void WINAPIV GetString(LPTSTR key, int keyLen,
                        LPTSTR iniVec, int iniVecLen,
                        LPTSTR input, int inputLen,
                        LPTSTR output, int outputLen)
{
  std::string localKey;
  std::string localIv;
  // First decode from base64 key and IV passed in. Decoded results stored in
  // localKey and localIv.
  StringSource((const byte*)key, keyLen, true, new Base64Decoder(new StringSink(localKey)));
  StringSource((const byte*)iniVec, iniVecLen, true, new Base64Decoder(new StringSink(localIv)));

  // We're using AES decryption - set it up with base64 decoded version of the key and initialization
  // vector passed in.
  CFB_Mode<AES>::Decryption
    Decryptor( (const byte*)localKey.c_str(), AES::DEFAULT_KEYLENGTH, (const byte*)localIv.c_str() );

  std::string ciphertext((LPSTR)input, inputLen);
  std::string recoveredText;
  // Wrapping the StringSink with a Base64Decoder performs a base64 decode
  // on the ciphertext prior to feeding it into the transformation filter.
  // We do this because the ciphertext was base64 encoded during encryption.
  StringSource( ciphertext, true,
    new Base64Decoder(new StreamTransformationFilter( Decryptor,
    new StringSink( recoveredText )
    ) // StreamTransformationFilter
    ) // Base64Decoder
    ); // StringSource
//  printf("Recovered text = %s\n", recoveredText.c_str());
  lstrcpynA((LPSTR)output, recoveredText.c_str(), outputLen);
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

