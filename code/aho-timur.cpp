const int K=26;
struct aho {
  struct node {
    int link, p;
    int c[K];
    int end;
    int go[K];
    char pch;
    node() {
      link=end=p=-1;
      forn(i,K) c[i]=go[i]=-1;
    }
  };
 
  vector<node> t;
  aho() {
    t.pb(node());
  }
  vector<int> vis;
 
  void add(int v, string&s, int i, int ind) {
    if (i>=s.size()) {
      t[v].end = ind;
      return;
    }
    int b=s[i]-'a';
    if (t[v].c[b]==-1) {
      int z=t.size();
      t[v].c[b]=z;
      t.push_back(node());
      t[z].pch=s[i];
      t[z].p=v;
    }
    add(t[v].c[b],s,i+1,ind);
  }
  void add(string&s, int ind) {
    add(0,s,0,ind);
  }
 
  int getlink(int v) {
    if (t[v].link==-1) {
      if (v==0 || t[v].p==0) {
        t[v].link = 0;
      } else {
        t[v].link = go(getlink(t[v].p), t[v].pch);
      }
    }
    return t[v].link;
  }
  int go(int v, char c) {
    return go(v,(int)c-'a');
  }
  int go(int v, int c) {
    if (t[v].go[c]==-1) {
      if (t[v].c[c]==-1) {
        t[v].go[c] = (v==0) ? 0 : go(getlink(v), c);
      } else {
        t[v].go[c]=t[v].c[c];
      }
    }
    return t[v].go[c];
  }
 
  void answer(int v) {
    if (!v) return;
    if (vis[v]) return;
    vis[v]=1;
    if (t[v].end!=-1) ans[t[v].end]=1;
    answer(getlink(v));
   }
  void query(int v, string&s, int i) {
    answer(v);
    if (i>=s.size()) return;
    query(go(v,s[i]),s,i+1);
  }
  void query(string&s) {
    vis.assign(t.size(),0);
    query(0,s,0);
  }
};


