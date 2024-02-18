// adder_cthread.h
#include "systemc.h"

SC_MODULE(adder_cthread) {
  sc_in<int> A, B;
  sc_out<int> C;
  sc_in<bool> clk;

  void do_adder() {
    while (1) {
      C = A + B;
      wait();
    }
  }

  SC_CTOR(adder_cthread) {
    SC_CTHREAD(do_adder, clk.pos());
  }
};
 

