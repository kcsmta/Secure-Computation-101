#include "openfhe.h"
#include <vector>
#include <iostream>
#include <chrono> // For measuring time

using namespace lbcrypto;

int main() {
    // Step 1: Set up crypto context
    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetMultiplicativeDepth(4);
    parameters.SetScalingModSize(50);
    parameters.SetBatchSize(4); // Small batch size for testing

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);

    cc->Enable(PKE);
    cc->Enable(LEVELEDSHE);
    cc->Enable(ADVANCEDSHE);
    std::cout << "CKKS scheme is using ring dimension " << cc->GetRingDimension() << std::endl << std::endl;

    // Step 2: Key generation
    auto keys = cc->KeyGen();
    cc->EvalMultKeyGen(keys.secretKey);
    cc->EvalSumKeyGen(keys.secretKey);
    cc->EvalAtIndexKeyGen(keys.secretKey, {1, 2, 3}); // Generate all required rotation keys

    // Step 3: Small fixed matrix initialization (4x4 matrix)
    size_t dim = 4; // Small dimension for testing
    std::vector<std::vector<double>> matrix = {
        {1.0, 2.0, 3.0, 4.0},
        {5.0, 6.0, 7.0, 8.0},
        {9.0, 10.0, 11.0, 12.0},
        {13.0, 14.0, 15.0, 16.0}
    };

    // Step 4: Small fixed input vector
    std::vector<double> inputVector = {1.0, 2.0, 3.0, 4.0};

    Plaintext plaintextVector = cc->MakeCKKSPackedPlaintext(inputVector);
    auto ciphertextVector = cc->Encrypt(keys.publicKey, plaintextVector);

    // Step 5: Diagonal encoding of the matrix
    std::vector<Plaintext> diagonalEncodings(dim);
    for (size_t d = 0; d < dim; ++d) {
        std::vector<double> diagonal(dim, 0.0);
        for (size_t i = 0; i < dim; ++i) {
            size_t j = (i + d) % dim; // Wrap-around for diagonals
            diagonal[i] = matrix[i][j];
        }
        diagonalEncodings[d] = cc->MakeCKKSPackedPlaintext(diagonal);

        // Debug: Print encoded diagonals
        std::cout << "Diagonal " << d << ": ";
        for (const auto& val : diagonal) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Step 6: Perform encrypted matrix-vector multiplication
    auto start = std::chrono::high_resolution_clock::now();

    Ciphertext<DCRTPoly> resultCiphertext = cc->Encrypt(keys.publicKey, cc->MakeCKKSPackedPlaintext(std::vector<double>(dim, 0.0)));
    for (size_t d = 0; d < dim; ++d) {
        // Rotate the ciphertext vector
        auto rotatedCiphertext = cc->EvalAtIndex(ciphertextVector, static_cast<int>(d)); // Positive rotation index

        // Debug: Decrypt and check rotated vector
        Plaintext rotatedPlaintext;
        cc->Decrypt(keys.secretKey, rotatedCiphertext, &rotatedPlaintext);
        rotatedPlaintext->SetLength(dim);
        std::cout << "Rotated vector for diagonal " << d << ": " << rotatedPlaintext << std::endl;

        // Multiply with diagonal encoding
        auto tempCiphertext = cc->EvalMult(rotatedCiphertext, diagonalEncodings[d]);

        // Add to the result
        resultCiphertext = cc->EvalAdd(resultCiphertext, tempCiphertext);
    }


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time for matrix-vector computation with diagonal encoding: " << elapsed.count() << " seconds" << std::endl;

    // Step 7: Decrypt and display results
    Plaintext resultPlaintext;
    cc->Decrypt(keys.secretKey, resultCiphertext, &resultPlaintext);
    resultPlaintext->SetLength(dim);
    std::cout << "Result vector: " << resultPlaintext << std::endl;

    return 0;
}
