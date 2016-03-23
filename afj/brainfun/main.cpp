#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void Fatal(string s) {
    cerr << "FATAL: " << s << endl;
    exit(1);
}

void Log(string s) {
    clog << "LOG: " << s << endl;
}

class Machine {
public:

    static void Run(string source, string input = ""){
        Machine m;
        Instructions in;

        m.parseSource(source, in);
        m.readInput(input);
        m.exec(in, 0, in.size());
        m.writeOutput();
    }

private:

    Machine() {
        input.reserve(1024);
        output.reserve(1024);
    }

    using byte = unsigned char;

    enum instruction {
        INC_DPTR = 1,
        DEC_DPTR,
        READ,
        WRITE,
        INC,
        DEC,
        NUL,
        NEG,
        BLOCK_BEGIN,
        BLOCK_END
    };

    using Instructions = vector<instruction>;

    void parseSource(string &source, Instructions &inst) {

        ifstream in(source);
        if (!in.is_open()) {
            Fatal("cannot open file " + source);
        }

        // b checks for brackets pairs
        int b = 0;

        for(;;) {
            if(in.eof()){
                break;
            }
            byte t = 0;
            in >> t;

            instruction i;
            switch(t){
            case '>': i = INC_DPTR; break;
            case '<': i = DEC_DPTR; break;
            case 'R': i = READ; break;
            case 'W': i = WRITE; break;
            case '+': i = INC; break;
            case '-': i = DEC; break;
            case 'N': i = NUL; break;
            case '!': i = NEG; break;
            case '[': i = BLOCK_BEGIN; b++; break;
            case ']': i = BLOCK_END; b--; break;
            default: continue;
            }
            inst.push_back(i);
        }

        if(b != 0) {
            Fatal("not valid program");
        }
    }

    // findBlockEnd finds pair to '[' and return its posxition
    int findBlockEnd(Instructions& in, int i, int to) {
        int b = 0;
        for(;i < to; i++) {
            instruction ins = in[i];

            if(ins == BLOCK_BEGIN) {
                b++;
            } else if(ins == BLOCK_END) {
                b--;
            }

            // we found pair
            if(b == 0) {
                break;
            }
        }
        return i;
    }

    void exec(Instructions& in, int i, int to) {
        for(;i < to; i++) {
            switch(in[i]) {
            case INC_DPTR: inc_dptr();  break;
            case DEC_DPTR: dec_dptr();  break;
            case READ: read(); break;
            case WRITE: write(); break;
            case INC: inc(); break;
            case DEC: dec(); break;
            case NUL: nul(); break;
            case NEG: neg(); break;

            case BLOCK_BEGIN:
                // Idea: find end of current block ']'
                // and recursive simulate loop
            {
                int end = findBlockEnd(in, i, to);
                while(mem[ptr] > 0) {
                    // i+1 == instruction after '['
                    // end == ']' instruction
                    exec(in, i+1, end);
                }
                // set last intruction to ']'
                i = end;
            }
                break;

            case BLOCK_END:
                // this should always be last instruction in block
                if(mem[ptr] == 0){
                    break;
                }
                break;

            default:
                Fatal("undefined intruction");
            }
        }
    }

    // inc_dptr '>'
    void inc_dptr(){
        ptr++;
        if(ptr > mem.size()){
            Fatal("memory pointer out of range");
        }
    }

    // '<'
    void dec_dptr(){
        ptr--;
        if(ptr < 0){
            Fatal("memory pointer out of range");
        }
    }

    // read reads one byte from standard input
    void read(){
        if(input.size() > 0) {
            mem[ptr] = input[input.size() - 1];
            input.pop_back();
        } else {
            Fatal("R: excepted byte");
        }
    }

    // readInput read bytes from line
    // then saves bytes to input variable in
    // reverse order
    void readInput(string &file){
        if(file.empty()){
            return;
        }

        ifstream in(file);
        if(!in.is_open()) {
            Fatal("cannot open file " + file);
        }

        // Read input to buffer
        vector<byte> buf;
        buf.reserve(1024);
        for(byte b = 0;;){
            in >> b;
            if(in.eof() || b == '\n'){
                break;
            }
            buf.push_back(b);
        }

        // write buffer in reverse
        while(buf.size() > 0){
            byte b = buf.back();
            input.push_back(b);
            buf.pop_back();
        }
    }

    // write writes one byte to standard outup
    void write(){
        byte b = mem[ptr];
        output.push_back(b);
    }

    void writeOutput(){
        for(byte &b : output){
            cout << b;
        }
        // cout << endl;
    }

    void inc(){
        mem[ptr]++;
    }

    void dec(){
        mem[ptr]--;
    }

    void nul(){
        mem[ptr] = 0;
    }

    void neg(){
        mem[ptr] = byte(~mem[ptr]);
    }

    vector<byte> input;
    vector<byte> output;

    int ptr = 0;
    vector<byte> mem = vector<byte>(100000, 0);
};

void
usage(string prog){
    cout << "Usage: " + prog + " source.txt" + " input_file" << endl;
    cout << "input_file is optional, but expeted when R instruction is used." << endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    switch(argc) {
    case 2:
        Machine::Run(argv[1]);
        break;
    case 3:
        Machine::Run(argv[1], argv[2]);
        break;
    default:
        usage(string(argv[0]));
    }
    return 0;
}
