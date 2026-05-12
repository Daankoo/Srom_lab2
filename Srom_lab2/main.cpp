#include "header.h"

int main() {
	string hex1 = "";
	string hex2 = "";
	string line;

	ifstream Hex1("hex1.txt");
	if (!Hex1) {
		cout << "File is not open\n";
		return -1;
	}

	ifstream Hex2("hex2.txt");
	if (!Hex2) {
		cout << "File is not open\n";
		return -1;
	}

	ofstream Result("result.txt");
	if (!Result) {
		cout << "Failed to create write file\n";
		return -1;
	}

	ofstream Corect("corect.txt");
	if (!Corect) {
		cout << "Failed to create write file\n";
		return -1;
	}

	ofstream Time("time.txt");
	if (!Time) {
		cout << "Failed to create write file\n";
		return -1;
	}

	while (getline(Hex1, line)) {
		hex1 += line;
	}

	while (getline(Hex2, line)) {
		hex2 += line;
	}

	Hex1.close();
	Hex2.close();

	auto printLog = [](const string& text, ofstream& file) {
		cout << text << endl;
		file << text << endl;
		};

	BigInt A_orig(hex1);
	BigInt B_orig(hex2);
	BigInt N("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
	BigInt mu_N = mu_const(N);

	BigInt A = BarrettReduction(A_orig, N, mu_N);
	BigInt B = BarrettReduction(B_orig, N, mu_N);

	// ========================================================================
	// BLOCK 1: DEMONSTRATION OF ALL FUNCTIONS
	// ========================================================================
	printLog("=== BLOCK 1: RESULTS OF BASIC AND MODULAR OPERATIONS ===", Result);
	printLog("A (mod N) = " + A.Hex_convert(), Result);
	printLog("B (mod N) = " + B.Hex_convert(), Result);
	printLog("Module N  = " + N.Hex_convert() + "\n", Result);

	printLog("1. GCD (A_orig, B_orig):\n" + LongGCD(A_orig, B_orig).Hex_convert() + "\n", Result);
	printLog("2. LCM (A_orig, B_orig):\n" + LongLCM(A_orig, B_orig).Hex_convert() + "\n", Result);

	printLog("3. Addition (A + B) mod N:\n" + LongModAdd(A, B, N).Hex_convert() + "\n", Result);
	printLog("4. Subtraction (A - B) mod N:\n" + LongModSub(A, B, N).Hex_convert() + "\n", Result);

	printLog("5. Multiplication (A * B) mod N:\n" + LongModMul(A, B, N, mu_N).Hex_convert() + "\n", Result);
	printLog("6. Square (A^2) mod N:\n" + LongModSquare(A, N, mu_N).Hex_convert() + "\n", Result);

	BigInt powerExp("10001");
	printLog("7. Power (A ^ 10001) mod N:\n" + LongModPowerBarrett(A, powerExp, N).Hex_convert() + "\n", Result);

	// ========================================================================
	// BLOCK 2: CORRECTNESS VERIFICATION
	// ========================================================================
	printLog("=== BLOCK 2: CORRECTNESS VERIFICATION ===", Corect);

	BigInt C("1A2B3C4D5E");
	BigInt sumAB = LongModAdd(A, B, N);
	BigInt leftSide1 = LongModMul(sumAB, C, N, mu_N);

	BigInt ac = LongModMul(A, C, N, mu_N);
	BigInt bc = LongModMul(B, C, N, mu_N);
	BigInt rightSide1 = LongModAdd(ac, bc, N);

	printLog("\nTest 1: (A + B) * C == A * C + B * C (mod N)", Corect);
	if (LongCmp(leftSide1, rightSide1) == 0) printLog("Result: PASSED!", Corect);
	else printLog("Result: FAILED!", Corect);

	int m_val = 100;
	BigInt M(m_val);
	BigInt leftSide2 = LongModMul(M, A, N, mu_N);

	BigInt rightSide2(0);
	for (int i = 0; i < m_val; i++) {
		rightSide2 = LongModAdd(rightSide2, A, N);
	}

	printLog("\nTest 2: 100 * A == A + A + ... + A (mod N)", Corect);
	if (LongCmp(leftSide2, rightSide2) == 0) printLog("Result: PASSED!", Corect);
	else printLog("Result: FAILED!", Corect);

	BigInt n_euler("1B");
	BigInt a_euler("2");
	BigInt phi_n("12");

	BigInt euler_res = LongModPowerBarrett(a_euler, phi_n, n_euler);

	printLog("\nTest 3: a^phi(n) == 1 (mod n) for n = 27", Corect);
	if (LongCmp(euler_res, BigInt(1)) == 0) printLog("Result: PASSED!", Corect);
	else printLog("Result: FAILED!", Corect);

	// ========================================================================
	// BLOCK 3: PERFORMANCE BENCHMARKS
	// ========================================================================
	printLog("\n=== BLOCK 3: PERFORMANCE BENCHMARKS (IN MILLISECONDS) ===", Time);
	printLog("------------------------------------------------", Time);

	int iters_fast = 1000;
	int iters_slow = 100;

	auto benchmark = [&](const string& name, auto operation, int iters) {
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < iters; i++) {
			operation();
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> time_span = end - start;
		double avg_time = time_span.count() / iters;

		Time << left << setw(25) << name << "| " << fixed << setprecision(6) << avg_time << " ms\n";
		cout << left << setw(25) << name << "| " << fixed << setprecision(6) << avg_time << " ms\n";
		};

	benchmark("LongGCD", [&]() { LongGCD(A_orig, B_orig); }, iters_slow);
	benchmark("LongLCM", [&]() { LongLCM(A_orig, B_orig); }, iters_slow);
	benchmark("LongModAdd", [&]() { LongModAdd(A, B, N); }, iters_fast);
	benchmark("LongModSub", [&]() { LongModSub(A, B, N); }, iters_fast);
	benchmark("LongModMul (Barrett)", [&]() { LongModMul(A, B, N, mu_N); }, iters_fast);
	benchmark("LongModSquare (Barrett)", [&]() { LongModSquare(A, N, mu_N); }, iters_fast);
	benchmark("LongModPower (Barrett)", [&]() { LongModPowerBarrett(A, B, N); }, iters_slow);

	cout << "\n[SUCCESS] All operations completed. Results written to files." << endl;

	Result.close();
	Corect.close();
	Time.close();

	return 0;
}
