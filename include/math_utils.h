#pragma once

class Calculator {
public:
  Calculator(); // The Constructor declaration
  double subtract(int a, int b);
  double divide(double a, double b);
  double getLastResult(); // A "getter" function to see the result

private:
  double lastResult; // Member variable (state)
};
