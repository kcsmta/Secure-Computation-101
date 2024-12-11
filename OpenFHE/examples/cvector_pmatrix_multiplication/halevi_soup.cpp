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
    parameters.SetBatchSize(4096); // Large batch size for testing

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);

    cc->Enable(PKE);
    cc->Enable(LEVELEDSHE);
    cc->Enable(ADVANCEDSHE);
    std::cout << "CKKS scheme is using ring dimension " << cc->GetRingDimension() << std::endl << std::endl;

    // Step 2: Key generation
    auto keys = cc->KeyGen();
    cc->EvalMultKeyGen(keys.secretKey);
    cc->EvalSumKeyGen(keys.secretKey);

    // Generate all required rotation keys for a large matrix
    std::vector<int> rotationIndices;
    size_t numCols = 4096; // Number of columns in the matrix
    for (size_t i = 1; i < numCols; ++i) {
        rotationIndices.push_back(static_cast<int>(i));
    }
    cc->EvalAtIndexKeyGen(keys.secretKey, rotationIndices);

    // Step 3: Large random matrix initialization
    size_t numRows = 1024; // Define number of rows

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 10.0); // Random values between 0 and 10

    std::vector<std::vector<double>> matrix(numRows, std::vector<double>(numCols));
    for (size_t i = 0; i < numRows; ++i) {
        for (size_t j = 0; j < numCols; ++j) {
            matrix[i][j] = dis(gen);
        }
    }

    // Step 4: Large random input vector
    std::vector<double> inputVector(numCols);
    for (size_t i = 0; i < numCols; ++i) {
        inputVector[i] = dis(gen);
    }

    Plaintext plaintextVector = cc->MakeCKKSPackedPlaintext(inputVector);
    auto ciphertextVector = cc->Encrypt(keys.publicKey, plaintextVector);

    // Step 5: Perform encrypted matrix-vector multiplication
    auto start = std::chrono::high_resolution_clock::now();

    Ciphertext<DCRTPoly> resultCiphertext = cc->Encrypt(keys.publicKey, cc->MakeCKKSPackedPlaintext(std::vector<double>(numRows, 0.0)));
    for (size_t row = 0; row < numRows; ++row) {
        // Multiply each row of the matrix with the encrypted vector
        std::vector<double> rowVector = matrix[row];
        Plaintext plaintextRow = cc->MakeCKKSPackedPlaintext(rowVector);
        auto tempCiphertext = cc->EvalMult(ciphertextVector, plaintextRow);

        // Sum the elements (dot product)
        resultCiphertext = cc->EvalAdd(resultCiphertext, tempCiphertext);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time for matrix-vector computation with encrypted vector and plain matrix: " << elapsed.count() << " seconds" << std::endl;

    // Step 6: Decrypt and display results (only partial for large size)
    Plaintext resultPlaintext;
    cc->Decrypt(keys.secretKey, resultCiphertext, &resultPlaintext);
    resultPlaintext->SetLength(numRows);

    std::cout << "Result vector (first 10 elements): ";
    for (size_t i = 0; i < 10; ++i) {
        std::cout << resultPlaintext->GetCKKSPackedValue()[i] << " ";
    }
    std::cout << "..." << std::endl;

    return 0;
}
