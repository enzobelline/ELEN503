// adder_thread1.h
#include "systemc.h"

SC_MODULE(adder_thread) {
  sc_in<int> A, B;
  sc_out<int> C;
  sc_in<bool> clk;

  void do_adder() {
    while (1) {
      C = A + B;
      wait();
    }
  }

  SC_CTOR(adder_thread) {
    SC_THREAD(do_adder);
    sensitive_pos(clk); // sensitive_pos << clk;
  }
};


