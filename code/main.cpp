//
// Copyright 2009 Arun Saha
//
#include <cli/main/ParserMain.h>
#include <mol/common/MolVp.h>
#include <dataplane/DataPlaneMgr.h>

int
main()
{

    cout << "/////////////////////////////////////////////////////////" << endl;
    cout << "\t Prototype simulator of basic configuration model." << endl;
    cout << "\t Type a command or \"quit\" to exit. or '?' for help." << endl;
    cout << "/////////////////////////////////////////////////////////" << endl;

	ParserMain * pm = new ParserMain;
	assert( pm );

	MolVp * mvp		= new MolVp;
	assert( mvp );

	pm->molVp( mvp );

    DataPlaneMgr * dp  = new DataPlaneMgr;
    assert( dp );

	pm->main();

	return 0;
}


