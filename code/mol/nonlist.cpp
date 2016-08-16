//
// Copyright 2009 Arun Saha
//
signed     whichTag;
VlanTag    vlanTag;
VlanHeader vlanHeader;

whichTag = -1;
vlanTag.clear();
vlanHeader.clear();

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
  printHelp(Dot1adNode);
  printHelp(Dot1qNode);
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
}

void doVhCloseBraceHelp(const char* a, const char* b) {
  printHelp(ReturnNode);
  printHelp(Dot1adNode);
  printHelp(Dot1qNode);
}

void acceptVhCloseBraceTokEol(const char* a, const char* b) {
  // vlanHeader ready
  // call backend
}

// Vh = Vlan Header

VhDot1adRuleNode = Dot1adTok[&acceptVhDot1adTok] >>
                   (Vh1Help | VhCosRuleNode | VhOpenBraceRuleNode);
VhDot1qRuleNode = Dot1qTok[&acceptVhDot1qTok] >>
                  (Vh1Help | VhCosRuleNode | VhOpenBraceRuleNode);
Vh1Help = HelpTok[&doVh1Help];

VhCosRuleNode  = CosTok[&acceptVhCosTok] >> (VhCosHelp | VhCosValueNode);
VhCosHelp      = (HelpTok)[&doVhCosHelp];
VhCosValueNode = uint_p[&getCosVal] >> (VhCosValueHelp | VhOpenBraceRuleNode);
VhCosValueHelp = (HelpTok)[&doVhCosValueHelp];

VhOpenBraceRuleNode = OpenBraceTok[&acceptVhOpenBraceTok] >>
                      (VhOpenBraceHelp | VhVlanIdRuleNode);
VhOpenBraceHelp = (HelpTok)[&doVhOpenBraceHelp];

VhVlanIdRuleNode = VlanIdTok[&acceptVhVlanIdTok] >>
                   (VhVlanIdHelp | VhVlanIdValueRuleNode);
VhVlanIdHelp          = (HelpTok)[&doVhVlanIdHelp];
VhVlanIdValueRuleNode = uint_p[&getVlanIdVal] >>
                        (VhVlanIdValueHelp | VhCloseBraceRuleNode);
VhVlanIdValueHelp = (HelpTok)[&doVhVlanIdValueHelp];

VhCloseBraceRuleNode = CloseBraceTok[&acceptVhCloseBraceTok] >>
                       (VhCloseBraceHelp | VhCloseBraceEol | VhDot1adRuleNode |
                        VhDot1qRuleNode);
VhCloseBraceHelp = (HelpTok)[&doVhCloseBraceHelp];
VhCloseBraceEol  = end_p[&acceptCloseBraceTokEol];

VhDot1adRuleNode, VhDot1qRuleNode, Vh1Help, VhCosRuleNode, VhCosHelp,
    VhCosValueNode, VhCosValueHelp, VhOpenBraceRuleNode, VhOpenBraceHelp,
    VhVlanIdRuleNode, VhVlanIdHelp, VhVlanIdValueRuleNode, VhVlanIdValueHelp,
    VhCloseBraceRuleNode, VhCloseBraceHelp, VhCloseBraceEol
