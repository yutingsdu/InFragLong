#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<algorithm>
#include<vector>
#include<map>
#include <getopt.h>
#include "SplitLongReadsIntoShort.h"
using namespace std;
int read_length = 76;
int fragment_length = 200;
int Isize; //fragment_length - read_length
int step_length = 200;//50;
int ReadNumber=3000000;
int minReadLength=1000;
string readFile;
string prefix = "short";
string FAorFQ(const char* file)
{
    ifstream in(file);
    istringstream istr;
    string s;
    getline(in,s);
    if(s[0] == '>') return "fa";
    else if(s[0] == '@') return "fq";
    else cerr<<"please input fastq or fasta format file!"<<endl;

    return "";

}
void load_and_process(const char* file1,const  char* file2,const char* file3)
{
    ifstream in(file1);
    ofstream out(file2);
    ofstream out1(file3);
    string id,seq,temp1,temp2;
    istringstream istr;
    int i=0;
    while(getline(in,id))
    {
	i++;
	//if(i>ReadNumber) return;
	getline(in,seq);
	getline(in,temp1);
	getline(in,temp2);
	int a = 0;
	if(seq.length() < fragment_length ) continue;
	if(seq.length() < minReadLength ) continue;
        for(size_t i=0;i<seq.length();)
	{
	    out<<"@"<<a<<"_"<<id.substr(1)<<'\n';
	    out1<<"@"<<a<<"_"<<id.substr(1)<<'\n';
	    if(seq.length() - i > fragment_length )
	    {
	        out<<convertUtoT(seq.substr(i,read_length))<<'\n';
		out<<temp1<<'\n';
		out<<temp2.substr(i,read_length)<<'\n';

	        //out1<<revcomp(seq.substr(i + Isize, read_length))<<'\n';
	        out1<<convertUtoT(seq.substr(i + Isize, read_length))<<'\n';
		out1<<temp1<<'\n';
		out1<<temp2.substr(i + Isize, read_length)<<'\n';
	    }
	    else 
	    {
		int k = seq.length() - fragment_length;
	    	out<<convertUtoT(seq.substr(k,read_length))<<'\n';
		out<<temp1<<'\n';
		out<<temp2.substr(k,read_length)<<'\n';

	    	//out1<<revcomp(seq.substr(k + Isize))<<'\n';
	    	out1<<convertUtoT(seq.substr(k + Isize))<<'\n';
		out1<<temp1<<'\n';
		out1<<temp2.substr(k + Isize)<<'\n';
		break;
	    }
	    //out<<temp1<<'\n';
	    //out<<temp2.substr(i,50)<<'\n';
	    i += step_length;
	    a++;
	}
    }
    in.close();
    out.close();
    out1.close();
    return;
}
void load_and_process_fa(const char*file1,const char* file2,const char* file3)
{
    ifstream in(file1);
    ofstream out(file2);
    ofstream out1(file3);
    string id,seq,temp1,temp2;
    istringstream istr;
    int i=0;
    while(getline(in,id))
    {
	i++;
	//if(i>ReadNumber) return;
	getline(in,seq);
	int a = 0;
	if(seq.length() < fragment_length ) continue;
	if(seq.length() < minReadLength ) continue;
        for(size_t i=0;i<seq.length();)
	{
	    out<<">"<<a<<"_"<<id.substr(1)<<'\n';
	    out1<<">"<<a<<"_"<<id.substr(1)<<'\n';
	    if(seq.length() - i > fragment_length )
	    {
	        out<<seq.substr(i,read_length)<<'\n';

	        //out1<<revcomp(seq.substr(i + Isize, read_length))<<'\n';
	        out1<<seq.substr(i + Isize, read_length)<<'\n';
	    }
	    else 
	    {
		int k = seq.length() - fragment_length;
	    	out<<seq.substr(k,read_length)<<'\n';

	    	//out1<<revcomp(seq.substr(k + Isize))<<'\n';
	    	out1<<seq.substr(k + Isize)<<'\n';
		break;
	    }
	    //out<<temp1<<'\n';
	    //out<<temp2.substr(i,50)<<'\n';
	    i += step_length;
	    a++;
	}
    }
    in.close();
    out.close();
    out1.close();
    return;
}
bool Help,version_flag,onlyDataType;
static const char* short_options = "i:r:g:M:p:h";//RevisionNC
static struct option long_options[] = {
    {"input",             	1,  0,  'i'},
    {"read-length",             1,  0,  'r'},
    {"fragment-length",         1,  0,  'f'},
    {"minlen-be-fragmented",    1,  0,  'M'},
    {"prefix",                  1,  0,  'p'},
    {"help",                    0,  0,  'h'},
    {0,0,0,0}
  };
int parse_options(int argc, char* argv[]) {
    int option_index = 0;
    int next_option;

    do {
        next_option = getopt_long(argc, argv, short_options, long_options, &option_index);
        switch (next_option) {
            case -1:     /* Done with options. */
                break;
            case 'i': //read length
		readFile = optarg;
                break;
            case 'r': //read length
		read_length = atoi(optarg);
                break;
            case 'f': //"fragment length"
		fragment_length = atoi(optarg);
                break;
            case 'p': //"prefix"
		prefix = optarg;
                break;
            case 'M': //"prefix"
		minReadLength = atoi(optarg);
                break;
            case 'h':
                Help = true;
                break;
            default:
                exit(1);
        }
    } while(next_option != -1);
    if(Help){
        //cout << usage() ;
        exit(1);
    }
    return 0;
}
int main(int argc,char* argv[])
{
    if(argc == 1) {
        cout<<"-"<<endl;
        cout<<"This is a simple program to split long reads into short reads."<<endl;
        cout<<"./exe long_reads.fq -i read.fastq(or fasta) --prefix short --read-length 76 --fragment-length 200 --minlen-be-fragmented 1000"
	    <<" ("<<read_length<<"bp-length, "<<step_length<<"bp-step_length)."<<endl;
        cout<<"-"<<endl;
        return 0;
    }
    int parse_ret = parse_options(argc, argv);
    if (parse_ret)
      return parse_ret;

    cout<<readFile<<endl;
    cout<<"synthetic read-length: "<<read_length<<endl;
    cout<<"synthetic fragment length: "<<fragment_length<<endl;
    cout<<"minlen-be-fragmented: "<<minReadLength<<endl;

    Isize = fragment_length - read_length;
    const char* file1 = readFile.c_str();
    string format = FAorFQ(file1);
    if(format == "fq")
    {
	const char* out_file1 = (prefix + "_1.fastq").c_str();
	const char* out_file2 = (prefix + "_2.fastq").c_str();
        load_and_process(file1,out_file1,out_file2);

    }
    else if(format == "fa")
    {
	const char* out_file1 = (prefix + "_1.fasta").c_str();
	const char* out_file2 = (prefix + "_2.fasta").c_str();
        load_and_process_fa(file1, out_file1,out_file2);
    }
    return 0;
}
