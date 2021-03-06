#include <arpa/inet.h>
#include <unistd.h>

_CQCTOOL_BEGIN_VERSION_NS

in_addr _inet_addr(const char *str) {
    in_addr addr;
    memset(&addr, 0, sizeof(addr));
    
    if (str != nullptr) {
        inet_pton(AF_INET, str, &addr);
    }
    return addr;
}

in6_addr _inet6_addr(const char *str) {
    in6_addr addr;
    memset(&addr, 0, sizeof(addr));
    
    if (str != nullptr) {
        inet_pton(AF_INET6, str, &addr);
    }
    return addr;
}

in_addr _inet_addr(const std::string &str) {
    return _inet_addr(str.c_str());
}

in6_addr _inet6_addr(const std::string &str) {
    return _inet6_addr(str.c_str());
}

std::string _inet_str(in_addr addr) {
    char str[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &addr, str, sizeof(str));
    return str;
}

std::string _inet6_str(in6_addr addr) {
    char str[INET6_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET6, &addr, str, sizeof(str));
    return str;
}

static _socket_t pkg(int so) {return (_socket_t)(intptr_t)so;}
static int raw(_socket_t so) {return *(int *)&so;}

static _socket_t _socket(int af, int sock, int ipproto) {
    int so = socket(af, sock, ipproto);
    if (so == -1) {
        return nullptr;
    }
    
    //if the tcp disconnected by the other peer, don't emit SIGPIPE.
#if CQ_ON_IOS || CQ_ON_OSX
    if (ipproto == IPPROTO_TCP) {
        int yes = 1;
        setsockopt(so, SOL_SOCKET, SO_NOSIGPIPE, &yes, sizeof(yes));
    }
#endif
    return pkg(so);
}

_socket_t _tcp_socket () { return _socket(AF_INET , SOCK_STREAM, IPPROTO_TCP); }
_socket_t _tcp_socket6() { return _socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP); }
_socket_t _udp_socket () { return _socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP); }
_socket_t _udp_socket6() { return _socket(AF_INET6, SOCK_DGRAM , IPPROTO_UDP); }

void _close(_socket_t so) {
    close(raw(so));
}

bool _bind(_socket_t localso, _sockaddr localaddr) {
    int code = bind(raw(localso), localaddr.raw(), (socklen_t)localaddr.len());
    return code == 0;
}

bool _listen(_socket_t localso) {
    int code = listen(raw(localso), SOMAXCONN);
    return code == 0;
}

_socket_t _accept(_socket_t localso, _sockaddr *endaddr) {
    sockaddr *addr = endaddr ? endaddr->raw() : nullptr;
    socklen_t addrlen = endaddr ? (socklen_t)endaddr->len() : 0;
    
    int endso = accept(raw(localso), addr, &addrlen);
    return pkg(endso);
}

bool _connect(_socket_t localso, _sockaddr endaddr) {
    int code = connect(raw(localso), endaddr.raw(), (socklen_t)endaddr.len());
    return code == 0;
}

int _send(_socket_t so, const void *dat, int datlen) {
    return (int)send(raw(so), dat, (size_t)datlen, 0);
}

int _recv(_socket_t so, void *buf, int buflen) {
    return (int)recv(raw(so), buf, (size_t)buflen, 0);
}

int _sendto(_socket_t localso, _sockaddr endaddr, const void *dat, int datlen) {
    return (int)sendto(raw(localso), dat, (size_t)datlen, 0, endaddr.raw(), (socklen_t)endaddr.len());
}

int _recvfrom(_socket_t localso, _sockaddr *endaddr, void *buf, int buflen) {
    sockaddr *addr = endaddr ? endaddr->raw() : nullptr;
    socklen_t addrlen = endaddr ? (socklen_t)endaddr->len() : 0;
    
    return (int)recvfrom(raw(localso), buf, (size_t)buflen, 0, addr, &addrlen);
}

const char *_sockerr() {
    std::string msg;
    
    int code = errno;
    if (code != 0) {
        msg.append("errno:");
        msg.append(std::to_string(code));
        msg.append(", ");
        msg.append(strerror(code));
    }
    
    return cq_str_store(msg.c_str());
}

_CQCTOOL_END_VERSION_NS
