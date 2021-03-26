#include "matrix.h"


// Constructor: vector der matrix zuweisen
//   vec: vector, der der matrix zugewiesen werden soll

matrix::matrix(const vector& vec)
{
	m=vec.dim();
	n=1;
	// rowvec=new vector[m](1);    // Diese Art der Initialisierung ist eine GCC-Erweiterung!
	rowvec=new vector[m];
	for(unsigned int i=0;i<m;i++)
	{
		rowvec[i] = vector (1);
		rowvec[i][0]=vec[i];
	}
}


// 1x1-matrix als double casten

matrix::operator double()
{
	if((m!=1) || (n!=1))
	{
#ifdef BS_DOS
		cerr<<"In matrix::operator double() : illegal size ("<<m<<","<<n<<")"<<endl;
#else
		std::cerr<<"In matrix::operator double() : illegal size ("<<m<<","<<n<<")"<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	return rowvec[0][0];
}


// auf Zeilenvektor zugreifen
//   i: Index der Zeile

vector& matrix::operator[](const unsigned int i)
{
	if(i>=m)
	{
#ifdef BS_DOS	
		cerr<<"In matrix::operator[] : illegal index ("<<i<<")"<<endl;
#else
		std::cerr<<"In matrix::operator[] : illegal index ("<<i<<")"<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	return rowvec[i];
}


// auf Zeilenvektor zugreifen bei konstantem Objekt
//   i: Index der Zeile

const vector& matrix::operator[](const unsigned int i) const
{
	if(i>=m)
	{
#ifdef BS_DOS	
		cerr<<"In matrix::operator[] : illegal index ("<<i<<")"<<endl;
#else
		std::cerr<<"In matrix::operator[] : illegal index ("<<i<<")"<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	return rowvec[i];
}


// c. Spalte zurückgeben
//   c: Index der Spalte

vector matrix::col(const unsigned int c) const
{
	vector v(m);
	for(unsigned int i=0;i<m;i++)
		v[i]=rowvec[i][c];
	return v;
}

// Neudimensionieren der matrix
//   r: neue Anzahl Zeilen
//   c: neue Anzahl Spalten
//   save: bisherigen Inhalt retten

void matrix::dim(const unsigned int r,const unsigned int c,bool save)
{
   unsigned int i;

	if(save)
	{
		vector* tmp=new vector[r];
		for(i=0;i<((r<m)?r:m);i++)
		{
			tmp[i].element=rowvec[i].element;
			rowvec[i].element=0;
			tmp[i].n=rowvec[i].n;
		}
		if(rowvec)
			delete[] rowvec;
		rowvec=tmp;
		m=r;
		n=c;

		for(i=0;i<m;i++)
			rowvec[i].dim(c,save);
	} else
		*this=matrix(r,c);	
}


// alle Elemente auf einen Wert setzen
//   s: Wert

void matrix::set(const double s)
{
	for(unsigned int i=0;i<m;i++)
		rowvec[i].set(s);
}


// Einheitsmatrix erzeugen
//   d: Dimension der matrix

void matrix::identity(const unsigned int d)
{
	dim(d,d,false);
	for(unsigned int i=0;i<m;i++)
		rowvec[i][i]=1.0;
}


// Transponierte der matrix

matrix matrix::T() const
{
	matrix mat(n,m);
	for(unsigned int i=0;i<m;i++)
		for(unsigned int j=0;j<n;j++)
			mat[j][i]=rowvec[i][j];
	return mat;
}


// zwei Zeilen vertauschen
//   r1: Index der ersten Zeile
//   r2: Index der zweiten Zeile

void matrix::rowexchange(const unsigned int r1,const unsigned int r2)
{
	double* tmp=rowvec[r1].element;
	rowvec[r1].element=rowvec[r2].element;
	rowvec[r2].element=tmp;
}


// zwei Spalten vertauschen
//   c1: Index der ersten Spalte
//   c2: Index der zweiten Spalte

void matrix::colexchange(const unsigned int c1,const unsigned int c2)
{
	for(unsigned int i=0;i<m;i++)
	{
		double tmp=rowvec[i][c1];
		rowvec[i][c1]=rowvec[i][c2];
		rowvec[i][c2]=tmp;
	}
}


// eine Zeile anhängen
//   v: neue Zeile

void matrix::rowappend(const vector& v)
{
	dim(m+1,v.dim());
	rowvec[m-1]=v;
}


// eine Spalte anhängen
//   v: neue Spalte

void matrix::colappend(const vector& v)
{
	dim(v.dim(),n+1);
	for(unsigned int i=0;i<m;i++)
		rowvec[i][n-1]=v[i];
}


// eine Zeile einfügen
//   r: Index der Zeile
//   v: neue Zeile

void matrix::rowinsert(const unsigned int r, const vector& v)
{
	dim(m+1,v.dim());
	double* tmp=rowvec[m-1].element;
	for(unsigned int i=m-1;i>r;i--)
		rowvec[i].element=rowvec[i-1].element;
	rowvec[r].element=tmp;
	rowvec[r]=v;
}


// Zuweisungs-Operator
//   mat: matrix, die zugewiesen werden soll

matrix& matrix::operator=(const matrix& mat)
{
	if(this!=&mat)              // Selbstzuweisung vermeiden
	{
		if(rowvec)
			delete[] rowvec;
		m=mat.m;
		n=mat.n;
		if(m!=0)
		{
			rowvec=new vector[m];
			for(unsigned int i=0;i<m;i++)
				rowvec[i]=mat.rowvec[i];
		} else
			rowvec=0;				
	}
	return *this;
}


// matrix zur matrix hinzuaddieren
//   mat: matrix, die hinzuaddiert werden soll

matrix& matrix::operator+=(const matrix& mat)
{
	if((m!=mat.m) || (n!=mat.n))
	{
#ifdef BS_DOS	
		cerr<<"In matrix::operator+= : incompatible sizes."<<endl;
#else
		std::cerr<<"In matrix::operator+= : incompatible sizes."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	for(unsigned int i=0;i<m;i++)
		rowvec[i]+=mat.rowvec[i];
	return *this;
}


// matrix von matrix subtrahieren
//   mat: matrix, die subtrahiert werden soll

matrix& matrix::operator-=(const matrix& mat)
{
	if((m!=mat.m) || (n!=mat.n))
	{
#ifdef BS_DOS	
		cerr<<"In matrix::operator-= : incompatible sizes."<<endl;
#else
		std::cerr<<"In matrix::operator-= : incompatible sizes."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	for(unsigned int i=0;i<m;i++)
		rowvec[i]-=mat.rowvec[i];
	return *this;
}


// matrix elementweise mit double multiplizieren
//   s: Wert, der elementweise multipliziert werden soll

matrix& matrix::operator*=(const double s)
{
	for(unsigned int i=0;i<m;i++)
		rowvec[i]*=s;
	return *this;
}


// matrix elementweise durch double dividieren
//   s: Wert, durch den elementweise dividiert werden soll

matrix& matrix::operator/=(const double s)
{
	if(s==0)
	{
#ifdef BS_DOS	
		cerr<<"In matrix::operator/= : division by zero."<<endl;
#else
		std::cerr<<"In matrix::operator/= : division by zero."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	for(unsigned int i=0;i<m;i++)
		rowvec[i]/=s;
	return *this;
}


// Vergleich auf Gleichheit
//   mat1, mat2: matrizen, die verglichen werden sollen

bool operator==(const matrix& mat1,const matrix& mat2)
{
	if(mat1.m!=mat2.m)
		return false;
	for (unsigned int i=0;i<mat1.m;i++)
		if(mat1.rowvec[i]!=mat2.rowvec[i])
			return false;
	return true;
}


// Matrizenaddition
//   mat1, mat2: matrizen, die addiert werden sollen

matrix operator+(const matrix& mat1,const matrix& mat2)
{
	matrix mat(mat1);
	mat+=mat2;
	return mat;
}


// Matrizensubtraktion
//   mat1, mat2: matrizen, die subtrahiert werden sollen

matrix operator-(const matrix& mat1,const matrix& mat2)
{
	matrix mat(mat1);
	mat-=mat2;
	return mat;
}


// double mit matrix multiplizieren
//   s: Wert
//   mat1: matrix

matrix operator*(const double s,const matrix& mat1)
{
	matrix mat(mat1);
	mat*=s;
	return mat;
}


// matrix mit double multiplizieren
//   mat1: matrix
//   s: Wert

matrix operator*(const matrix& mat1,const double s)
{
	matrix mat(mat1);
	mat*=s;
	return mat;
}


// matrix durch double dividieren
//   mat1: matrix
//   s: Wert

matrix operator/(const matrix& mat1,const double s)
{
	matrix mat(mat1);
	mat/=s;
	return mat;
}


// Matrizenmultiplikation
//   mat1, mat2: matrizen, die multipliziert werden sollen

matrix operator*(const matrix& mat1,const matrix& mat2)
{
	if(mat1.n!=mat2.m)
	{
#ifdef BS_DOS	
		cerr<<"In matrix::operator* : incompatible sizes."<<endl;
#else
		std::cerr<<"In matrix::operator* : incompatible sizes."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
	matrix matp(mat1.m,mat2.n);                               // Ergebnismatrix
	for(unsigned int i=0;i<mat1.m;i++)
		for(unsigned int j=0;j<mat2.n;j++)
			matp[i][j]=mat1.row(i)*mat2.col(j);                   // Zeile * Spalte
	return matp;
}


// Ausgabe-Operator
//   mat: matrix, die ausgegeben werden soll

#ifdef BS_DOS	
ostream& operator<<(ostream& ostr,const matrix& mat)
{
	for(unsigned int i=0;i<mat.m;i++)
		ostr<<mat[i]<<endl;
	return ostr;
}


// Eingabe-Operator
//   mat: matrix, in die eingelesen werden soll

istream& operator>>(istream& istr,matrix& mat)
{
	for(unsigned int i=0;i<mat.m;i++)
		istr>>mat[i];
	return istr;
}
#else
std::ostream& operator<<(std::ostream& ostr,const matrix& mat)
{
	for(unsigned int i=0;i<mat.m;i++)
		ostr<<mat[i]<<std::endl;
	return ostr;
}


// Eingabe-Operator
//   mat: matrix, in die eingelesen werden soll

std::istream& operator>>(std::istream& istr,matrix& mat)
{
	for(unsigned int i=0;i<mat.m;i++)
		istr>>mat[i];
	return istr;
}
#endif /* BS_DOS */
