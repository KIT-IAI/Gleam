#include <ctype.h>
#include <stdio.h>
#include "rcStrings.h"


inline static int scmp(const char* a, const char* b)
{
  if (b == 0)
    return *a != 0;
  else {
    signed char diff = 0;
    while ((diff = *a - *b++) == 0 && *a++ != 0);
    return diff;
  }
}


inline static int slen(const char* t)
{
  if (t == 0)
    return 0;
  else {
    const char* a = t;
    while (*a++ != 0);
    return a - 1 - t;
  }
}


inline static void scopy(const char* from, char* to)
{
  if (from != 0) while((*to++ = *from++) != 0);
}


inline static void sncopy(const char* from, char* to, int n)
{
  if ((from != 0) && (n > 0))
    while (((*to++ = *from++) != 0) && (--n > 0));
  *to = 0;
}


inline static void scat(const char* str, const char* with, char* to)
{
  *to = 0;
  if (str != 0) {
    while((*to++ = *str++) != 0);
    to--;
  }
  if (with != 0)
    while((*to++ = *with++) != 0);
}




string::string()
{
  s = new char[1];
  s[0] = '\0';
  l = 0;
  tk = NULL;
}

string::string(const char a)
{
  l = 1;
  s = new char[l + 1];
  s[0] = a; s[1] = '\0';
  tk = NULL;
}


string::string(const char *a)
{
  l = slen(a);
  s = new char[l + 1];
  s[0] = '\0';
  scopy(a, s);
  tk = NULL;
}

string::string(const string &a)
{
  l = a.l;
  s = new char[l + 1];
  scopy(a.s, s);
  tk = NULL;
}

string::~string()
{
  if (s != 0)
    delete s;
  if (tk != 0)
    delete tk;
  if (!sep)
    delete sep;
}

void string::error(const char *msg) const
{
  std::cerr << "Error in Class string: " << msg << std::endl;
//  exit(1);
}

string string::substr(int pos, int len)
{
  string r;
  
  r.l = 0;
  r.s = new char[len+1];
  sncopy(s+pos, r.s, len);
  while (r.s[r.l] != 0) r.l++;
  
  return(r);
}

int string::search(int start, int sl, char c) const
{
  if (sl > 0) {
    if (start >= 0) {
      const char* a = &(s[start]);
      const char* lasta = &(s[sl]);
      while (a < lasta) if (*a++ == c) return --a - s;
    } else {
      const char* a = &(s[sl + start + 1]);
      while (--a >= s) if (*a == c) return a - s;
    }
  }
  return -1;
}

int string::search(int start, int sl, const char* t, int tl) const
{
  if (tl < 0) tl = slen(t);
  if (sl > 0 && tl > 0) {
    if (start >= 0) {
      const char* lasts = &(s[sl - tl]);
      const char* lastt = &(t[tl]);
      const char* p = &(s[start]);

      while (p <= lasts) {
        const char* x = p++;
        const char* y = t;
        while (*x++ == *y++) if (y >= lastt) return --p - s;
      }
    } else {
      const char* firsts = &(s[tl - 1]);
      const char* lastt =  &(t[tl - 1]);
      const char* p = &(s[sl + start + 1]); 

      while (--p >= firsts) {
        const char* x = p;
        const char* y = lastt;
        while (*x-- == *y--) if (y < t) return ++x - s;
      }
    }
  }
  return -1;
}


string& string::operator=(const char a)
{
  if (s != 0)
    delete s;
  l = 1;
  s = new char[l + 1];
  s[0] = a; s[1] = '\0';
  
  return(*this);
}

string& string::operator=(const char *a)
{
  if (s != 0)
    delete s;
  l = slen(a);
  s = new char[l + 1];
  s[0] = '\0';
  scopy(a, s);
  
  return(*this);
}

string& string::operator=(const string &a)
{
  if (s != 0)
    delete s;
  l = a.l;
  s = new char[l + 1];
  scopy(a.s, s);
  
  return(*this);
}


string operator+(const string &a, const string &b)
{
  string x;
  
  x.l = a.l + b.l;
  x.s = new char[x.l + 1];
  scat(a.s, b.s, x.s);
  
  return(x);
}

string& string::operator+=(char b)
{
  char *x;
  
  l++;
  x = new char[l + 1];
  if (s != 0)
    scopy(s, x);
  else 
    scopy("", x);
  x[l-1] = b; x[l] = '\0';
  if (s != 0)
    delete s;
  s = x;
  
  return(*this);
}


string& string::operator+=(const char *b)
{
  char *x;
  
  l += slen(b);
  x = new char[l + 1];
  scat(s, b, x);
  if (s != 0)
    delete s;
  s = x;

  return(*this);
}


string& string::operator+=(const string &b)
{
  char *x;
  
  l += b.l;
  x = new char[l + 1];
  scat(s, b.s, x);
  if (s != 0)
    delete s;
  s = x;

  return(*this);
}

char& string::operator[](int i)
{
  if (i < 0 || i > l)
    error("index out of range");
  return(s[i]);
}

const char& string::operator[](int i) const
{
  if (i < 0 || i > l)
    error("index out of range");
  return(s[i]);
}

bool operator==(const string &x, const char *s)
{ 
  return(scmp(x.s, s) == 0);
}

bool operator==(const string &x, const string &y)
{
  return(scmp(x.s, y.s) == 0);
}

bool operator!=(const string &x, const char *s)
{
  return(scmp(x.s, s) != 0);
}

bool operator!=(const string &x, const string &y)
{
  return(scmp(x.s, y.s) != 0);
}
 
std::ostream& operator<<(std::ostream& s, const string &x) 
{
  return(s << x.s);
}

std::istream& operator>>(std::istream& s, string &x)
{
  char buf[256], c;

  s.get(buf, 255, '\n');
  x = buf;
  while (s.get(c) && c != '\n') {
    s.get(buf, 255, '\n');
    x += buf;
  }
  return(s);
}


string upcase(const string &s)
{
  string x;
  
  x = s;
  for (int i = 0; i < x.l; i++)
    x.s[i] = toupper(x.s[i]);

  return(x);
}

string downcase(const string &s)
{
  string x;
  
  x = s;
  for (int i = 0; i < x.l; i++)
    x.s[i] = tolower(x.s[i]);

  return(x);
}

double val(const string &s)
{ int		sign = 1;
  double	pos = 1;
  int		exponent = 0;
  int		expsign = 1;
  double        v;
  int           p = 0;
  
  while(s.s[p] == ' ') p++;
  v = 0;
  if (s.s[p] == '-') {
    sign = -1;
    p++;
  }
  while (isdigit(s.s[p])) {
    v = v * 10 + ((int)s.s[p] - (int)'0');
    p++;
  }
  if (s.s[p] == '.') {
    p++;
    while (isdigit(s.s[p])) {
      pos = pos / 10;
      v = v + pos * ((int)s.s[p] - (int)'0');
      p++;
    }
  }
  if ((s.s[p] == 'E') || (s.s[p] == 'e')) {
    p++;
    if (s.s[p] == '+')
      p++;
    else
      if (s.s[p] == '-') {
        expsign = -1;
        p++;
      }
    while (isdigit(s.s[p])) {
      exponent = exponent * 10 + ((int)s.s[p] - (int)'0');
      p++;
    }
  }
  v = sign * v;
  if (expsign > 0)
    for ( ;exponent > 0; exponent--)
      v *= 10;
  else 
    for ( ;exponent > 0; exponent--)
      v /= 10;
  return(v);
}


int vallen(const string &s)
{
  int           p = 0;
  
  while(s.s[p] == ' ') p++;
  if (s.s[p] == '-') {
    p++;
  }
  while (isdigit(s.s[p])) {
    p++;
  }
  if (s.s[p] == '.') {
    p++;
    while (isdigit(s.s[p])) {
      p++;
    }
  }
  if ((s.s[p] == 'E') || (s.s[p] == 'e')) {
    p++;
    if (s.s[p] == '+')
      p++;
    else
      if (s.s[p] == '-') {
        p++;
      }
    while (isdigit(s.s[p])) {
      p++;
    }
  }

  return(p);
}


/*
string str(const long v)
{
  char sx[16];
  int p = 0, z = 0;
  int d = 1000000000;
  long x = v;
  
  if (x < 0) {
    sx[p++] = '-';
    x = -x;
  }
  while (d > 0) {
    if (x > d) {
      z = x / d;
      sx[p++] = '0' + z;
      x = x - z * d;
    }
    d /= 10;
  }
  sx[p] = 0;

  return(string(sx));
}
*/

string str(const double v)
{
  char sx[80];

  sprintf(sx, "%g", v);
  
  return(string(sx));
}


string string::token()
{
  if (tk != 0)
    return(string(tk));
  else
    return(string(""));
}


int string::token(const string &a)
{
  tp = 0;
  if (sep == 0) delete sep;
  sep = new char[a.l + 1];
  scopy(a.s, sep);
  return(1);
}


int string::next()
{
  int i, j, tkl;
  char *otk;
  
  if ((tp >= l) || (sep == 0)) return(0);
  
  if (tk != 0) delete tk;
  tkl = 0;
  tk = new char[1];
  for (i = 0; (sep[i] != '\0') && (s[tp] != sep[i]); i++) ;
  while (sep[i] != '\0') {
    tp++;
    if (tp >= l) return(0);
    for (i = 0; (sep[i] != '\0') && (s[tp] != sep[i]); i++) ;
  }
  while (sep[i] == '\0') {
    otk = tk;
    tk = new char[tkl+1];
    for (j = 0; j < tkl; j++)
      tk[j] = otk[j];
    tk[tkl++] = s[tp++];
    for (i = 0; (sep[i] != '\0') && (s[tp] != sep[i]); i++) ;
  }
  tk[tkl] = '\0';
  
  return(1);
}

