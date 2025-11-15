vector<int> man_odd(string t) {
  int n=t.size();
  vector<int> z(n,0);
  int r=1;
  int last=0;
  for (int i=1; i<n; ++i) {
    r=max(r,i);
    if (r>i) {
      if (i+z[last-(i-last)]<r) {
        z[i]=z[last-(i-last)];
        continue;
      }
      z[i]=r-i;
    }
    while (t[i+z[i]]==t[i-z[i]]) ++z[i];
    if (i+z[i] > r) {
      last=i;
      r=i+z[i];
    }
  }
  return z;
}
vector<int> man(string s) {
  string t="$";
  for(auto&x:s) {
    t+='#';
    t+=x;
  }
  t+="#*";
  auto z1=man_odd(t);
  vector<int> z;
  for (int i=1; i<z1.size()-1; ++i) z.pb(z1[i]);
  return z;
}


