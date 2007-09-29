#define KDTREE_DEFINE_OSTREAM_OPERATORS

#include <kdtree++/kdtree.hpp>

#include <iostream>
#include <vector>

struct triplet {
  typedef int value_type;

  inline value_type operator[](size_t const N) const { return d[N]; }
  
  value_type d[3];
};

inline bool operator==(triplet const& A, triplet const& B) {
  return A.d[0] == B.d[0] && A.d[1] == B.d[1] && A.d[2] == B.d[2];
}

std::ostream& operator<<(std::ostream& out, triplet const& T)
{
  return out << '(' << T.d[0] << ',' << T.d[1] << ',' << T.d[2] << ')';
}

double tac( triplet t, int k ) { return t[k]; }


typedef KDTree::KDTree<3, triplet, std::pointer_to_binary_function<triplet,int,double> > tree_type;

int main()
{

  tree_type t(std::ptr_fun(tac));

  triplet c0 = {5, 4, 0}; t.insert(c0);
  triplet c1 = {4, 2, 1}; t.insert(c1);
  triplet c2 = {7, 6, 9}; t.insert(c2);
  triplet c3 = {2, 2, 1}; t.insert(c3);
  triplet c4 = {8, 0, 5}; t.insert(c4);
  triplet c5 = {5, 7, 0}; t.insert(c5);
  triplet c6 = {3, 3, 8}; t.insert(c6);
  triplet c7 = {9, 7, 3}; t.insert(c7);
  triplet c8 = {2, 2, 6}; t.insert(c8);
  triplet c9 = {2, 0, 6}; t.insert(c9);

  std::cout << t << std::endl;

  t.erase(c0);
  t.erase(c1);
  t.erase(c3);
  t.erase(c5);

  t.optimise();
  
  std::cout << std::endl << t << std::endl;

  triplet s = {5, 4, 3};
  std::vector<triplet> v;
  unsigned int const RANGE = 3;
  std::cout << "counted " << t.count_within_range(s, RANGE) 
    << " nodes within range " << RANGE << " of " << s << ".\n";
  t.find_within_range(s, RANGE, std::back_inserter(v));
  std::cout << "found   " << v.size() << " nodes within range " << RANGE
    << " of " << s << ".\n";
  std::vector<triplet>::const_iterator ci = v.begin();
  for (; ci != v.end(); ++ci)
    std::cout << *ci << std::endl;
  std::cout << std::endl;
  
  std::cout << "Nearest to " << s << ": " <<
     *t.find_nearest(s,std::numeric_limits<double>::max()).first << std::endl;

  triplet s2 = {10, 10, 2};
  std::cout << "Nearest to " << s2 << ": " << 
     *t.find_nearest(s2,std::numeric_limits<double>::max()).first << std::endl;

  std::cout << std::endl;

  std::cout << t << std::endl;

  return 0;
}

/* COPYRIGHT --
 *
 * This file is part of libkdtree++, a C++ template KD-Tree sorting container.
 * libkdtree++ is (c) 2004-2007 Martin F. Krafft <libkdtree@pobox.madduck.net>
 * and distributed under the terms of the Artistic License 2.0.
 * See the ./COPYING file in the source tree root for more information.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
