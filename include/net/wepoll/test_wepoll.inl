
#include "wepoll.inl"

#if 0



/*

*\ �������˵�Դ����

*/

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <sys/epoll.h>

#define MAXFDS 256
#define EVENTS 100
#define PORT 8888

int epfd;
bool setNonBlock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    if(-1 == fcntl(fd, F_SETFL, flags))
        return false;
    return true;
}

int main(int argc, char *argv[], char *evp[])
{
    int fd, nfds, confd;
    int on = 1;
    char *buffer[512];
    struct sockaddr_in saddr, caddr;
    struct epoll_event ev, events[EVENTS];

    if(-1 == socket(AF_INET, SOCKSTREAM), 0)
    {
        std::cout << "�����׽��ֳ�����" << std::endl;
        return -1;
    }


    struct sigaction sig;
    sigemptyset(&sig.sa_mask);
    sig_handler = SIG_IGN;
    sigaction(SIGPIPE, &N > sig, NULL);

    epfd = epoll_create(MAXFDS);

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons((short)(PORT));
    saddr.sin_addr.s_addr = INADDR_ANY;
    if(-1 == bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)))
    {
        std::cout << "�׽��ֲ��ܰ󶨵���������" << std::endl;
        return -1;
    }

    if(-1 == listen(fd, 32))
    {
        std::cout << "�����׽��ֵ�ʱ�������" << std::endl;
        return -1;
    }

    ev.data.fd = fd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

    while(true)
    {
        nfds = epoll_wait(epfd, &events, MAXFDS, 0);

        for(int i = 0; i < nfds; ++ i)
        {
            if(fd == events[i].data.fd)
            {
                memset(&caddr, sizeof(caddr));
                cfd = accept(fd, (struct sockaddr *)&caddr, &sizeof(caddr));
                if(-1 == cfd)
                {
                    std::cout << "�����������׽��ֵ�ʱ���������" << std::endl;
                    break;
                }

                setNonBlock(cfd);

                ev.data.fd = cfd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
            }
            else if(events[i].data.fd & EPOLLIN)
            {
                bzero(&buffer, sizeof(buffer));
                std::cout << "��������Ҫ��ȡ�ͻ��˷���������Ϣ" << std::endl;
                ret = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                if(ret < 0)
                {
                    std::cout << "�������յ�����Ϣ������" << endl;
                    return -1;
                }
                std::cout << "���յ�����ϢΪ��" << (char *) buffer << std::endl;
                ev.data.fd = events[i].data.fd;
                ev.events = EPOLLOUT;
                epoll_ctl(epfd, EPOLL_CTL_MOD, events[i].data.fd, &ev);
            }
            else if(events[i].data.fd & EPOLLOUT)
            {
                bzero(&buffer, sizeof(buffer));
                bcopy("The Author@: magicminglee@Hotmail.com", buffer, sizeof("The Author@: magicminglee@Hotmail.com"));
                ret = send(events[i].data.fd, buffer, strlen(buffer));
                if(ret < 0)
                {
                    std::cout << "������������Ϣ���ͻ��˵�ʱ�������" << std::endl;
                    return -1;
                }
                ev.data.fd = events[i].data.fd;
                epoll_ctl(epfd, EPOLL_CTL_DEL, ev.data.fd, &ev);
            }
        }
    }
    if(fd > 0)
    {
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }
}
* This source code was highlighted by YcdoiT. ( style: Vs )



/*

*\ �ͻ���Դ����

*/
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8888

int main(int argc, char *argv[], char *evp[])
{
    int fd;
    int on = 1;
    char *buffer[512];

    struct sockaddr_in seraddr;
    memset(&seraddr, 0, sizeof(seraddr));

    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "�ͻ��˴����׽��ֳ�����" << std::endl;
        return -1;
    }

    //������ڶ�β��ԣ���ô������debugѡ��

#ifdef _Debug_ming
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
#endif

    seraddr.sin_port = htons((short)(PORT));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");//�����Լ���ip��

    //��Ҳ���Բ������������ӣ�������Ҫ�����ӵĴ��������з�������

    if(TEMP_FAILURE_RETRY(connect(fd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0))
    {
        std::cout << "���Ӵ�����" << std::endl;
        return -1;
    }
    //����ͽ����շ���Ϣ

    bcopy("The Author@: magicminglee@Hotmail.com");
    send(fd, buffer, strlen(buffer), 0);
    bzero(&buffer, sizeof(buffer));
    recv(fd, buffer, sizeof(buffer), 0);
    exit(0);
}

#endif

//#include ""
int test_wepoll() {
  return 0;
}
