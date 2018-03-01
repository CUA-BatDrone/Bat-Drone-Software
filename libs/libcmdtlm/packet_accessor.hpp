#ifndef PACKET_ACCESSOR_HPP
#define PACKET_ACCESSOR_HPP

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
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
  virtual void write(void *buffer, int length) = 0;
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
protected:
  int sockfd;
  char read_buffer[MAX_PACKET_SIZE];
  char *read_index;
  char *read_end;

  char write_buffer[MAX_PACKET_SIZE];
  char *write_index;

  int socket;
  struct sockaddr_storage src_addr;
public:
  UDPPacketAccessor(int udp_socket);
  UDPPacketAccessor(char *address, char *port);
  virtual int read_packet();
  virtual void read(void *buffer, int length);
  virtual void write(const void *buffer, int length);
  virtual void write_packet();
};

#endif
