#define KDTREE_DEFINE_OSTREAM_OPERATORS

#include <kdtree++/kdtree.hpp>

#include <iostream>
#include <vector>
#include <limits>
#include <functional>
#include <set>

// used to ensure all triplets that are accessed via the operator<< are initialised.
std::set<const void*> registered;

struct triplet
{
  typedef int value_type;

   triplet(value_type a, value_type b, value_type c)
   {
      d[0] = a;
      d[1] = b;
      d[2] = c;
      bool reg_ok = (registered.find(this) == registered.end());
      assert(reg_ok);
      registered.insert(this).second;
   }

   triplet(const triplet & x)
   {
      d[0] = x.d[0];
      d[1] = x.d[1];
      d[2] = x.d[2];
      bool reg_ok = (registered.find(this) == registered.end());
      assert(reg_ok);
      registered.insert(this).second;
   }

   ~triplet()
   {
      bool unreg_ok = (registered.find(this) != registered.end());
      assert(unreg_ok);
      registered.erase(this);
   }

  inline value_type operator[](KDTREE_SIZE_T const N) const { return d[N]; }

  value_type d[3];
};

inline bool operator==(triplet const& A, triplet const& B) {
  return A.d[0] == B.d[0] && A.d[1] == B.d[1] && A.d[2] == B.d[2];
}

std::ostream& operator<<(std::ostream& out, triplet const& T)
{
   assert(registered.find(&T) != registered.end());
  return out << '(' << T.d[0] << ',' << T.d[1] << ',' << T.d[2] << ')';
}

inline double tac( triplet t, KDTREE_SIZE_T k ) { return t[k]; }


typedef KDTree::KDTree<3, triplet, std::pointer_to_binary_function<triplet,KDTREE_SIZE_T,double> > tree_type;

int main()
{

  tree_type t(std::ptr_fun(tac));

  triplet c0(5, 4, 0); t.insert(c0);
  triplet c1(4, 2, 1); t.insert(c1);
  triplet c2(7, 6, 9); t.insert(c2);
  triplet c3(2, 2, 1); t.insert(c3);
  triplet c4(8, 0, 5); t.insert(c4);
  triplet c5(5, 7, 0); t.insert(c5);
  triplet c6(3, 3, 8); t.insert(c6);
  triplet c7(9, 7, 3); t.insert(c7);
  triplet c8(2, 2, 6); t.insert(c8);
  triplet c9(2, 0, 6); t.insert(c9);

  std::cout << t << std::endl;

  t.erase(c0);
  t.erase(c1);
  t.erase(c3);
  t.erase(c5);

  t.optimise();

  std::cout << std::endl << t << std::endl;

  int i=0;
  for (tree_type::const_iterator iter=t.begin(); iter!=t.end(); ++iter, ++i);
  std::cout << "iterator walked through " << i << " nodes in total" << std::endl;
  if (i!=6)
    {
      std::cerr << "Error: does not tally with the expected number of nodes (6)" << std::endl;
      return 1;
    }
  i=0;
  for (tree_type::const_reverse_iterator iter=t.rbegin(); iter!=t.rend(); ++iter, ++i);
  std::cout << "reverse_iterator walked through " << i << " nodes in total" << std::endl;
  if (i!=6)
    {
      std::cerr << "Error: does not tally with the expected number of nodes (6)" << std::endl;
      return 1;
    }

  triplet s(5, 4, 3);
  std::vector<triplet> v;
  unsigned int const RANGE = 3;

  KDTREE_SIZE_T count = t.count_within_range(s, RANGE);
  std::cout << "counted " << count
	    << " nodes within range " << RANGE << " of " << s << ".\n";
  t.find_within_range(s, RANGE, std::back_inserter(v));

  std::cout << "found   " << v.size() << " nodes within range " << RANGE
	    << " of " << s << ":\n";
  std::vector<triplet>::const_iterator ci = v.begin();
  for (; ci != v.end(); ++ci)
    std::cout << *ci << " ";
  std::cout << "\n" << std::endl;

  std::cout << "Nearest to " << s << ": " <<
     *t.find_nearest(s,std::numeric_limits<double>::max()).first << std::endl;

  triplet s2(10, 10, 2);
  std::cout << "Nearest to " << s2 << ": " <<
     *t.find_nearest(s2,std::numeric_limits<double>::max()).first << std::endl;

  std::cout << std::endl;

  std::cout << t << std::endl;

  // Testing iterators
  {
    std::cout << "Testing iterators" << std::endl;

    t.erase(c2);
    t.erase(c4);
    t.erase(c6);
    t.erase(c7);
    t.erase(c8);
//    t.erase(c9);

    std::cout << std::endl << t << std::endl;

    std::cout << "Forward iterator test..." << std::endl;
    std::vector<triplet> forwards;
    for (tree_type::iterator i = t.begin(); i != t.end(); ++i)
    { std::cout << *i << " " << std::flush; forwards.push_back(*i); }
    std::cout << std::endl;
    std::cout << "Reverse iterator test..." << std::endl;
    std::vector<triplet> backwards;
    for (tree_type::reverse_iterator i = t.rbegin(); i != t.rend(); ++i)
    { std::cout << *i << " " << std::flush; backwards.push_back(*i); }
    std::cout << std::endl;
    std::reverse(backwards.begin(),backwards.end());
    assert(backwards == forwards);
  }

  return 0;
}

/* COPYRIGHT --
 *
 * This file is part of libkdtree++, a C++ template KD-Tree sorting container.
 * libkdtree++ is (c) 2004-2007 Martin F. Krafft <libkdtree@pobox.madduck.net>
 * and Sylvain Bougerel <sylvain.bougerel.devel@gmail.com> distributed under the
 * terms of the Artistic License 2.0. See the ./COPYING file in the source tree
 * root for more information.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
