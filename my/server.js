import express from "express";
import SEAL from "node-seal";
import cors from "cors";
const app = express();
app.use(express.json());
app.use(cors());



let seal;
let context;
let encoder;
let encryptor;
let decryptor;
let evaluator;
let publicKey;
let secretKey;

// Initialize SEAL once at server startup
(async () => {
    seal = await SEAL();

    const parms = seal.EncryptionParameters(seal.SchemeType.bfv);
    parms.setPolyModulusDegree(4096);
    parms.setCoeffModulus(seal.CoeffModulus.BFVDefault(4096));
    parms.setPlainModulus(seal.PlainModulus.Batching(4096, 20));

    context = seal.Context(parms, true, seal.SecurityLevel.tc128);
    if (!context.parametersSet()) {
        throw new Error("Invalid encryption parameters");
    }

    const keygen = seal.KeyGenerator(context);
    publicKey = keygen.createPublicKey();
    secretKey = keygen.secretKey();

    encoder = seal.BatchEncoder(context);
    encryptor = seal.Encryptor(context, publicKey);
    decryptor = seal.Decryptor(context, secretKey);
    evaluator = seal.Evaluator(context);

    console.log("SEAL initialized in Node.js ✅");
})();

// POST /api/seal/evaluate
app.post("/api/seal/evaluate", (req, res) => {
    console.log("Received SEAL operation request:", req.body);
    try {
        const { operation, num1, num2 } = req.body;
        if (typeof num1 !== "number" || (num2 !== undefined && typeof num2 !== "number")) {
            return res.status(400).json({ error: "Invalid numbers" });
        }

        // Encode numbers
        const plain1 = encoder.encode(Int32Array.from([num1]));
        const plain2 = encoder.encode(Int32Array.from([num2 ?? 0]));

        // Encrypt numbers
        const encrypted1 = encryptor.encrypt(plain1);
        const encrypted2 = encryptor.encrypt(plain2);

        let resultCipher;

        switch (operation) {
            case "add":
                resultCipher = evaluator.add(encrypted1, encrypted2);
                break;
            case "subtract":
                resultCipher = evaluator.sub(encrypted1, encrypted2);
                break;
            case "multiply":
                resultCipher = evaluator.multiply(encrypted1, encrypted2);
                break;
            case "square":
                resultCipher = evaluator.square(encrypted1);
                break;
            default:
                return res.status(400).json({ error: "Unknown operation" });
        }

        // Decrypt result
        const plainResult = decryptor.decrypt(resultCipher);
        const decodedResult = encoder.decode(plainResult);

        res.json({ result: decodedResult[0] });
    } catch (err) {
        console.error("Error in SEAL operation:", err);
        res.status(500).json({ error: err.message });
    }
});

const PORT = process.env.PORT || 5000;
app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
});
