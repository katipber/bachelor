#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <complex>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */


const double PI = acos(-1);
const complex<double> I(0,1);
const int PWI = ios_base::xalloc();

void delPolVar(std::ios_base::event evt, std::ios_base& sb, int idx) {
	if(sb.pword(idx)) {
		delete (string*)sb.pword(idx);
		sb.pword(idx) = nullptr;
	}
}

class polynomial_variable {
	
	string varName;
	
	public:
	
		polynomial_variable(const string & s) : varName(s) {}
		
		friend ostream & operator << (ostream & os, const polynomial_variable & x) {
			
			if(os.pword(PWI)) delete (string*)os.pword(PWI);
			os.pword(PWI) = (void*)new string(x.varName);
			os.register_callback(*delPolVar, PWI);
//cout << x.varName << " " << *(string*)os.pword(PWI) << endl;
			return os;
		}
};

/*
ios_base & dummy(ios_base & x) {

	return x;
}

ios_base & (*(polynomial_variable(const string & name)))(ios_base & x) {
	
	cout.pword(PW) = (void*)&name;
	cout.iword(IW) = 1;
	return dummy;
}
*/
			
class CPolynomial {

	constexpr static const double CMP = 10e-4;
	
	vector<double> val;

	CPolynomial(const vector<double> & v) : val(v) {}
	
	inline complex<double> root(size_t n) const {
		
		return exp(2*PI*I/(double)n);
	}
	
	inline complex<double> iroot(size_t n) const {
		
		return exp(-2*PI*I/(double)n);
	}
			
	public:
	
		CPolynomial() : val(1) {}
		
		size_t Degree() const {
			
			for(int i = val.size() - 1; i >= 0; --i)
				if(abs(val.at(i)) > CMP) return i;
			
			return 0;
		}

		double operator [] (const size_t & i) const {
			
			if(i >= val.size()) return 0;
			
			return val.at(i);
		}
		
		double & operator [] (const size_t & i) {
			
			if(i >= val.size()) val.resize(i + 1);
			
			return val.at(i);
		}
		
		double operator () (const double d) const {
			
			double rez = 0;
			
			for(size_t i = 0; i < val.size(); ++i)
				rez += val.at(i) * pow(d, i);
			
			return rez;
		}
		
		CPolynomial operator * (const double d) const {
			
			vector<double> nval;
			
			if(abs(d) < CMP) {
				nval.push_back(0);
				CPolynomial np(nval);
				return np;
			}
			
			for(auto i : val) nval.push_back(i * d);

			CPolynomial np(nval);
			return np;
		}
		
		CPolynomial operator + (const CPolynomial & p) const {
			
			size_t i = 0;
			vector<double> nval;
			
			nval.resize(max(val.size(), p.val.size()));
			
			try {
				for(; i < nval.size(); ++i)
					nval.at(i) = val.at(i) + p.val.at(i);
			}
			catch(const out_of_range & e) {
				try {
					for(; i < nval.size(); ++i)
						nval.at(i) = val.at(i);
				} 
				catch(const out_of_range & e) {
					try {
						for(; i < nval.size(); ++i)
							nval.at(i) = p.val.at(i);
					}
					catch(const out_of_range & e) {}
				}
			}
			
			CPolynomial np(nval);

			return np;
		}
		
		CPolynomial operator - (const CPolynomial & p) const {
			
			size_t i = 0;
			vector<double> nval;
			
			nval.resize(max(val.size(), p.val.size()));
			
			try {
				for(; i < nval.size(); ++i)
					nval.at(i) = val.at(i) - p.val.at(i);
			}
			catch(const out_of_range & e) {
				try {
					for(; i < nval.size(); ++i)
						nval.at(i) = val.at(i);
				} 
				catch(const out_of_range & e) {
					try {
						for(; i < nval.size(); ++i)
							nval.at(i) = -p.val.at(i);
					}
					catch(const out_of_range & e) {}
				}
			}
			
			CPolynomial np(nval);

			return np;
		}
		
		void fft(vector<complex<double>> & vx, vector<complex<double>> & vy) const {
			
			size_t m(vx.size()/2);
			size_t n(1);
			size_t k(m);

			while(k >= 1) {
				n *= 2;
				size_t i(0);
				complex<double> w(1);
				complex<double> wn = root(n);

				for(size_t j = 0; j < m; ++j) {
					vy[j] = vx[i] + w*vx[i+k];
					vy[j+m] = vx[i] - w*vx[i+k];
					
//cout << j << ":" << j+m << "\t" << i << ":" << i+k << "\t" << n << endl;
//cout << setw(25) << vx[j] << setw(25) << vy[i] << ":" << setw(25) << vy[i+k] << setw(25) << w << endl;
					
					if(!((i+1)%k)) {
						i += k;
						w *= wn;
					}
					
					++i;
				}

//cout << endl;				

				vx.swap(vy);
				k /= 2;
			}
		}
		
		void ifft(vector<complex<double>> & vx, vector<complex<double>> & vy) const {
			
			size_t m(vx.size()/2);
			size_t n(1);
			size_t k(m);

			while(k >= 1) {
				n *= 2;
				size_t i(0);
				complex<double> w(1);
				complex<double> wn = iroot(n);

				for(size_t j = 0; j < m; ++j) {
					vy[j] = vx[i] + w*vx[i+k];
					vy[j+m] = vx[i] - w*vx[i+k];
					
//cout << j << ":" << j+m << "\t" << i << ":" << i+k << "\t" << n << endl;
//cout << setw(25) << vx[j] << setw(25) << vy[i] << ":" << setw(25) << vy[i+k] << setw(25) << w << endl;
					
					if(!((i+1)%k)) {
						i += k;
						w *= wn;
					}
					
					++i;
				}

//cout << endl;				

				vx.swap(vy);
				k /= 2;
			}
		}
		
		CPolynomial operator * (const CPolynomial & b) const {
			
			size_t n = pow(2, ceil(log2(Degree() + b.Degree() + 1)));
			
			vector<complex<double>> va(n), vb(n), vc(n);
			vector<double> vp;
			
			for(size_t i = 0; i <= Degree(); ++i)
				va.at(i) = val[i];
			for(size_t i = 0; i <= b.Degree(); ++i)
				vb.at(i) = b.val[i];
			
			fft(va, vc);
			fft(vb, vc);
			
			for(size_t i = 0; i < va.size(); ++i) {
//cout << setw(25) << va.at(i) << setw(25) << vb.at(i) << setw(25) << vc.at(i) << endl;			
				va.at(i) *= vb.at(i);
			}
			
			ifft(va, vc);
			
			for(auto i : va)
				vp.push_back(i.real()/n);
			
			CPolynomial p(vp);
			
			return p;
		}

		bool operator == (const CPolynomial & p) const {
			
			size_t i = 0;
			size_t imax = max(val.size(), p.val.size());
			
			try {
				for(; i < imax; ++i)
					if(abs(val.at(i) - p.val.at(i)) > CMP) return false;
			}
			catch(const out_of_range & e) {
				try {
					for(; i < imax; ++i)
						if(abs(val.at(i)) > CMP) return false;
				} 
				catch(const out_of_range & e) {
					try {
						for(; i < imax; ++i)
							if(abs(p.val.at(i)) > CMP) return false;
					}
					catch(const out_of_range & e) {}
				}
			}
			
			return true;
		}

		bool operator != (const CPolynomial & p) const {
			
			return !(*this == p);
		}
		
		friend ostream & operator << (ostream & os, const CPolynomial & p) {

			if(!p.Degree()) return os << p.val.at(0);
			
			string vn("x");
			
			if(os.pword(PWI)) vn = *(string*)os.pword(PWI);
			
			bool f = 0;
		
			for(size_t i = 0; i < p.val.size(); ++i) {
			
				size_t idx = p.val.size() - i - 1;
				double coe = p.val.at(idx);
			
				if(abs(coe) > CMP) {
			
					if(f) os << " ";
	
					if(coe < 0) os << "- ";
					else if(f) os << "+ ";
	
					if(!idx) os << abs(coe);
					else if(abs(abs(coe) - 1) > CPolynomial::CMP) os << abs(coe) << "*";
	
					if(idx) os << vn << "^" << idx;
				
					f = 1;
				}
			}
			
			return os;
		}
		
    // ! default constructor
    // ! operator +
    // ! operator -
    // !? operator * (polynomial, double)
    // operator ==
    // operator !=
    // ! operator []
    // ! operator ()
    // operator <<
    // ! Degree (), returns unsigned value
		
	
		void printPoly() {
			cout << endl << "printing.." << endl ;
			for(auto i : val) cout << (abs(i) < CMP ? 0 : i) << " | ";
			cout << endl;
		}
		
		vector<double> getVal() const {
			return val;
		}
		
};


#ifndef __PROGTEST__
bool               smallDiff                               ( double            a,
                                                             double            b )
{
//cout << a << ":" << b << endl;
	return (abs(a-b) < pow(10, -9) ? true : false); 
}

bool               dumpMatch                               ( const CPolynomial & x,
                                                             const vector<double> & ref )
{
	vector<double> v = x.getVal();
	for(size_t i = 0; i < ref.size(); ++i)
		if(abs(v.at(i) - ref.at(i)) > pow(10, -9)) return false;
	return true; 
}

int                main                                    ( void )
{



//cout << "TEST" << endl;

  CPolynomial a, b, c, d, e, f, g;
  ostringstream out;
//cout << polynomial_variable ( "y" ) << endl; 
/*
	a[0] = 0.005;
  a[1] = -2;
	a[2] = 3;

	b[0] = -1;
  b[1] = 2;
	b[2] = -3;

	c[0] = 0;
  c[1] = -2;
	c[2] = -3;

	d[0] = -1;
  d[1] = -0;
	d[2] = -3;

	e[0] = -1;
  e[1] = -2;
//	e[2] = 0;

	f[0] = -0;
  f[1] = -2;
//	f[2] = 0;
  
  cout << a << "\n";
  g = a + b;
	cout << polynomial_variable("y");
	cout << g << "\n";
  g = a + c;
	cout << g << "\n";
  g = a + d;
	cout << g << "\n";
  g = a + e;
	cout << g << "\n";
  g = a + f;
	cout << g << "\n";
	cout << polynomial_variable("z");
  g = a - b;
	cout << g << "\n";
  g = a - c;
	cout << g << "\n";
  g = a - d;
	cout << g << "\n";
  g = a - e;
	cout << g << "\n";
  g = a - f;
	cout << g << "\n";
	cout << polynomial_variable("q");
  g = a * b;
	cout << g << "\n";
  g = a * c;
	cout << g << "\n";
  g = a * d;
	cout << g << "\n";
  g = a * e;
	cout << g << "\n";
  g = e * f;
	cout << g << "\n\n";
	cout << d[0] << "\n";
	cout << d[1] << "\n";
	cout << d[2] << "\n";
	cout << d[3] << "\n";


	a[0]=-1;
//	a[1]=2;
//	a[2]=3;
	a[16]=-0;
	b[1]=5;
//	b[5]=6;
	g = g * g;
	cout << g << "\n";
	g = b * b;
	cout << g << "\n";
	g = a * c;
	cout << g << "\n";
	g = a * d;
	cout << g << "\n";
	g = a * e;
	cout << g << "\n";
	g = a * f;
	cout << g << "\n";

*/	

/*
  a[0] = -10;
  a[1] = 3.5;
  a[3] = 1;
  assert ( smallDiff ( a ( 2 ), 5 ) );
//a.printPoly();

  out . str ("");
  out << a;
//cout << "a = " << a << endl;
  assert ( out . str () == "x^3 + 3.5*x^1 - 10" );

  a = a * -2;
  assert ( a . Degree () == 3
           && dumpMatch ( a, vector<double>{ 20.0, -7.0, -0.0, -2.0 } ) );

  out . str ("");
  out << a;
  assert ( out . str () == "- 2*x^3 - 7*x^1 + 20" );
  out . str ("");
  out << b;
  assert ( out . str () == "0" );
	
  b[5] = -1;

  out . str ("");
  out << b;
  assert ( out . str () == "- x^5" );

  c = a + b;
  assert ( c . Degree () == 5
           && dumpMatch ( c, vector<double>{ 20.0, -7.0, 0.0, -2.0, 0.0, -1.0 } ) );
	
  out . str ("");
  out << c;
  assert ( out . str () == "- x^5 - 2*x^3 - 7*x^1 + 20" );
  
  c = a - b;
  assert ( c . Degree () == 5
           && dumpMatch ( c, vector<double>{ 20.0, -7.0, -0.0, -2.0, -0.0, 1.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "x^5 - 2*x^3 - 7*x^1 + 20" );

  c = a * b;
cout << c << endl;
  assert ( c . Degree () == 8
           && dumpMatch ( c, vector<double>{ 0.0, -0.0, 0.0, -0.0, -0.0, -20.0, 7.0, 0.0, 2.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "2*x^8 + 7*x^6 - 20*x^5" );
  assert ( a != b );
  b[5] = 0;
  assert ( !(a == b) );
  a = a * 0;
  assert ( a . Degree () == 0
           && dumpMatch ( a, vector<double>{ 0.0 } ) );

  assert ( a == b );
  
cout << polynomial_variable ( "y" );
cout << c << endl;

  // bonus
  a[2] = 4;
  a[1] = -3;
  b[3] = 7;
  out . str ("");
  out << polynomial_variable ( "y" ) << "a=" << a << ", b=" << b;
  assert ( out . str () == "a=4*y^2 - 3*y^1, b=7*y^3" );
cout << "a = " << a << polynomial_variable ( "boOm" ) << "\nb = " << b << endl;
cout << polynomial_variable ( "lalalalala" ) << "a = " << a << polynomial_variable ( "omg" ) << "\nb = " << b << endl;
cout << "c = " << c << endl;  
  
  out . str ("");
  out << polynomial_variable ( "test" ) << c;
  assert ( out . str () == "2*test^8 + 7*test^6 - 20*test^5" );

*/

  return 0;
}
#endif /* __PROGTEST__ */
