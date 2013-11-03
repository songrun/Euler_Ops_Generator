#include "objReader.h"
#include "model.h"
#include <list>
#include <deque>
#include <float.h>
using namespace std;

//-----------------------------------------------------------------------------
bool parseArg(int argc, char ** argv);
void printUsage(char * name);
bool readfromfile(model& m, const string& filename);
string obj2gwb(const string& objfile);   //TODO #1: replace file extension (i.e. abc.obj to abc.gwb)
void saveGWB(model& m, string& filename); //TODO #2: save model to gwb
void printVertices(deque<uint>& vertices);

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
	
// 	cout << input_filename << ' ' << outfilename << endl;	
	//TODO #2: save to gwb
	saveGWB(m,outfilename);
	
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
	string key = ".obj";
 	unsigned found = objfile.rfind(key);
 	string gwbfile( objfile );
	if (found != -1)
		gwbfile.replace( found, gwbfile.length(), ".gwb");
	else 
	{
		cerr<<"! Error: input file is not obj file"<<endl;
		gwbfile.append(".gwb");
	}
	/// for test:
	key = "/";
	unsigned pos = objfile.rfind(key);
	gwbfile = gwbfile.substr( pos );
	gwbfile.insert(0, "output");
	cout << gwbfile << endl;
	//
	return gwbfile;
}

//-----------------------------------------------------------------------------
//TODO #2: save model to gwb

void saveGWB(model& m, string& filename)
{
	//95 pts
	ofstream out;
	out.open( filename.c_str() );
	out << "# This is songrun's gwb file.\n" << endl;
	
	uint s = 1;
	uint f, v, last_f;
	uint v1, v2, v3, v4, f1, f2;
	std::deque<uint> vertexstack;
	std::list<uint> facestack;
	std::list<uint> front_pos;
	double x, y, z;
	s = f = v = 0;
	x = y = z = 0.0;
	double scale = 200;
	
	// Draw triangle tris[0]
	edge& me = m.edges[0];
	uint vi = me.vid[0];
	x = m.vertices[vi].p[0] * scale;
	y = m.vertices[vi].p[1] * scale;
	z = m.vertices[vi].p[2] * scale;
	f1 = f2 = m.t_size; 
	out << "mvfs "<< ++s <<" "<< f1 <<" "<< vi <<" "<<x<<" "<<y<<" "<<z<<endl;
	vertexstack.push_back(vi);
	front_pos.push_back(0);
	
	f = me.fid[0];
	triangle& mt = m.tris[f];
	vertexstack.push_back(me.vid[1]);
	for ( uint i=0; i<3; i++ )
		if( mt.v[i] != me.vid[0] &&  mt.v[i] != me.vid[1] ) 
			vertexstack.push_back(mt.v[i]);

	for ( deque<uint>::iterator v_i=++vertexstack.begin(); v_i!=vertexstack.end(); v_i++ ) {
		vi = *v_i;
		x = m.vertices[vi].p[0] * scale;
		y = m.vertices[vi].p[1] * scale;
		z = m.vertices[vi].p[2] * scale;
		v1 = *(v_i-1);
		v2 = v3 = *vertexstack.begin();
		out << "mev "<< s <<" "<< f1 <<" "<< f2 <<" "<< v1 <<" "<< v2 <<" "<< v3 <<" "<< vi <<" "<< x <<" "<< y <<" "<< z << endl;		
		
	}
	v1 = vertexstack.back();
	v3 = vertexstack.front();
	v2 = v4 = *(++vertexstack.begin());
	out << "mef "<< s <<" "<< f1 <<" "<< v1 <<" "<< v2 <<" "<< v3 <<" "<< v4 <<" "<< f <<endl;
 	last_f = f;


	// Expand from drawn triangles to adjacent triangles until t_size facets are drawn. 
	int t_count = 2;
	while( t_count != m.t_size ) {
		
		uint pos = front_pos.back();
	
		v2 = v3 = vertexstack.back();
		vertexstack.pop_back();
		v1 = vertexstack.back();
		vertexstack.pop_back();
		
		// find the edge between v1 and v2.
		uint ei = -1;
		for ( list<uint>::iterator e_i=m.vertices[v1].m_e.begin(); e_i!=m.vertices[v1].m_e.end(); e_i++) {
			edge& me = m.edges[*e_i];
			if ( (me.vid[0] == v1 && me.vid[1] == v2) || (me.vid[0] == v2 && me.vid[1] == v1) ){
				ei = *e_i;
				break;
			} 
		}
		if (ei == -1) {
			cerr << " e error:" << t_count << endl;
			return;
		}
			
		me = m.edges[ei];
 		// find the next face
		f = -1;
		for (vector<uint>::iterator t_i=me.fid.begin(); t_i!=me.fid.end(); t_i++ ) {
			if ( *t_i != last_f ) {
				f = *t_i;
				mt = m.tris[*t_i];	
			}
		}
		if (f == -1) {
			cerr << "f error:" << t_count << endl;
			return;
		}
		// find the vertex to be generated in the next face
		vi = -1;
		for (uint i=0; i<3; i++){
			if ( mt.v[i] != me.vid[0] && mt.v[i] != me.vid[1] )
				vi = mt.v[i];
		}
		if (vi == -1) {
			cerr << "v error:" << t_count << endl;
			return;
		}

		
		// judge we can add a triangle and a vertex, or add a triangle and remove a vertex, or split the loop, or end the loop
		bool flag = true;		
 		for ( deque<uint>::iterator v_i=vertexstack.begin()+pos; v_i!=vertexstack.end(); v_i++) {
			if (*v_i == vi ) {
				flag = false;
				// end the loop
				if ( vertexstack.end() - vertexstack.begin() == pos + 1 ) {
//  					cout << t_count << " case -1: ";
					vertexstack.pop_back();
					front_pos.pop_back();
					f1 = f2 = facestack.back();
					last_f = f1 - m.t_size;
					facestack.pop_back();				
				}
				// add a triangle and remove a vertex		
				else if ( vertexstack.begin() + pos == v_i ) {
//  					cout << t_count << " case 1: ";
					v3 = *v_i;
					v4 = *(v_i+1);
					vertexstack.push_back(v1);
		 			vertexstack.push_back(v3);	
					vertexstack.erase (vertexstack.begin() + pos);
					out << "mef "<< s <<" "<< f1 <<" "<< v1 <<" "<< v2 <<" "<< v3 <<" "<< v4 <<" "<< f <<endl;
					last_f = f;
				}

				 // delete current loop from back 
				else if (vertexstack.end() - v_i == 1) {
//  				 	cout << t_count << " case 2: ";	
				 	uint v_next_v2 = *(vertexstack.begin() + pos);
					v4 = v1;
				 	
				 	out << "mef "<< s <<" "<< f1 <<" "<< vi <<" "<< v4 <<" "<< v2 <<" "<< v_next_v2 <<" "<<  f <<endl;

					vertexstack.push_back(v2);
					last_f = f;

				 }
				 // split the loop
				 else {
// 				 	cout << t_count << " case 3: ";	
				 	
				 	uint v_next_v2 = *(vertexstack.begin() + pos);
					uint v_next_vi = *(v_i+1);
				 	uint new_pos = (v_i+2)-vertexstack.begin();
				 	
				 	out << "mef "<< s <<" "<< f1 <<" "<< v2 <<" "<< v_next_v2 <<" "<< vi <<" "<< v_next_vi <<" "<< m.t_size + f <<endl;
				 	facestack.push_back(m.t_size + f);
				 	front_pos.push_back(new_pos);
				 	
				 	out << "mef "<< s <<" "<< f1 <<" "<< v1 <<" "<< v2 <<" "<< vi <<" "<< v_next_vi <<" "<< f <<endl;
					last_f = f;
				 	
				 	vertexstack.push_back(v1);
					vertexstack.push_back(vi);
					vertexstack.insert(v_i+1, v2);

				}
				
				break;
			}
		}
		
		// add a triangle and a vertex
		if ( flag ) {
// 	  		cout << t_count << " case 0: ";
			x = m.vertices[vi].p[0] * scale;
			y = m.vertices[vi].p[1] * scale;
			z = m.vertices[vi].p[2] * scale;
			out << "mev "<< s <<" "<< f1 <<" "<< f2 <<" "<< v1 <<" "<< v2 <<" "<< v3 <<" "<< vi <<" "<< x <<" "<< y <<" "<< z << endl;		
		
			v2 = v1;
			v1 = vi;
			v4 = *(vertexstack.begin()+pos);
			out << "mef "<< s <<" "<< f1 <<" "<< v1 <<" "<< v2 <<" "<< v3 <<" "<< v4 <<" "<< f <<endl;
			vertexstack.push_back(v2);
			vertexstack.push_back(v1);
			vertexstack.push_back(v3);
			last_f = f;
		}
		
// 	 	printVertices(vertexstack);
		t_count++;
	}
	
  	out.close();
}	


void printVertices(deque<uint>& vertices)
{
	if (vertices.empty()) {
		cout << " empty stack " << endl;
		return;
	}
	for (deque<uint>::iterator v_i=vertices.begin();v_i!=vertices.end();v_i++ ) {
		cout << *v_i << " ";	
	}
	cout << endl;
}


