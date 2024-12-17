#include <abycore/aby/abyparty.h>
#include <abycore/circuit/arithmeticcircuits.h>
#include <abycore/circuit/booleancircuits.h>
#include <abycore/sharing/sharing.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>

using namespace std;

const int SCALE = 100;

void scale_vector(const vector<double>& input, vector<uint64_t>& output) {
    for (size_t i = 0; i < input.size(); ++i) {
        output[i] = static_cast<uint64_t>(round(input[i] * SCALE));
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <ROLE: 0=SERVER, 1=CLIENT>" << endl;
        return 1;
    }

    uint32_t role = atoi(argv[1]);
    const uint32_t port = 8888;
    const string address = "127.0.0.1";

    vector<double> u = {1.5}; // Client's input
    vector<double> v = {0.5}; // Server's input
    size_t n = u.size();

    vector<uint64_t> u_scaled(n), v_scaled(n);
    scale_vector(u, u_scaled);
    scale_vector(v, v_scaled);

    cout << "Debug: Setting up ABYParty, role = " << role << endl;
    ABYParty* party = new ABYParty((e_role)role, address, port, LT, 32, 30);
    if (!party) {
        cerr << "Error: Failed to create ABYParty!" << endl;
        return 1;
    }

    vector<Sharing*>& sharings = party->GetSharings();
    BooleanCircuit* circuit = (BooleanCircuit*)sharings[S_YAO]->GetCircuitBuildRoutine();

    cout << "Debug: Input shares size = " << n << ", role = " << role << endl;

    // Simplified Test: Add two numbers
    cout << "Debug: Setting up inputs" << endl;
    share* a = nullptr;
    share* b = nullptr;

    if (role == 1) {
        a = circuit->PutINGate(&u_scaled[0], 64, CLIENT);
        cout << "Debug: Client input = " << u_scaled[0] << endl;
    } else {
        b = circuit->PutINGate(&v_scaled[0], 64, SERVER);
        cout << "Debug: Server input = " << v_scaled[0] << endl;
    }

    cout << "Debug: Adding inputs" << endl;
    share* sum = circuit->PutADDGate(a, b);

    cout << "Debug: Setting output gate" << endl;
    sum = circuit->PutOUTGate(sum, CLIENT);

    cout << "Debug: Before ExecCircuit" << endl;
    party->ExecCircuit();
    cout << "Debug: After ExecCircuit" << endl;

    if (role == 1) {
        uint32_t* clear_value = nullptr;
        uint32_t bitlen = 0;
        uint32_t nvals = 0;

        sum->get_clear_value_vec(&clear_value, &bitlen, &nvals);
        cout << "Debug: Sum = " << clear_value[0] << endl;

        delete[] clear_value;
    }

    delete party;
    return 0;
}
