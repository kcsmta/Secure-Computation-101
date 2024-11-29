#include <emp-tool/emp-tool.h>  // for NetIO, etc
#include <emp-ot/emp-ot.h>      // for OTs

int main(int argc, char** argv) {
    // Parse arguments for party (1 = ALICE, 2 = BOB), length, and port
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <party> <length> <port>" << std::endl;
        return 1;
    }
    int party = std::stoi(argv[1]);
    int length = std::stoi(argv[2]);
    int port = std::stoi(argv[3]);

    block b0[length], b1[length];
    bool c[length];

    // Create a network with Bob connecting to 127.0.0.1
    NetIO io(party == ALICE ? nullptr : "127.0.0.1", port);

    // Create a Naor-Pinkas OT using the network above
    OTNP<NetIO> np(&io);

    if (party == ALICE) {
        // ALICE is the sender, sending b0[i] and b1[i]
        np.send(b0, b1, length);
        std::cout << "ALICE sent the messages." << std::endl;
    } else if (party == BOB)
    {
        // BOB is the receiver, receiving b0[i] if c[i] == 0, or b1[i] if c[i] == 1
        np.recv(b0, c, length);
        std::cout << "BOB received the messages." << std::endl;
    }else
    {
        std::cout << "<party> can either be 1 (for ALICE) or 2 (for BOB)" << std::endl;
    }

    return 0;
}
