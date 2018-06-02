
const char* curl_easy_strerror_cn(CURLcode code)
{  
    switch (code)  
    {  
    case CURLE_UNSUPPORTED_PROTOCOL:  
        return "�����͸� libcurl ����ַʹ���˴� libcurl ��֧�ֵ�Э�顣 ��������û��ʹ�õı���ʱѡ����������������������Э���ַ���ƴд���󣬻�û��ָ��Э�� libcurl ���룩��";  
  
    case CURLE_FAILED_INIT:  
        return "�ǳ����ڵĳ�ʼ������ʧ�ܡ� �������ڲ���������⡣";  
  
    case CURLE_URL_MALFORMAT:  
        return "��ַ��ʽ����ȷ��";  
  
    case CURLE_COULDNT_RESOLVE_PROXY:  
        return "�޷���������������� ָ���Ĵ�������������޷�������";  
  
    case CURLE_COULDNT_RESOLVE_HOST:  
        return "�޷����������� ָ����Զ�������޷�������";  
  
    case CURLE_COULDNT_CONNECT:  
        return "�޷�ͨ�� connect() ����������������������";  
  
    case CURLE_FTP_WEIRD_SERVER_REPLY:  
        return "�����ӵ� FTP ��������libcurl ��Ҫ�յ��ض��Ļظ��� �˴�������ʾ�յ��˲���������ȷ�Ļظ��� ָ����Զ�̷��������ܲ�����ȷ�� FTP ��������";  
  
    case CURLE_REMOTE_ACCESS_DENIED:  
        return "�����޷�������ַ��ָ������Դ�� ���� FTP��������Ը���ΪԶ��Ŀ¼���ͻᷢ�����������";  
  
    case CURLE_FTP_WEIRD_PASS_REPLY:  
        return "�ڽ� FTP ���뷢�͵���������libcurl ��Ҫ�յ���ȷ�Ļظ��� �˴�������ʾ���ص�������Ĵ��롣";  
  
    case CURLE_FTP_WEIRD_PASV_REPLY:  
        return "libcurl �޷��ӷ��������յ����õĽ������Ϊ�� PASV �� EPSV �������Ӧ�� �����������⡣";  
  
    case CURLE_FTP_WEIRD_227_FORMAT:  
        return "FTP ���������� 227 ����Ϊ�� PASV �������Ӧ�� ��� libcurl �޷��������У��ͻ᷵�ش˴��롣";  
  
    case CURLE_FTP_CANT_GET_HOST:  
        return "�ڲ������������ӵ�����ʱ�����ڲ�����";  
  
    case CURLE_FTP_COULDNT_SET_TYPE:  
        return "�ڳ��Խ�����ģʽ����Ϊ�����ƻ� ascii ʱ��������";  
  
    case CURLE_PARTIAL_FILE:  
        return "�ļ�����ߴ�С�ڻ����Ԥ�ڡ� ���������ȱ�����һ��Ԥ�ڵĴ���ߴ磬Ȼ�������͵���������ǰָ���ߴ粻���ʱ���ͻᷢ���˴���";  
  
    case CURLE_FTP_COULDNT_RETR_FILE:  
        return "��RETR�� �����յ��˲������Ļظ�������ɵĴ���ߴ�Ϊ���ֽڡ�";  
  
    case CURLE_QUOTE_ERROR:  
        return "����Զ�̷����������Զ��� ��QUOTE�� ����ʱ������һ������صĴ������Ϊ 400 ���������֣����� FTP��������������ʽ���������޷��ɹ���ɡ�";  
  
    case CURLE_HTTP_RETURNED_ERROR:  
        return "��� CURLOPT_FAILONERROR ����Ϊ TRUE���� HTTP ���������� >= 400 �Ĵ�����룬�ͻ᷵�ش˴��롣 ���˴��������ǰ�ֳ�Ϊ CURLE_HTTP_NOT_FOUND����";  
  
    case CURLE_WRITE_ERROR:  
        return "���򱾵��ļ�д�����յ�������ʱ�������󣬻���д��ص� (write callback) �� libcurl ������һ������";  
  
    case CURLE_UPLOAD_FAILED:  
        return "�޷���ʼ�ϴ��� ���� FTP��������ͨ����ܾ�ִ�� STOR ��� ���󻺳���ͨ�����ṩ�������Դ������˵���� ���˴��������ǰ�ֳ�Ϊ CURLE_FTP_COULDNT_STOR_FILE����";  
  
    case CURLE_READ_ERROR:  
        return "��ȡ�����ļ�ʱ�������⣬���ɶ�ȡ�ص� (read callback) ������һ������";  
  
    case CURLE_OUT_OF_MEMORY:  
        return "�ڴ��������ʧ�ܡ� �˴���Ƚ����أ��������˴�������������˷ǳ����ص����⡣";  
  
    case CURLE_OPERATION_TIMEDOUT:  
        return "������ʱ�� �Ѵﵽ������Ӧ���ָ���ĳ�ʱʱ�䡣 ��ע�⣺ �� Urchin 6.6.0.2 ��ʼ����ʱʱ��������и��ġ� Ҫָ��Զ����־���س�ʱ����� urchin.conf �ļ���ȡ�������е�ע�ͱ�ǣ�#DownloadTimeout: 30";  
  
    case CURLE_FTP_PORT_FAILED:  
        return "FTP PORT ����ش��� ��û��Ϊ libcurl ָ���ʵ��ĵ�ַʹ��ʱ�����п��ܷ��������⡣ ����� CURLOPT_FTPPORT��";  
  
    case CURLE_FTP_COULDNT_USE_REST:  
        return "FTP REST ����ش��� �����������������Ӧ�����ᷢ�����������";  
  
    case CURLE_RANGE_ERROR:  
        return "��������֧�ֻ򲻽��ܷ�Χ����";  
  
    case CURLE_HTTP_POST_ERROR:  
        return "������Ƚ��ټ�����Ҫ���ڲ�����������";  
  
    case CURLE_SSL_CONNECT_ERROR:  
        return "ͬʱʹ�� SSL/TLS ʱ���ܻᷢ���˴��� �����Է��ʴ��󻺳����鿴��Ӧ��Ϣ�����л�Դ�������и���ϸ�Ľ��ܡ� ������֤�飨�ļ���ʽ��·������ɣ������뼰�������ص����˴����⡣";  
  
    case CURLE_FTP_BAD_DOWNLOAD_RESUME:  
        return "���Իָ������ļ���С���Ƶ� FTP ���ӡ�";  
  
    case CURLE_FILE_COULDNT_READ_FILE:  
        return "�޷��� FILE:// ·���µ��ļ��� ԭ��ܿ������ļ�·���޷�ʶ�������ļ��� ����������ļ��ķ���Ȩ�ޡ�";  
  
    case CURLE_LDAP_CANNOT_BIND:  
        return "LDAP �޷��󶨡�LDAP �󶨲���ʧ�ܡ�";  
  
    case CURLE_LDAP_SEARCH_FAILED:  
        return "LDAP �����޷����С�";  
  
    case CURLE_FUNCTION_NOT_FOUND:  
        return "�Ҳ��������� �Ҳ�����Ҫ�� zlib ������";  
  
    case CURLE_ABORTED_BY_CALLBACK:  
        return "�ɻص���ֹ�� �ص��� libcurl ������ ��abort����";  
  
    case CURLE_BAD_FUNCTION_ARGUMENT:  
        return "�ڲ����� ʹ���˲���ȷ�Ĳ������ú�����";  
  
    case CURLE_INTERFACE_FAILED:  
        return "������� ָ�����ⲿ�����޷�ʹ�á� ��ͨ�� CURLOPT_INTERFACE ����Ҫʹ���ĸ������������ⲿ���ӵ���Դ IP ��ַ�� ���˴��������ǰ�ֳ�Ϊ CURLE_HTTP_PORT_FAILED����";  
  
    case CURLE_TOO_MANY_REDIRECTS:  
        return "�ض�����ࡣ �����ض���ʱ��libcurl �ﵽ����ҳ������ޡ� ��ʹ�� CURLOPT_MAXREDIRS �������ޡ�";  
  
    case CURLE_UNKNOWN_TELNET_OPTION:  
        return "�޷�ʶ���� CURLOPT_TELNETOPTIONS ���õ�ѡ� ���������ĵ���";  
  
    case CURLE_TELNET_OPTION_SYNTAX:  
        return "telnet ѡ���ַ����ĸ�ʽ����ȷ��";  
  
    case CURLE_PEER_FAILED_VERIFICATION:  
        return "Զ�̷������� SSL ֤��� SSH md5 ָ�Ʋ���ȷ��";  
  
    case CURLE_GOT_NOTHING:  
        return "������δ�����κ����ݣ�����Ӧ����£�δ�����κ����ݾ����ڳ��ִ���";  
  
    case CURLE_SSL_ENGINE_NOTFOUND:  
        return "�Ҳ���ָ���ļ������档";  
  
    case CURLE_SSL_ENGINE_SETFAILED:  
        return "�޷���ѡ���� SSL ����������ΪĬ��ѡ�";  
  
    case CURLE_SEND_ERROR:  
        return "�޷������������ݡ�";  
  
    case CURLE_RECV_ERROR:  
        return "������������ʧ�ܡ�";  
  
    case CURLE_SSL_CERTPROBLEM:  
        return "���ؿͻ���֤��������";  
  
    case CURLE_SSL_CIPHER:  
        return "�޷�ʹ��ָ������Կ";  
  
    case CURLE_SSL_CACERT:  
        return "�޷�ʹ����֪�� CA ֤����֤�Ե�֤��";  
  
    case CURLE_BAD_CONTENT_ENCODING:  
        return "�޷�ʶ�������";  
  
    case CURLE_LDAP_INVALID_URL:  
        return "LDAP ��ַ��Ч";  
  
    case CURLE_FILESIZE_EXCEEDED:  
        return "�������ļ���С����";  
  
    case CURLE_USE_SSL_FAILED:  
        return "����� FTP SSL ����ʧ��";  
  
    case CURLE_SEND_FAIL_REWIND:  
        return "���з��Ͳ���ʱ��curl �����ת�����Ա����´��䣬����ת����δ�ܳɹ�";  
  
    case CURLE_SSL_ENGINE_INITFAILED:  
        return "SSL �����ʼ��ʧ��";  
  
    case CURLE_LOGIN_DENIED:  
        return "Զ�̷������ܾ� curl ��¼��7.13.1 �������ܣ�";  
  
    case CURLE_TFTP_NOTFOUND:  
        return "�� TFTP ���������Ҳ����ļ�";  
  
    case CURLE_TFTP_PERM:  
        return "�� TFTP ������������Ȩ������";  
  
    case CURLE_REMOTE_DISK_FULL:  
        return "���������̿ռ䲻��";  
  
    case CURLE_TFTP_ILLEGAL:  
        return "TFTP �����Ƿ�";  
  
    case CURLE_TFTP_UNKNOWNID:  
        return "TFTP ���� ID δ֪";  
  
    case CURLE_REMOTE_FILE_EXISTS:  
        return "�ļ��Ѵ��ڣ��޷�����";  
  
    case CURLE_TFTP_NOSUCHUSER:  
        return "���������� TFTP ���������᷵�ش˴���";  
  
    case CURLE_CONV_FAILED:  
        return "�ַ�ת��ʧ��";  
  
    case CURLE_CONV_REQD:  
        return "���÷�����ע��ת���ص�";  
  
    case CURLE_SSL_CACERT_BADFILE:  
        return "��ȡ SSL CA ֤��ʱ�������⣨������·����������Ȩ�����⣩";  
  
    case CURLE_REMOTE_FILE_NOT_FOUND:  
        return "��ַ�����õ���Դ������";  
  
    case CURLE_SSH:  
        return "SSH �Ự�з����޷�ʶ��Ĵ���";  
  
    case CURLE_SSL_SHUTDOWN_FAILED:  
        return "�޷���ֹ SSL ����";  
  
    default:  
        return "δ֪�� curl ���ش���";  
    }  
}