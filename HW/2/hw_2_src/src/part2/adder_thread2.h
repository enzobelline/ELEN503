// adder_thread.h
#include "systemc.h"

SC_MODULE(adder_thread2) {
  sc_in<int> A, B;
  sc_out<int> C;

  void do_adder() {
	while (1) {
  	C = A + B;
  	wait();
	}
  }

  SC_CTOR(adder_thread2) {
	SC_THREAD(do_adder);
	sensitive(A); sensitive(B);
  }
};



