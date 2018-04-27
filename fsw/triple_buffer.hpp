#ifndef TRIPPLE_BUFFER_HPP
#define TRIPPLE_BUFFER_HPP

#include <mutex>
#include <condition_variable>

/**
 * Manages a tripple buffer of type T. Front is written, and back is read.
 * One thread writes to the front buffer using getFront(), then calls swapFront.
 * Another thread calls swapBack and reads the back buffer using getBack().
 */
template <class T> 
class TripleBuffer {
protected:
  T buffer[3], *front, *middle, *back;
  mutex m;
  condition_variable c;
  int count;
  void swap(T *&a, T *&b) {
    T* temp = a;
    a = b;
    b = temp;
  }
public:
  TripleBuffer() : count(0) {
    front = buffer;
    middle = buffer + 1;
    back = buffer + 2;
  }
  T & getFront() {
    return *front;
  }
  T & getBack() {
    return *back;
  }
  /**
  * Swaps the front buffer with the middle buffer.
  */
  void swapFront() {
    unique_lock<mutex> ul(m);
    swap(front, middle);
    count++;
    c.notify_all();
  }
  /**
   * Waits for swapFront() to be called at least once, then swaps the back buffer with the middle buffer.
   * @return The number of times swapFront() was called.
   */
  int swapBack() {
    unique_lock<mutex> ul(m);
    while (count == 0) c.wait(ul);
    swap(back, middle);
    int r = count;
    count = 0;
    return r;
  }
  /**
   * Swaps the back buffer with the middle buffer if swapFront() has been called at least once.
   * @return The number of times swapFront() was called.
   */
  int swapBackIfReady() {
    unique_lock<mutex> ul(m);
    if (count > 0) swap(back, middle);
    int r = count;
    count = 0;
    return r;
  }
};

#endif