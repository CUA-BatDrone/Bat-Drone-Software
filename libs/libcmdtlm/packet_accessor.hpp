#ifndef PACKET_ACCESSOR_HPP
#define PACKET_ACCESSOR_HPP

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <string.h>


#define MAX_PACKET_SIZE 64

class Reader {
public:
  virtual void read(void *buffer, int length) = 0;
  template <typename T> void read(T* data) {
    read(data, sizeof(T));
  }
};

class PacketReader : public Reader{
public:
  virtual int read_packet() = 0;
};

class Writer {
public:
  virtual void write(const void *buffer, int length) = 0;
  template <typename T> void write(T* data) {
    write((void *) data, sizeof(T));
  }
};

class PacketWriter : public Writer{
public:
  virtual void write_packet() = 0;
};

class PacketAccessor : public PacketReader, public PacketWriter {
};

class UDPPacketAccessor : public PacketAccessor {
private:
  void init();
  void createSocket();
  void bindSocket(int port);
  void connectSocket(const char *addr_str, int port);
  bool has_src_addr;
protected:
  int sockfd;
  char read_buffer[MAX_PACKET_SIZE];
  char *read_index;
  char *read_end;

  char write_buffer[MAX_PACKET_SIZE];
  char *write_index;

  struct sockaddr_storage src_addr;
public:
  // write_packet replies to last received packet sender address
  UDPPacketAccessor();
  UDPPacketAccessor(int bind_port);
  // write_packet replies to connected address
  UDPPacketAccessor(int bind_port, const char *conn_addr, int conn_port);
  ~UDPPacketAccessor();
  virtual int read_packet();
  virtual void read(void *buffer, int length);
  virtual void write(const void *buffer, int length);
  virtual void write_packet();
};

class LoopbackPacketAccessor : public PacketAccessor {
protected:
  char buffer[MAX_PACKET_SIZE];
  char *read_index;
  char *read_end;
  char *write_index;
public:
  LoopbackPacketAccessor();
  virtual int read_packet();
  virtual void read(void *buffer, int length);
  virtual void write(const void *buffer, int length);
  virtual void write_packet();
};

#endif
