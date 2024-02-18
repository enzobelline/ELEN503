// Code your testbench here
// or browse Examples
#include <systemc.h>

class write_if : virtual public sc_interface{
public:
    virtual void write(int) = 0;
    virtual void reset() = 0;
};

class read_if : virtual public sc_interface{
public:
    virtual void read(int &) = 0;
    virtual int num_available() = 0;
};

class fifo : public sc_channel, public write_if, public read_if{
public:
    fifo(sc_module_name name) : sc_channel(name), num_elements(0), first(0) {}
    void write(int c) {
        if (num_elements == max)
            wait(read_event);
        data[(first + num_elements) % max] = c;
        ++ num_elements;
        write_event.notify();
    }

    void read(int &c){
        if (num_elements == 0)
            wait(write_event);
        c = data[first];
        -- num_elements;
        first = (first + 1) % max;
        read_event.notify();
    }

    void reset() { num_elements = first = 0; }
    int num_available() { return num_elements;}

private:
    enum e { max = 10 };
    int data[max];
    int num_elements, first;
    sc_event write_event, read_event;
};

class fibonacci_gen : public sc_module{
public:
    sc_port<write_if> out;
    SC_HAS_PROCESS(fibonacci_gen);
    fibonacci_gen(sc_module_name name) : sc_module(name){
        SC_THREAD(main);
    }

    void main(){
        int first = 0, second = 1, next = 0;
        for (int i = 0; i < 10; i++){
            if(i <= 1)
                next = i;
            else {
                next = first + second;
                first = second;
                second = next;
            }
            out->write(next);
        }
    }
};

class consumer : public sc_module{
public:
    sc_port<read_if> in;
    SC_HAS_PROCESS(consumer);
    consumer(sc_module_name name) : sc_module(name) {
        SC_THREAD(main);
    }
    void main(){
        int c;
        cout << endl << "Fibonacci sequence: " << endl;
        while (true) {
            in->read(c);
            cout << c << endl;
            if(in->num_available() == 0)
                break;
        }
    }
};

class top : public sc_module{
public:
    fifo *fifo_inst;
    fibonacci_gen *prod_inst;
    consumer *cons_inst;
    top(sc_module_name name) : sc_module(name){
        fifo_inst = new fifo("Fifo1");
        prod_inst = new fibonacci_gen("FibonacciGen1");
        prod_inst->out(*fifo_inst);
        cons_inst = new consumer("Consumer1");
        cons_inst->in(*fifo_inst);
    }
};

int sc_main (int, char *[]) {
    top top1("Top1");
    sc_start();
    return 0;
}
