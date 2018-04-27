
template <typename T>
class PIDController {
public:
  T p, i, d, processMax, sum, sumMax, lastError;
  PIDController(T p = 0, T i = 0, T d = 0, T processMax = 1 , T lastError = 0, T sum = 0, T sumMax = 1) : p(p), i(i), d(d), processMax(processMax), lastError(lastError), sum(sum), sumMax(sumMax) {};
  T process(T error, T time = 1) {
    T r = p * error;
    r += d * (lastError - error) / time;
    lastError = error;
    sum += i * error * time;
    if (sum > sumMax) sum = sumMax;
    else if (sum < -sumMax) sum = -sumMax;
    r += sum;
    if (r > processMax) r = processMax;
    else if (r < -processMax) r = -processMax;
    return r;
  }
};