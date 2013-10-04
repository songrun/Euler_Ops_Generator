#ifndef _MKSUM_DRAW_H_
#define _MKSUM_DRAW_H_

#include <GL/gli.h>
#include <GL/gliDump.h>
#include <time.h>
#include <GL/gl2ps.h>

#include <map>
using namespace std;

#include "Basic.h"
#include "model.h"
using namespace mathtool;

//-----------------------------------------------------------------------------
//variables used in rendering

bool showWire=false; //on/off wireframe
bool showSharpEdgeOnly=false;
bool saveImg=false;
bool printPDF=false;
bool rotateScene=false;
bool randomColor=false;
bool background=true; //on/off background
bool light=true; //on/off background
uint current_imgID=0; //id for dumping images

bool breakearly=false; //tmp

map<model*,int> model_solid_gids;
map<model*,int> model_wire_gids;
map<model*,Vector3d> model_colors;

void DisplayPolygon(polygon& p);

inline void DisplayModel(model& M, bool randcolor=false)
{
    if(model_solid_gids.find(&M)==model_solid_gids.end())
    {
        const Point3d& O=COM;
        model_solid_gids[&M]=glGenLists(1);
        glNewList(model_solid_gids[&M],GL_COMPILE);

	
		//Draw facets
		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( 0.5f, 0.5f );
		//for(list<polygon>::iterator i=M.polys.begin();i!=M.polys.end();i++)
		glBegin(GL_TRIANGLES);
		for(int i=0;i<M.t_size;i++)
		{
            const triangle & t=M.tris[i];
            glNormal3dv(M.tris[i].n.get());
            for(int k=0;k<3;k++){
                const Point3d& pt=M.vertices[t.v[k]].p;
                glVertex3d(pt[0]-O[0],pt[1]-O[1],pt[2]-O[2]);
            }
		}
		glEnd();
		glDisable( GL_POLYGON_OFFSET_FILL );
		glEndList();
    }
    
    //draw
    if(randcolor){
        if(model_colors.find(&M)==model_colors.end())
            model_colors[&M]=Vector3d(drand48()+0.5,drand48()+0.5,drand48(),+0.5).normalize()+Vector3d(0.25,0.25,0.25);
        glColor3dv(model_colors[&M].get());
    }

    glCallList(model_solid_gids[&M]);
}

inline void DisplayModelWireFrame(model& M, bool randcolor=false)
{
    //Draw Edges
    if(showWire){

	    if(model_wire_gids.find(&M)==model_wire_gids.end())
    	{
	        const Point3d& O=COM;
	        model_wire_gids[&M]=glGenLists(1);
			glNewList(model_wire_gids[&M],GL_COMPILE);
			glBegin(GL_LINES);
            for(uint i=0;i<M.e_size;i++){
                glColor3f(0,0,0);
                const edge & e=M.edges[i];
                if(e.fid.size()==2){//normal case, check if e is sharp
                    triangle& f1=M.tris[e.fid.front()];
                    triangle& f2=M.tris[e.fid.back()];
                    if(fabs(1-f1.n*f2.n)<1e-2){
                        if(showSharpEdgeOnly) continue; //not sharp
                        else
                            glColor3f(0.7f,0.7f,0.7f);
                    }
                }

                Point3d& p1=M.vertices[e.vid[0]].p;
                Point3d& p2=M.vertices[e.vid[1]].p;
                glVertex3d(p1[0]-O[0],p1[1]-O[1],p1[2]-O[2]);
                glVertex3d(p2[0]-O[0],p2[1]-O[1],p2[2]-O[2]);
            }
            glEnd();
			glEndList();
		}

		glCallList(model_wire_gids[&M]);
    }
}


//copied from meshlab
void DisplayBackground(void)
{
	float topcolor[]={1,1,1};
	float bottomcolor[]={1,0,0};
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
		glColor3fv(topcolor);  	glVertex2f(-1, 1);
		glColor3fv(bottomcolor);	glVertex2f(-1,-1);
		glColor3fv(topcolor);	glVertex2f( 1, 1);
		glColor3fv(bottomcolor);	glVertex2f( 1,-1);
	glEnd();
	
	glPopAttrib();
	glPopMatrix(); // restore modelview
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void drawAll()
{
    glEnable(GL_LIGHTING);

    //show the inputs
    glColor3f(1,1,1);
    if(light) glEnable(GL_LIGHTING);
    else glDisable(GL_LIGHTING);
    for(list<model>::iterator i=models.begin();i!=models.end();i++){
        DisplayModel(*i,randomColor);
        if(breakearly) break;
    }
    for(list<model>::iterator i=models.begin();i!=models.end();i++){
        DisplayModelWireFrame(*i);
        if(breakearly) break;
    }
}

//-----------------------------------------------------------------------------
void save_PDF()
{
    //////////////////////////////////////////////////////////////////////////////
    string eps_name;
    if(!input_filenames.empty()) eps_name=string("snapshop-")+input_filenames.front()+".pdf";
    else eps_name="snapshot-objview.pdf";
    FILE *fp = fopen(eps_name.c_str(), "wb");
    GLint buffsize = 20480*20480;
    GLint viewport[4];
    GLdouble modelM[16];  glGetDoublev(GL_MODELVIEW_MATRIX,modelM);
    GLdouble projM[16];   glGetDoublev(GL_PROJECTION_MATRIX,projM);


    glGetIntegerv(GL_VIEWPORT, viewport);
    GLint rbegin=
    gl2psBeginPage (eps_name.c_str(), "Jyh-Ming Lien", viewport,
        GL2PS_PDF, GL2PS_BSP_SORT,
        GL2PS_SILENT | GL2PS_SIMPLE_LINE_OFFSET | GL2PS_POLYGON_OFFSET_FILL |
        GL2PS_NO_BLENDING | GL2PS_OCCLUSION_CULL | GL2PS_BEST_ROOT | GL2PS_COMPRESS |
        GL2PS_TIGHT_BOUNDING_BOX | GL2PS_USE_CURRENT_VIEWPORT,
        GL_RGBA, 0, NULL, 0, 0, 0, buffsize,
        fp, NULL );
    
    if(rbegin==GL2PS_ERROR){
        cerr<<"! ERROR: glps failed to begin"<<endl;
    }
    else{
        gl2psLineWidth(0.05);
        //show the inputs
        drawAll();
        GLint rend=gl2psEndPage();
        switch(rend){
            case GL2PS_NO_FEEDBACK: cerr<<"! ERROR: GL2PS_NO_FEEDBACK"<<endl; break;
            case GL2PS_OVERFLOW: cerr<<"! ERROR: GL2PS_OVERFLOW"<<endl; break;
            case GL2PS_UNINITIALIZED: cerr<<"! ERROR: GL2PS_UNINITIALIZED"<<endl; break;
            case GL2PS_ERROR: cerr<<"! ERROR: GL2PS_ERROR"<<endl; break;
            default: cout<<"- "<<eps_name<<" saved"<<endl;
        }
    }

    fclose(fp);
}

//-----------------------------------------------------------------------------
void Display( void )
{
    //Init Draw
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    if(background) DisplayBackground();
    
    
    glPushMatrix();
    glLoadIdentity();
    static GLfloat light_position1[] = {  100, 100, 100.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    static GLfloat light_position2[] = { -100, -100, 50.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
    glPopMatrix();

    //glRotatef(current_y_rot,0,1,0); //rotate along Y-axis

//    if(R>0)
//        glTranslatef(-COM[0],-COM[1],-COM[2]);
    drawAll();

	if(saveImg){

	    string ppm_name;
	    if(!input_filenames.empty()) ppm_name=string("snapshop-")+input_filenames.front()+".ppm";
	    else ppm_name="snapshot-objview.ppm";

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        dump(ppm_name.c_str(),viewport[2],viewport[3]);
        cout<<"- "<<ppm_name<<" saved"<<endl;
        saveImg=false;
	}

	if(printPDF){
		save_PDF();
		printPDF=false;
	}

    glDisable(GL_LIGHTING);
}


//-----------------------------------------------------------------------------
// regular openGL callback functions
bool InitGL()
{
    // transparent
    glShadeModel(GL_SMOOTH);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_LINE_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    // others
    glEnable( GL_DEPTH_TEST);
    //glDepthMask(GL_TRUE);
	glEnable(GL_NORMALIZE);
    glClearColor(1.0, 1.0, 1.0, 0.0);

    //Let's have light!
    GLfloat Diffuse[] =  { 0.9f, 0.9f, 0.9f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, Diffuse);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat WhiteLight[] =  { 0.75f, 0.75f, 0.75f, 1.0f };
    glLightfv(GL_LIGHT0,GL_DIFFUSE,WhiteLight);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,WhiteLight);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    return true;
}



void Reshape( int w, int h)
{
    if(w>h)
        glViewport( 0, 0, (GLsizei)w, (GLsizei)w );
    else
        glViewport( 0, 0, (GLsizei)h, (GLsizei)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60, 1, R/100, R*100 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void TimerCallback(int value)
{
    //in simuation state
    glutPostRedisplay();
    glutTimerFunc(30, TimerCallback,value);
}

void Keyboard( unsigned char key, int x, int y )
{
    // find closest colorPt3D if ctrl is pressed...
    switch( key ){
        case 27: exit(0);
        case 'w' : showWire=!showWire; break;
        case 'r' : randomColor=!randomColor; break;
		case 'R' : model_colors.clear(); break;
		case 's' : saveImg=!saveImg; break;
		case 'L' : light=!light; break;
		case 'p' : printPDF=!printPDF; break;
		case 'b' : background=!background; break;
		case 'S' : showSharpEdgeOnly=!showSharpEdgeOnly;
		           for(map<model*,int>::iterator i=model_wire_gids.begin();i!=model_wire_gids.end();i++) glDeleteLists(i->second,1);
		           model_wire_gids.clear();
		           break;
        case ' ' : rotateScene=!rotateScene; //rotate scene
                   if(rotateScene)
                   	glutTimerFunc(10, TimerCallback, clock());
                   else
                    glutTimerFunc(10,NULL,0);
                   break;
        case '?' : breakearly=!breakearly; break;
    }
    glutPostRedisplay();
}



#endif //_MKSUM_DRAW_H_


