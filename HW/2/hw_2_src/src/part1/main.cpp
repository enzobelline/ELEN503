#include "systemc.h"
SC_MODULE(generator) {
  sc_out<short> sig;
  sc_out<short> buf;

  void do_it(void) {
    wait(5, SC_NS); //5 5,5
    sig.write(5);
    buf.write(5);
    short value = 10;  
    wait(10, SC_NS); //15 0,0
    
    // Modifying the value through the signal
    sig.write(5); // Update the value in the signal
    buf.write(5); // Update the value in the buffer
    
    // Additional code to demonstrate the difference between sc_signal and sc_buffer
    // Modifying the value through the signal
    sig.write(value); // Update the value in the signal
    buf.write(value); // Update the value in the buffer
    wait(10, SC_NS);    
    

    // Modifying the value through the buffer
    buf.write(value); // Update the value in the buffer
    value = 20; // Modify the local variable value
    wait(10, SC_NS);

    // Output the final values of the signal and buffer
    cout << "Final value of sig: " << sig.read() << endl;
    cout << "Final value of buf: " << buf.read() << endl;

    sc_stop(); // Stop simulation
  }

  SC_CTOR(generator) {
    SC_THREAD(do_it);
    sig.initialize(0);
    buf.initialize(0);
  }
};

SC_MODULE(receiver) {
  sc_in<short> iport;

  void do_it(void) {
    cout << sc_time_stamp() << ":" << name() << " got " << iport.read() << endl;
  }

  SC_CTOR(receiver) {
    SC_METHOD(do_it);
    sensitive << iport;
    dont_initialize();
  }
};

int sc_main(int argc, char *argv[]) {
  sc_signal<short> sig;
  sc_buffer<short> buf;

  generator GEN("GEN");
  GEN.sig(sig);
  GEN.buf(buf);

  receiver REV_SIG("REV_SIG");
  REV_SIG.iport(sig);
  receiver REV_BUF_A("REV_BUF");
  REV_BUF_A.iport(buf);

  // trace file creation
  sc_trace_file *tf = sc_create_vcd_trace_file("wave");
  sc_trace(tf, sig, "sig");
  sc_trace(tf, buf, "buf");

  sc_start();

  sc_close_vcd_trace_file(tf);
  return (0);
}