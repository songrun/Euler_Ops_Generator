//------------------------------------------------------------------------------
//  Copyright 2007-2008 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#include "main.h"

//
//
//
//
//   The MAIN function
//
//
//
//

int main(int argc, char ** argv)
{
    //
    //cout.precision(8);
    //cout<<fixed;
    //
    if(!parseArg(argc,argv)){
        printUsage(argv[0]);
        return 1;
    }

	//
	readfromfiles();
	//

    /////////////////////////////////////////////////////////////////
    //setup glut/gli
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH );
    glutInitWindowSize( 680, 680);
    glutInitWindowPosition( 50, 50 );
    glutCreateWindow( "objview" );

    InitGL();
    gli::gliInit();
    gli::gliDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);

    //set camera position
    gli::setCameraPosZ(R*2.1);
    /////////////////////////////////////////////////////////////
    gli::gliMainLoop();

    return 0;
}

