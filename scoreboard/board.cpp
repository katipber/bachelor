#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
using namespace std;
#endif /* __PROGTEST__ */

class OrderingDoesNotExistException {
};

class DuplicateMatchException {
};

template <typename _M> class CContest {
	
	struct SMatch {
		
		string a, b;
		_M s;
		
		SMatch (const string& a, const string& b, const _M& x) : a(a), b(b), s(x) {}
		
		bool operator < (const SMatch& x) const {
			if (min(a, b) != min(x.a, x.b)) return min(a, b) < min(x.a, x.b);
			return max(a, b) < max(x.a, x.b);
		}
	};
		
	set<SMatch> match;
	
	int dig (const string & l, map<string, set<string>> &w, map<string, int> &d, int cd, int m) const {
		
//cout << "digging: " << l << " " << cd << endl;
		
		int lm(0);
		if (cd > m) return -1;
		
		auto itw = w.find(l);
		if (itw != w.end()) {
			for (auto i : itw->second) {
				auto itd = d.find(i);
				if (itd != d.end()) lm = max(lm, itd->second+1);
				else {
					int ld = dig(i, w, d, cd+1, m);
					if (ld < 0) return -1;
					lm = max(lm, ld);
				}
			}
		}
		else lm = 1;

		d[l] = lm;
		
//for (auto i : d) cout << i.first << " " << i.second << " ";
//cout << l << " " << lm << endl;

		return lm+1;
	}
	
public:
	
	CContest& AddMatch (const string& a, const string& b, const _M& x) {
		auto i = match.insert(SMatch(a, b, x));
		if (!i.second) throw DuplicateMatchException();
		return *this;
	}
	
	template <typename _T>
	list<string> Results (_T f) const {
		
		map<string, set<string>> winset;
		set<string> loser;
		set<string> winner;
		map<string, int> depth;
		map<int, string> rmap;
		list<string> rez;
		
		for (auto i : match) {		
			if (f(i.s) < 0) {
				winset[i.a].insert(i.b);
				loser.insert(i.a);
				winner.insert(i.b);
			}
			else if (f(i.s) > 0) {
				winset[i.b].insert(i.a);
				loser.insert(i.b);
				winner.insert(i.a);
			}
			else throw OrderingDoesNotExistException();
		}
		
		for (auto i : winner)
			loser.erase(i);
		
//cout << loser.size() << endl;
		
		if (loser.size() != 1) throw OrderingDoesNotExistException();
		
		if (dig(*loser.begin(), winset, depth, 0, winset.size() + 1) < 0)
			throw OrderingDoesNotExistException();
		if (depth[*loser.begin()] != (int)winset.size() + 1) 
			throw OrderingDoesNotExistException();
		
		for (auto i : depth)
			rmap.insert(make_pair(i.second, i.first));
			
		for (auto i : rmap)
			rez.push_back(i.second);

		return rez;
	}
	
	template <typename _T>
	bool IsOrdered (_T f) const {
		
		try {
			Results(f);
		} catch (...) {
			return false;
		}
		
		return true;
	}
	
	// default constructor
	// destructor
	// AddMatch ( contestant1, contestant2, result )
	// IsOrdered ( comparator )
	// Results ( comparator )
};

#ifndef __PROGTEST__
struct CMatch {
	
	public:
	CMatch (int a, int b) : m_A (a), m_B (b) {}

	int m_A;
	int m_B;

};

class HigherScoreThreshold {

	public:
	HigherScoreThreshold (int diffAtLeast) : m_DiffAtLeast (diffAtLeast) {}

	int operator () (const CMatch & x) const {
	return (x . m_A > x . m_B + m_DiffAtLeast) - (x . m_B > x . m_A + m_DiffAtLeast);
	}

	private:
	int m_DiffAtLeast;

};

int HigherScore (const CMatch & x) {
	return (x . m_A > x . m_B) - (x . m_B > x . m_A);
}

int main (void) {
	
	
	
  CContest<CMatch>  x;
  
  x . AddMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . AddMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . AddMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . AddMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . AddMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . AddMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );
    
  
  assert ( ! x . IsOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . Results ( HigherScore );
    assert ( "Exception missing!" == NULL );
  }
  catch ( const OrderingDoesNotExistException & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == NULL );
  }
/* */
  x . AddMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) ); 

  assert ( x . IsOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . Results ( HigherScore );
    assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == NULL );
  }

  
  assert ( ! x . IsOrdered ( HigherScoreThreshold ( 3 ) ) );
  try
  {
    list<string> res = x . Results ( HigherScoreThreshold ( 3 ) );
    assert ( "Exception missing!" == NULL );
  }
  catch ( const OrderingDoesNotExistException & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == NULL );
  }
  
  assert ( x . IsOrdered ( [] ( const CMatch & x )
  {
    return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
  } ) );
  try
  {
    list<string> res = x . Results ( [] ( const CMatch & x )
    {
      return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
    } );
    assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == NULL );
  }
  
  CContest<bool>  y;
  
  y . AddMatch ( "Python", "PHP", true )
    . AddMatch ( "PHP", "Perl", true )
    . AddMatch ( "Perl", "Bash", true )
    . AddMatch ( "Bash", "JavaScript", true )
    . AddMatch ( "JavaScript", "VBScript", true );
  
  assert ( y . IsOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . Results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    });
    assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == NULL );
  }
    
  y . AddMatch ( "PHP", "JavaScript", false );
  assert ( !y . IsOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . Results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } );
    assert ( "Exception missing!" == NULL );
  }
  catch ( const OrderingDoesNotExistException & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == NULL );
  }
  
  try
  {
    y . AddMatch ( "PHP", "JavaScript", false );
    assert ( "Exception missing!" == NULL );
  }
  catch ( const DuplicateMatchException & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == NULL );
  }
  
  try
  {
    y . AddMatch ( "JavaScript", "PHP", true );
    assert ( "Exception missing!" == NULL );
  }
  catch ( const DuplicateMatchException & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == NULL );
  }
	  
/*	 */
 
  return 0;
}
#endif /* __PROGTEST__ */