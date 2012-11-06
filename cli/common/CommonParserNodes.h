//
// Copyright 2009 Arun Saha
//
#ifndef CLI_COMMON_COMMONPARSERNODES_H
#define CLI_COMMON_COMMONPARSERNODES_H

#include <cli/main/ParserMain.h>

extern ParseNode NoNode;
extern ParseNode ExitNode;
extern ParseNode EndNode;
extern ParseNode ReturnNode;
extern ParseNode InputNumberNode;
extern ParseNode ShutdownNode;
extern ParseNode SimulationNode;
extern ParseNode HostnameNode;
extern ParseNode CountersNode;
extern ParseNode MacTableNode;

extern ParseNode PortNode;
extern ParseNode VbNode;
extern ParseNode EpuNode;

extern ParseNode Dot1adNode;
extern ParseNode Dot1qNode;
extern ParseNode CosNode;
extern ParseNode VlanIdNode;
extern ParseNode OpenBraceNode;
extern ParseNode CloseBraceNode;

void printPortIdHelp();
void printEpuIdHelp();
void printVbIdHelp();

/*
void doVlanHeaderHelp( const char * a, const char * b );
void acceptVhDot1adTok( const char * a, const char * b );
void acceptVhDot1qTok( const char * a, const char * b );
void doVh1Help( const char * a, const char * b );
void acceptVhCosTok( const char * a, const char * b );
void doVhCosHelp( const char * a, const char * b );
void getCosVal( const unsigned x );
void doVhCosValueHelp( const char * a, const char * b );
void acceptVhOpenBraceTok( const char * a, const char * b );
void doVhOpenBraceHelp( const char * a, const char * b );
void acceptVhVlanIdTok( const char * a, const char * b );
void getVlanIdVal( const unsigned x );
void doVhVlanIdHelp( const char * a, const char * b );
void doVhVlanIdValueHelp( const char * a, const char * b );
void acceptVhCloseBraceTok( const char * a, const char * b );
void doVhCloseBraceHelp( const char * a, const char * b );
*/

#endif
