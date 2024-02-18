// stimulus.h
#include "systemc.h"

SC_MODULE(stimulus) {
  sc_out<int> A, B;
  sc_in<bool> clk;

  void do_stim() {
    while (1) {
      A = 10; B = 20;
      wait();
      A = 13; B = 11;
      wait(); wait();
      A = 1; B = 3;
      wait();
      A = 11;
      wait();
      A = 100; B = 1;
      wait(); wait(); wait();
    }
  }



  SC_CTOR(stimulus) {
    SC_THREAD(do_stim);
    sensitive << clk;
  }
};

