#ifndef PACKET_ACCESSOR_HPP
#define PACKET_ACCESSOR_HPP
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#endif
#include <string>
#include "packet_element.hpp"
#include <list>

#define DEFAULT_BUFFER_SIZE 1000000
// #define DEFAULT_BUFFER_SIZE 2047

class Socket {
public:
  static sockaddr_in stringToAddr(const char *addr, int port);
#ifdef _WIN32
  typedef SOCKET sockfd_t;
#else
  typedef int sockfd_t;
#endif
  sockfd_t sockfd;
};

class UDPSocket : public Socket {
public:
  UDPSocket();
  virtual ~UDPSocket();
  void bind(int port);
  void connect(sockaddr_in addr);
  void connect(const char *addr, int port);
  void add(in_addr grpaddr, in_addr srcaddr);
  void add(const char *group, const char *source);
  void add(in_addr grpaddr);
  void add(const char *group);
  void drop(in_addr grpaddr, in_addr srcaddr);
  void drop(const char *group, const char *source);
  void drop(in_addr grpaddr);
  void drop(const char *group);
  unsigned int getMTU();
  enum IP_PMTUDISC_ENUM {
    WANT,
    DONT,
    DO,
    PROBE
  };
  void setMTUDiscovery(enum IP_PMTUDISC_ENUM e);
};

class Reader {
public:
  virtual void read(void *buffer, int length) = 0;
  Reader & operator>>(PacketElement &e);
  template <typename T> void read(T* data) {
    read(data, sizeof(T));
  }
  template <typename T> Reader & operator>>(T &data) {
    read(&data, sizeof(T));
    return *this;
  }
};

class Writer {
public:
  virtual void write(const void *buffer, int length) = 0;
  Writer & operator<<(const PacketElement &e);
  template <typename T> void write(T* data) {
    write(data, sizeof(T));
  }
  template <typename T> Writer & operator<<(const T &data) {
    write(&data, sizeof(T));
    return *this;
  }
};

class PacketReader : public virtual Reader {
public:
  virtual void read_packet() = 0;
};

class PacketWriter : public virtual Writer {
public:
  virtual void write_packet() = 0;
};

class Buffer {
protected:
  typedef char buf_t;
public:
  buf_t *buf_start;
  buf_t *buf_current;
  buf_t *buf_end;
  Buffer(int size);
  virtual ~Buffer();
};

class BufferReader : public Buffer, public virtual Reader {
protected:
public:
  buf_t *read_end;
  BufferReader(int size);
  virtual void read(void *buffer, int length);
  virtual int remaining();
};

class BufferWriter : public Buffer, public virtual Writer {
public:
  BufferWriter(int size);
  virtual void write(const void *buffer, int length);
};

class UDPPacketReader : public BufferReader, public virtual PacketReader {
protected:
  Socket::sockfd_t socket;
  virtual int recv(void *data, int length);
public:
  UDPPacketReader(Socket &socket, int buf_size = DEFAULT_BUFFER_SIZE);
  UDPPacketReader(Socket::Socket::sockfd_t socket, int buf_size = DEFAULT_BUFFER_SIZE);
  virtual void read_packet();
};

class UDPAddrPacketWriter;

class UDPAddrPacketReader : public UDPPacketReader {
protected:
  virtual int recv(void *data, int length);
public:
  struct sockaddr_in reply_addr;
  UDPAddrPacketReader(Socket &socket, int buf_size = DEFAULT_BUFFER_SIZE);
  UDPAddrPacketReader(Socket::sockfd_t socket, int buf_size = DEFAULT_BUFFER_SIZE);
  UDPAddrPacketWriter getReplyPacketWriter(int buf_size = DEFAULT_BUFFER_SIZE);
};

class UDPPacketWriter : public BufferWriter, public virtual PacketWriter {
protected:
  Socket::sockfd_t socket;
  virtual void send(void *data, int length);
public:
  UDPPacketWriter(Socket &socket, int buf_size = DEFAULT_BUFFER_SIZE);
  UDPPacketWriter(Socket::sockfd_t socket, int buf_size = DEFAULT_BUFFER_SIZE);
  virtual void write_packet();
};

class UDPAddrPacketWriter : public UDPPacketWriter {
protected:
  virtual void send(void *data, int length);
public:
  struct sockaddr_in address;
  UDPAddrPacketWriter(const struct sockaddr_in &address, Socket &socket, int buf_size = DEFAULT_BUFFER_SIZE);
  UDPAddrPacketWriter(const struct sockaddr_in &address, Socket::sockfd_t socket, int buf_size = DEFAULT_BUFFER_SIZE);
};

class UDPSplitPacketWriter : public BufferWriter, public virtual PacketWriter {
protected:
  Socket::sockfd_t socket;
  int mtu;
  uint16_t id;
  uint16_t count;
  virtual void send(void *data, int length);
public:
  // MTU 2047
  UDPSplitPacketWriter(Socket &socket, uint16_t id = 0, int mtu = 2047, int buf_size = DEFAULT_BUFFER_SIZE);
  UDPSplitPacketWriter(Socket::sockfd_t socket, uint16_t id = 0, int mtu = 2047, int buf_size = DEFAULT_BUFFER_SIZE);
  virtual void write_packet();
};

class UDPSplitPacketReader : public virtual PacketReader {
protected:
  int recv(void *data, int length);
  class SplitPacket : public BufferReader {
  public:
    SplitPacket(int size);
    bool isStart();
    bool isEnd();
    bool isMiddle();
    bool isFull();
    int getCount();
    int getNextCount();
    int getPreviousCount();
    int getID();

    std::list<SplitPacket>::iterator next, previous, start;
    bool hasNext;
    bool hasPrevious;
    bool hasStart;
    bool hasEnd;
  };
  std::list<SplitPacket> receivedPackets;
  bool receivedPacket;
  std::list<SplitPacket>::iterator currentPacket;
  int maxReceivedPackets;
public:
  Socket::sockfd_t socket;
  int max;
  int mtu;
  UDPSplitPacketReader(Socket &socket, int max = 1000, int mtu = 2047);
  UDPSplitPacketReader(Socket::sockfd_t socket, int max = 1000, int mtu = 2047);
  void read(void *buffer, int length);
  virtual void read_packet();
};

class UDPSplitAddrPacketWriter;

class UDPSplitAddrPacketReader : public UDPSplitPacketReader {
protected:
  virtual int recv(void *data, int length);
public:
  UDPSplitAddrPacketReader(Socket &socket, int max = 1000, int mtu = 2047);
  UDPSplitAddrPacketReader(Socket::sockfd_t socket, int max = 1000, int mtu = 2047);
  struct sockaddr_storage reply_addr;
  UDPSplitAddrPacketWriter getReplyPacketWriter(int id = 0, int buf_size = DEFAULT_BUFFER_SIZE);
};

class UDPSplitAddrPacketWriter : public UDPSplitPacketWriter {
protected:
  virtual void send(void *data, int length);
public:
  struct sockaddr_storage address;
  // MTU 2047
  UDPSplitAddrPacketWriter(struct sockaddr_storage &address, Socket &socket, uint16_t id = 0, int mtu = 2047, int buf_size = DEFAULT_BUFFER_SIZE);
  UDPSplitAddrPacketWriter(struct sockaddr_storage &address, Socket::sockfd_t socket, uint16_t id = 0, int mtu = 2047, int buf_size = DEFAULT_BUFFER_SIZE);
};

#endif
