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

uint32_t compute_groundtruth_argmax(const vector<double>& u, const vector<double>& v) {
    assert(u.size() == v.size());
    size_t n = u.size();
    double max_val = u[0] + v[0];
    uint32_t max_idx = 0;

    for (uint32_t i = 1; i < n; ++i) {
        double sum = u[i] + v[i];
        if (sum > max_val) {
            max_val = sum;
            max_idx = i;
        }
    }
    return max_idx;
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

    size_t n = 1000; // Example: Large vector size
    vector<double> u(n), v(n);

    // Random initialization for vectors
    for (size_t i = 0; i < n; ++i) {
        u[i] = static_cast<double>(rand() % 100) / 10.0; // Random values [0, 10)
        v[i] = static_cast<double>(rand() % 100) / 10.0;
    }

    vector<uint64_t> u_scaled(n), v_scaled(n);
    scale_vector(u, u_scaled);
    scale_vector(v, v_scaled);

    // Compute ground truth
    uint32_t groundtruth_argmax = compute_groundtruth_argmax(u, v);
    if (role == 1) { // Only one party prints the ground truth
        cout << "Groundtruth Argmax Index: " << groundtruth_argmax << endl;
    }

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
        cout << "Secure Computation Argmax Index: " << *output << endl;

        // Compare with ground truth
        if (*output == groundtruth_argmax) {
            cout << "Secure computation result matches the ground truth!" << endl;
        } else {
            cout << "Secure computation result does NOT match the ground truth!" << endl;
        }
    }

    delete party;
    return 0;
}
