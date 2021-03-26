#include "vector.h"
#include "matrix.h"


// Constructor: Dimension und Anfangswerte vorgebbar
//   l: Dimension des vectors
//   initval: Anfangswert

vector::vector(const unsigned int l, const double initval)
{
	n=l;
	element=new double[n];
	for(unsigned int i=0;i<n;i++)
		element[i]=initval;
}


// Constructor: double-array dem vector zuweisen
//   l: Dimension des vectors
//   array: double-array, das dem vector zugewiesen werden soll

vector::vector(const unsigned int l, const double array[])
{
	n=l;
	element=new double[n];
	for(unsigned int i=0;i<n;i++)
		element[i]=array[i];
}


// Constructor: matrix dem vector zuweisen
//   mat: matrix, die dem vector zugewiesen werden soll

vector::vector(const matrix& mat)
{
	if(mat.cols()==1)           // Spaltenvektor
	{
		n=mat.rows();
		element=new double[n];
		for(unsigned int i=0;i<n;i++)
			element[i]=mat[i][0];
	} else if(mat.rows()==1)    // Zeilenvektor
	{
		n=mat.cols();
		element=new double[n];
		for(unsigned int i=0;i<n;i++)
			element[i]=mat[0][i];
	} else {
#ifdef BS_DOS	
		cerr<<"In vector::vector(const matrix&) : illegal matrix size."<<endl;
#else
		std::cerr<<"In vector::vector(const matrix&) : illegal matrix size."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}	
}


// Elementzugriffs-Operator
//   i: Index des Elements

double& vector::operator[](const unsigned int i)
{
	// Zulässigkeit des Index überprüfen
	if(i>=n)
	{
#ifdef BS_DOS	
		cerr<<"In vector::operator[] : illegal index ("<<i<<")."<<endl;
#else
		std::cerr<<"In vector::operator[] : illegal index ("<<i<<")."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	return element[i];
}


// Elementzugriffs-Operator für konstantes Objekt
//   i: Index des Elements

const double vector::operator[](const unsigned int i) const
{
	// Zulässigkeit des Index überprüfen
	if(i>=n)
	{
#ifdef BS_DOS	
		cerr<<"In vector::operator[] : illegal index ("<<i<<")."<<endl;
#else
		std::cerr<<"In vector::operator[] : illegal index ("<<i<<")."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	return element[i];
}


// Neudimensionierung des vectors
//   l: neue Dimension des vectors
//   save: bisherigen Inhalt retten

void vector::dim(const unsigned int l,const bool save)
{
	if(l!=n)
	{
		vector v(l);
		if(save)
		{
			for(unsigned int i=0;i<((l<n)?l:n);i++)
				v.element[i]=element[i];
		}
		*this=v;
	}
}


// alle Elemente auf einen Wert setzen
//   s: Wert

void vector::set(const double s)
{
	for(unsigned int i=0;i<n;i++)
		element[i]=s;	
}


// Achseneinheitsvektor erzeugen
//   e: Index der Achse (0..(n-1))

void vector::axis(const unsigned int e)
{
	if(e>=n)
	{
#ifdef BS_DOS	
		cerr<<"In vector::axis : illegal index ("<<e<<")."<<endl;
#else
		std::cerr<<"In vector::axis : illegal index ("<<e<<")."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	for(unsigned int i=0;i<n;i++)
		element[i]=0.0;
	element[e]=1.0;		
}


// zwei Elemente vertauschen
//   e1: Index des ersten Elements
//   e2: Index des zweiten Elements

void vector::exchange(const unsigned int e1,const unsigned int e2)
{
	if((e1>=n) || (e2>=n))
	{
#ifdef BS_DOS
		cerr<<"In vector::exchange : illegal index ("<<e1<<", "<<e2<<")."<<endl;
#else
		std::cerr<<"In vector::exchange : illegal index ("<<e1<<", "<<e2<<")."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	double tmp=element[e1];
	element[e1]=element[e2];
	element[e2]=tmp;
}


// double an den vector anhängen
//   s: Wert des neuen Elements

void vector::append(const double s)
{
	dim(n+1);
	element[n-1]=s;
}


// vector an den vector anhängen
//   s: Wert des neuen Elements

void vector::append(const vector& v)
{
	dim(n+v.n);
	for(unsigned int i=1;i<=v.n;i++)
		element[n-i]=v.element[v.n-i];
}


// double in den vector einfügen
//   e: Index des Elements
//   s: Wert des neuen Elements

void vector::insert(const unsigned int e,const double s)
{
	dim(n+1);
	for(unsigned int i=n-1;i>e;i--)
		element[i]=element[i-1];
	element[e]=s;
}


// Zuweisungs-Operator
//   v: vector, der zugewiesen werden soll

vector& vector::operator=(const vector& v)
{
	if(this!=&v)                // Selbstzuweisung vermeiden
	{
		if(element)
			delete[] element;
		n=v.n;
		if(n!=0)                  // Wenn kein leerer Vektor
		{                         // elementweise kopieren
			element=new double[n];
			for(unsigned int i=0;i<n;i++)
				element[i]=v.element[i];
		} else
			element=0;
	}
	return *this;
}


// vector zum vector hinzuaddieren
//  v: vector, der hinzuaddiert werden soll

vector& vector::operator+=(const vector& v)
{
	if(n!=v.n)
	{
#ifdef BS_DOS
		cerr<<"In vector::operator+= : incompatible sizes."<<endl;
#else
		std::cerr<<"In vector::operator+= : incompatible sizes."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	for(unsigned int i=0;i<n;i++)
		element[i]+=v.element[i];
	return *this;
}


// vector vom vector subtrahieren
//  v: vector, der subtrahiert werden soll

vector& vector::operator-=(const vector& v)
{
	if(n!=v.n)
	{
#ifdef BS_DOS
		cerr<<"In vector::operator-= : incompatible sizes."<<endl;
#else
		std::cerr<<"In vector::operator-= : incompatible sizes."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	for(unsigned int i=0;i<n;i++)
		element[i]-=v.element[i];
	return *this;
}


// vector elementweise mit vector multiplizieren
//   v: vector, der elementweise multipliziert werden soll

vector& vector::operator*=(const vector& v)
{
	if(n!=v.n)
	{
#ifdef BS_DOS
		cerr<<"In vector::operator*= : incompatible sizes."<<endl;
#else
		std::cerr<<"In vector::operator*= : incompatible sizes."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	for(unsigned int i=0;i<n;i++)
		element[i]*=v.element[i];
	return *this;
}


// vector elementweise durch vector dividieren
//   v: vector, durch den elementweise dividiert werden soll

vector& vector::operator/=(const vector& v)
{
	if(n!=v.n)
	{
#ifdef BS_DOS
		cerr<<"In vector::operator/= : incompatible sizes."<<endl;
#else
		std::cerr<<"In vector::operator/= : incompatible sizes."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	for(unsigned int i=0;i<n;i++)
	{
	 	if(v.element[i]==0)
		{
#ifdef BS_DOS
			cerr<<"In vector::operator/= : division by zero."<<endl;
#else
			std::cerr<<"In vector::operator/= : division by zero (vector element)."<<std::endl;
#endif /* BS_DOS */
			exit(1);
		}
		element[i]/=v.element[i];
	}
	return *this;
}


// vector elementweise mit double multiplizieren
//   s: Wert, mit dem elementweise multipliziert werden soll

vector& vector::operator*=(const double s)
{
	for(unsigned int i=0;i<n;i++)
		element[i]*=s;
	return *this;
}


// vector elementweise durch double dividieren
//   s: Wert, durch den elementweise dividiert werden soll

vector& vector::operator/=(const double s)
{
 	if(s==0)
	{
#ifdef BS_DOS
		cerr<<"In vector::operator/= : division by zero."<<endl;
#else
		std::cerr<<"In vector::operator/= : division by zero value."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	for(unsigned int i=0;i<n;i++)
		element[i]/=s;
	return *this;
}


// Vergleich auf Gleichheit
//   v1, v2: vectoren, die verglichen werden sollen

bool operator==(const vector& v1,const vector& v2)
{
	if(v1.n!=v2.n)
		return false;
	for (unsigned int i=0;i<v1.n;i++)
		if(v1[i]!=v2[i])
			return false;
	return true;
}


// lexikographischer Vergleich
//   v1, v2: vectoren, die verglichen werden sollen

bool operator<(const vector& v1,const vector& v2)
{
	unsigned int i;
	for (i=0;i<v1.n;i++)
		if(v1[i]!=v2[i])
			return (v1[i]<v2[i]);
	return false;
}


// lexikographischer Vergleich
//   v1, v2: vectoren, die verglichen werden sollen

bool operator>(const vector& v1,const vector& v2)
{
	unsigned int i;
	for (i=0;i<v1.n;i++)
		if(v1[i]!=v2[i])
			return (v1[i]>v2[i]);
	return false;
}


// Vektoraddition
//   v1, v2: vectoren, die addiert werden sollen

vector operator+(const vector& v1,const vector& v2)
{
	if(v1.n!=v2.n)
	{
#ifdef BS_DOS
		cerr<<"In vector::operator+ : incompatible sizes."<<endl;
#else
		std::cerr<<"In vector::operator+ : incompatible sizes."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	vector v(v1);
	v+=v2;
	return v;
}


// Vektorsubtraktion
//   v1, v2: vectoren, die subtrahiert werden sollen

vector operator-(const vector& v1,const vector& v2)
{
	if(v1.n!=v2.n)
	{
#ifdef BS_DOS
		cerr<<"In vector::operator- : incompatible sizes."<<endl;
#else
		std::cerr<<"In vector::operator- : incompatible sizes."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	vector v(v1);
	v-=v2;
	return v;
}


// double mit vector multiplizieren
//   s: Wert
//   v1: vector

vector operator*(const double s,const vector& v1)
{
	vector v(v1);
	v*=s;
	return v;
}


// vector mit double multiplizieren
//   v1: vector
//   s: Wert

vector operator*(const vector& v1,const double s)
{
	vector v(v1);
	v*=s;
	return v;
}


// vector durch double dividieren
//   v1: vector
//   s: Wert

vector operator/(const vector& v1,const double s)
{
	vector v(v1);
	v/=s;
	return v;
}


// Skalarprodukt
//   v1, v2: vectoren, die skalarmultipliziert werden sollen

double operator*(const vector& v1,const vector& v2)
{
	if(v1.n!=v2.n)
	{
#ifdef BS_DOS
		cerr<<"In vector::operator* : incompatible sizes."<<endl;
#else
		std::cerr<<"In vector::operator* : incompatible sizes."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	double sp=0.0;
	for(unsigned int i=0;i<v1.n;i++)
		sp+=v1.element[i]*v2.element[i];
	return sp;
}


// vectoren elementweise multiplizieren
//   v1,v2: vectoren, die elementweise multipliziert werden sollen

vector product(const vector& v1,const vector& v2)
{
	if(v1.n!=v2.n)
	{
#ifdef BS_DOS
		cerr<<"In vector::product : incompatible sizes."<<endl;
		cerr<<"v1: "<<v1<<endl;
		cerr<<"v2: "<<v2<<endl;
#else
		std::cerr<<"In vector::product : incompatible sizes."<<std::endl;
		std::cerr<<"v1: "<<v1<<std::endl;
		std::cerr<<"v2: "<<v2<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	vector v(v1);
	v*=v2;
	return v;
}


// vectoren elementweise dividieren
//   v1,v2: vectoren, die elementweise dividiert werden sollen

vector quotient(const vector& v1,const vector& v2)
{
	if(v1.n!=v2.n)
	{
#ifdef BS_DOS
		cerr<<"In vector::quotient : incompatible sizes."<<endl;
#else
		std::cerr<<"In vector::quotient : incompatible sizes."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	vector v(v1);
	v/=v2;
	return v;
}


// Ausgabe-Operator
//   v: vector, der ausgegeben werden soll

#ifdef BS_DOS
ostream& operator<<(ostream& ostr,const vector& v)
#else
std::ostream& operator<<(std::ostream& ostr,const vector& v)
#endif /* BS_DOS */
{
	for(unsigned int i=0;i<v.n;i++)
		ostr<<v.element[i]<<' ';
	return ostr;
}


// Eingabe-Operator
//   v: vector, in den eingelesen werden soll

#ifdef BS_DOS
istream& operator>>(istream& istr,vector& v)
#else
std::istream& operator>>(std::istream& istr,vector& v)
#endif /* BS_DOS */
{
	for(unsigned int i=0;i<v.n;i++)
		istr>>v[i];
	return istr;
}


// Compare-Funktion für qsort()
int compare_PointWValue (const void *a, const void *b)
{
  PointWValue *A = (PointWValue *) a;
  PointWValue *B = (PointWValue *) b;

  if (A->Value > B->Value)
  {
    return (1);
  }
  else
  {
    if (A->Value < B->Value)
      return (-1);
    else
      return (0);
  }
}
