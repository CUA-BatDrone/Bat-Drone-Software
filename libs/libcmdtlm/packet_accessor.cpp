
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#endif

#include <string.h>
#include "packet_accessor.hpp"


UDPPacketAccessor::UDPPacketAccessor(int socket) {
  read_index = read_buffer;
  read_end = read_buffer;
  write_index = write_buffer;
  this->socket = socket;
}
int UDPPacketAccessor::read_packet() {
  read_index = read_buffer;
  socklen_t addrlen = sizeof(src_addr);
  int length = 0;
  while (length = recvfrom(sockfd, read_buffer, MAX_PACKET_SIZE, 0,(sockaddr *) &src_addr, &addrlen) == 0);
  read_end = read_buffer + length;
  return length;
}
void UDPPacketAccessor::read(void *buffer, int length) {
  char *next = read_index + length;
  if (next > read_end) {
    throw "Read out of range of UDP packet";
  }
  memcpy(buffer, read_index, length);
  read_index = next;
}
void UDPPacketAccessor::write(const void *buffer, int length) {
  char *next = write_index + length;
  if (next - write_buffer > MAX_PACKET_SIZE) {
    write_index = write_buffer;
    throw "Write out of range of UDP packet";
  }
  memcpy(write_index, buffer, length);
  write_index = next;
}
void UDPPacketAccessor::write_packet() {
  socklen_t addrlen = sizeof(src_addr);
  sendto(sockfd, write_buffer, (int) (write_index - write_buffer), 0, (sockaddr *) &src_addr, addrlen);
  write_index = write_buffer;
}

LoopbackPacketAccessor::LoopbackPacketAccessor() {
  read_index = read_end = write_index = buffer;
}
void LoopbackPacketAccessor::read(void *buffer, int length) {
  char *next = read_index + length;
  if (next > read_end) {
    throw "Read out of range of UDP packet";
  }
  memcpy(buffer, read_index, length);
  read_index = next;
}
void LoopbackPacketAccessor::write(const void *buffer, int length) {
  char *next = write_index + length;
  if (next - this->buffer > MAX_PACKET_SIZE) {
    write_index = this->buffer;
    throw "Write out of range of max packet size";
  }
  memcpy(write_index, buffer, length);
  write_index = next;
}
void LoopbackPacketAccessor::write_packet() {
  read_end = write_index;
  write_index = buffer;
}
int LoopbackPacketAccessor::read_packet() {
  read_index = buffer;
  return read_end - read_index;
}
