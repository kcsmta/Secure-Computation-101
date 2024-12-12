#include "openfhe.h"
#include <vector>
#include <iostream>
#include <chrono> // For measuring time
#include <random> // For generating random values

using namespace lbcrypto;

int main() {
    // Step 1: Set up crypto context
    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetMultiplicativeDepth(4);
    parameters.SetScalingModSize(50);
    parameters.SetBatchSize(4096); // Increased batch size for vector size

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);
    
    cc->Enable(PKE);
    cc->Enable(LEVELEDSHE);
    cc->Enable(ADVANCEDSHE);
    std::cout << "CKKS scheme is using ring dimension " << cc->GetRingDimension() << std::endl << std::endl;

    // Step 2: Key generation
    auto keys = cc->KeyGen();
    cc->EvalMultKeyGen(keys.secretKey);
    cc->EvalSumKeyGen(keys.secretKey);

    // Step 3: Plain matrix initialization
    // A 1024x4096 matrix
    size_t rows = 1024;
    size_t cols = 4096;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 10.0); // Random values between 0 and 10

    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols));
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            matrix[i][j] = dis(gen);
        }
    }

    // Step 4: Input vector encryption
    std::vector<double> inputVector(cols);
    for (size_t i = 0; i < cols; ++i) {
        inputVector[i] = dis(gen);
    }

    Plaintext plaintextVector = cc->MakeCKKSPackedPlaintext(inputVector);
    auto ciphertextVector = cc->Encrypt(keys.publicKey, plaintextVector);

    // Step 5: Perform multiplication for each row in the matrix
    std::vector<Ciphertext<DCRTPoly>> resultCiphertexts(rows);

    auto start = std::chrono::high_resolution_clock::now(); // Start timing

    for (size_t i = 0; i < rows; ++i) {
        // Multiply each matrix row by the encrypted vector
        std::vector<double> row = matrix[i];
        Plaintext plaintextRow = cc->MakeCKKSPackedPlaintext(row);

        // Perform element-wise multiplication
        auto tempCiphertext = cc->EvalMult(ciphertextVector, plaintextRow);

        // Sum up the results (dot product)
        resultCiphertexts[i] = cc->EvalSum(tempCiphertext, cols);
    }

    auto end = std::chrono::high_resolution_clock::now(); // End timing
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time for matrix-vector computation: " << elapsed.count() << " seconds" << std::endl;

    // // Step 6: Decrypt and display results
    // for (size_t i = 0; i < rows; ++i) {
    //     Plaintext resultPlaintext;
    //     cc->Decrypt(keys.secretKey, resultCiphertexts[i], &resultPlaintext);

    //     resultPlaintext->SetLength(1); // Set the plaintext length for proper output
    //     std::cout << "Result for row " << i << ": " << resultPlaintext << std::endl;
    // }

    return 0;
}
