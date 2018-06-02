#if 0
У���ļ�����ǩ���ĺϷ���(VerifyPE)
Authenticode ? ��һ������ǩ����ʽ������������֤�������������Դ�������ԡ�Authenticode�ǻ��ڹ����ܳ׼��ܱ�׼(PKCS) #7 ��ǩ�����ݣ���ʹ��X.509֤�����󶨾�������ǩ���Ķ����Ƴ���������������ߵ���ݵ���ϵ������ĵ�����Authenticodeǩ���Ľṹ�ͼ���ϸ�ڡ�
����ĵ����������ǩ�� / ����X.509֤�飬���ʹ��Windows SDK������ǩ������Ƴ�����β������ǩ��������ʩ��������ص�Windows ? API�������ⷽ��������Ϣ���Բ鿴�ڱ��ĵ����ġ���Դ���½ڡ�
������Ӧ�������в���ϵͳ��
Windows Server ? 2008
Windows Vista ?
Windows Server 2003
Windows ? XP
Windows 2000
�������ú����۵������Դ��������ĵ��Ľ�β��
����Overview
��PE�ļ��еĿ��Ŵ�������ǩ����PKCS #7 ǩ����ṹ����ǩ�����Ա�֤��
�� ������Դ��ĳ��ȷ������������ߡ�
�� �������Դ�ǩ������û�о����޸ġ�
һ��PKCS #7 ǩ����ṹ������PE�ļ��Ĺ�ϣֵ��ͨ�����������˽�״�����ǩ���Լ�����������ߵ�ǩ���ܳװ󶨵�һ���Ϸ�ʵ���X.509 v3֤�顣һ��PKCS #7ǩ������԰������¿�ѡ��Ϣ��
�� ������������ߵ�����
�� ��������ߵ�����
�� ���Ŵ���ǩ����ʱ���
ǩ��ʱ�����ʱ���Ȩ������(TSA)���ɣ����ұ�֤���������������ǩ�������ʱ���֮ǰ�Ѿ����ڡ����ʱ��������ӳ������ǩ���������ڣ�������ص�ǩ��֤���Ѿ����ڻ��ߺ������ϳ���
���Ŵ���ǩ�����Ա�Ƕ�뵽Windows��PE�ļ��У�λ��PE�ļ���Optional Header Data Directories�ṹ��Certificate Table��ָ���λ�á������Ŵ���ǩ��������ǩ��һ��Windows PE�ļ�ʱ�������ļ�����ǩ����ϣֵ���㷨�Թ�PE�ļ��ṹ�е��ض��ֶΡ���������ǩ��Ƕ���ļ�ʱ��ǩ�����̿����޸���Щ�ֶΣ���������Ӱ���ļ��Ĺ�ϣֵ��
ͼ1��ʾ��һ���򵥵�PE�ļ�ȫ��ͼ��������������ǩ������ΰ�����PE�ļ��еġ���������Ƕ��Ŀ��Ŵ�������ǩ����ָ����Щ���Թ�����PE�ļ���ϣֵ��PE�ṹ�ֶΡ�
����PE�ļ��ṹ��ϸ�ڣ�����ġ�Microsoft Portable Executable and Common Object File Format Specification��(PE / COFF specification)��
���ڿ��Ŵ�������ǩ����PKCS #7���ֵ�ϸ�ڣ�����ı��ĵ������Abstract Syntax Notation version 1 (ASN.1) �ṹ���塣
���ڿ��Ŵ�������ǩ����μ���PE�ļ���ϣֵ��ϸ�ڣ����ı��ĵ�����ġ�Calculating the PE Image Hash����
PE�ṹ�е�����λ��
һ���� ����ǩ���� ����ƫ�Ƶ�ַ һ���� ǩ���ļ��ĳ���
���彲���� PE�ṹ�� IMAGE_NT_HEADERS�ṹ����һ�� IMAGE_OPTIONAL_HEADER32
��IMAGE_OPTIONAL_HEADER32����һ��IMAGE_DATA_DIRECTORY��
��IMAGE_DATA_DIRECTORY����һϵ�е����� ���еĵ�5����ָ�� ��ȫ��Ϣ��
ֱ���޸������ dwSingSize��VirtualAddress�Ϳ�����
��֤��������ǩ����PE�ļ�����
#endif
#define _UNICODE 1
#define UNICODE 1
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <Softpub.h>
#include <wincrypt.h>
#include <wintrust.h>
// Link with the Wintrust.lib file.
#pragma comment (lib, "wintrust")
BOOL VerifyEmbeddedSignature(LPCWSTR pwszSourceFile)
{
    LONG lStatus;
    DWORD dwLastError;
    // Initialize the WINTRUST_FILE_INFO structure.
    WINTRUST_FILE_INFO FileData = {0};
    memset(&FileData, 0, sizeof(FileData));
    FileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
    FileData.pcwszFilePath = pwszSourceFile;
    FileData.hFile = NULL;
    FileData.pgKnownSubject = NULL;
    /*
    WVTPolicyGUID specifies the policy to apply on the file
    WINTRUST_ACTION_GENERIC_VERIFY_V2 policy checks:
    1) The certificate used to sign the file chains up to a root
    certificate located in the trusted root certificate store. This
    implies that the identity of the publisher has been verified by
    a certification authority.
    2) In cases where user interface is displayed (which this example
    does not do), WinVerifyTrust will check for whether the
    end entity certificate is stored in the trusted publisher store,
    implying that the user trusts content from this publisher.
    3) The end entity certificate has sufficient permission to sign
    code, as indicated by the presence of a code signing EKU or no
    EKU.
    */
    GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    WINTRUST_DATA WinTrustData = {0};
    // Initialize the WinVerifyTrust input data structure.
    // Default all fields to 0.
    memset(&WinTrustData, 0, sizeof(WinTrustData));
    WinTrustData.cbStruct = sizeof(WinTrustData);
    // Use default code signing EKU.
    WinTrustData.pPolicyCallbackData = NULL;
    // No data to pass to SIP.
    WinTrustData.pSIPClientData = NULL;
    // Disable WVT UI.
    WinTrustData.dwUIChoice = WTD_UI_NONE;
    // No revocation checking.
    WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
    // Verify an embedded signature on a file.
    WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;
    // Default verification.
    WinTrustData.dwStateAction = 0;
    // Not applicable for default verification of embedded signature.
    WinTrustData.hWVTStateData = NULL;
    // Not used.
    WinTrustData.pwszURLReference = NULL;
    // This is not applicable if there is no UI because it changes
    // the UI to accommodate running applications instead of
    // installing applications.
    //WinTrustData.dwUIContext = 0;
    // Set pFile.
    WinTrustData.pFile = &FileData;
    // WinVerifyTrust verifies signatures as specified by the GUID
    // and Wintrust_Data.
    lStatus = WinVerifyTrust(NULL, &WVTPolicyGUID, &WinTrustData);
#define wprintf_s wprintf
    switch (lStatus)
    {
    case ERROR_SUCCESS:
        /*
        Signed file:
        - Hash that represents the subject is trusted.
        - Trusted publisher without any verification errors.
        - UI was disabled in dwUIChoice. No publisher or
        time stamp chain errors.
        - UI was enabled in dwUIChoice and the user clicked
        "Yes" when asked to install and run the signed
        subject.
        */
        wprintf_s(L"The file \"%s\" is signed and the signature was verified.\n",
                  pwszSourceFile);
        break;
    case TRUST_E_NOSIGNATURE:
        // The file was not signed or had a signature
        // that was not valid.
        // Get the reason for no signature.
        dwLastError = GetLastError();
        if (TRUST_E_NOSIGNATURE == dwLastError ||
                TRUST_E_SUBJECT_FORM_UNKNOWN == dwLastError ||
                TRUST_E_PROVIDER_UNKNOWN == dwLastError)
        {
            // The file was not signed.
            wprintf_s(L"The file \"%s\" is not signed.\n",
                      pwszSourceFile);
        }
        else
        {
            // The signature was not valid or there was an error
            // opening the file.
            wprintf_s(L"An unknown error occurred trying to "
                      L"verify the signature of the \"%s\" file.\n",
                      pwszSourceFile);
        }
        break;
    case TRUST_E_EXPLICIT_DISTRUST:
        // The hash that represents the subject or the publisher
        // is not allowed by the admin or user.
        wprintf_s(L"The signature is present, but specifically "
                  L"disallowed.\n");
        break;
    case TRUST_E_SUBJECT_NOT_TRUSTED:
        // The user clicked "No" when asked to install and run.
        wprintf_s(L"The signature is present, but not trusted.\n");
        break;
    case CRYPT_E_SECURITY_SETTINGS:
        /*
        The hash that represents the subject or the publisher
        was not explicitly trusted by the admin and the
        admin policy has disabled user trust. No signature,
        publisher or time stamp errors.
        */
        wprintf_s(L"CRYPT_E_SECURITY_SETTINGS - The hash "
                  L"representing the subject or the publisher wasn't "
                  L"explicitly trusted by the admin and admin policy "
                  L"has disabled user trust. No signature, publisher "
                  L"or timestamp errors.\n");
        break;
    default:
        // The UI was disabled in dwUIChoice or the admin policy
        // has disabled user trust. lStatus contains the
        // publisher or time stamp chain error.
        wprintf_s(L"Error is: 0x%x.\n", lStatus);
        break;
    }
    return true;
}
int test_VerifyPE()
{
    const wchar_t* fn;
    fn = L"D:\\keypair.p12";
    fn = L"D:\\Security-SPP-Component-SKU-Ultimate-OEM-NONSLP-pl.xrm-ms";
    int x = VerifyEmbeddedSignature(fn);
    return 0;
}

