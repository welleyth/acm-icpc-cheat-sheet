Map Pi(ll n, VLL CR a) { // a = {n // 1, n // 2, n // 3, ..., 1}
    Map res(n); // 2 arrays, s = sqrt(n): [0, s], and N / k \in [0, N / s + 1]
    for (auto i : a)
        res.at(i) = i - 1;

    for (int i = 2; i <= res.s; i++) {
        if (res.at(i) == res.at(i - 1))
            continue;

        ll f = res.at(i - 1);
        for (auto j : a) {
            if (j < (ll)i * i)
                break;

            res.at(j) -= res.at(j / i) - f;
        }
    }
    return res;
}

// cnt - global cnt for n // i, upto - precount on small primes
ll get(ll n, int ind) {
    ll p = pr[ind];
    if (p > n)
        return 1;

    if (p * p > n)
        return (1 + (cnt.at(n) - upto[p] + 1) * pre[1]) % MOD;

    __int128_t res = 0;
    for (ll f = 1, c = 0; f <= n; f *= p, c++)
        res += pre[c] * get(n / f, ind + 1);
    return res % MOD;
}

