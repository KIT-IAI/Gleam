#ifndef _strings_
#define _strings_

// #include <iostream.h>
#include <iostream>  /* wj! */

#include "schalter.h"  /* wj! */
#include "bcc.h"       /* wj! */


class string {
  char *s;
  int   l;
  char *sep;
  char *tk;
  int   tp;
public:
  string substr(int pos, int len);
  int search(int, int, const char*, int = -1) const;
  int search(int, int, char) const;
public:
  string();
  string(const char);
  string(const char *);
  string(const string &);
  ~string();
  
  string& operator=(const char);
  string& operator=(const char *);
  string& operator=(const string &);
  
  friend string operator+(const string &, const string &);
  
  string& operator+=(char);
  string& operator+=(const char *);
  string& operator+=(const string &);
  
  char& operator[](int i);
  const char& operator[](int i) const;
  
  friend const char *pointer(const string &a) { return(a.s); };
  
  friend std::ostream& operator<<(std::ostream&, const string &);  /* WJ: std::   */
  friend std::istream& operator>>(std::istream&, string &);        /*  eingefuegt */
  
  friend bool operator==(const string &x, const char *s);
  friend bool operator==(const string &x, const string &y);
  friend bool operator!=(const string &x, const char *s);
  friend bool operator!=(const string &x, const string &y);
  
  friend string upcase(const string &);
  friend string downcase(const string &);
  int length() const { return(l); };

  friend string str(const double v);
//  friend string str(const long v);
  friend double val(const string &s);
  friend int vallen(const string &s);

  string token();
  int token(const string &s);
  int next();
  
  int index(char c, int startpos = 0) const    
    { return search(startpos, l, c); };
  int index(const string& y, int startpos = 0) const     
    { return search(startpos, l, y.s, y.l); };
  int index(const char* t, int startpos = 0) const 
    { return search(startpos, l, t); };
  int contains(const string &x)
    { return(index(x) >= 0); };
    
  string at(int pos, int len)    { return(substr(pos,len)); };
  string before(int pos)         { return(substr(0,pos)); };
  string before(const string &x) { return(substr(0,index(x))); };
  string after(int pos)          { return(substr(pos+1,l-pos)); };
  string after(const string &x)  
    { int p = index(x); return(substr(p+x.l, l-p-x.l)); };
  string through(const string &x)
    { return(substr(0,index(x)+x.l)); };
  string from(const string &x)
    { int p = index(x); return(substr(p, l-p)); };
  
  void error(const char *) const;
};
  
string str(const double v);  /* wj */


#endif

