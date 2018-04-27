
template <typename T>
class PIDController {
public:
  T p, i, d, sumMax, sum, errorMax;
  PIDController(T p = 0, T i = 0, T d = 0, T lastError = 0, T sum = 0, T sumMax = 1) : p(p), i(i), d(d), lastError(lastError), sum(sum), sumMax(i_max) {};
  T process(T error, T time = 1) {
    T r = p * error;
    r += d * (lastError - error) / time;
    lastError = error;
    sum += i * error * time;
    if (sum > sumMax) sum = sumMax;
    r += sum;
    return r;
  }
};