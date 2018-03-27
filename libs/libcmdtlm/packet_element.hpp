#ifndef PACKET_ELEMENT_HPP
#define PACKET_ELEMENT_HPP

// #include "packet_elements.hpp"

class Writer;
class Reader;

class PacketElement {
public:
  virtual void write(Writer *) const = 0;
  virtual void read(Reader *) = 0;
};

#endif
