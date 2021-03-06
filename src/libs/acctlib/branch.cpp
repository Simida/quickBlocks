/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "branch.h"
#include "treeroot.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CBranch, CTreeNode);

//---------------------------------------------------------------------------
static SFString nextBranchChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextBranchChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CBranch::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, dataPtr))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextBranchChunk, this);
}

//---------------------------------------------------------------------------
SFString nextBranchChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CBranch *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CBranch::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTreeNode::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "branchValue" ) { branchValue = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CBranch::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CBranch::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CBranch*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    CTreeNode::Serialize(archive);

    // EXISTING_CODE
    for (int i=0;i<16;i++) {
        if (nodes[i]) {
            delete nodes[i];
            nodes[i] = NULL;
        }

        bool has_val = false;
        archive >> has_val;
        if (has_val) {
            SFString className;
            archive >> className;
            nodes[i] = createTreeNode(className);
            if (!nodes[i])
                return false;
            nodes[i]->Serialize(archive);
        }
    }
    // EXISTING_CODE
    archive >> branchValue;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CBranch::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CTreeNode::SerializeC(archive);

    // EXISTING_CODE
    for (int i=0;i<16;i++) {
        archive << bool(nodes[i] != NULL);
        if (nodes[i]) {
            SFString className = nodes[i]->getRuntimeClass()->getClassNamePtr();
            archive << className;
            nodes[i]->SerializeC(archive);
        }
    }
    // EXISTING_CODE
    archive << branchValue;

    return true;
}

//---------------------------------------------------------------------------
void CBranch::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTreeNode::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(CBranch, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CBranch, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBranch, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBranch, "branchValue", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CBranch, "schema");
    HIDE_FIELD(CBranch, "deleted");
    HIDE_FIELD(CBranch, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextBranchChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CBranch *bra = (const CBranch *)dataPtr;
    if (bra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'n':
                if ( fieldIn % "nodes" ) {
                    SFString ret;
                    for (int i=0;i<16;i++) {
                        if (bra->nodes[i]) {
                            ret += bra->nodes[i]->Format();
                        }
                    }
                    return ret;
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, bra);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CBranch::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CBranch::readBackLevel(SFArchive& archive) {

    CTreeNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CBranch::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextBranchChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "branchValue" ) return branchValue;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTreeNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CBranch& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
    //-----------------------------------------------------------------------------
    CBranch::CBranch(const SFString& _value) : branchValue(_value) {
        memset(nodes, 0, sizeof(CTreeNode*) * 16);
    }

    //-----------------------------------------------------------------------------
    CBranch::CBranch(char _i1, CTreeNode* _n1, const SFString& _value) : branchValue(_value) {
        memset(nodes, 0, sizeof(CTreeNode*) * 16);
        nodes[nodeIndex(_i1)] = _n1;
    }

    //-----------------------------------------------------------------------------
    CBranch::CBranch(char _i1, CTreeNode* _n1, char _i2, CTreeNode* _n2) {
        memset(nodes, 0, sizeof(CTreeNode*) * 16);
        nodes[nodeIndex(_i1)] = _n1;
        nodes[nodeIndex(_i2)] = _n2;
    }

    //-----------------------------------------------------------------------------
    SFString CBranch::at(const SFString& _key) const {
        if (_key.empty())
            return branchValue;

        int idx = nodeIndex(_key[0]);
        if (nodes[idx])
            return nodes[idx]->at(_key.substr(1));

        return "";
    }

    //-----------------------------------------------------------------------------
    CTreeNode* CBranch::insert(const SFString& _key, const SFString& _value) {
        if (verbose == 2) { cerr << "\tbranch inserting " << _key << " at " << _value << "\n"; }
        if (_key.empty()) {
            // We've reached the end of the key, so store the value here
            if (branchValue.empty()) {
                // store the first encountered block
                branchValue = _value;

            } else {
                // preserve the most recent block encountered
                branchValue = nextTokenClear(branchValue, '|');
                branchValue += "|" + _value;
            }

        } else {
            // Figure out which bucket to store the value in by the next character in the key
            int idx = nodeIndex(_key[0]);
            if (nodes[nodeIndex(_key[0])]) {
                // There is already something stored here, so we need to find room for it
                nodes[idx] = nodes[idx]->insert(_key.substr(1), _value);

            } else {
                // we've reached a leaf
                nodes[idx] = new CLeaf(_key.substr(1), _value);
            }
        }
        return this;
    }

    //-----------------------------------------------------------------------------
    CTreeNode* CBranch::remove(const SFString& _key) {
        if (verbose) {
            cerr << endl<< endl<< endl
            << idnt << SFString('-', 80) << endl
            << idnt << SFString('-', 80) << endl
            << idnt << "remove branch at [" << _key << "]: ";
            idnt+="\t";
        }

        if (_key.empty()) {
            if (branchValue.length()) {
                if (verbose)
                    cerr << "and non-empty value " << endl;
                branchValue = "";
                return rejig();
            }
            if (verbose)
                cerr << "and empty value" << endl;
            idnt.Replace("\t", "");
            return this;
        }

        int idx = nodeIndex(_key[0]);
        if (nodes[idx]) {
            const char *k = (const char*)_key;
            CTreeNode *ret = nodes[idx]->remove(&k[1]);
            nodes[idx] = ret;
            CTreeNode *n = rejig();
            if (verbose)
                idnt.Replace("\t", "");
            return n;
        }

        if (verbose) {
            cerr << endl;
            idnt.Replace("\t", "");
        }
        return this;
    }

    //-----------------------------------------------------------------------------
#define NO_BRANCHS   ((char)-1)
#define MULTI_BRANCH ((char)16)
    CTreeNode *CBranch::rejig() {
        char n = activeBranch();
        if (n == NO_BRANCHS && branchValue.length()) {
            if (verbose)
                cerr << "No branches, but we have a value, so we save it as a leaf" << endl;
            // revert back down to a leaf
            CTreeNode *r = new CLeaf("", branchValue);
            delete this;
            return r;

        } else if (n != MULTI_BRANCH && branchValue.empty()) {
            if (verbose == 2)
                cerr << idnt << "This single (empty) branch has a single child at [" << idex(n) << "]" << endl;
            // only branching to n...
            int nn = static_cast<int>(n);
            if (auto b = dynamic_cast<CBranch*>(nodes[nn])) {
                // switch to infix
                nodes[nn] = NULL;
                delete this;
                SFString x = idex(n);
                return new CInfix(x, b);

            } else {
                auto x = nodes[nn];
                ASSERT(x);
                // include in child
                x->prefixS = idex(n) + x->prefixS;
                nodes[nn] = NULL;
                delete this;
                return x;
            }
        }

        return this;
    }

    //-----------------------------------------------------------------------------
    /// @returns (byte)-1 when no active branches, 16 when multiple active and the index of the active branch otherwise.
    char CBranch::activeBranch() const {
        char n = NO_BRANCHS;
        for (int i = 0 ; i < 16 ; i++) {
            if (nodes[i]) {
                if (n == NO_BRANCHS)
                    n = static_cast<char>(i);
                else
                    return MULTI_BRANCH;
            }
        }
        return n;
    }

    //------------------------------------------------------------------
    bool CBranch::visitItems(ACCTVISITOR func, void *data) const {
        ASSERT(func);
        CVisitData *vd = reinterpret_cast<CVisitData*>(data);
        uint32_t save = vd->type;
        vd->type = T_BRANCH;
        vd->counter = 0;
        vd->strs = vd->strs + branchValue + "+";
        //vd->strs = vd->strs + "+";
        (*func)(this, data);
        for (uint32_t i = 0; i < 16; ++i) {
            if (nodes[i]) {
                vd->level++;
                vd->strs = vd->strs + "-" + idex((char)i);
                nodes[i]->visitItems(func, data);
                nextTokenClearReverse(vd->strs, '-');
                vd->level--;
            }
        }
        nextTokenClearReverse(vd->strs, '+');
        vd->type = save;
        return true;
    }
// EXISTING_CODE
}  // namespace qblocks

