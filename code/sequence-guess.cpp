#include <bits/stdc++.h>

using namespace std;
using int64 = long long;

// This helper works with prime moduli (default 1e9+7). Change MOD if needed.
static const int64 DEFAULT_MOD = 1000000007LL;

int64 mod_pow(int64 a, int64 e, int64 mod) {
	a %= mod;
	int64 r = 1 % mod;
	while (e > 0) {
		if (e & 1) r = (__int128)r * a % mod;
		a = (__int128)a * a % mod;
		e >>= 1;
	}
	return r;
}

int64 mod_inv(int64 a, int64 mod) {
	a %= mod;
	if (a < 0) a += mod;
	// mod must be prime, or you must replace this with extended gcd.
	return mod_pow(a, mod - 2, mod);
}

// Builds the binomial-basis coefficients (forward differences) of the sequence.
// coeffs[i] = Δ^i a_0. Works modulo mod (prime).
vector<int64> binomial_basis_coeffs(vector<int64> seq, int64 mod = DEFAULT_MOD) {
	vector<int64> coeffs;
	if (seq.empty()) return coeffs;
	for (int i = 0; !seq.empty(); ++i) {
		coeffs.push_back((seq[0] % mod + mod) % mod);
		if (seq.size() == 1) break;
		vector<int64> nxt(seq.size() - 1);
		bool zero = true;
		for (size_t j = 1; j < seq.size(); ++j) {
			int64 val = (seq[j] - seq[j - 1]) % mod;
			if (val < 0) val += mod;
			nxt[j - 1] = val;
			if (val != 0) zero = false;
		}
		if (zero) break;
		seq.swap(nxt);
	}
	return coeffs;
}

int64 eval_binomial_poly(const vector<int64> &coeffs, int64 n, int64 mod) {
	int64 ans = 0;
	int64 comb = 1;
	for (size_t i = 0; i < coeffs.size(); ++i) {
		ans = (ans + coeffs[i] * comb) % mod;
		int64 numer = (n - (int64)i) % mod;
		if (numer < 0) numer += mod;
		int64 denom = mod_inv((int64)i + 1, mod);
		comb = comb * numer % mod * denom % mod;
	}
	return ans;
}

// Evaluate P(n) where P interpolates points (i, y[i]).
int64 lagrange_eval(const vector<int64> &y, int64 n, int64 mod = DEFAULT_MOD) {
	int m = (int)y.size();
	if (m == 0) return 0;
	if (n < m) return (y[n] % mod + mod) % mod;
	vector<int64> fact(m), inv_fact(m);
	fact[0] = 1;
	for (int i = 1; i < m; ++i) fact[i] = fact[i - 1] * i % mod;
	inv_fact[m - 1] = mod_inv(fact[m - 1], mod);
	for (int i = m - 1; i > 0; --i) inv_fact[i - 1] = inv_fact[i] * i % mod;
	vector<int64> pref(m), suf(m);
	pref[0] = (n % mod + mod) % mod;
	for (int i = 1; i < m; ++i) {
		int64 val = (n - i) % mod;
		if (val < 0) val += mod;
		pref[i] = pref[i - 1] * val % mod;
	}
	suf[m - 1] = (n - (m - 1)) % mod;
	if (suf[m - 1] < 0) suf[m - 1] += mod;
	for (int i = m - 2; i >= 0; --i) {
		int64 val = (n - i) % mod;
		if (val < 0) val += mod;
		suf[i] = suf[i + 1] * val % mod;
	}
	int64 ans = 0;
	for (int i = 0; i < m; ++i) {
		int64 numer = 1;
		if (i > 0) numer = pref[i - 1];
		if (i + 1 < m) numer = numer * suf[i + 1] % mod;
		int64 denom = fact[i] * fact[m - 1 - i] % mod;
		if ((m - 1 - i) & 1) denom = (mod - denom) % mod;
		int64 term = (y[i] % mod + mod) % mod;
		term = term * numer % mod * mod_inv(denom, mod) % mod;
		ans = (ans + term) % mod;
	}
	return ans;
}

// Standard Berlekamp-Massey over prime field.
vector<int64> berlekamp_massey(const vector<int64> &s, int64 mod = DEFAULT_MOD) {
	vector<int64> C(1, 1), B(1, 1);
	int64 L = 0, m = 1, b = 1;
	for (int64 n = 0; n < (int64)s.size(); ++n) {
		int64 d = 0;
		for (int64 i = 0; i <= L; ++i) {
			d = (d + C[i] * s[n - i]) % mod;
		}
		if (d == 0) {
			++m;
			continue;
		}
		vector<int64> T = C;
		int64 coef = d * mod_inv(b, mod) % mod;
		if (C.size() < B.size() + m) C.resize(B.size() + m, 0);
		for (size_t i = 0; i < B.size(); ++i) {
			int64 val = (C[i + m] - coef * B[i]) % mod;
			if (val < 0) val += mod;
			C[i + m] = val;
		}
		if (2 * L <= n) {
			L = n + 1 - L;
			B = T;
			b = d;
			m = 1;
		} else {
			++m;
		}
	}
	C.erase(C.begin());
	for (auto &x : C) {
		x = (mod - x) % mod;
	}
	return C; // s_n = sum_{i=1}^{L} C[i-1] * s_{n-i}
}

vector<int64> combine_poly(const vector<int64> &a, const vector<int64> &b, const vector<int64> &rec, int64 mod) {
	int k = (int)rec.size();
	vector<int64> res(2 * k + 1);
	for (int i = 0; i < k; ++i) {
		for (int j = 0; j < k; ++j) {
			res[i + j] = (res[i + j] + (__int128)a[i] * b[j]) % mod;
		}
	}
	for (int i = 2 * k - 1; i >= k; --i) {
		if (res[i] == 0) continue;
		for (int j = 1; j <= k; ++j) {
			res[i - j] = (res[i - j] + (__int128)res[i] * rec[j - 1]) % mod;
		}
	}
	res.resize(k);
	return res;
}

int64 linear_recurrence_nth(const vector<int64> &rec, const vector<int64> &init, int64 n, int64 mod = DEFAULT_MOD) {
	int k = (int)rec.size();
	if (k == 0) return (init.empty() ? 0 : (init[0] % mod + mod) % mod);
	if (n < (int)init.size()) return (init[n] % mod + mod) % mod;
	vector<int64> pol(k), e(k);
	pol[0] = 1;
	if (k == 1) {
		e[0] = rec[0] % mod;
	} else {
		e[1] = 1;
	}
	for (int64 m = n; m > 0; m >>= 1) {
		if (m & 1) pol = combine_poly(pol, e, rec, mod);
		e = combine_poly(e, e, rec, mod);
	}
	int64 ans = 0;
	for (int i = 0; i < k; ++i) {
		ans = (ans + (__int128)pol[i] * init[i]) % mod;
	}
	if (ans < 0) ans += mod;
	return ans;
}

struct SequenceGuesser {
	int64 mod;
	vector<int64> values;
	explicit SequenceGuesser(int64 mod = DEFAULT_MOD) : mod(mod) {}
	void add(int64 v) {
		values.push_back((v % mod + mod) % mod);
	}
	vector<int64> polynomial_basis() const {
		return binomial_basis_coeffs(values, mod);
	}
	vector<int64> recurrence() const {
		return berlekamp_massey(values, mod);
	}
	int64 polynomial_nth(int64 n) const {
		return lagrange_eval(values, n, mod);
	}
	int64 recurrence_nth(int64 n) const {
		if (values.empty()) return 0;
		if (n < (int64)values.size()) {
			return (values[n] % mod + mod) % mod;
		}
		auto rec = recurrence();
		if (rec.empty()) return (values[0] % mod + mod) % mod;
		int need = (int)rec.size();
		if ((int)values.size() < need) {
			// Not enough initial terms to apply the recurrence, fall back to polynomial interpolation.
			return lagrange_eval(values, n, mod);
		}
		vector<int64> init(values.begin(), values.begin() + need);
		return linear_recurrence_nth(rec, init, n, mod);
	}
};

/*
Usage sketch:
	vector<long long> sample = {0, 1, 4, 9, 16};
	auto coeffs = binomial_basis_coeffs(sample);
	long long a10 = eval_binomial_poly(coeffs, 10, DEFAULT_MOD); // polynomial guess
	auto rec = berlekamp_massey(sample);
	long long a100 = linear_recurrence_nth(rec, sample, 100, DEFAULT_MOD);
*/

