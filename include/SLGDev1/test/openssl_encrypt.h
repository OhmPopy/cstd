
#ifndef _OPENSSL_ENCRYPT_INL_
#define _OPENSSL_ENCRYPT_INL_

#include "openssl_lib.inl"

// RSA���ܲ��ִ���demo
static int EncodeRSAKeyFile(const char* strPemFileName, const char* data, int data_len, char* strRet, int strRet_maxlen)
{
    FILE* hPubKeyFile = NULL;
    RSA* pRSAPublicKey;
    int nLen;

    if (strPemFileName == NULL || data == NULL)
    {
        assert(false);
        return -1;
    }

    hPubKeyFile = fopen(strPemFileName, "rb");

    if (hPubKeyFile == NULL)
    {
        assert(false);
        return -1;
    }

    pRSAPublicKey = RSA_new();

    if (PEM_read_RSA_PUBKEY(hPubKeyFile, &pRSAPublicKey, 0, 0) == NULL)
    {
        assert(false);
        return -1;
    }

    nLen = RSA_size(pRSAPublicKey);

    if (strRet_maxlen >= nLen)
    {
        nLen = RSA_public_encrypt(data_len, (const unsigned char*)data, (unsigned char*)strRet, pRSAPublicKey, RSA_PKCS1_PADDING);
    }

    RSA_free(pRSAPublicKey);
    fclose(hPubKeyFile);
    CRYPTO_cleanup_all_ex_data();
    return nLen;
}

// RSA���ܲ��ִ���demo��
static int DecodeRSAKeyFile(const char* strPemFileName, const char* data, int data_len, char* strRet, int strRet_maxlen)
{
    FILE* hPriKeyFile = NULL;
    RSA* pRSAPriKey;
    int nLen;

    if (strPemFileName == NULL || data == NULL)
    {
        assert(false);
        return -1;
    }

    hPriKeyFile = fopen(strPemFileName, "rb");

    if (hPriKeyFile == NULL)
    {
        assert(false);
        return -1;
    }

    pRSAPriKey = RSA_new();

    if (PEM_read_RSAPrivateKey(hPriKeyFile, &pRSAPriKey, 0, 0) == NULL)
    {
        assert(false);
        return -1;
    }

    nLen = RSA_size(pRSAPriKey);

    if (strRet_maxlen >= nLen)
    {
        nLen = RSA_private_decrypt(data_len, (const unsigned char*)data, (unsigned char*)strRet, pRSAPriKey, RSA_PKCS1_PADDING);
    }

    RSA_free(pRSAPriKey);
    fclose(hPriKeyFile);
    CRYPTO_cleanup_all_ex_data();
    return nLen;
}

// RSA��API�е�ʹ�ò���RSA_PKCS1_PADDINGʱ�����ĳ��Ȳ��ܴ������ĳ���-11����ʹ�ò���RSA_NO_PADDINGʱ�����ĳ�����Ҫ������128��
// AES���ܲ��ִ��룺
static int EncodeAES(const char* key, int key_len, const char* data, int data_len, char* strRet, int strRet_maxlen)
{
    AES_KEY aes_key = {0};
    char* data_bak = NULL;
    int i, data_bak_len;

    if (AES_set_encrypt_key((const unsigned char*)key, key_len * 8, &aes_key) < 0)
    {
        assert(false);
        return -1;
    }

    data_bak_len = data_len + (AES_BLOCK_SIZE - data_len % AES_BLOCK_SIZE);

    if (data_bak_len <= strRet_maxlen)
    {
        data_bak = (char*)malloc(data_bak_len);
        memcpy(data_bak, data, data_len);
        memset(data_bak + data_len, 0, data_bak_len - data_len);

        for (i = 0; i < data_bak_len / AES_BLOCK_SIZE; i++)
        {
            const unsigned char* str16 = (const unsigned char*)data_bak + i * AES_BLOCK_SIZE; //AES_BLOCK_SIZE
            unsigned char* out = (unsigned char*)strRet + i * AES_BLOCK_SIZE; //[AES_BLOCK_SIZE];
            memset(out, 0, AES_BLOCK_SIZE);
            AES_encrypt(str16, out, &aes_key);
        }

        free(data_bak);
    }

    return data_bak_len;
}

// AES���ܲ��ִ��룺
static int DecodeAES(const char* key, int key_len, const char* data, int data_len, char* strRet, int strRet_maxlen)
{
    AES_KEY aes_key;
    int i;

    if (AES_set_decrypt_key((const unsigned char*)key, key_len * 8, &aes_key) < 0)
    {
        assert(false);
        return -1;
    }

    for (i = 0; i < data_len / AES_BLOCK_SIZE; i++)
    {
        const unsigned char* str16 = (const unsigned char*)data + i * AES_BLOCK_SIZE; //AES_BLOCK_SIZE;
        unsigned char* out = (unsigned char*)strRet + i * AES_BLOCK_SIZE; //[AES_BLOCK_SIZE];
        memset(out, 0, AES_BLOCK_SIZE);
        AES_decrypt((const unsigned char*)str16, out, &aes_key);
    }

    return data_len;
}

#define AES_BITS 128
#define MSG_LEN 128
static int check_key_copy(const void* key, int key_len, void* key1, int max_key_len)
{
    if (key_len > max_key_len)
    {
        key_len = max_key_len;
    }

    else
    {
        while (max_key_len >= 32 && (max_key_len / 2) > key_len)
        {
            max_key_len /= 2;
        }

    }

    memcpy(key1, key, key_len);
    return max_key_len;
}

#define MAX_KEY_LEN 128
static int aes_encrypt(const void* key, int key_len, void* in, int in_len, void* out, int out_maxlen)//, int olen)���ܻ�����buf����
{
    //���ܵĳ�ʼ������,ivһ������Ϊȫ0,�����������������Ǽ��ܽ���Ҫһ������
    unsigned char iv[AES_BLOCK_SIZE] = {0};
    int len1 = in_len & 15;
    AES_KEY aes = {0};
    char key1[MAX_KEY_LEN] = {0};

    if (!in || !key || !out)
    {
        return 0;
    }

    key_len = check_key_copy(key, key_len, key1, MAX_KEY_LEN);
    key = key1;

    if (AES_set_encrypt_key((const unsigned char*)key, key_len * 8, &aes) < 0)
    {
        return 0;
    }

    in_len -= len1;
    //����ĳ�����char*in�ĳ��ȣ��������in�м����'\0'�ַ��Ļ�
    //��ô��ֻ�����ǰ��'\0'ǰ���һ�Σ����ԣ����len������Ϊ��������������¼in�ĳ���
    //���ڽ���Ҳ��һ����������'\0'���ж��ַ������ȣ�ȷ�в��ף����涼��һ������
    AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, in_len, &aes, iv, AES_ENCRYPT);
    memcpy((char*)out + in_len, (char*)in + in_len, len1);
    return in_len;
}

static int aes_decrypt(const void* key, int key_len, void* in, int in_len, void* out, int out_maxlen)
{
    unsigned char iv[AES_BLOCK_SIZE] = {0};//���ܵĳ�ʼ������
    int len1 = in_len & 15;
    AES_KEY aes = {0};
    char key1[MAX_KEY_LEN] = {0};

    if (!in || !key || !out)
    {
        return 0;
    }

    key_len = check_key_copy(key, key_len, key1, MAX_KEY_LEN);
    key = key1;

    if (AES_set_decrypt_key((const unsigned char*)key, key_len * 8, &aes) < 0)
    {
        return 0;
    }

    in_len -= len1;
    AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, in_len, &aes, iv, AES_DECRYPT);
    memcpy((char*)out + in_len, (char*)in + in_len, len1);
    return in_len;
}

#ifdef TEST
#include "cstd.h"
int test_openssl()
{
    char* aa = NULL, *bb, *cc;
    int sz = 1001;
    char key[AES_BLOCK_SIZE + 1] = {0};
    aa = (char*)malloc(sz * 5);
    memset(aa, '1', sz);
    memset(key, '1', AES_BLOCK_SIZE);
    bb = aa + sz;
    cc = bb + sz * 2;
    aes_encrypt(key, AES_BLOCK_SIZE, aa, sz, bb, sz);
    bb[1] = 0;
    aes_decrypt(key, AES_BLOCK_SIZE, bb, sz, cc, sz);
    ASSERT(0 == memcmp(aa, cc, sz));
    free(aa);
    return 0;
}

#endif // TEST

#endif // _OPENSSL_ENCRYPT_INL_

