#include <iostream>
#include "seal/seal.h"

using namespace std;
using namespace seal;

// encrypted addition using Microsoft SEAL library.
int main()
{
    try
    {
        // 1. Set encryption parameters for BFV
        EncryptionParameters parms(scheme_type::bfv);
        size_t poly_modulus_degree = 2048;
        parms.set_poly_modulus_degree(poly_modulus_degree);
        parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
        parms.set_plain_modulus(1023); // now safe with bigger degree

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

        int num1 = 6;
        int num2 = 3;

        // Encode as plaintexts
        Plaintext plain1(to_string(num1));
        Plaintext plain2(to_string(num2));

        // Encrypt
        Ciphertext encrypted1, encrypted2;
        encryptor.encrypt(plain1, encrypted1);
        encryptor.encrypt(plain2, encrypted2);

        // Homomorphic addition
        Ciphertext encrypted_sum;
        evaluator.sub(encrypted1, encrypted2, encrypted_sum);

        // Decrypt
        Plaintext decrypted_result;
        decryptor.decrypt(encrypted_sum, decrypted_result);

        int result = stoi(decrypted_result.to_string());
        cout << "Encrypted subtraction of " << num1 << " - " << num2 << " = " << result << endl;

        if (result == (num1 - num2) % parms.plain_modulus().value())
            cout << "Result is correct." << endl;
        else
            cout << "Result is incorrect. Expected: " << (num1 + num2) % parms.plain_modulus().value() << endl;
    }
    catch (const exception &e)
    {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
