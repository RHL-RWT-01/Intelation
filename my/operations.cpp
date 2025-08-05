#include <iostream>
#include <vector>
#include "seal/seal.h"

using namespace std;
using namespace seal;

int decode_signed_result(const Plaintext &plain, uint64_t plain_modulus) {
    int value = stoi(plain.to_string(), nullptr, 16);
    if (value > static_cast<int>(plain_modulus / 2))
        value -= static_cast<int>(plain_modulus);
    return value;
}

int main() {
    try {
        // Encryption parameters
        EncryptionParameters parms(scheme_type::bfv);
        size_t poly_modulus_degree = 2048;
        parms.set_poly_modulus_degree(poly_modulus_degree);
        parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
        parms.set_plain_modulus(1023); 

        SEALContext context(parms);

        // Keys
        KeyGenerator keygen(context);
        PublicKey public_key;
        keygen.create_public_key(public_key);
        SecretKey secret_key = keygen.secret_key();

        Encryptor encryptor(context, public_key);
        Evaluator evaluator(context);
        Decryptor decryptor(context, secret_key);

        uint64_t plain_modulus = parms.plain_modulus().value();

        while (true) {
            cout << "\n==== Homomorphic Operations Menu ====\n";
            cout << " 1. add_inplace (Enc + Enc)\n";
            cout << " 2. add_many (Vector of Enc)\n";
            cout << " 3. add_plain (Enc + Plain)\n";
            cout << " 4. add_plain_inplace (Enc + Plain)\n";
            cout << " 5. square (Enc)\n";
            cout << " 6. square_inplace (Enc)\n";
            cout << " 7. sub (Enc - Enc)\n";
            cout << " 8. sub_inplace (Enc - Enc)\n";
            cout << " 9. sub_plain (Enc - Plain)\n";
            cout << "10. sub_plain_inplace (Enc - Plain)\n";
            cout << "11. multiply (Enc * Enc)\n";
            cout << "12. multiply_inplace (Enc * Enc)\n";
            cout << "13. multiply_plain (Enc * Plain)\n";
            cout << "14. multiply_plain_inplace (Enc * Plain)\n";
            cout << " 0. Exit\n";
            cout << "=====================================\n";
            cout << "Enter choice: ";

            int choice;
            cin >> choice;
            if (choice == 0) break;

            Ciphertext c1, c2, result;
            Plaintext p1, p2;
            Plaintext plain_res;

            switch (choice) {
                case 1: { // add_inplace
                    int a, b;
                    cout << "Enter first number: "; cin >> a;
                    cout << "Enter second number: "; cin >> b;
                    encryptor.encrypt(Plaintext(to_string(a)), c1);
                    encryptor.encrypt(Plaintext(to_string(b)), c2);
                    evaluator.add_inplace(c1, c2);
                    decryptor.decrypt(c1, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                case 2: { // add_many
                    int n; cout << "How many numbers to add? "; cin >> n;
                    vector<Ciphertext> vec;
                    for (int i = 0; i < n; i++) {
                        int num; cout << "Enter number " << i+1 << ": "; cin >> num;
                        Ciphertext temp;
                        encryptor.encrypt(Plaintext(to_string(num)), temp);
                        vec.push_back(temp);
                    }
                    evaluator.add_many(vec, result);
                    decryptor.decrypt(result, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                case 3: { // add_plain
                    int enc_num, plain_num;
                    cout << "Enter encrypted number: "; cin >> enc_num;
                    cout << "Enter plaintext number: "; cin >> plain_num;
                    encryptor.encrypt(Plaintext(to_string(enc_num)), c1);
                    p1 = Plaintext(to_string(plain_num));
                    evaluator.add_plain(c1, p1, result);
                    decryptor.decrypt(result, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                case 4: { // add_plain_inplace
                    int enc_num, plain_num;
                    cout << "Enter encrypted number: "; cin >> enc_num;
                    cout << "Enter plaintext number: "; cin >> plain_num;
                    encryptor.encrypt(Plaintext(to_string(enc_num)), c1);
                    p1 = Plaintext(to_string(plain_num));
                    evaluator.add_plain_inplace(c1, p1);
                    decryptor.decrypt(c1, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                case 5: { // square
                    int num; cout << "Enter number to square: "; cin >> num;
                    encryptor.encrypt(Plaintext(to_string(num)), c1);
                    evaluator.square(c1, result);
                    decryptor.decrypt(result, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                case 6: { // square_inplace
                    int num; cout << "Enter number to square: "; cin >> num;
                    encryptor.encrypt(Plaintext(to_string(num)), c1);
                    evaluator.square_inplace(c1);
                    decryptor.decrypt(c1, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                case 7: { // sub
                    int a, b;
                    cout << "Enter first number: "; cin >> a;
                    cout << "Enter second number: "; cin >> b;
                    encryptor.encrypt(Plaintext(to_string(a)), c1);
                    encryptor.encrypt(Plaintext(to_string(b)), c2);
                    evaluator.sub(c1, c2, result);
                    decryptor.decrypt(result, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                case 8: { // sub_inplace
                    int a, b;
                    cout << "Enter first number: "; cin >> a;
                    cout << "Enter second number: "; cin >> b;
                    encryptor.encrypt(Plaintext(to_string(a)), c1);
                    encryptor.encrypt(Plaintext(to_string(b)), c2);
                    evaluator.sub_inplace(c1, c2);
                    decryptor.decrypt(c1, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                case 9: { // sub_plain
                    int enc_num, plain_num;
                    cout << "Enter encrypted number: "; cin >> enc_num;
                    cout << "Enter plaintext number: "; cin >> plain_num;
                    encryptor.encrypt(Plaintext(to_string(enc_num)), c1);
                    p1 = Plaintext(to_string(plain_num));
                    evaluator.sub_plain(c1, p1, result);
                    decryptor.decrypt(result, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                case 10: { // sub_plain_inplace
                    int enc_num, plain_num;
                    cout << "Enter encrypted number: "; cin >> enc_num;
                    cout << "Enter plaintext number: "; cin >> plain_num;
                    encryptor.encrypt(Plaintext(to_string(enc_num)), c1);
                    p1 = Plaintext(to_string(plain_num));
                    evaluator.sub_plain_inplace(c1, p1);
                    decryptor.decrypt(c1, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                case 11: { // multiply
                    int a, b;
                    cout << "Enter first number: "; cin >> a;
                    cout << "Enter second number: "; cin >> b;
                    encryptor.encrypt(Plaintext(to_string(a)), c1);
                    encryptor.encrypt(Plaintext(to_string(b)), c2);
                    evaluator.multiply(c1, c2, result);
                    decryptor.decrypt(result, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                case 12: { // multiply_inplace
                    int a, b;
                    cout << "Enter first number: "; cin >> a;
                    cout << "Enter second number: "; cin >> b;
                    encryptor.encrypt(Plaintext(to_string(a)), c1);
                    encryptor.encrypt(Plaintext(to_string(b)), c2);
                    evaluator.multiply_inplace(c1, c2);
                    decryptor.decrypt(c1, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                case 13: { // multiply_plain
                    int enc_num, plain_num;
                    cout << "Enter encrypted number: "; cin >> enc_num;
                    cout << "Enter plaintext number: "; cin >> plain_num;
                    encryptor.encrypt(Plaintext(to_string(enc_num)), c1);
                    p1 = Plaintext(to_string(plain_num));
                    evaluator.multiply_plain(c1, p1, result);
                    decryptor.decrypt(result, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                case 14: { // multiply_plain_inplace
                    int enc_num, plain_num;
                    cout << "Enter encrypted number: "; cin >> enc_num;
                    cout << "Enter plaintext number: "; cin >> plain_num;
                    encryptor.encrypt(Plaintext(to_string(enc_num)), c1);
                    p1 = Plaintext(to_string(plain_num));
                    evaluator.multiply_plain_inplace(c1, p1);
                    decryptor.decrypt(c1, plain_res);
                    cout << "Decrypted integer result = "
                         << decode_signed_result(plain_res, plain_modulus) << endl;
                    break;
                }
                default:
                    cout << "Invalid choice!\n";
            }
        }

    } catch (const exception &e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }
    return 0;
}
