#pragma once

#include "cqctoolbase.hh"

# if CQ_ON_WINDOWS
#   include <winsock2.h>
#   include <ws2tcpip.h>
# else
#   include <netinet/in.h>
# endif

_CQCTOOL_BEGIN_VERSION_NS

struct cq_sockaddr_in {
    
protected:
    
    cq_sockaddr_in(size_t len);
    
public:
    
    sockaddr *addr();
    
    uint32_t ulen();
    int32_t slen();
    
protected:
    
    char _dat[sizeof(sockaddr_in6)];
    int  _len;
};

struct cq_sockaddr_in4 : cq_sockaddr_in {
    
    cq_sockaddr_in4();
    cq_sockaddr_in4(const char *host, uint16_t port);
    cq_sockaddr_in4(const std::string &host, uint16_t port);
    
    void reset(const char *host, uint16_t port);
    void reset(const std::string &host, uint16_t port);
    
    std::string host();
    uint16_t port();
    
    sockaddr_in *addr_in4();
};

struct cq_sockaddr_in6 : cq_sockaddr_in {
    
    cq_sockaddr_in6();
    cq_sockaddr_in6(const char *host, uint16_t port);
    cq_sockaddr_in6(const std::string &host, uint16_t port);
    
    void reset(const char *host, uint16_t port);
    void reset(const std::string &host, uint16_t port);
    
    std::string host();
    uint16_t port();
    
    sockaddr_in6 *addr_in6();
};

in_addr  cq_inet4_addr(const char *str);
in6_addr cq_inet6_addr(const char *str);
in_addr  cq_inet4_addr(const std::string &str);
in6_addr cq_inet6_addr(const std::string &str);

std::string cq_inet4_str(in_addr  addr);
std::string cq_inet6_str(in6_addr addr);

typedef struct _CQ_ST_SOCK *cq_sock;

cq_sock cq_open_tcp_sock4();
cq_sock cq_open_tcp_sock6();
cq_sock cq_open_udp_sock4();
cq_sock cq_open_udp_sock6();

void cq_close_sock(cq_sock sock);

const char *cq_sock_error();

bool cq_bind_sock(cq_sock sock, cq_sockaddr_in local);

int cq_sock_sendto(cq_sock sock, cq_sockaddr_in remote, const void *dat, int datlen);
int cq_sock_recvfrom(cq_sock sock, cq_sockaddr_in *remote, void *buf, int buflen);

_CQCTOOL_END_VERSION_NS
