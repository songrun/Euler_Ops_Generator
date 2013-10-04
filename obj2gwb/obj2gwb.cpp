
#include "objReader.h"
#include "model.h"
#include <list>
#include <float.h>
using namespace std;

//-----------------------------------------------------------------------------
bool parseArg(int argc, char ** argv);
void printUsage(char * name);
bool readfromfile(model& m, const string& filename);
string obj2gwb(const string& objfile);   //TODO #1: replace file extension (i.e. abc.obj to abc.gwb)
void saveGWB(model& m, string filename); //TODO #2: save model to gwb

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
    //
    string input_filename;
    for(int i=1;i<argc;i++) input_filename=argv[i];
	if(input_filename.empty())
	{
		cerr<<"! Error: no filename is given"<<endl;
		printUsage(argv[0]);
		return 1;
	}
	
	//read obj files
	model m;
	readfromfile(m, input_filename);
	
	//TODO #1: replace file extension (i.e. abc.obj to abc.gwb)
	string outfilename=obj2gwb(input_filename);
		
	//TODO #2: save to gwb
	saveGWB(m,input_filename);
	
	return 0;
}

//-----------------------------------------------------------------------------
bool parseArg(int argc, char ** argv)
{


    return true;
}

void printUsage(char * name)
{
    cerr<<"\nUsage: "<<name<<" *.obj"<<endl;;
}

//-----------------------------------------------------------------------------

bool readfromfile(model& m, const string& filename)
{
	cout<<"- Start reading..."<<endl;
	if(!m.build(filename)) return false;
	cout<<"- Done reading "<<m.v_size<<" vertices and "<<m.t_size<<" facets"<<endl;
    return true;
}

//-----------------------------------------------------------------------------
//TODO #1: replace file extension (i.e. abc.obj to abc.gwb)
string obj2gwb(const string& objfile)   
{
	//5 pts
	return "tmp.gwb";
}

//-----------------------------------------------------------------------------
//TODO #2: save model to gwb
void saveGWB(model& m, string filename)
{
	//95 pts
}

