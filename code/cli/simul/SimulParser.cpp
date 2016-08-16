//
// Copyright 2009 Arun Saha
//
#include <cli/common/CommonParserNodes.h>
#include <cli/common/Frame.h>
#include <cli/simul/SimulParser.h>
#include <cli/simul/SimulParserHandler.h>

#include <spirit/spiritinclude.h>
using namespace boost::spirit;
using namespace boost::spirit::classic;

static SimulParserHandler* sph_Copy_;

extern ParserMain* MyParserMain_;

SimulParser::SimulParser() {
  sph_ = new SimulParserHandler;
  assert(sph_);

  sph_Copy_ = sph_;
}

SimulParser::~SimulParser() {
  if (sph_) {
    delete sph_;
    sph_ = 0;

    sph_Copy_ = 0;
  }
}

///////////////////////////////////////////////////////////////////////////

namespace SimulParserNameSpace {
bool       ParserModeChanged;
ParserMode NewParserMode;
bool       HelpFlag;

short nthTag;

unsigned long da, sa;
PortId        portNumber;

signed     whichTag;
VlanTag    vlanTag;
VlanHeader vlanHeader;

/////////////////////////////////////////////////////////////////

ParseNode DaNode("da=", "destimation MAC address");
ParseNode SaNode("sa=", "source MAC address");

/////////////////////////////////////////////////////////////////

void acceptSimulHelpEol(const char* a, const char* b) { HelpFlag = true; }

void doSimulHelp(const char* a, const char* b) {
  printHelp(ExitNode);
  printHelp(EndNode);
  printHelp(PortNode);
}

void doSimulExitHelp(const char* a, const char* b) { printHelp(ReturnNode); }

void doSimulExitEol(const char* a, const char* b) {
  ParserModeChanged = true;
  NewParserMode     = ParserModeGlobal;
}

void doSimulEndHelp(const char* a, const char* b) { printHelp(ReturnNode); }

void doSimulEndEol(const char* a, const char* b) {
  ParserModeChanged = true;
  NewParserMode     = ParserModeGlobal;
}

void doPortHelp(const char* a, const char* b) { printHelp(InputNumberNode); }

void getPortId(unsigned x) { portNumber = x; }

void doPortIdHelp(const char* a, const char* b) { printHelp(DaNode); }

void doDaHelp(const char* a, const char* b) { printHelp(InputNumberNode); }

void getDaValue(unsigned x) { da = x; }

void doDaValueHelp(const char* a, const char* b) { printHelp(SaNode); }

void doSaHelp(const char* a, const char* b) { printHelp(InputNumberNode); }

void getSaValue(unsigned x) { sa = x; }

void doSaValueHelp(const char* a, const char* b) {
  printHelp(ReturnNode);
  printHelp(Dot1adNode);
  printHelp(Dot1qNode);
}

///////////////////////////////////////////////////////////
void doVlanHeaderHelp(const char* a, const char* b) {
  printHelp(Dot1adNode);
  printHelp(Dot1qNode);
}

void acceptVhDot1adTok(const char* a, const char* b) {
  if (whichTag == -1 || whichTag == 0) ++whichTag;

  vlanTag.vlanType_ = VlanTypeDot1ad;
}

void acceptVhDot1qTok(const char* a, const char* b) {
  if (whichTag == -1 || whichTag == 0) ++whichTag;

  vlanTag.vlanType_ = VlanTypeDot1q;
}

void doVh1Help(const char* a, const char* b) {
  printHelp(CosNode);
  printHelp(OpenBraceNode);
}

void acceptVhCosTok(const char* a, const char* b) {}

void doVhCosHelp(const char* a, const char* b) { printHelp(InputNumberNode); }

void getCosVal(const unsigned x) { vlanTag.cosValue_ = x; }

void doVhCosValueHelp(const char* a, const char* b) {
  printHelp(OpenBraceNode);
}

void acceptVhOpenBraceTok(const char* a, const char* b) {}

void doVhOpenBraceHelp(const char* a, const char* b) { printHelp(VlanIdNode); }

void acceptVhVlanIdTok(const char* a, const char* b) {}

void getVlanIdVal(const unsigned x) { vlanTag.vlanidValue_ = x; }

void doVhVlanIdHelp(const char* a, const char* b) {
  printHelp(InputNumberNode);
}

void doVhVlanIdValueHelp(const char* a, const char* b) {
  printHelp(CloseBraceNode);
}

void acceptVhCloseBraceTok(const char* a, const char* b) {
  vlanHeader.tag_[whichTag] = vlanTag;
  vlanTag.clear();
}

void doVhCloseBraceHelp(const char* a, const char* b) {
  printHelp(ReturnNode);
  printHelp(Dot1adNode);
  printHelp(Dot1qNode);
}
///////////////////////////////////////////////////////////

void acceptSimulEol(const char* a, const char* b) {
  Frame f;
  f.dstAddr(da);
  f.srcAddr(sa);
  f.vlanHeader(vlanHeader);

  sph_Copy_->rxFrame(portNumber, f);
}
}

//////////////////////////////////////////////////////////////////////////

// Grammar
struct SimulGrammar : public grammar<SimulGrammar> {
  template <typename ScannerT>
  struct definition {
    definition(SimulGrammar const& /*self*/) {
      using namespace SimulParserNameSpace;

      // Expected tokens
      HelpTok       = ch_p('?') >> *print_p >> end_p[&acceptSimulHelpEol];
      ExitTok       = str_p(ExitNode.token());
      EndTok        = str_p(EndNode.token());
      PortTok       = str_p(PortNode.token());
      DaTok         = str_p(DaNode.token());
      SaTok         = str_p(SaNode.token());
      Dot1adTok     = str_p(Dot1adNode.token());
      Dot1qTok      = str_p(Dot1qNode.token());
      CosTok        = str_p(CosNode.token());
      VlanIdTok     = str_p(VlanIdNode.token());
      OpenBraceTok  = ch_p('(');
      CloseBraceTok = ch_p(')');

      // Production rules
      expression = SimulHelp | ExitRuleNode | EndRuleNode | PortRuleNode;
      SimulHelp  = (HelpTok)[&doSimulHelp];

      ExitRuleNode = ExitTok >> (ExitHelp | ExitEol);
      ExitHelp     = (HelpTok)[&doSimulExitHelp];
      ExitEol      = end_p[&doSimulExitEol];

      EndRuleNode = EndTok >> (EndHelp | EndEol);
      EndHelp     = (HelpTok)[&doSimulEndHelp];
      EndEol      = end_p[&doSimulEndEol];

      PortRuleNode   = PortTok >> (PortHelp | PortIdRuleNode);
      PortHelp       = (HelpTok)[&doPortHelp];
      PortIdRuleNode = uint_p[&getPortId] >> (PortIdHelp | DaRuleNode);
      PortIdHelp     = (HelpTok)[&doPortIdHelp];

      DaRuleNode      = DaTok >> (DaHelp | DaValueRuleNode);
      DaHelp          = (HelpTok)[&doDaHelp];
      DaValueRuleNode = uint_p[&getDaValue] >> (DaValueHelp | SaRuleNode);
      DaValueHelp     = (HelpTok)[&doDaValueHelp];

      SaRuleNode = SaTok >> (SaHelp | SaValueRuleNode);
      SaHelp     = (HelpTok)[&doSaHelp];
      SaValueRuleNode =
          uint_p[&getSaValue] >>
          (SaValueHelp | SimulEol | VhDot1adRuleNode | VhDot1qRuleNode);
      SaValueHelp = (HelpTok)[&doSaValueHelp];

      SimulEol = end_p[&acceptSimulEol];

      VhDot1adRuleNode = Dot1adTok[&acceptVhDot1adTok] >>
                         (Vh1Help | VhCosRuleNode | VhOpenBraceRuleNode);
      VhDot1qRuleNode = Dot1qTok[&acceptVhDot1qTok] >>
                        (Vh1Help | VhCosRuleNode | VhOpenBraceRuleNode);
      Vh1Help = HelpTok[&doVh1Help];

      VhCosRuleNode = CosTok[&acceptVhCosTok] >> (VhCosHelp | VhCosValueNode);
      VhCosHelp     = (HelpTok)[&doVhCosHelp];
      VhCosValueNode =
          uint_p[&getCosVal] >> (VhCosValueHelp | VhOpenBraceRuleNode);
      VhCosValueHelp = (HelpTok)[&doVhCosValueHelp];

      VhOpenBraceRuleNode = OpenBraceTok[&acceptVhOpenBraceTok] >>
                            (VhOpenBraceHelp | VhVlanIdRuleNode);
      VhOpenBraceHelp = (HelpTok)[&doVhOpenBraceHelp];

      VhVlanIdRuleNode = VlanIdTok[&acceptVhVlanIdTok] >>
                         (VhVlanIdHelp | VhVlanIdValueRuleNode);
      VhVlanIdHelp = (HelpTok)[&doVhVlanIdHelp];
      VhVlanIdValueRuleNode =
          uint_p[&getVlanIdVal] >> (VhVlanIdValueHelp | VhCloseBraceRuleNode);
      VhVlanIdValueHelp = (HelpTok)[&doVhVlanIdValueHelp];

      VhCloseBraceRuleNode =
          CloseBraceTok[&acceptVhCloseBraceTok] >>
          (VhCloseBraceHelp | SimulEol | VhDot1adRuleNode | VhDot1qRuleNode);
      VhCloseBraceHelp = (HelpTok)[&doVhCloseBraceHelp];
    }

    // Declarations
    rule<ScannerT> HelpTok, ExitTok, EndTok, Dot1adTok, Dot1qTok, CosTok,
        VlanIdTok, OpenBraceTok, CloseBraceTok, expression, SimulHelp,
        ExitRuleNode, ExitHelp, ExitEol, EndRuleNode, EndHelp, EndEol, PortTok,
        DaTok, SaTok, PortRuleNode, PortHelp, PortIdRuleNode, PortIdHelp,
        DaRuleNode, DaHelp, SaRuleNode, SaHelp, DaValueRuleNode, DaValueHelp,
        SaValueRuleNode, SaValueHelp, SimulEol, VhRuleNode, VlanHeaderHelp,
        VhDot1adRuleNode, VhDot1qRuleNode, Vh1Help, VhCosRuleNode, VhCosHelp,
        VhCosValueNode, VhCosValueHelp, VhOpenBraceRuleNode, VhOpenBraceHelp,
        VhVlanIdRuleNode, VhVlanIdHelp, VhVlanIdValueRuleNode,
        VhVlanIdValueHelp, VhCloseBraceRuleNode, VhCloseBraceHelp,
        VhCloseBraceEol;

    rule<ScannerT> const& start() const { return expression; }
  };
};

void SimulParser::simulParser(string const& inputString, bool& helpRequested,
                              ConfigState* c) {
  using namespace SimulParserNameSpace;

  ParserModeChanged  = false;
  NewParserMode      = ParserModeSimul;
  c->currParserMode_ = ParserModeSimul;
  HelpFlag           = false;

  nthTag = -1;

  da         = 0;
  sa         = 0;
  portNumber = 0;

  whichTag = -1;
  vlanTag.clear();
  vlanHeader.clear();

  SimulGrammar g;
  parse_info<> parseInfo = parse(inputString.c_str(), g, space_p);

  if (parseInfo.full) {
    // cout << "Parsed successfully" << endl;

    if (ParserModeChanged) {
      assert(!HelpFlag);

      c->currParserMode_ = NewParserMode;
    }

    if (HelpFlag) {
      assert(!ParserModeChanged);
      helpRequested = true;
    }
  } else {
    cout << "Syntax ERROR: Input unrecognized." << endl;
  }

  return;
}
