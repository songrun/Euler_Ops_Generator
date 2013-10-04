/* file: gwb.cpp
 *
 * Author: Kai Cao (Supervised by Professor Yap, July 2012)
 *
 * Since Core 2.1.
 * 
 ***************************************************/


/***************************************************
 * include files:
 ***************************************************/
#include "gwb_gui.h"
#include "Euler_Ops.h"
#include <cassert>

Euler_Ops *eo;

/***************************************************
 * CONSTANTS:
 ***************************************************/
#define SQRTWO sqrt(2)
#define SQRTFIVE sqrt(5)
#define PHI ((1+SQRTFIVE)/2)
#define SINGLE_SURFACE

/**************************************************
* PARAMETERS:
**************************************************/
int interactive=0;                  // interactive or not:
                                    // <=0 means non-interactive, >0 means interactive.

//**************************************************
// MAIN:
//*************************************************

int main(int argc,char **argv)
{
	
	/*The Euler_Ops Object*/
	eo=new Euler_Ops();
	assert(eo);

	if (argc > 1) fileName = argv[1]; 		      // Input file name

	/*The model name*/
	model=fileName;
		
	/*Read from file*/  	  
	ifstream ifile;
	ifile.open(fileName.c_str());
	
	if (ifile.good()==false)
	{
		cerr<<"! Error: Cannot open file: "<<fileName<<endl;
		return 1;
	}

	/*Variable Maps*/
	map<string, double> variables;
	  
	/*Read every line*/
	string line;

	//start parsing the gwb file
	while (ifile.good())
	{
	    
	    /*Read a line*/
	    getline(ifile,line);
	    
	    /*If it is an empty line,go ahead*/
	    if(line.length()<=0)
			continue;

	    /*If the line begins with '#' it must be a comment*/
	    if(line[0]=='#')
	    	continue;

	    int eqPos=line.find('=');
	    if (eqPos<line.size()&&eqPos>=0)
	    {
			/*Read variables*/
	        string first=line.substr(0,eqPos);
			istringstream iss2(first);
			string var;
			iss2>>var;

			string second=line.substr(eqPos+1);
			istringstream iss3(second);
			double num;
			iss3>>num;
			
			variables[var]=num;
		}//variables
	    else
	    {
	        /*Analyze the line*/
	        string command;
	        Vec<double> *ids= new Vec<double>();
	    
	        /*Read first word*/
	        string first;
	        istringstream iss(line);
	        iss>>first;

	        double fileScalar=variables["scalar"]==0?1:variables["scalar"];
	        
	        /*Then the first word must be command*/
	        command=first;
	        while(iss)
	        {
		
			 	
			 	/*Read the variable*/
				string var;
				 
				/*Push the number*/
	            double num;
	            iss>>var;

				/*Parse the variable*/
				int varPos=var.find('$');
				if (varPos<var.size()&&varPos>=0)
				{
					string tempVar;          // string which will contain the result

					ostringstream convert;   // stream used for the conversion

					convert <<variables[var.substr(varPos+1)];      // insert the constant value

					tempVar = convert.str(); // set 'Result' to the contents of the stream
					num=atof((var.substr(0,varPos)+tempVar).c_str());
				}
				else 
					num=atof(var.c_str());
			 
	            ids->push_back(num);
	        }//while(iss)

	        /*We need to erase the last one*/
	        ids->erase(ids->end()-1);

	        /*Different commands*/
	        if (command.compare("mvfs")==0)
	        {		  
	         	(*ids)[ids->size()-1]*=fileScalar;
		 	 	(*ids)[ids->size()-2]*=fileScalar;
		  		(*ids)[ids->size()-3]*=fileScalar;
	            eo->mvfs(ids);
	    	}
	        else if(command.compare("kvfs")==0)
	        {
	            eo->kvfs(ids);
	        }
	        else if(command.compare("mev")==0)
	        {
		        (*ids)[ids->size()-1]*=fileScalar;
		        (*ids)[ids->size()-2]*=fileScalar;
		        (*ids)[ids->size()-3]*=fileScalar;
	             eo->mev(ids);
	    	}
	        else if(command.compare("kev")==0)
	              eo->kev(ids);
	        else if(command.compare("mef")==0)
	              eo->mef(ids);
	        else if(command.compare("kef")==0)
	              eo->kef(ids);
	        else if(command.compare("kemr")==0)
	              eo->kemr(ids);
	        else if(command.compare("mekr")==0)
	              eo->mekr(ids);
	        else if(command.compare("kfmrh")==0)
	              eo->kfmrh(ids);
	        else if(command.compare("mfkrh")==0)
	              eo->mfkrh(ids);
	        else if(command.compare("ringmv")==0)
	              eo->ringmv(ids);
	        else 
	        {
	              cout<<command<<" is not a valid command."<<endl;
	        }
		}//command

	}//while (ifile.good())
	
	//create GUI
	createGUI(argc,argv);
	
}//main


//**************************************************
//* END
//**************************************************
