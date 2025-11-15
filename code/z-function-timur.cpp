vector<int> zf(string s) {
  int n=s.size();
  vector<int> z(n,0);
  int r=1, last=0;
  s+='#';
  for (int i=1; i<n; ++i) {
    r=max(r,i);
    if (i<r) {
      if (i+z[i-last] < r) {
        z[i]=z[i-last];
        continue;
      }
      z[i]=r-i;
    }
    while (s[i+z[i]]==s[z[i]]) ++z[i];
    if (i+z[i] > r) {
      r=i+z[i];
      last=i;
    }
  }
  return z;
}


