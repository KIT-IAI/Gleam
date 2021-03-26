#ifndef _function_
#define _function_

#include <math.h>

#include "rcStrings.h"   /* wj */
#include "map.h"
#include "vector.h"


#define ERR_OK        0

#define ERR_LFTPAR  -500   /* Left parent expected */
#define ERR_RGTPAR  -501   /* Right parent expected */
#define ERR_COMMA   -502   /* Comma expected */
#define ERR_WRGCHR  -503   /* Unexpected character */
#define ERR_WRGVAR  -504   /* Undefined variable */
#define ERR_EQUAL   -505   /* Equal expected */
#define ERR_FNNAME  -506   /* Function name expected */
#define ERR_DBLVAR  -507   /* Double defined variable */
#define ERR_VAREXP  -508   /* variable expected */

#define EOL '\n'

enum operators {o_nop, o_add, o_sub, o_mul, o_div, o_pwr, o_val, o_var,
		o_neg, o_abs, o_ln, o_log, o_exp, o_sqrt, o_sin, o_cos, 
		o_tan, o_arcsin, o_arccos, o_arctan,  o_sinh, o_cosh,  
		o_tanh, o_arctanh, o_min, o_max, o_if};

#define Null (Node *)NULL

typedef enum operators Ops;

typedef Map<string, double> Varlist;
typedef Mapiter<string, double> Varlistiter;
typedef Map<string, double> Paramlist;
typedef Mapiter<string, double> Paramlistiter;

class Node {
public:
  Node *left;
  Node *right;
public:
  Node() 
    {left = Null; right = Null; };
  Node(Node *l, Node *r)
    { left = l; right = r; };
  virtual ~Node()
    { if (left) delete left; if (right) delete right; } ;
  virtual Node *copy() 
    { return(new Node(left ? left->copy() : Null, 
    		right ? right->copy() : Null)); };
  virtual string toString() 
    { return(""); };
  virtual operator double() 
    { return(0); };
  virtual void setvar(string n, double v) 
    { if (left) left->setvar(n,v); if (right) right->setvar(n,v); };
  virtual int getvar(string n, double &v)
    { if (left) left->getvar(n, v); if (right) right->getvar(n, v); return 0; };
  virtual int isvar(string n)
    { return (left ? left->isvar(n) : 0) || (right ? right->isvar(n) : 0); };
  virtual void varlist(string &vl) 
    { if (left) left->varlist(vl); if (right) right->varlist(vl); };
  virtual int isconst()
    { return((left ? left->isconst() : true) && (right ? right->isconst() : true)); };
  virtual int islinear(string &v)
    { return((left ? left->islinear(v) : true) && (right ? right->islinear(v) : true)); };
  virtual Node *derive(string n)
    { return(Null); };
  virtual Ops type()
    { return(o_nop); };
  virtual Node *simplify(int &s)
    { return(new Node(left ? left->simplify(s) : Null, 
    		right ? right->simplify(s) : Null)); };
};


class Function : public Node {
  Ops op;
  Node *cond;
public:
  Function(Ops o, Node *c, Node *l, Node *r) : Node(l, r) { op = o; cond = c; };
  Function(Ops o, Node *l, Node *r) : Node(l, r) { op = o; cond = Null; };
  Function(Ops o, Node *l) : Node(l, Null) { op = o; cond = Null; };
  Node *copy() 
    { return(new Function(op, (cond ? cond->copy() : Null), left->copy(), (right ? right->copy() : Null))); };
  string toString();
  operator double();
  Node *derive(string n);
  Ops type() { return(op); };
  Node *simplify(int &s);
  int islinear(string &v) { return !left->isvar(v) && (right ? !right->isvar(v) : true); };
  void setvar(string n, double v) 
    { if (cond) cond->setvar(n,v); if (left) left->setvar(n,v); if (right) right->setvar(n,v); };
};


class Value : public Node {
  double value;
public:
  Value(double v) { value = v; };
  Node *copy() { return(new Value(value)); };
  string toString() { return(str(value)); };
  operator double() { return(value); };
  Node *derive(string n) { return(new Value(0.0)); };
  Ops type() { return(o_val); };
  Node *simplify(int &s) { return(new Value(value)); };
};


class Variable : public Node {
  string name;
  double value;
public:
  Variable(string n) : Node(Null, Null) { name = n; value = 0.0; };
  Variable(string n, double v) : Node(Null, Null) { name = n; value = v; };
  Node *copy() { return(new Variable(name, value)); };
  string toString() { return(name); };
  operator double() { return(value); };
  void setvar(string n, double v) { if (name == n) value = v; };
  int getvar(string n, double &v)
    { if (name == n) v = value; return(name == n); };
  int isvar(string n)
    { return (name == n); };
  void varlist(string &vl)
    { if (!vl.contains(" "+name+",")) vl += " "+name+","; };
  int isconst() { return(false); };
  Node *derive(string n)
    { return ((n == name) ? new Value(1.0) : new Value(0.0)); };
  Ops type() { return(o_var); };
  Node *simplify(int &s) { return(new Variable(name, value)); };
};


class Negate : public Node {
public:
  Negate(Node *n) : Node(n, Null) { ; } ;
  Node *copy() { return(new Negate(left->copy())); };
  string toString()
    { return "-" + left->toString(); };
  operator double() { return(-(double)*left); };
  Node *derive(string n) { return(new Negate(left->derive(n))); };
  Ops type() { return(o_neg); };
  Node *simplify(int &s);
};


class Power : public Node {
public:
  Power(Node *l, Node *r) : Node(l, r) { ; } ;
  Node *copy() { return(new Power(left->copy(), right->copy())); };
  string toString();
  operator double() { return(pow((double)*left, (double)*right)); };
  Node *derive(string n);
  Ops type() { return(o_pwr); };
  Node *simplify(int &s);
  int islinear(string &v) { return !left->isvar(v) && !right->isvar(v); };
};


class Product : public Node {
  Ops op;
public:
  Product(Ops o, Node *l, Node *r) : Node(l, r) { op = o; };
  Node *copy() { return(new Product(op, left->copy(), right->copy())); };
  string toString();
  operator double() 
    { return((op == o_mul) ? (double)*left * (double)*right
      : (double)*left / (double)*right); };
  Node *derive(string n);
  Ops type() { return(op); };
  Node *simplify(int &s);
  int islinear(string &v);
};


class Sum : public Node {
  Ops op;
public:
  Sum(Ops o, Node *l, Node *r) : Node(l, r) { op = o; };
  Node *copy() { return(new Sum(op, left->copy(), right->copy())); };
  string toString();
  operator double()
    { return ((op == o_add) ? (double)*left + (double)*right : 
      (double)*left - (double)*right); };
  Node *derive(string n);
  Ops type() { return(op); };
  Node *simplify(int &s);
};


class function {
protected:
  int err;
  string fname;
  Map<string, double> vars;
  Map<string, double> params;
  Node *root;
private:
  Node *sum(string &f);
  Node *product(string &f);
  Node *power(string &f);
  Node *signfactor(string &f);
  Node *factor(string &f);
  Ops isfunction1(string s);
  Ops isfunction2(string s);
  Ops isfunction3(string s);
  void checkparam();
public:
  function();
  function(const function &f);
  function& operator=(const string &s);
  function& operator=(const function &f);
  string toString(bool vv = false);
  operator double();
  double operator()(const vector&);
  double& operator[](const string &n)
    { return (vars.defined(n) ? vars[n] : params[n]); };
  Map<string, double>& variables() { return vars; };
  Map<string, double>& parameters() { return params; };
  string& name() { return fname; };
  function derive(string n);
  void simplify();
  int islinear(string v) { return root->islinear(v); };
};

#endif
