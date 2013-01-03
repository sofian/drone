// -*- C++ -*-
 
// PLearn (A C++ Machine Learning Library)
// Copyright (C) 1998 Pascal Vincent
// Copyright (C) 1999-2002 Pascal Vincent, Yoshua Bengio and University of Montreal
//
 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
// 
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
// 
//  3. The name of the authors may not be used to endorse or promote
//     products derived from this software without specific prior written
//     permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
// NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// This file is part of the PLearn library. For more information on the PLearn
// library, go to the PLearn Web site at www.plearn.org
 
 
  
 
/* *******************************************************      
 * $Id: stringutils.h,v 1.26 2004/07/21 16:30:51 chrish42 Exp $
 * AUTHORS: Pascal Vincent
 * This file is part of the PLearn library.
 ******************************************************* */
 
// This file contains useful functions for string manipulation
// that are used in the PLearn Library
 
 
#ifndef stringutils_INC
#define stringutils_INC
 
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
//#include <plearn/io/PStream.h>
// #include "TMat.h"
#include "error.h"

using namespace std;
 
// THIS slash CHAR HAS TO GO AWAY!!!!
#if defined(_MINGW_)
#define slash "\\"
#define slash_char '\\'
#else
#define slash "/"
#define slash_char '/'
#endif
 
template<class T> string tostring(const T& x);
 
inline string tostring(const char* s) { return string(s); }
   
string left(const string& s,   size_t width, char padding=' ');
string right(const string& s,  size_t width, char padding=' ');
string center(const string& s, size_t width, char padding=' ');
 
// this function handle numbers with exponents (such as 10.2E09)
// as well as Nans. String can have trailing whitespaces on both sides
bool pl_isnumber(const string& s,double* dbl=NULL);
bool pl_isnumber(const string& s,float* dbl);
 
long tolong(const string& s, int base=10);  
double todouble(const string& s);
bool tobool(const string& s);
inline int toint(const string& s, int base=10) { return int(tolong(s,base)); }
inline float tofloat(const string& s) { return float(todouble(s)); }
     
 
string removeblanks(const string& s);
 
string removeallblanks(const string& s);
 
string removenewline(const string& s);
 
string lowerstring(const string& s);
   
string upperstring(const string& s);
     
string pgetline(istream& in=cin);
 
bool isBlank(const string& s);
 
bool isParagraphBlank(const string& s);
 
 
string space_to_underscore(string str);
 
string underscore_to_space(string str);
 
string backslash_to_slash(string str);
 
int search_replace(string& text, const string& searchstr, const string& replacestr);
 
vector<string> split(const string& s, char delimiter);
 
vector<string> split(const string& s, const string& delimiters=" \t\n\r", bool keepdelimiters=false);
 
void split_on_first(const string& s, const string& delimiters, string& left, string& right);
 
pair<string,string> split_on_first(const string& s,
                                   const string& delimiters=" \t\n\r");
   
string join(const vector<string>& s, const string& separator=" ");
   
vector<string> addprepostfix(const string& prefix, const vector<string>& names, const string& postfix);
 
inline vector<string> addprefix(const string& prefix, const vector<string>& names)
{ return addprepostfix(prefix, names, ""); }
 
inline vector<string> addpostfix(const vector<string>& names, const string& postfix)
{ return addprepostfix("", names, postfix); }
 
string addprepostfix(const string& prefix, const string& text, const string& postfix);
 
inline string addprefix(const string& prefix, const string& text)
{ return addprepostfix(prefix, text, ""); }
 
inline string addpostfix(const string& text, const string& postfix)
{ return addprepostfix("", text, postfix); }
 
vector<string> stringvector(int argc, char** argv);
 
string get_option(const vector<string> &command_line, 
                  const string& option, const string& default_value);
 
bool find(const vector<string> &command_line, const string& option);
 
int findpos(const vector<string> &v, string element);
 
vector<string> remove(const vector<string> &v, string element);
 
 
string extract_filename(const string& filepath);
 
string extract_directory(const string& filepath);
 
string extract_extension(const string& filepath);
 
//'.' and everything after it
string remove_extension(const string& filename);
 
string remove_trailing_slash(const string& path);    
 
string append_slash(const string& path);
 
string extract_filename_without_extension(const string& filepath);
 
void remove_comments(string& text, const string& commentstart="#");
 
vector<string> getNonBlankLines(const string & in);
 
//string* data_filename_2_filenames(const string& filename, int& nb_files);
 
ostream& operator<<(ostream& out, const vector<string>& vs);
 
template <class U, class V>
ostream& operator<<(ostream& out, const pair<U,V>& p)
{
  return out << p.first << ':' << p.second;
}
 
 
 
template<class T> string tostring(const T& x)
{
  ostringstream out;
  // Commented out because this adds a trailing space.
  //      PStream pout(&out);
  //      pout << x;
  out << x;
  return out.str();
 
  /* Old strstream code
     ostrstream out;
     out << setprecision(8) << x;
     char* buf = out.str();
     int n = out.pcount();
     string s(buf,n);
     out.freeze(false); // return ownership to the stream, so that it may free it...
     return s;
  */
}
 
string tostring(const double& x);
 
string tostring(const float& x);
 
 
#endif
 
 
 
 
