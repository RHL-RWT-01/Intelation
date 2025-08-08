#include <iostream>
#include <limits>
#include "seal/seal.h"

using namespace std;
using namespace seal;

// Function to get safe user input
long long getSafeInput(const string& prompt, long long max_value) {
    long long value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (abs(value) <= max_value) {
                return value;
            } else {
                cout << "Value too large! Maximum safe value is: " << max_value << endl;
            }
        } else {
            cout << "Invalid input! Please enter a valid integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// encrypted addition using Microsoft SEAL library.
int main()
{
    try
    {
        // 1. Set encryption parameters for BFV
        EncryptionParameters parms(scheme_type::bfv);
        size_t poly_modulus_degree = 8192; // Increased for larger plaintext space
        parms.set_poly_modulus_degree(poly_modulus_degree);
        parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
        parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20)); // Larger plain modulus for bigger values

        // SEALContext      
        SEALContext context(parms);

        // Key generation
        KeyGenerator keygen(context);
        SecretKey secret_key = keygen.secret_key();
        SecretKey secret_key_copy = secret_key;
        // cout << secret_key_copy << endl;
        PublicKey public_key;
        keygen.create_public_key(public_key);
        // cout << public_key<< endl;

        // Encryptor, Evaluator, Decryptor
        Encryptor encryptor(context, public_key);
        Evaluator evaluator(context);
        Decryptor decryptor(context, secret_key);

        // Print encryption parameters
        cout << "Encryption parameters:" << endl;
        cout << "Polynomial modulus degree: " << poly_modulus_degree << endl;
        cout << "Plain modulus: " << parms.plain_modulus().value() << endl;
        cout << "Max safe integer: " << (parms.plain_modulus().value() - 1) / 2 << endl;
        cout << endl;

        long long num1, num2;
        long long max_safe_value = (parms.plain_modulus().value() - 1) / 2;
        
        cout << "Choose input method:" << endl;
        cout << "1. Use default large values" << endl;
        cout << "2. Enter custom values" << endl;
        cout << "Choice (1 or 2): ";
        
        int choice;
        cin >> choice;
        
        if (choice == 2) {
            cout << "Enter two integers to subtract (max safe value: " << max_safe_value << "):" << endl;
            num1 = getSafeInput("First number: ", max_safe_value);
            num2 = getSafeInput("Second number: ", max_safe_value);
        } else {
            // Use default larger test values
            num1 = min(123456789LL, max_safe_value / 2);
            num2 = min(987654321LL, max_safe_value / 4);
            cout << "Using default values: " << num1 << " and " << num2 << endl;
        }

        cout << "Computing: " << num1 << " - " << num2 << endl;
        cout << "Processing..." << endl;

        // Encode as plaintexts
        Plaintext plain1(to_string(num1));
        Plaintext plain2(to_string(num2));

        // Encrypt
        Ciphertext encrypted1, encrypted2;
        encryptor.encrypt(plain1, encrypted1);
        encryptor.encrypt(plain2, encrypted2);

        // Homomorphic subtraction
        Ciphertext encrypted_result;
        evaluator.sub(encrypted1, encrypted2, encrypted_result);

        // Decrypt
        Plaintext decrypted_result;
        decryptor.decrypt(encrypted_result, decrypted_result);

        // Convert result back to integer with better error handling
        long long result;
        try {
            string result_str = decrypted_result.to_string();
            result = stoll(result_str);
        } catch (const exception& e) {
            cout << "Error converting result: " << e.what() << endl;
            return 1;
        }

        cout << "Encrypted subtraction result: " << num1 << " - " << num2 << " = " << result << endl;

        // Verify result (handling modular arithmetic)
        long long expected = (num1 - num2) % (long long)parms.plain_modulus().value();
        if (expected < 0) {
            expected += parms.plain_modulus().value(); // Handle negative results in modular arithmetic
        }
        
        if (result == expected)
            cout << "Result is correct!" << endl;
        else {
            cout << "Result verification failed." << endl;
            cout << "Expected: " << expected << ", Got: " << result << endl;
        }
    }
    catch (const exception &e)
    {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
