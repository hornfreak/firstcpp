#include "math_utils.h"
#include <stdexcept>

Calculator::Calculator() { lastResult = 0; }
double Calculator::subtract(int a, int b) {
  lastResult = a - b;
  return lastResult;
}

double Calculator::getLastResult() { return lastResult; }

double Calculator::divide(double a, double b) {
  if (b == 0) {
    throw std::invalid_argument("Division by zero is not allowed.");
    return 0;
  }
  lastResult = a / b;
  return lastResult;
}
