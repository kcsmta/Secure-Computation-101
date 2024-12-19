#include <abycore/aby/abyparty.h>
#include <abycore/circuit/arithmeticcircuits.h>
#include <abycore/circuit/booleancircuits.h>
#include <abycore/sharing/sharing.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>

using namespace std;

const int SCALE = 1000; // Fixed-point scaling factor

void scale_vector(const vector<double>& input, vector<uint64_t>& output) {
    for (size_t i = 0; i < input.size(); ++i) {
        output[i] = static_cast<uint64_t>(round(input[i] * SCALE));
    }
}

share* secure_argmax(vector<share*>& shares, uint32_t size, BooleanCircuit* circuit) {
    share* max_val = shares[0];

    uint32_t zero = 0;
    share* max_idx = circuit->PutCONSGate(&zero, 32); // Index starts from 0

    for (uint32_t i = 1; i < size; ++i) {
        share* is_greater = circuit->PutGTGate(shares[i], max_val);
        max_val = circuit->PutMUXGate(shares[i], max_val, is_greater);
        uint32_t i_val = i;
        share* new_idx = circuit->PutCONSGate(&i_val, 32);
        max_idx = circuit->PutMUXGate(new_idx, max_idx, is_greater);
    }

    return max_idx;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <ROLE: 0=SERVER, 1=CLIENT>" << endl;
        return 1;
    }

    uint32_t role = atoi(argv[1]);
    const uint32_t port = 8888;
    const string address = "127.0.0.1";

    vector<double> u = {1.5, 7.5, 100.5}; // Client's vector
    vector<double> v = {0.5, 1.5, 2.5}; // Server's vector
    size_t n = u.size();

    vector<uint64_t> u_scaled(n), v_scaled(n);
    scale_vector(u, u_scaled);
    scale_vector(v, v_scaled);

    ABYParty* party = new ABYParty((e_role)role, address, port, LT, 32, 1);
    vector<Sharing*>& sharings = party->GetSharings();
    BooleanCircuit* circuit = (BooleanCircuit*)sharings[S_YAO]->GetCircuitBuildRoutine();

    vector<share*> u_shares(n), v_shares(n);
    for (size_t i = 0; i < n; ++i) {
        if (role == 1) { // Client inputs u
            u_shares[i] = circuit->PutINGate(&u_scaled[i], 64, CLIENT);
            v_shares[i] = circuit->PutDummySIMDINGate(1, 64);
        } else { // Server inputs v
            u_shares[i] = circuit->PutDummySIMDINGate(1, 64);
            v_shares[i] = circuit->PutINGate(&v_scaled[i], 64, SERVER);
        }
    }

    vector<share*> sum_shares(n);
    for (size_t i = 0; i < n; ++i) {
        sum_shares[i] = circuit->PutADDGate(u_shares[i], v_shares[i]);
    }

    share* argmax_result = secure_argmax(sum_shares, n, circuit);
    argmax_result = circuit->PutOUTGate(argmax_result, ALL);

    party->ExecCircuit();

    if (role == 1) {
        uint32_t* output = (uint32_t*)argmax_result->get_clear_value_ptr();
        cout << "Argmax index: " << *output << endl;
    }

    delete party;
    return 0;
}
