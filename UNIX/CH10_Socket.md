# CH10 Socket
1. [Introduction](#1.-Introduction)

2. [Addressing](#2.-Addressing)

3. [Socket interface](#3.-Socket-interface)
    * [`socket()`](#`socket()`)

4. [TCP 방식](#4.-TCP-방식)
    * [`bind()`](#`bind()`)
    * [`listen()`](#`listen()`)
    * [`accept()`](#`accept()`)
    * [`connect()`](#`connect()`)
    * [`send()`](#`send()`)
    * [`recv()`](#`recv()`)

5. [UDP 방식](#5.-UDP-방식)
    * [`sendto()`](#`sendto()`)
    * [`recvfrom()`](#`recvfrom()`)

* * *
## 1. Introduction
### Sockets
* unix file descriptor을 이용하여 다른 프로그램과 소통하는 방법
* 2개의 socket 타입
    1. UDP 
        + connectionless protocol
        + flow와 에러 컨트롤 없음
        + small message
        + muticast and broadcast
        + 보안에 취약함
    2. TCP
        + connection-oriented protocol
        + flow와 에러 컨트롤 메커니즘 사용

||pipe|FIFO|Message Q|socket|
|---|---|---|---|---|
|permanent|X|O|O|X|
|fifo|O|O|△|O|
|distance|parent-child|the same system|the same system|remote site|

* * *
## 2. Addressing
### Internet Addressing
* Host는 32 비트set에 IP 주소를 매핑한다.
* domain name을 가질 수 있다.

* general address
    ```c++
    struct sockaddr {   
        u_short sa_family;	        /* address family */   
        char sa_data[14]; 	        /* variable-length address */
    };
    ```
* IP address
    ```c++
    struct in_addr {    
        u_long s_addr;  	        /* IPv4 address */
    }; 
    ```

* TCP/UDP address
    ```c++
    struct sockaddr_in {   
        u_short sin_family;	        /* address family */  
        u_short sin_port;	        /* port number */   
        struct in_addr sin_addr;    /* IPv4 address , INADDR_ANY */   
        char sin_zero[8];
    }; 
    ```

* * *
## 3. Socket interface
### `socket()`
```c++
int socket(int domain, int type, int protocol);
```
|status|return value|
|---|---|
|success|file(socket) descriptor|
|error|-1|

* argument
    - domain
        |domain|desc|
        |---|---|
        |AF_INET|IPv4 internet domain|
        |AF_INET6|IPv6 internet domain|
        |AF_UNIX|UNIX domain|
        |AF_UNSPEC|unspecified|
    - type
        |type|desc|
        |---|---|
        |SOCK_DGRAM|길이 고정, connectionless, unreliable messages|
        |SOCK_RAW|datagram interface to IP|
        |SOCK_SEQPACKET|길이 고정, sequenced, reliable, connection-oriented messages|
        |SOCK_STREAM|sequenced, reliable, bidirectional, connection-oriented by stream|
    - protocol
        |protocol|
        |---|
        |IPPROTO_TCP|
        |IPPROTO_UDP|

### Selecting the Protocol
1. Connection oriented(stream)
    - TCP
    ```c++
    sd = socket(AF_INET, SOCK_STREAM, 0);
    ```
2. Connectionless(datagrams)
    - UDP
    ```c++
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    ```

* * *
## 4. TCP 방식
* 서버에서
    1. socket 생성 - [`socket()`](#`socket()`)
    2. IP 주소 할당 - [`bind()`](#`bind()`)
    3. 연결 요청 대기 상태 - [`listen()`](#`listen()`)
    4. 연결 수락 - [`accept()`](#`accept()`)
* 클라이언트에서
    1. socket 생성 - [`socket()`](#`socket()`)
    2. 연결 요청 - [`connect()`](#`connect()`)

### `bind()`
```c++
int bind(int sockfd, const struct sockaddr* addr, socklen_t len);
```
|status|return value|
|---|---|
|success|0|
|error|-1|


### `listen()`
```c++
int listen(int sockfd, int backlog);
```
|status|return value|
|---|---|
|success|0|
|error|-1|

* argument
    - backlog : 들어오는 연결 요처을 `backlog`개까지 받을 수 있다.

### `accept()`
* 서버가 클라이언트에게서 `connect()` 요청을 받았을 때, 그 특정 소통을 처리하기 위해 완전히 새로운 socket을 생성한다.
```c++
int accept(int sockfd, struct sockaddr* restrict addr, socklen_t* restrict len);
```
|status|return value|
|---|---|
|success|file(socket) descriptor|
|error|-1|

* argument
     - addr : NULL
     - len : NULL

### `connect()`
```c++
int connect(int sockfd, const struct sockaddr* addr, socklen_t len);
```
|status|return value|
|---|---|
|success|0|
|error|-1|

* argument
    - addr : server address

### `send()`
```c++
ssize_t send(int sockfd, const void *buf, size_t nbytes, int flags);
```
|status|return value|
|---|---|
|success|0|
|error|-1|

* argument
    - flag
        |flag|desc|
        |---|---|
        |MSG_DONTWAIT|non-blocking 연산|
        |MSG_OOB|프로토콜이 지원한다면 out-of-band 데이터를 보냄|
        |MSG_DONTROUTE|패킷을 로컬 네트워크 밖으로 라우팅하지 않음|
        |MSG_EOR|프로토콜이 지원한다면 end of record|


### `recv()`
```c++
ssize_t recv(int sockfd, void *buf, size_t nbytes, int flags);
```
|status|return value|
|---|---|
|success|메세지 길이(바이트)|
|클라이언트가 죽었거나 메세지가 없을 때|0|
|error|-1|

* argument
    - flag
        |flag|desc|
        |---|---|
        |MSG_PEEK|엿보기, 패킷을 소비하지 않고 패킷 리턴|
        |MSG_OOB|프로토콜이 지원되는 경우 out-of-band 검색|
        |MSG_WAITALL|(SOCK_STREAM only)모든 데이터가 가능할 때까지 대기|

### 연결 종료
* socket의 반대쪽에서 예기치 않으 프로세스 종료를 처리하는 것은 중요하다
* 연결이 끊긴 socket에 대해 프로세스가 `write()`이나 `send()`를 하면, `SIGPIPE` signal을 받는다.
* `read()`나 `recv()` 0을 return하는 경우, 파일의 끝과 연결 상태를 나타낸다.


### Example
- 서버 자체와 클라이언트가 소통하지 않고, 클라이언트의 요청이 들어오면 `fork()`를 이용하여 자식 프로세스를 생성한다.
- 그 후 서버의 자식 프로세스와 클라이언트가 연결된다.
- Server
```c++
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#define SIZE  sizeof (struct sockaddr_in)
int newsockfd;

void catcher (int sig){ 
    close (newsockfd); 
    exit (0);
}

main(){ 
    int sockfd; 
    char c; 
    struct sockaddr_in server = {AF_INET, 7000, INADDR_ANY}; 

    static struct sigaction act; 
    act.sa_handler = catcher; 
    sigfillset (&(act.sa_mask)); sigaction (SIGPIPE, &act, NULL);

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){	 
        perror ("socket call failed");
        exit (1); 
    }

    if (bind (sockfd, (struct sockaddr *) &server, SIZE) == -1) {	 
        perror ("bind call failed");	 
        exit (1);
    }

    if ( listen(sockfd, 5) == -1 ){
        /* 들어오는 연결을 듣기 시작한다. */ 	 
        perror ("listen call failed");
        exit (1);
    }

    for (;;){	
        if ( (newsockfd = accept (sockfd, NULL, NULL)) == -1){		 
            perror ("accept call failed");		 
            continue; 	
            }

 	/* 연결을 처리할 자식을 하나 낳는다. */	
    if ( fork() == 0){		
        while (recv (newsockfd, &c, 1, 0) > 0){			
            c = toupper(c);			
            send (newsockfd, &c, 1, 0);	    	
        }
        close (newsockfd);		
        exit (0);	
    }

        /* 부모는 newsockfd를 필요로 하지 않는다. */	
        close (newsockfd); 
    }
}
```
* Client
```c++
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define SIZE sizeof (struct sockaddr_in)

main(){ 
    int sockfd; 
    char c, rc; 
    struct sockaddr_in server = {AF_INET, 7000}; 
    server.sin_addr.s_addr = inet_addr("197.45.10.2"); 

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){	 
        perror ("socket call failed"); 
        exit (1); 
    }

    if ( connect(sockfd, (struct sockaddr *) &server, SIZE) == -1){	 
        perror ("connect call failed");	 
        exit (1); 
    } 
    
    for (rc = '\n'; ;){	 
        if (rc == '\n') printf ("Input a lower case character\n");	 

        c = getchar();	 
        send (sockfd, &c, 1, 0);	 

        if (recv(sockfd, &rc, 1, 0)>0) printf ("%c", rc);	 
        else {		 
            printf ("server has died\n");		 
            close (sockfd);		 
            exit (1);	 
        } 
    }
}
```
* * *
## 5. UDP 방식
### `sendto()`
```c++
ssize_t sendto(int sockfd, const void *buf, size_t nbytes, int flags,
    struct sockaddr* restrict dest_addr, socklen_t destlen);
```
|status|return value|
|---|---|
|success|보낸 바이트 수|
|error|-1|

* argument
    - dest_addr : 보낸 메세지를 받을 상대의 주소


### `recvfrom()`
```c++
ssize_t recvfrom(int sockfd, void* restrict buf, size_t len, int flags
    struct sockaddr* restrict send_addr, socklen_t* restrict addrlen);
```
|status|return value|
|---|---|
|success|메세지 길이(바이트)|
|클라이언트가 죽었거나 메세지가 없을 때|0|
|error|-1|

* argument
    - send_addr : 메세지를 보낸 머신의 주소 정보

### 예시
* 우편처럼
* Server
```c++
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define SIZE sizeof (struct sockaddr_in)

main(){ 
    int sockfd; 
    char c; 
    struct sockaddr_in server = {AF_INET, 7000, INADDR_ANY};
    struct sockaddr_in client;
    int client_len = SIZE;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){ 	
        perror ("socket call failed");
        exit(1);
    } 
    
    if ( bind(sockfd, (struct sockaddr *) &server, SIZE)== -1){	 
        perror ("bind call failed");
        exit (1);
    }

    for( ; ; ){ 	
        if (recvfrom(sockfd, &c, 1, 0,&client, &client_len)== -1){		 
            perror ("server: receiving");
            continue;
        }
        
        c = toupper(c);
        
        if ( sendto(sockfd, &c, 1, 0, &client, client_len) == -1){ 		
            perror ("server: sending");
            continue;
        }
    }
}
```
* Client
```c++
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define SIZE sizeof (struct sockaddr_in)

main(){ 
    int sockfd;
    char c;

    struct sockaddr_in client = {AF_INET, INADDR_ANY, INADDR_ANY};
    struct sockaddr_in server = {AF_INET, 7000};
    server.sin_addr.s_addr = inet_addr("197.45.10.2");

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){	
        perror ("socket call failed");
        exit (1);
    } 

    if ( bind(sockfd, (struct sockaddr *) &client, SIZE)== -1) {	 
        perror ("bind call failed");
        exit (1);
    }

    while ( read(0, &c, 1) != 0){	 
        if ( sendto (sockfd, &c, 1, 0, &server, SIZE) == -1){		 
            perror ("client: sending");
            continue;
    	 }

        if (recv(sockfd, &c, 1, 0)== ―1){		 
            perror ("client: receiving");
            continue;
    	 }	 
        write (1, &c, 1);
    }
}
```



