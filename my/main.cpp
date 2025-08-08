#include <seal/seal.h>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <thread>

// Utility: Base64 encoding for returning ciphertext as string
static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string base64_encode(const std::vector<unsigned char> &buf) {
    std::string ret;
    int val = 0, valb = -6;
    for (unsigned char c : buf) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            ret.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) ret.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (ret.size() % 4) ret.push_back('=');
    return ret;
}

class EncryptionServer {
public:
    // EncryptionServer(Pistache::Address addr)
        : httpEndpoint(std::make_shared<Pistache::Http::Endpoint>(addr)) {
        setupSeal();
    }

    void init(size_t threads = 1) {
        auto opts = Pistache::Http::Endpoint::options()
                        .threads(static_cast<int>(threads))
                        .flags(Pistache::Tcp::Options::InstallSignalHandler);
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

private:
   

    // SEAL components
    seal::EncryptionParameters parms;
    std::shared_ptr<seal::SEALContext> context;
    seal::PublicKey public_key;
    seal::SecretKey secret_key;
    std::unique_ptr<seal::Encryptor> encryptor;
    std::unique_ptr<seal::KeyGenerator> keygen;

    void setupSeal() {
        parms = seal::EncryptionParameters(seal::scheme_type::bfv);
        parms.set_poly_modulus_degree(4096);
        parms.set_coeff_modulus(seal::CoeffModulus::BFVDefault(4096));
        parms.set_plain_modulus(1024);

        context = std::make_shared<seal::SEALContext>(parms);

        keygen = std::make_unique<seal::KeyGenerator>(*context);
        public_key = keygen->public_key();
        secret_key = keygen->secret_key();

        encryptor = std::make_unique<seal::Encryptor>(*context, public_key);
    }

    

//     void encryptHandler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
//         auto data = request.query().get("data").getOrElse("");
//         if (data.empty()) {
//             response.send(Pistache::Http::Code::Bad_Request, "Missing 'data' parameter\n");
//             return;
//         }

//         try {
//             // Convert input string to integer values
//             std::vector<int64_t> values;
//             for (char c : data) {
//                 values.push_back(static_cast<int64_t>(c));
//             }

//             // Encode & encrypt
//             seal::Plaintext plain;
//             seal::BatchEncoder batch_encoder(*context);
//             batch_encoder.encode(values, plain);

//             seal::Ciphertext encrypted;
//             encryptor->encrypt(plain, encrypted);

//             // Serialize ciphertext
//             std::stringstream ss;
//             encrypted.save(ss);
//             std::string binary_str = ss.str();
//             std::vector<unsigned char> buffer(binary_str.begin(), binary_str.end());

//             // Base64 encode
//             std::string encoded_cipher = base64_encode(buffer);

//             // Return as JSON
//             std::string jsonResp = "{ \"ciphertext\": \"" + encoded_cipher + "\" }";
//             response.send(Pistache::Http::Code::Ok, jsonResp, MIME(Application, Json));

//         } catch (std::exception &e) {
//             response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
//         }
//     }
// };

int main() {
    // Pistache::Port port(8080);
    // Pistache::Address addr(Pistache::Ipv4::any(), port);

    EncryptionServer server(addr);
    // server.init(std::thread::hardware_concurrency());
    std::cout << "Server listening on port 8080..." << std::endl;
    // server.start();

    return 0;
}
