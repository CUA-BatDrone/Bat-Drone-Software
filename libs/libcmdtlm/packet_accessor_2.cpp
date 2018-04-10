#include "packet_accessor_2.hpp"
#include <string.h>
#include <string>
#include <errno.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#endif
#include <iostream>

UDPSocket::UDPSocket() {
#ifdef _WIN32
  WSADATA d = WSADATA();
  int err = WSAStartup(MAKEWORD(2, 2), &d);
  if (err != 0) {
    throw std::string("WSAStartup failed with error ") + std::to_string(err);
  }
#endif
  if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
#ifdef _WIN32
    throw std::string("UDPSocket::UDPSocket(): ") + std::string("WSA Error: ")  + std::to_string(WSAGetLastError());
#else
    throw std::string("UDPSocket::UDPSocket():") + std::string(strerror(errno));
#endif
  }
}

UDPSocket::~UDPSocket() {
#ifdef _WIN32
  closesocket(sockfd);
  WSACleanup();
#else
  close(sockfd);
#endif
}

sockaddr_storage Socket::stringToAddr(const char *addr_in, int port) {
  struct sockaddr_storage storage = {};
  struct sockaddr_in &addr_out = (struct sockaddr_in &) storage;
  addr_out.sin_family = AF_INET;
  addr_out.sin_port = htons(port);
  inet_pton(AF_INET, addr_in, &addr_out.sin_addr);
  return storage;
}

void UDPSocket::bind(int port) {
  sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  if (::bind(sockfd, (sockaddr *) &addr, sizeof(addr)) < 0) {
#ifdef _WIN32
    throw std::string("WSA Error: ")  + std::to_string(WSAGetLastError());
#else
    throw std::string(strerror(errno));
#endif
  }
}

void UDPSocket::connect(sockaddr_storage addr) {
  if (::connect(sockfd, (sockaddr *) &addr, sizeof(addr)) < 0) {
#ifdef _WIN32
    throw std::string("UDPSocket::UDPSocket()") + std::string("WSA Error: ")  + std::to_string(WSAGetLastError());
#else
    throw std::string(strerror(errno));
#endif
  }
}

void UDPSocket::connect(const char *addr, int port) {
  connect(stringToAddr(addr, port));
}

void UDPSocket::add(in_addr grpaddr, in_addr srcaddr) {
  struct ip_mreq_source imr = {};
  imr.imr_multiaddr = grpaddr;
  imr.imr_sourceaddr = srcaddr;
  imr.imr_interface.s_addr = INADDR_ANY;
#ifdef _WIN32
  setsockopt(sockfd, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (const char *) &imr, sizeof(imr));
#else
  setsockopt(sockfd, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (void *) &imr, sizeof(imr));
#endif
}

void UDPSocket::add(in_addr grpaddr) {
  struct ip_mreq imr = {};
  imr.imr_multiaddr = grpaddr;
  imr.imr_interface.s_addr = INADDR_ANY;
#ifdef _WIN32
  if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char *)&imr, sizeof(imr)) < 0) {
    throw std::string("UDPSocket::UDPSocket()") + std::string("WSA Error: ")  + std::to_string(WSAGetLastError());
  }
#else
  if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&imr, sizeof(imr)) < 0) {
    throw std::string(strerror(errno));
  }
#endif
}

void UDPSocket::add(const char *group, const char *source) {
  in_addr grpaddr = {};
  in_addr srcaddr = {};
  inet_pton(AF_INET, group, &grpaddr);
  inet_pton(AF_INET, source, &srcaddr);
  add(grpaddr, srcaddr);
}

void UDPSocket::add(const char *group) {
  in_addr grpaddr = {};
  inet_pton(AF_INET, group, &grpaddr);
  add(grpaddr);
}

void UDPSocket::drop(in_addr grpaddr, in_addr srcaddr) {
  struct ip_mreq_source imr = {};
  imr.imr_multiaddr = grpaddr;
  imr.imr_sourceaddr = srcaddr;
  imr.imr_interface.s_addr = INADDR_ANY;
#ifdef _WIN32
  if (setsockopt(sockfd, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP, (const char *)&imr, sizeof(imr)) < 0) {
    throw std::string("UDPSocket::UDPSocket()") + std::string("WSA Error: ")  + std::to_string(WSAGetLastError());
  }
#else
  if (setsockopt(sockfd, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP, (void *)&imr, sizeof(imr)) < 0) {
    throw std::string(strerror(errno));
  }
#endif
}

void UDPSocket::drop(in_addr grpaddr) {
  struct ip_mreq imr = {};
  imr.imr_multiaddr = grpaddr;
  imr.imr_interface.s_addr = INADDR_ANY;
#ifdef _WIN32
  setsockopt(sockfd, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP, (const char *) &imr, sizeof(imr));
#else
  setsockopt(sockfd, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP, (void *) &imr, sizeof(imr));
#endif
}

void UDPSocket::drop(const char *group, const char *source) {
  in_addr grpaddr = {};
  in_addr srcaddr = {};
  inet_pton(AF_INET, group, &grpaddr);
  inet_pton(AF_INET, source, &srcaddr);
  drop(grpaddr, srcaddr);
}

void UDPSocket::drop(const char *group) {
  in_addr grpaddr = {};
  inet_pton(AF_INET, group, &grpaddr);
  drop(grpaddr);
}

unsigned int UDPSocket::getMTU() {
  unsigned int v;
  unsigned int l = sizeof(v);
#ifdef _WIN32
  getsockopt(sockfd, IPPROTO_IP, IP_MTU, (char *) &v, (int *) &l);
#else
  getsockopt(sockfd, IPPROTO_IP, IP_MTU, &v, &l);
#endif
  return v;
}

void UDPSocket::setMTUDiscovery(enum IP_PMTUDISC_ENUM e) {
  unsigned int v;
  unsigned int l = sizeof(v);
  switch (e) {
  case WANT:
#ifdef _WIN32
    return;
#else
    v = IP_PMTUDISC_WANT;
    break;
#endif
  case DONT:
    v = IP_PMTUDISC_DONT;
    break;
  case DO:
    v = IP_PMTUDISC_DO;
    break;
  case PROBE:
    v = IP_PMTUDISC_PROBE;
  }
  setsockopt(sockfd, IPPROTO_IP, IP_MTU_DISCOVER, (char *)&v, l);
}


Reader & Reader::operator>>(PacketElement &e) {
  e.read(this);
  return *this;
}


Writer & Writer::operator<<(const PacketElement &e) {
  e.write(this);
  return *this;
}


Buffer::Buffer(int size) {
  buf_start = buf_current = new buf_t [size];
  buf_end = buf_start + size;
}

Buffer::~Buffer() {
  delete [] buf_start;
}


BufferReader::BufferReader(int size) : Buffer(size) {
  read_end = buf_start;
}

void BufferReader::read(void *data, int length) {
  buf_t *buf_next = buf_current + length;
  if (buf_next > read_end) {
    throw std::string("BufferReader::read: Read out of range");
  }
  memcpy(data, buf_current, length);
  buf_current = buf_next;
}

int BufferReader::remaining() {
  return read_end - buf_current;
}


BufferWriter::BufferWriter(int size) : Buffer(size) {}

void BufferWriter::write(const void *data, int length) {
  buf_t *buf_next = buf_current + length;
  if (buf_next > buf_end) {
    throw std::string("BufferWriter::write: Write out of range");
  }
  memcpy(buf_current, data, length);
  buf_current = buf_next;
}


UDPPacketReader::UDPPacketReader(Socket &socket, int buf_size) : UDPPacketReader(socket.sockfd, buf_size) {}

UDPPacketReader::UDPPacketReader(Socket::sockfd_t socket, int buf_size) : BufferReader(buf_size) {
  this->socket = socket;
}

int UDPPacketReader::recv(void *data, int length) {
  if ((length = ::recv(socket, (char *) data, length, 0)) < 0) {
    buf_current = buf_start;
    throw std::string(strerror(errno));
  }
  return length;
}

void UDPPacketReader::read_packet() {
  int length;
  length = recv(buf_start, buf_end - buf_start);
  read_end = buf_start + length;
  buf_current = buf_start;
}


UDPAddrPacketReader::UDPAddrPacketReader(Socket &socket, int buf_size) : UDPAddrPacketReader(socket.sockfd, buf_size) {}

UDPAddrPacketReader::UDPAddrPacketReader(Socket::sockfd_t socket, int buf_size) : UDPPacketReader(socket, buf_size) {
  this->socket = socket;
}

int UDPAddrPacketReader::recv(void *data, int length) {
  socklen_t addrlen = sizeof(reply_addr);
  if ((length = ::recvfrom(socket, (char *) data, length, 0, (sockaddr *) &reply_addr, &addrlen)) < 0) {
    buf_current = buf_start;
    throw std::string(strerror(errno));
  }
  return length;
}

UDPAddrPacketWriter UDPAddrPacketReader::getReplyPacketWriter(int buf_size) {
  return UDPAddrPacketWriter(reply_addr, socket, buf_size);
}


UDPPacketWriter::UDPPacketWriter(Socket &socket, int buf_size) : UDPPacketWriter(socket.sockfd, buf_size) {}

UDPPacketWriter::UDPPacketWriter(Socket::sockfd_t socket, int buf_size) : BufferWriter(buf_size) {
  this->socket = socket;
}

void UDPPacketWriter::send(void *data, int length) {
  if (::send(socket, (char *) data, length, 0) < 0) {
    throw std::string(strerror(errno));
  }
}

void UDPPacketWriter::write_packet() {
  send(buf_start, buf_current - buf_start);
  buf_current = buf_start;
}



UDPAddrPacketWriter::UDPAddrPacketWriter(const struct sockaddr_storage &address, Socket &socket, int buf_size) : UDPAddrPacketWriter(address, socket.sockfd, buf_size) {}

UDPAddrPacketWriter::UDPAddrPacketWriter(const struct sockaddr_storage &address, Socket::sockfd_t socket, int buf_size) : UDPPacketWriter(socket, buf_size) {
  this->socket = socket;
  this->address = address;
}

void UDPAddrPacketWriter::send(void *data, int length) {
  if (::sendto(socket, (char *) data, length, 0, (sockaddr *) &address, sizeof(address)) < 0) {
#ifdef _WIN32
    throw std::string("WSA Error: ") + std::to_string(WSAGetLastError());
#else
    throw std::string(strerror(errno));
#endif
  }
}



UDPSplitPacketReader::SplitPacket::SplitPacket(int size) : BufferReader(size) {
  hasNext = hasPrevious = hasStart = hasEnd = false;
}

bool UDPSplitPacketReader::SplitPacket::isStart() {
  return (((uint16_t *) buf_start)[1] & 0xC000) == 0x4000;
}

bool UDPSplitPacketReader::SplitPacket::isEnd() {
  return (((uint16_t *) buf_start)[1] & 0xC000) == 0x8000;
}

bool UDPSplitPacketReader::SplitPacket::isMiddle() {
  return (((uint16_t *) buf_start)[1] & 0xC000) == 0xC000;
}

bool UDPSplitPacketReader::SplitPacket::isFull() {
  return (((uint16_t *) buf_start)[1] & 0xC000) == 0x0000;
}

int UDPSplitPacketReader::SplitPacket::getCount() {
  return ((uint16_t *) buf_start)[1] & 0x3FFF;
}

int UDPSplitPacketReader::SplitPacket::getNextCount() {
  return getCount() + 1 & 0x3FFF;
}

int UDPSplitPacketReader::SplitPacket::getPreviousCount() {
  return getCount() - 1 & 0x3FFF;
}

int UDPSplitPacketReader::SplitPacket::getID() {
  return ((uint16_t *) buf_start)[0];
}


UDPSplitPacketReader::UDPSplitPacketReader(Socket &socket, int max, int mtu) : UDPSplitPacketReader(socket.sockfd, max, mtu) {}

UDPSplitPacketReader::UDPSplitPacketReader(Socket::sockfd_t socket, int max, int mtu) {
  this->socket = socket;
  this->max = max;
  this->mtu = mtu;
  receivedPacket = false;
}

int UDPSplitPacketReader::recv(void *data, int length) {
  while (true) {
    if ((length = ::recv(socket, (char *)data, length, 0)) < 0) {
#ifdef _WIN32
      if (WSAGetLastError() == WSAECONNRESET) {
        std::cout << "reset" << std::endl;
        continue;
      } else {
        throw std::string("WSA Error: ") + std::to_string(WSAGetLastError());
      }
#else
      throw std::string(strerror(errno));
#endif
    } else {
      break;
    }
  }
  return length;
}

void UDPSplitPacketReader::read(void *buffer, int length) {
  if(receivedPacket) {
    while(length > 0) {
      int currentLength = currentPacket->remaining();
      if (currentLength == 0) {
        currentPacket = currentPacket->next;
        currentLength = currentPacket->remaining();
      }
      if (currentLength < length) {
        if (!currentPacket->hasNext) {
          throw std::string("UDPSplitPacketReader::read: no more packets to read");
        }
        currentPacket->read(buffer, currentLength);
        buffer = (char *) buffer + currentLength;
        length -= currentLength;
      } else {
        currentPacket->read(buffer, length);
        break;
      }
    }
  } else {
    throw std::string("UDPSplitPacketReader::read: no packets read");
  }
}

void UDPSplitPacketReader::read_packet() {
  while(1) {
    // erase old packets
    if (receivedPackets.size() >= max || receivedPacket) {
      std::list<SplitPacket>::iterator current;
      if(receivedPacket) {
        current = currentPacket;
        receivedPacket = false;
      } else {
        current = --receivedPackets.end();
      }
      if (current->hasNext) {
        std::list<SplitPacket>::iterator it;
        it = current->next;
        while (it->hasNext) {
          std::list<SplitPacket>::iterator temp;
          temp = it->next;
          receivedPackets.erase(it);
          it = temp;
        }
        receivedPackets.erase(it);
      }
      if (current->hasPrevious) {
        std::list<SplitPacket>::iterator it;
        it = current->previous;
        while (it->hasPrevious) {
          std::list<SplitPacket>::iterator temp;
          temp = it->previous;
          receivedPackets.erase(it);
          it = temp;
        }
        receivedPackets.erase(it);
      }
      receivedPackets.erase(current);
    }

    // create packet
    receivedPackets.emplace_front(mtu);
    std::list<SplitPacket>::iterator current;
    current = receivedPackets.begin();

    // read until copy not read;
    while (1) {
      // read packet
      int length = recv(current->buf_start, current->buf_end - current->buf_start);
      current->read_end = current->buf_start + length;
      current->buf_current += 4;

      //std::cout << current->getCount() << std::endl;
      //std::cout << *((uint32_t *)current->buf_start) << std::endl;

      // find copies
      std::list<SplitPacket>::iterator copy;
      for (copy = ++receivedPackets.begin(); copy != receivedPackets.end(); copy++) {
        if ((current->getID() == copy->getID()) && (current->getCount() == copy->getCount())) {
          break;
        }
      }
      // if copy found
      if (copy != receivedPackets.end()) {
        // replace old with new;
        if (copy->hasNext) {
          current->hasNext = true;
          current->next = copy->next;
          current->next->previous = current;
        }
        if (copy->hasPrevious) {
          current->hasPrevious = true;
          current->previous = copy->previous;
          current->previous->next = current;
        }
        current->hasStart = copy->hasStart;
        current->start = copy->start;
        current->hasEnd = copy->hasEnd;
        receivedPackets.erase(copy);
      } else {
        break;
      }
    }

    // if full packet
    if (current->isFull()) {
      currentPacket = current;
      receivedPacket = true;
      return;
    }

    // setup next and previous links
    // for start and middle packets
    if (!current->isEnd()) {
      // find the next one.
      std::list<SplitPacket>::iterator next;
      // findNext
      for (next = ++receivedPackets.begin(); next != receivedPackets.end(); next++) {
        if ((current->getID() == next->getID()) && (current->getNextCount() == next->getCount())) {
          break;
        }
      }
      // next found
      if (next != receivedPackets.end()) {
        next->hasPrevious = true;
        next->previous = current;
        current->hasNext = true;
        current->next = next;
        current->hasEnd = next->hasEnd;
      }
    // for end packets
    } else {
      current->hasEnd = true;
    }
    // for middle and end packets
    if (!current->isStart()) {
      std::list<SplitPacket>::iterator previous;
      // findPrevious
      // previous = findPrevious(current);
      for (previous = ++receivedPackets.begin(); previous != receivedPackets.end(); previous++) {
        if ((current->getID() == previous->getID()) && (current->getPreviousCount() == previous->getCount())) {
          break;
        }
      }
      // if previous found
      if (previous != receivedPackets.end()) {
        previous->hasNext = true;
        previous->next = current;
        current->hasPrevious = true;
        current->previous = previous;
        if(previous->hasStart) {
          current->hasStart = true;
          current->start = previous->start;
        } else if (previous->isStart()) {
          current->hasStart = true;
          current->start = previous;
        }
      }
    // for start packets
    } else {
      current->start = current;
      current->hasStart = true;
    }

    // shortcut
    if(current->hasStart && current->hasEnd) {
      currentPacket = current->start;
      receivedPacket = true;
      return;
    }

    // test if all packet available and propogate values of hasEnd and hasStart
    // if connected to or is a start packet
    if (current->hasStart) {
      std::list<SplitPacket>::iterator it;
      it = current;
      // Indicate to following packet that there is a start
      // or return if found an end
      while(it->hasNext) {
        it = it->next;
        if (it->hasEnd) {
          currentPacket = current->start;
          receivedPacket = true;
          return;
        }
        it->hasStart = true;
        it->start = current->start;
      }
    }
    // if connected to end packet
    if (current->hasEnd) {
      std::list<SplitPacket>::iterator it;
      it = current;
      while(it->hasPrevious) {
        it = it->previous;
        if (it->hasStart) {
          currentPacket = it->start;
          receivedPacket = true;
          return;
        }
        it->hasEnd = true;
      }
    }

  }
}



UDPSplitAddrPacketReader::UDPSplitAddrPacketReader(Socket &socket, int max, int mtu) : UDPSplitPacketReader(socket.sockfd, max, mtu) {}

UDPSplitAddrPacketReader::UDPSplitAddrPacketReader(Socket::sockfd_t socket, int max, int mtu) : UDPSplitPacketReader(socket, max, mtu) {}

int UDPSplitAddrPacketReader::recv(void *data, int length) {
  socklen_t addrlen = sizeof(reply_addr);
  if ((length = ::recvfrom(socket, (char *) data, length, 0, (sockaddr *) &reply_addr, &addrlen)) < 0) {
#ifdef _WIN32
    throw std::string("WSA Error: ")  + std::to_string(WSAGetLastError());
#else
    throw std::string(strerror(errno));
#endif
  }
  return length;
}

UDPSplitAddrPacketWriter UDPSplitAddrPacketReader::getReplyPacketWriter(int id, int buf_size) {
  return UDPSplitAddrPacketWriter(reply_addr, socket, id, mtu, buf_size);
}


UDPSplitPacketWriter::UDPSplitPacketWriter(Socket &socket, uint16_t id, int mtu, int buf_size) : UDPSplitPacketWriter(socket.sockfd, id, mtu, buf_size) {}

UDPSplitPacketWriter::UDPSplitPacketWriter(Socket::sockfd_t socket, uint16_t id, int mtu, int buf_size) : BufferWriter(buf_size) {
  this->mtu = mtu;
  this->id = id;
  this->socket = socket;
  count = 0;
  buf_current += sizeof(id) + sizeof(count);
}

void UDPSplitPacketWriter::send(void *data, int length) {
  if (::send(socket, (char *) data, length, 0) < 0) {
#ifdef _WIN32
    throw std::string("WSA Error: ")  + std::to_string(WSAGetLastError());
#else
    throw std::string(strerror(errno));
#endif
  }
}

void UDPSplitPacketWriter::write_packet() {
  int header_size = sizeof(id) + sizeof(count);
  // great... buffer needs to be split
  if (buf_current - buf_start > mtu) {

    // Send a start packet
    // Write multiplex header
    memcpy(buf_start, &id, sizeof(id));
    // type and count are merged together
    uint16_t type_count = count | 0x4000;
    memcpy(buf_start + sizeof(id), &type_count, sizeof(type_count));
    send(buf_start, mtu);
    count = (count + 1) & 0x3FFFF;

    // Send middle packets
    buf_t *current;
    for (current = buf_start + mtu - header_size; current + mtu - header_size < buf_end; current += mtu - header_size) {
      // Write multiplex header
      memcpy(current, &id, sizeof(id));
      // type and count are merged together
      uint16_t type_count = count | 0xC000;
      memcpy(current + sizeof(id), &type_count, sizeof(type_count));
      send(current, mtu);
      count = (count + 1) & 0x3FFFF;
    }

    // Send end packet;
    memcpy(current, &id, sizeof(id));
    // type and count are merged together
    type_count = count | 0x8000;
    memcpy(current + sizeof(id), &type_count, sizeof(type_count));
    send(current, buf_end - current);
    count = (count + 1) & 0x3FFFF;

  // Sweet, buffer doesn't need to be split!
  } else {
    // Send a full packet
    memcpy(buf_start, &id, sizeof(id));
    // type and count are merged together
    uint16_t type_count = count;
    memcpy(buf_start + sizeof(id), &type_count, sizeof(type_count));
    send(buf_start, buf_current - buf_start);
    count = (count + 1) & 0x3FFFF;
  }
  buf_current = buf_start + header_size;
}


UDPSplitAddrPacketWriter::UDPSplitAddrPacketWriter(struct sockaddr_storage &address, Socket &socket, uint16_t id, int mtu, int buf_size) : UDPSplitAddrPacketWriter(address, socket.sockfd, id, mtu, buf_size) {
  this->address = address;
}

UDPSplitAddrPacketWriter::UDPSplitAddrPacketWriter(struct sockaddr_storage &address, Socket::sockfd_t socket, uint16_t id, int mtu, int buf_size) : UDPSplitPacketWriter(socket, id, mtu, buf_size) {
  this->address = address;
}

void UDPSplitAddrPacketWriter::send(void *data, int length) {
  if (::sendto(socket, (char *) data, length, 0, (sockaddr *) &address, sizeof(address)) < 0) {
#ifdef _WIN32
    throw std::string("WSA Error: ")  + std::to_string(WSAGetLastError());
#else
    throw std::string(strerror(errno));
#endif
  }
}
