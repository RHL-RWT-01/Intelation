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
        <div className="p-4 m-4 bg-gray-100 rounded-xl shadow-lg w-full max-w-md mx-auto">
            <h2 className="text-lg font-bold mb-4">SEAL Evaluator (Minimal)</h2>

            <label className="block font-medium mb-1">Operation</label>
            <select
                value={operation}
                onChange={(e) => setOperation(e.target.value)}
                className="border p-2 rounded w-full mb-4"
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

            <label className="block font-medium mb-1">Number 1</label>
            <input
                type="number"
                value={num1}
                onChange={(e) => setNum1(e.target.value)}
                className="border p-2 rounded w-full mb-4"
            />

            {needsSecondInput(operation) && (
                <>
                    <label className="block font-medium mb-1">Number 2</label>
                    <input
                        type="number"
                        value={num2}
                        onChange={(e) => setNum2(e.target.value)}
                        className="border p-2 rounded w-full mb-4"
                    />
                </>
            )}

            <button
                onClick={runOperation}
                className="bg-blue-600 text-white px-4 py-2 rounded hover:bg-blue-700 w-full"
            >
                Run Operation
            </button>

            {result !== "" && (
                <div className="mt-4 p-3 bg-white border rounded">
                    <strong>Result:</strong> {result}
                </div>
            )}
        </div>
    );
}
