#include <ctype.h>

#include "schalter.h"
#include "strings.h"
#include "function.h"




#define max(a,b) (((a)>(b)) ? (a) : (b))
#define min(a,b) (((a)<(b)) ? (a) : (b))

string getident(string &f)
{
  string id = "";
  while ((f.length() > 0) && (isalnum(f[0]) || (f[0] == '_'))) {
    id += f[0];
    f = f.after(0);
  }
  return(id);
}


function::function()
{
  err = ERR_OK;
  fname = "f";
  vars["x"] = 0.0;
  root = new Value(0.0);
}


function::function(const function &f)
{
  err = ERR_OK;
  fname = f.fname;
  vars = f.vars;
  params = f.params;
  root = f.root->copy();
}


double function::operator()(const vector& x)
{
  if((unsigned int) vars.length() != x.dim())
  {
#ifdef BS_DOS
    cerr<<"In function::operator() : illegal number of parameters ("<<x.dim()<<")."<<endl;
#else
    std::cerr<<"In function::operator() : illegal number of parameters ("<<x.dim()<<")."<<std::endl;
#endif /* BS_DOS */
    exit(1);
  }
  unsigned int i=0;
  for  (Varlistiter v(vars); v; v++,i++)
    root->setvar(v.key(), x[i]);
  for (Paramlistiter p(params); p; p++)
    root->setvar(p.key(), p.value());
  return (double)*root;
}


function::operator double()
{
  for (Varlistiter v(vars); v; v++)
    root->setvar(v.key(), v.value());
  for (Paramlistiter p(params); p; p++)
    root->setvar(p.key(), p.value());
  return (double)*root;
}


function& function::operator=(const function &f)
{
  if (root != Null)
    delete root;
  vars.clear();
  params.clear();
  fname = f.fname;
  vars = f.vars;
  params = f.params;
  root = f.root->copy();
  
  return(*this);
}


function function::derive(string n)
{
  function g;

  if (g.root != Null)
    delete g.root;
  g.vars.clear();
  g.params.clear();
  g.fname = fname + "'";
  g.vars = vars;
  g.params = params;
  g.root = root->derive(n);
  // g.simplify();
  
  return(g);
}


void function::simplify()
{
  int s;

  do {
    s = false;
    Node *newroot = root->simplify(s);
    if (root != NULL) 
      delete root;
    root = newroot;
  } while (s);
  checkparam();
}


void function::checkparam()
{
//  std::cerr << "check parameters:" << std::endl;
  for (Paramlistiter p(params); p;) {
//    std::cerr << p.key();
    if (!root->isvar(p.key())) {
      p.remove();
//      std::cerr << " removed. " << p.key() << "is next." << std::endl;
    } else {
      p++;
//      std::cerr << " not removed." << std::endl;
    }
  }
}


Ops function::isfunction1(string s)
{
  if ("abs"    == s) return(o_abs);
  if ("sqrt"   == s) return(o_sqrt);
  if ("ln"     == s) return(o_ln);
  if ("log"    == s) return(o_log);
  if ("exp"    == s) return(o_exp);
  if ("sin"    == s) return(o_sin);
  if ("cos"    == s) return(o_cos);
  if ("tan"    == s) return(o_tan);
  if ("arcsin" == s) return(o_arcsin);
  if ("arccos" == s) return(o_arccos);
  if ("arctan" == s) return(o_arctan);
  if ("sinh"   == s) return(o_sinh);
  if ("cosh"   == s) return(o_cosh);
  if ("tanh"   == s) return(o_tanh);
  
  return(o_nop);
}


Ops function::isfunction2(string s)
{
  if ("min" == s) return(o_min);
  if ("max" == s) return(o_max);
  
  return(o_nop);
}


Ops function::isfunction3(string s)
{
  if ("if" == s) return(o_if);
  
  return(o_nop);
}


Node *function::factor(string &f)
{ 
  Node *n = (Node *) NULL;
  Ops o;
  
  if (isdigit(f[0]) || (f[0] == '.')) {
    n = new Value(val(f));
    f = f.after(vallen(f)-1);
  } else {
    if (isalpha(f[0]) || (f[0] == '_')) {
      string id = "";
      while ((f.length() > 0) && (isalnum(f[0]) || (f[0] == '_'))) {
        id += f[0];
        f = f.after(0);
      }
      if ((o = isfunction1(id)) != o_nop) {
        if (f[0] == '(') {
          f = f.after(0);
          n = new Function(o, sum(f));
          if (f[0] == ')')
            f = f.after(0);
          else
            err = ERR_RGTPAR; // Fehler
        } else
          err = ERR_LFTPAR; // Fehler
      } else {
        if ((o = isfunction2(id)) != o_nop) {
          if (f[0] == '(') {
            f = f.after(0);
            n = sum(f);
            if (f[0] == ',') {
              f = f.after(0);
              n = new Function(o, n, sum(f));
              if (f[0] == ')')
                f = f.after(0);
              else
                err = ERR_RGTPAR; // Fehler
            } else
              err = ERR_COMMA; // Fehler
          } else
            err = ERR_LFTPAR; // Fehler 
        } else {
          if ((o = isfunction3(id)) != o_nop) {
            if (f[0] == '(') {
              f = f.after(0);
              Node *n1 = sum(f);
              if (f[0] == ',') {
                f = f.after(0);
                Node *n2 = sum(f);
                if (f[0] == ',') {
                  f = f.after(0);
                  n = new Function(o, n1, n2, sum(f));
                  if (f[0] == ')')
                    f = f.after(0);
                  else
                    err = ERR_RGTPAR; // Fehler
                } else
                  err = ERR_COMMA; // Fehler
              } else
                err = ERR_COMMA; // Fehler
            }
          } else {
            n = new Variable(id);
            if (!vars.defined(id))
              if (!params.defined(id))
                params[id] = 0.0;
          }
        }
      }
    } else
      if (f[0] == '(') {
        f = f.after(0);
        n = sum(f);
        if (f[0] == ')')
          f = f.after(0);
        else
          err = ERR_RGTPAR;
      } else
        err = ERR_WRGCHR;
  }

// um abstuerze zu vermeiden

  if (n == NULL)
    n = new Value(0);
  return(n);
}


Node *function::power(string &f)
{ 
  Node *n = factor(f);
  while (f[0] == '^') {
    f = f.after(0);
    n = new Power(n, signfactor(f));
  }
  
  return(n);
}


Node *function::signfactor(string &f)
{ 
  Node *n = (Node *) NULL;
  
  if (f[0] == '-') {
    f = f.after(0);
    n = new Negate(power(f));
  } else {
    if (f[0] == '+')
      f = f.after(0);
    n = power(f);
  }

  return(n);
}


Node *function::product(string &f)
{ 
  Node *n = power(f);
  char t = f[0];
  while ((t == '*') || (t == '/')) {
    f = f.after(0);
    n = new Product((t == '*') ? o_mul : o_div, n, power(f));
    t = f[0];
  }
  
  return(n);
}


Node *function::sum(string &f)
{
  Node *n = product(f);
  char t = f[0];
  while ((t == '+') || (t == '-')) {
    f = f.after(0);
    n = new Sum((t == '+') ? o_add : o_sub, n, product(f));
    t = f[0];
  }
  
  return(n);
}


string function::toString(bool vv) 
{
  string f;
  Varlistiter v(vars);
  
  f = fname + '(';
  if (v) {
    f += v.key();
    if (vv) 
      f += "=" + str(v.value());
    v++;
  }
  for (; v; v++) {
    f += ',' + v.key();
      if (vv) 
        f += "=" + str(v.value());
  }
  f += ")";
  if (vv) {
    Paramlistiter p(params);
    if (p)
      f += "|" + p.key() + "=" + str(p.value());
    for (p++; p; p++)
      f += "," + p.key() + "=" + str(p.value());
  }  
  f += " = " +  root->toString();

  return(f); 
}


function &function::operator=(const string &s)
{
  string f,v;
  
  for (int i= 0; i < s.length(); i++)
    if (!isspace(s[i]))
      f += s[i];
      
  if (root != NULL)
    delete root;
  vars.clear();
  params.clear();
  
  fname = getident(f);
  if (fname != "") {
    while (f[0] == '\'') {
      fname += '\'';
      f = f.after(0);
    }
    if (f[0] == '(') {
      f = f.after(0);
      v = getident(f);
      if (v != "") {
        vars[v] = 0.0;
        if (f[0] == '=') {
          f = f.after(0);
          if (vallen(f) > 0) {
            vars[v] = val(f);
            f = f.after(vallen(f)-1);
          }
        }
        while (f[0] == ',') {
          f = f.after(0);
          v = getident(f);
          vars[v] = 0.0;
          if (f[0] == '=') {
            f = f.after(0);
            if (vallen(f) > 0) {
              vars[v] = val(f);
              f = f.after(vallen(f)-1);
            }
          }
        }
        if (f[0] == ')') {
          f = f.after(0);
          if (f[0] == '|') {
            f = f.after(0);
            v = getident(f);
            if (v != "") {
              params[v] = 0.0;
              if (f[0] == '=') {
                f = f.after(0);
                if (vallen(f) > 0) {
                  params[v] = val(f);
                  f = f.after(vallen(f)-1);
                }
              }
              while (f[0] == ',') {
                f = f.after(0);
                v = getident(f);
                params[v] = 0.0;
                if (f[0] == '=') {
                  f = f.after(0);
                  if (vallen(f) > 0) {
                    params[v] = val(f);
                    f = f.after(vallen(f)-1);
                  }
                }
              }
            }
          }
          if (f[0] == '=') {
            f = f.after(0);
            root = sum(f);
          } else
            err = ERR_COMMA;
        } else
          err = ERR_RGTPAR;
      } else
        err = ERR_VAREXP;
    } else
      err = ERR_LFTPAR;
  } else
    err = ERR_FNNAME;

  return *this;
}


// -=0=-=0=-=0=-=0=-=0=-=0=-=0=-=0=-=0=-=0=-=0=-=0=-=0=-=0=-=0=-=0=-=0=-

string Function::toString()
{
  string s;
  
  switch (op) {
    case o_abs:     s = "abs"; break;
    case o_sqrt:    s = "sqrt"; break;
    case o_ln:      s = "ln"; break;
    case o_log:     s = "log"; break;
    case o_exp:     s = "exp"; break;
    case o_sin:     s = "sin"; break;
    case o_cos:     s = "cos"; break;
    case o_tan:     s = "tan"; break;
    case o_arcsin:  s = "arcsin"; break;
    case o_arccos:  s = "arccos"; break;
    case o_arctan:  s = "arctan"; break;
    case o_sinh:    s = "sinh"; break;
    case o_cosh:    s = "cosh"; break;
    case o_tanh:    s = "tanh"; break;
    case o_min:     s = "min"; break;
    case o_max:     s = "max"; break;
    case o_if:      s = "if"; break;
    default:        break;
  }
  if (op != o_if) {
    if ((op != o_min) && (op != o_max)) {
      return(s + "(" + left->toString() + ")");
    } else {
      return(s + "(" + left->toString() + "," + 
    				right->toString() + ")");
    }
  } else {
    return(s + "(" + cond->toString() + "," + left->toString() + "," + 
    				right->toString() + ")");
  }
}

string Power::toString()
{
  string s;
  
  if ((left->type() == o_mul) || (left->type() == o_div) || 
      (left->type() == o_add) || (left->type() == o_sub) || 
      (left->type() == o_pwr)) {
    s = "(" + left->toString() + ")^";
  } else
    s = left->toString() + "^";
  if ((right->type() == o_mul) || (right->type() == o_div) || 
      (right->type() == o_add) || (right->type() == o_sub) || 
      (right->type() == o_pwr)) {
    s += "(" + right->toString() + ")";
  } else
    s += right->toString();

  return s;
} 


string Product::toString()
{
  string s;
  
  if ((left->type() == o_sub) || (left->type() == o_add)) {
    s = "(" + left->toString() + ")";
  } else
    s = left->toString();
  s += (op == o_mul ? "*" : "/");
  if ((right->type() == o_sub) || (right->type() == o_add) 
     || ((op == o_div) && (right->type() == o_mul))) {
    s += "(" + right->toString() + ")";
  } else
    s += right->toString(); 

  return s;
}

string Sum::toString()
{
  string s;
  
  s = left->toString() + (op == o_add ? "+" : "-");
  if ((type() == o_sub) 
     && ((right->type() == o_sub) || (right->type() == o_add)))
    s += "(" + right->toString() + ")";
  else 
    s += right->toString();

  return s;
}

Function::operator double()
{
  switch (op) {
    case o_abs:     return(fabs((double)*left));
    case o_sqrt:    return(sqrt((double)*left));
    case o_ln:      return(log((double)*left));
    case o_log:     return(log10((double)*left));
    case o_exp:     return(exp((double)*left));
    case o_sin:     return(sin((double)*left));
    case o_cos:     return(cos((double)*left));
    case o_tan:     return(tan((double)*left));
    case o_arcsin:  return(asin((double)*left));
    case o_arccos:  return(acos((double)*left));
    case o_arctan:  return(atan((double)*left));
    case o_sinh:    return(sinh((double)*left));
    case o_cosh:    return(cosh((double)*left));
    case o_tanh:    return(tanh((double)*left));
    case o_min:     return(min((double)*left, (double)*right));
    case o_max:     return(max((double)*left, (double)*right));
    case o_if:      return((double)*cond != 0 ? (double)*left : (double)*right);
    default:        return(0.0);
  }
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

int Product::islinear(string &v)
{
  if (left->isvar(v)) {
    if (!right->isvar(v))
      return left->islinear(v);
    return false;
  }
  if (right->isvar(v)) {
    if (!left->isvar(v) && (op == o_mul))
      return right->islinear(v);
    return false;
  }
  return true;
}

Node *Product::derive(string n)
{
  if (op == o_mul) {
    if (left->isvar(n))
      if (right->isvar(n)) 
        return new Sum(o_add, 
          new Product(o_mul, left->copy(), right->derive(n)),
          new Product(o_mul, left->derive(n), right->copy())); 
      else
        return new Product(o_mul, right->copy(), left->derive(n));
    else
      if (right->isvar(n))
        return new Product(o_mul, left->copy(), right->derive(n));
      else
        return new Value(0.0);
  } else {
    if (left->isvar(n))
      if (right->isvar(n)) 
        return 
          new Product(o_div,
          new Sum(o_sub, 
          new Product(o_mul, left->derive(n), right->copy() ),
          new Product(o_mul, left->copy(), right->derive(n) ) ),
          new Power(right->derive(n), new Value(2.0)) );
      else
        return new Product(o_div, left->derive(n), right->copy());
    else
      if (right->isvar(n))
        return
          new Negate( new Product(o_div,
          new Product(o_mul, left->copy(), right->derive(n)),
          new Power(right->copy(), new Value(2.0))));
      else
        return new Value(0.0);

  }
}


Node *Sum::derive(string n) 
{
  if (left->isvar(n))
    if (right->isvar(n))
      return new Sum(op, left->derive(n), right->derive(n));
    else
      return left->derive(n);
  else
    if (right->isvar(n))
      return (op == o_add ? right->derive(n) : new Negate(right->derive(n)));
    else
      return new Value(0.0);
}

Node *Function::derive(string n) 
{
  Node *d = Null;
  
  switch (op) {
    case o_sqrt:
      d = new Product(o_div, new Value(0.5), new Function(o_sqrt, left->copy()));
      break;
    case o_ln:
      d = new Product(o_div, new Value(1.0), left->copy());
      break;
    case o_log:
      d = new Product(o_div, new Value(log10(exp(1.0))), left->copy());
      break;
    case o_exp:
      d = new Function(o_exp, left->copy());
      break;
    case o_sin:
      d = new Function(o_cos, left->copy());
      break;
    case o_cos:
      d = new Negate(new Function(o_sin, left->copy()));
      break;
    case o_tan:
      d = new Product(o_div, new Value(1.0), new Power(new Function(o_cos, left->copy()), new Value(2.0)));
      break;
    case o_arcsin:
      d = new Product(o_div, new Value(1.0), 
      	new Function(o_sqrt, new Sum(o_sub, new Value(1.0), 
      	new Power(left->copy(), new Value(2.0)))));
      break;
    case o_arccos:
      d = new Negate(new Product(o_div, new Value(1.0), new Function(o_sqrt, 
        new Sum(o_sub, new Value(1.0), new Power(left->copy(), 
        new Value(2.0))))));
      break;
    case o_arctan:
      d = new Product(o_div, new Value(1.0), new Sum(o_add, new Value(1.0), 
        new Power(left->copy(), new Value(2.0))));
      break;
    case o_sinh:  
      d = new Function(o_cosh, left->copy());
      break;
    case o_cosh:  
      d = new Function(o_sinh, left->copy());
      break;
    case o_tanh:  
      d = new Product(o_div, new Value(1.0), new Power(
        new Function(o_cosh, left->copy()), new Value(2.0)));
      break;
    case o_abs:
#ifdef BS_DOS
      cerr << "Warning: Derivation of function abs not implemented correct." << endl;
#else
      std::cerr << "Warning: Derivation of function abs not implemented correct." << std::endl;
#endif /* BS_DOS */
      d = new Value(1.0);
      break;
    case o_max:
    case o_min:
#ifdef BS_DOS
      cerr << "Warning: Derivation of function max and min not implemented." << endl;
#else
      std::cerr << "Warning: Derivation of function max and min not implemented." << std::endl;
#endif /* BS_DOS */
      d = new Value(0.0);
      break;
    default:
      break;
  }
  return(new Product(o_mul, left->derive(n), d));
}


Node *Power::derive(string n) 
{
  if (left->isvar(n))
    return new Product(o_mul, 
             new Product(o_mul, right->copy(), left->derive(n)), 
             new Power(left->copy(), 
               new Sum(o_sub, right->copy(), new Value(1.0))));
  if (right->isvar(n))
    return new Product(o_mul, new Product(o_mul, right->derive(n), new Function(o_ln, left->copy())), new Power(left->copy(), right->copy()));
  return new Value(0.0); 
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


Node *Sum::simplify(int &s)
{
  if (left->isconst() && right->isconst()) {
    s = true;
    return((op == o_add ? new Value((double)*left + (double)*right) : 
    			  new Value((double)*left - (double)*right)));
  }
  if ((left->type() == o_val) && ((double)*left == 0.0)) {
    s = true;
    return(right->copy());
  }
  if ((right->type() == o_val) && ((double)*right == 0.0)) {
    s = true;
    return(left->copy());
  }
  return(new Sum(op, left->simplify(s), right->simplify(s)));
}


Node *Product::simplify(int &s)
{
  if (left->isconst() && right->isconst()) {
    s = true;
    return((op == o_mul ? new Value((double)*left * (double)*right) : 
    			  new Value((double)*left / (double)*right)));
  }
  if (op == o_mul) {
    if ((left->type() == o_val) && ((double)*left == 1.0)) {
      s = true;
      return(right->copy());
    }
    if ((right->type() == o_val) && ((double)*right == 0.0)) {
      s = true;
      return(new Value(0.0));
    }
  }
  if ((right->type() == o_val) && ((double)*right == 1.0)) {
    s = true;
    return(left->copy());
  }
  if ((left->type() == o_val) && ((double)*left == 0.0)) {
    s = true;
    return(new Value(0.0));
  }
  if ((left->type() == o_neg) && (right->type() == o_neg)) {
    s = true;
    return(new Product(op, left->left->copy(), right->left->copy()));
  }
  return(new Product(op, left->simplify(s), right->simplify(s)));
}

 
Node *Power::simplify(int &s)
{
  if (left->isconst() && right->isconst()) {
    s = true;
    return(new Value(pow((double)*left, (double)*right)));
  }
  if ((right->type() == o_val) && ((double)*right == 1.0)) {
    s = true;
    return(left->copy());
  }
  if ((right->type() == o_val) && ((double)*right == 0.0)) {
    s = true;
    return(new Value(1.0));
  }
  if ((left->type() == o_val) && (right->type() == o_val)) {
    s = true;
    return(new Value(pow((double)*left, (double)*right)));
  }
  return(new Power(left->simplify(s), right->simplify(s)));
}


Node *Negate::simplify(int &s)
{
  if (left->isconst()) {
    s = true;
    if ((double)*left < 0.0)
      return new Negate(new Value((double)*left));
    else 
      return new Value(-(double)*left);
  }
  return new Negate(left->simplify(s));
}


Node *Function::simplify(int &s)
{
  return(new Function(op, left->simplify(s), 
    (right ? right->simplify(s) : Null)));
}

