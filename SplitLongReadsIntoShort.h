#include <sstream>
#include <stdlib.h>
#include <math.h>
#include <libgen.h>
using namespace std;
std::string revcomp (const std::string& kmer) {
  std::string revstring;
  for (int i = kmer.size() -1; i >= 0; --i) {
    char c = kmer[i];
    char revchar;
    switch (c) {
      case 'g':
        revchar = 'c';
        break;
      case 'G':
        revchar = 'C';
        break;
      case 'a':
        revchar = 't';
        break;
      case 'A':
        revchar = 'T';
        break;
      case 't':
        revchar = 'a';
        break;
      case 'T':
        revchar = 'A';
        break;
      case 'c':
        revchar = 'g';
        break;
      case 'C':
        revchar = 'G';
        break;
      default:
        revchar = 'N';
    }
    revstring += revchar;
  }
  return (revstring);
}

std::string convertUtoT(const std::string &seq) {
    std::string result = seq;
    for (char &c : result) {
        if (c == 'U' || c == 'u') {
            c = 'T';
        } else {
            c = toupper(c);   // 其他字符一律转大写
        }
    }
    return result;
}

