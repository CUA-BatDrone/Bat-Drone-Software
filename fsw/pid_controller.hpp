
template <typename T = float, int SMOOTHLEN = 4>
class PIDController {
public:
  T p, i, d, processMax, sum, sumMax, lastDeriv[SMOOTHLEN], lastError;
  int lastDerivIndex;
  PIDController(T p = 0, T i = 0, T d = 0, T processMax = 0.5, T lastError = 0, T sum = 0, T sumMax = 1) : p(p), i(i), d(d), processMax(processMax), sum(sum), sumMax(sumMax), lastError(lastError), lastDerivIndex(0) {
    for (int i = 0; i < SMOOTHLEN; i++) lastDeriv[i] = 0;
  }

  T derivative(T error, T time = 1) {
    T deriv = (lastError - error) / time;
    T r = deriv;
    if (SMOOTHLEN > 0) {
      for (int i = 0; i < SMOOTHLEN; i++) {
        r += lastDeriv[i];
      }
      if (lastDerivIndex + 1 < SMOOTHLEN) {
        lastDerivIndex++;
      } else {
        lastDerivIndex = 0;
      }
      lastDeriv[lastDerivIndex] = deriv;
      r /= SMOOTHLEN + 1;
    }
    return r;
  }

  T integral(T error, T time = 1) {
    sum = i != 0 ? sum + error * time : 0;
    if (sum > sumMax) sum = sumMax;
    else if (sum < -sumMax) sum = -sumMax;
    return sum;
  }

  T process(T error, T time = 1) {
    T r = p * error + d * derivative(error, time) + i * integral(error, time);
    if (r > processMax) r = processMax;
    else if (r < -processMax) r = -processMax;
    lastError = error;
    return r;
  }
};

//
//template <typename T>
//class PIDController {
//public:
//  T p, i, d, processMax, sum, sumMax, lastError, errorAvg;
//  PIDController(T p = 0, T i = 0, T d = 0, T processMax = 0.5, T lastError = 0, T sum = 0, T sumMax = 1) : p(p), i(i), d(d), processMax(processMax), lastError(lastError), sum(sum), sumMax(sumMax), errorAvg(lastError) {};
//  T process(T error, T time = 1) {
//    T r = p * error;
//    errorAvg = errorAvg * 0.9f + error * 0.1f;
//    r += d * (lastError - errorAvg) / time;
//    lastError = errorAvg;
//    sum = i != 0.0f ? sum + i * error * time : 0;
//    if (sum > sumMax) sum = sumMax;
//    else if (sum < -sumMax) sum = -sumMax;
//    r += sum;
//    if (r > processMax) r = processMax;
//    else if (r < -processMax) r = -processMax;
//    return r;
//  }
//};
