<<<<<<< HEAD
cs795-pa1
=========

Part #1:

Total Points: 100

Objective: Write a program that reads in Obj file and output the mesh into Euler operators

Steps: Here are the steps that you should follow in order to finish this part of the
project

#1: Read how triangle mesh are organized (objviewer/model.*)

#2: Familiar yourself with Euler Operators:

	Here are the 8 Euler operations that you will see in the code.
	They also correspond to the Euler operations that you will
	write to the file. 

	/*Make vertex face solid*/
	mvfs(Id s,Id f,Id v,double x,double y,double z)

	/*Kill Edge Face*/
	kef(Id s,Id f,Id v1,Id v2)

	/*Make edge vertex from half_edege_1(v1,v2) in f1 to half_edege_2(v1,v3) in f2*/
	mev(Id s,Id f1,Id f2,Id v1,Id v2,Id v3,Id v4,double x,double y,double z)

	/*Kill edge&vertex*/
	kev(Id s,Id f,Id v1,Id v2)

	/*Make edge face from half_edege_1(v1,v2) in f1 to half_edege_2(v3,v4) in f2 , he1 in the new face*/
	mef(Id s,Id f1,Id v1,Id v2,Id v3,Id v4,Id f2);

	/*Kill edge make ring*/
	kemr(Id s,Id f,Id v1,Id v2)

	/*Make Edge kill Ring*/
	mekr(Id s,Id f,Id v1,Id v2,Id v3,Id v4)

	/*Make face , kill ring & hole*/
	mfkrh(Id s,Id f1,Id v1,Id v2,Id f2)

	Example: cube.gwb

	mvfs 1 1 1 -100 -100 -100
	mev 1 1 1 1 1 1 2 100 -100 -100
	mev 1 1 1 1 2 2 3 -100 100 -100
	mev 1 1 1 3 1 1 4 100 100 -100
	mef 1 1 2 1 4 3 2
	mev 1 2 2 1 3 3 5 -100 -100 100
	mev 1 2 2 2 1 1 6 100 -100 100
	mev 1 2 2 3 4 4 7 -100 100 100
	mev 1 2 2 4 2 2 8 100 100 100
	mef 1 2 6 2 5 1 3
	mef 1 2 5 1 7 3 4
	mef 1 2 7 3 8 4 5
	mef 1 2 8 4 6 5 6

#3 Read Euler_Primitives.cpp to understand how the primitive shapes are created.

#4 Implement a method that converts "struct model" in model.h to a list of Euler operators

#5 Verify your results by comparing them visually using gwb and objview


=======
Euler_Ops_Generator
===================

Convert 3d obj model to Euler Operations
>>>>>>> 05377386c5fc2d753e9efa85b7d52ecf8db0987b
