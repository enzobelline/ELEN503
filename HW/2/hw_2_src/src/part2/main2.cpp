// main.cpp
#include "adder_method.h"
#include "adder_thread1.h"
#include "adder_thread2.h"
#include "adder_thread3.h"
#include "adder_cthread.h"
#include "stimulus.h"
int sc_main(int argc, char *argv[]) {
  sc_signal<int> A, B, R_M, R_T1, R_T2, R_T3, R_C;
  sc_set_time_resolution(1, SC_NS); // V2.0
  sc_set_default_time_unit(1, SC_NS);
  sc_clock clock("clock", 20, SC_NS, 0.5, 15, SC_NS);

  // example: adder_method  
  adder_method AD_M("adder_method"); // instantiation
  AD_M(A, B, R_M);                   // port connection
  // instantiate other adders too
  // put your code here ...


  stimulus STIM("stimulus");  // instantiation
  STIM(A, B, clock); // port connection

  // trace file creation
  sc_trace_file *tf = sc_create_vcd_trace_file("wave");
  sc_trace(tf, clock, "clock");
  sc_trace(tf, A,     "A");
  sc_trace(tf, B,     "B");
  sc_trace(tf, R_M,   "R_M");
  sc_trace(tf, R_T1,  "R_T1");
  sc_trace(tf, R_T2,  "R_T2");
  sc_trace(tf, R_T3,  "R_T3");
  sc_trace(tf, R_C,   "R_C");
  sc_start(80, SC_NS);
  return(0);
}


