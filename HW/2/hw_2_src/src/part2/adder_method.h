// adder_method.h
#include "systemc.h"

SC_MODULE(adder_method) {
  sc_in<int> A, B;
  sc_out<int> C;

  void do_adder() { C = A + B; }

  SC_CTOR(adder_method) {
    SC_METHOD(do_adder);
    sensitive << A << B;
  }
};
