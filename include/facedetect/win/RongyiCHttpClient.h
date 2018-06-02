#pragma once
#include "string"
using namespace std;

const int POST_FILE_MAX_COUNT = 5;
const int POST_VALUES_MAX_COUNT = 10;
struct HttpParam
{
	std::string strName;
	std::string strValue;
	HttpParam()
	{
		strName = "";
		strValue = "";
	}
};
struct HttpPostFileParam
{
	HttpParam cFileParam[POST_FILE_MAX_COUNT];
	HttpParam cValueParam[POST_VALUES_MAX_COUNT];
};

class RongyiCHttpClient
{
public:
	RongyiCHttpClient(void);
	virtual ~RongyiCHttpClient(void);

	static void Init();
	static void DeInit();

	/**
	* @brief HTTP POST����
	* @param strUrl �������,�����Url��ַ,��:http://www.baidu.com
	* @param strPost �������,ʹ�����¸�ʽpara1=val2&��
	* @param strResponse �������,���ص�����
	* @return �����Ƿ�Post�ɹ�
	*/
	int Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse);

	/**
	* @brief HTTP GET����
	* @param strUrl �������,�����Url��ַ,��:http://www.baidu.com
	* @param strResponse �������,���ص�����
	* @return �����Ƿ�Post�ɹ�
	*/
	int Get(const std::string & strUrl, std::string & strResponse);

	/**
	* @brief HTTPS POST����,��֤��汾
	* @param strUrl �������,�����Url��ַ,��:https://www.alipay.com
	* @param strPost �������,ʹ�����¸�ʽpara1=val1&��
	* @param strResponse �������,���ص�����
	* @param pCaPath �������,ΪCA֤���·��.�������ΪNULL,����֤��������֤�����Ч��.
	* @return �����Ƿ�Post�ɹ�
	*/
	int Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath = NULL);

	/**
	* @brief HTTPS GET����,��֤��汾
	* @param strUrl �������,�����Url��ַ,��:https://www.alipay.com
	* @param strResponse �������,���ص�����
	* @param pCaPath �������,ΪCA֤���·��.�������ΪNULL,����֤��������֤�����Ч��.
	* @return �����Ƿ�Post�ɹ�
	*/
	int Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath = NULL);


	int BrokenDownload(const std::string & strUrl, const std::string& strFileName);

	int Download(const std::string & strUrl, const std::string& strFileName);

	int http_post_file(const char *url, const HttpPostFileParam& cParam, std::string& strResponse, unsigned dwSendSpeedKB = 0);
};
