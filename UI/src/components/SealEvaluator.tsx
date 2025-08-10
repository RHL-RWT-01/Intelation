"use client";

import { useState } from "react";

export default function SealEvaluatorMinimal() {
    const [operation, setOperation] = useState("add");
    const [num1, setNum1] = useState("");
    const [num2, setNum2] = useState("");
    const [result, setResult] = useState<number | string>("");

    const runOperation = async () => {
        try {
            const response = await fetch("http://localhost:5000/api/seal/evaluate", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                },
                body: JSON.stringify({
                    operation,
                    num1: parseFloat(num1),
                    num2: num2 ? parseFloat(num2) : undefined,
                }),
            });

            if (!response.ok) {
                throw new Error(`Server error: ${response.statusText}`);
            }

            const data = await response.json();
            setResult(data.result);
        } catch (error: any) {
            window.alert(
                "Error occurred while running operation:\n" + error.message
            );
        }
    };

    const needsSecondInput = (op: string) => {
        return !op.includes("square");
    };

    return (
        <div className="p-8 m-6 bg-gradient-to-br from-blue-100 via-white to-purple-100 rounded-3xl shadow-2xl w-full max-w-lg mx-auto border border-blue-200">
            <div className="flex items-center justify-center mb-6 gap-2">
                <span className="inline-block bg-blue-600 text-white rounded-full w-12 h-12 flex items-center justify-center text-2xl shadow-lg">
                    <svg width="28" height="28" fill="none" viewBox="0 0 24 24">
                        <path d="M12 2v20m10-10H2" stroke="white" strokeWidth="2" strokeLinecap="round"/>
                    </svg>
                </span>
                <h2 className="text-2xl font-extrabold text-blue-700 tracking-wide">SEAL Evaluator</h2>
            </div>
            <div className="space-y-5">
                <div>
                    <label className="block font-semibold text-blue-800 mb-2">Operation</label>
                    <select
                        value={operation}
                        onChange={(e) => setOperation(e.target.value)}
                        className="border-2 border-blue-300 bg-white p-3 rounded-xl w-full mb-2 focus:outline-none focus:ring-2 focus:ring-blue-400 transition"
                    >
                        <option value="add">Add</option>
                        <option value="add_inplace">Add Inplace</option>
                        <option value="add_many">Add Many</option>
                        <option value="add_plain">Add Plain</option>
                        <option value="add_plain_inplace">Add Plain Inplace</option>
                        <option value="square">Square</option>
                        <option value="square_inplace">Square Inplace</option>
                        <option value="sub">Subtract</option>
                        <option value="sub_inplace">Subtract Inplace</option>
                        <option value="sub_plain">Subtract Plain</option>
                        <option value="sub_plain_inplace">Subtract Plain Inplace</option>
                    </select>
                </div>
                <div>
                    <label className="block font-semibold text-blue-800 mb-2">Number 1</label>
                    <input
                        type="number"
                        value={num1}
                        onChange={(e) => setNum1(e.target.value)}
                        className="border-2 border-blue-300 p-3 rounded-xl w-full mb-2 bg-blue-50 focus:outline-none focus:ring-2 focus:ring-blue-400 transition"
                        placeholder="Enter first number"
                    />
                </div>
                {needsSecondInput(operation) && (
                    <div>
                        <label className="block font-semibold text-blue-800 mb-2">Number 2</label>
                        <input
                            type="number"
                            value={num2}
                            onChange={(e) => setNum2(e.target.value)}
                            className="border-2 border-blue-300 p-3 rounded-xl w-full mb-2 bg-blue-50 focus:outline-none focus:ring-2 focus:ring-blue-400 transition"
                            placeholder="Enter second number"
                        />
                    </div>
                )}
                <button
                    onClick={runOperation}
                    className="bg-gradient-to-r from-blue-500 to-purple-500 text-white px-6 py-3 rounded-xl font-bold hover:scale-105 hover:shadow-lg transition w-full flex items-center justify-center gap-2"
                >
                    <svg className="w-5 h-5 mr-1" fill="none" stroke="white" strokeWidth="2" viewBox="0 0 24 24">
                        <path d="M5 12h14M12 5l7 7-7 7" strokeLinecap="round" strokeLinejoin="round"/>
                    </svg>
                    Run Operation
                </button>
                {result !== "" && (
                    <div className="mt-4 p-4 bg-gradient-to-r from-white via-blue-50 to-purple-50 border border-blue-300 rounded-xl shadow-lg flex items-center justify-between animate-fade-in">
                        <strong className="text-blue-700 text-lg">Result:</strong>
                        <span className="text-purple-700 text-xl font-bold">{result}</span>
                    </div>
                )}
            </div>
            <style jsx>{`
                .animate-fade-in {
                    animation: fadeIn 0.8s ease;
                }
                @keyframes fadeIn {
                    from { opacity: 0; transform: translateY(20px);}
                    to { opacity: 1; transform: translateY(0);}
                }
            `}</style>
        </div>
    );
}
