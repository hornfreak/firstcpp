#include "math_utils.h"
#include <fmt/core.h>
#include <iostream>

int main() {
  Calculator myCalc;

  try {
    double result = myCalc.divide(10, 3);

    // {:.2f} means format as a float with 2 decimals places:
    fmt::print("The result of 10 / 3 is: {:.2f}\n", result);

    // You can also use named placeholders or positional ones
    fmt::print("Stored state in memory: {:.4f}\n", myCalc.getLastResult());

  } catch (const std::invalid_argument &e) {
    fmt::print(stderr, "Error: {}\n", e.what());
  }

  return 0;
}
