//
// Copyright 2009 Arun Saha
//
#ifndef DATAPLANEMGR_H
#define DATAPLANEMGR_H

#include <cli/common/Frame.h>

class DataPlaneMgr {

  public:

    DataPlaneMgr();
    ~DataPlaneMgr();

    void processFrame( PortId, Frame & );
    void epuTransmit( Epu const *, Frame );   // The 'frame' should not be reference
};

DataPlaneMgr * dataPlaneMgr();

#endif
