# Encrypted Addition using Microsoft SEAL

## Code 

**Source:** `my/add.cpp`

---

## Key Terms

- **Plaintext**: The original unencrypted data.
- **Ciphertext**: The encrypted form of the data.

---

## Keys

- **Public Key**: Used for encryption.
- **Private Key**: Used for decryption.

---

## Setup

- First, encryption parameters are set for the **BFV scheme** (suitable for exact integer arithmetic).
- I define:
  - **Polynomial modulus degree** – to ontrol the polynomial size. (Higher values increase security but reduce performance).
  - **Coefficient modulus** – A large prime value to control noise growth.
  - **Plaintext modulus** – Defines the range of values that can be represented in plaintext.

- Then, I:
  - Created a `SEALContext` with the parameters.
  - Initialized a `KeyGenerator` to generate keys.
  - Extracted both the **public** and **private** keys.
  - Created instances of `Encryptor`, `Evaluator`, and `Decryptor`.

---

## Encryption

- Converted raw integers to SEAL-compatible `Plaintext` format.
- Encrypt each `Plaintext` using the **Encryptor** and **public key** to get `Ciphertext`.

---

## Encrypted Addition

- Uses the `Evaluator` to add the two ciphertexts **homomorphically**.
- The result remains in encrypted form .

---

## Decryption

- Uses the `Decryptor` to decrypt the result using the **private key**.
- The secret key was already passed during the decryptor's initialization.

---

## Verification

- To verify result I perform a normal addition `(num1 + num2) % plain_modulus` and compare it with the decrypted result to verify correctness.

---

## Challenges I Faced

- Setting up Microsoft SEAL is a challenge in itself.
- Choosing the correct encryption parameters (`plain_modulus`, `poly_modulus_degree`) was tricky.
- Understanding how **modular arithmetic** affects encrypted computations required time and testing.

---

## Interesting Insights About SEAL

- SEAL performs all operations on **encrypted polynomials**, not plain integers.
- Even though the data is encrypted, we can still perform computations like addition or multiplication without decrypting it.
- This is possible through **homomorphic encryption**, enabling **privacy-preserving computation** on sensitive data.

---
