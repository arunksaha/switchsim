//
// Copyright 2009 Arun Saha
//
#include <cli/common/Types.h>
#include <cli/common/CommonParserNodes.h>

#include <mol/common/Types.h>
#include <mol/common/MolVp.h>
#include <mol/epu/Epu.h>

#include <dataplane/DataPlaneMgr.h>


/////////////////////////////////////////////////////////////////////////

DataPlaneMgr * MyDataPlaneMgr_;

DataPlaneMgr * dataPlaneMgr()
{
    assert( MyDataPlaneMgr_ );
    return MyDataPlaneMgr_;
}

/////////////////////////////////////////////////////////////////////////

DataPlaneMgr::DataPlaneMgr()
{
    MyDataPlaneMgr_ = this;
}

DataPlaneMgr::~DataPlaneMgr()
{
    MyDataPlaneMgr_ = 0;
}

void
DataPlaneMgr::processFrame( PortId rxPortId, Frame & frame )
{
    Port * rxPort = portMgr()->port( rxPortId );
    assert( rxPort );

    rxPort->incrInFrames();

    ///////////////////////   Classification  ///////////////////////////
    Epu const * rxEpu = rxPort->ingressMatchClassify( frame );

    if( rxEpu )
    {
        cout << "\t1. Frame classfication:" << endl;
        cout << "\t -> Ingress Frame at " <<  EpuNode.tokenString() << " " << rxEpu->epuId() << endl; 
    }
    else
    {
        cout << "\tIngress Frame did not match any " <<  EpuNode.tokenString() << endl; 
        return;
    }

    rxEpu->incrInFrames();

    ///////////////////////   Ingress Tag operation  ////////////////////
    (const_cast<Epu *>(rxEpu))->rewriteFrame( false, frame );      // Alert: 'false' denotes ingress
    cout << "\n\t2. Ingress Frame Rewrite:" << endl;
    cout << "\t -> After ingress rewrite, Frame := " << frame << endl;

    ///////////////////////   Forwarding  ///////////////////////////////
    FwdDomainId const fdId = rxEpu->fdId();
    FwdDomain * fwdDomain = fdMgr()->findFwdDomain( fdId );
    cout << "\n\t3. Frame forwarding: " << endl;
    if( fwdDomain )
    {
        cout << "\t -> Forwarding domain = " << VbNode.tokenString() << " " << fwdDomain->fdId() << endl; 
    }
    else
    {
        cout << "\t" << EpuNode.tokenString() << " does not belong to any domain. Frame dropped." << endl;
        return;
    }

    EpuId rxEpuId = rxEpu->epuId();
    MacTable * macTable = fdMgr()->macTable();

    if( fwdDomain->learningRequired() )
    {
        MacAddr frameSa = frame.srcAddr();
        macTable->learnSa( frameSa, fdId, rxEpuId );
    }

    MacAddr frameDa = frame.dstAddr();
    MacEntry * macEntry = 0;
    Epu * txEpu = 0;

    if( fwdDomain->fdType() == FdTypeMP2MP )
    {
        macEntry = macTable->lookupDa( frameDa, fdId );
    }

    if( macEntry )         ///////// DA lookup was required and successful /////////////////
    {
        EpuId txEpuId = macEntry->epuId_;
        txEpu = epuMgr()->findEpu( txEpuId );
        assert( txEpu );

        if( txEpu != rxEpu )
        {
            epuTransmit( txEpu, frame );
        }
    }
    else                  ////////// Either DA lookup was not required, or it is failure ///
    {
        EpuListConstItr cit;
        EpuList epuL = fwdDomain->epuList();

		for( cit = epuL.begin(); cit != epuL.end(); cit++ )
		{
    		txEpu = (*cit);
            assert( txEpu );

            if( txEpu != rxEpu )
            {
                epuTransmit( txEpu, frame );
            }
        }
    }
}

void
DataPlaneMgr::epuTransmit( Epu const * txEpu, Frame frame )
{
    PortId txPortId = txEpu->portId();
    Port * txPort   = portMgr()->port( txPortId );
    assert( txPort );

    ///////////////////////   Egress Tag operation  /////////////////
    (const_cast<Epu *>(txEpu))->rewriteFrame( true, frame );       // Alert: 'true' denotes egress
    cout << "\n\t4. Egress Frame Rewrite:" << endl;
    cout << "\t -> At " << EpuNode.tokenString() << " " << txEpu->epuId() << 
        " (" << PortNode.tokenString() << " = " << txPortId << "), after egress rewrite, Frame := " << 
        frame << endl;

    txEpu->incrOutFrames();

    txPort->incrOutFrames();
}

