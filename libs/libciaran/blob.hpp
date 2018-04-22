#ifndef BLOB_HPP
#define BLOB_HPP

#include <stdint.h>
#include <vector>

/**
 * Represents a detected blob
 */
class Blob {
public:
  // features
  int x, y, size;
  // feel free to add more fields to this class
};

vector<Blob> detectLWIRBlob(uint16_t image[60][80]);

#endif BLOB_HPP
