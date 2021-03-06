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
#include "block.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CBlock, CBaseNode);

//---------------------------------------------------------------------------
extern SFString nextBlockChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextBlockChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CBlock::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextBlockChunk, this);
}

//---------------------------------------------------------------------------
SFString nextBlockChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CBlock *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CBlock::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    if (fieldName % "number") {
        *(SFString*)&fieldName = "blockNumber";

    } else if (fieldName % "author") {
        *(SFString*)&fieldName = "miner";

    } else if (isTestMode() && fieldName % "blockHash") {
        *(SFString*)&fieldName = "hash";

    } else if (fieldName % "transactions") {
        // Transactions come to us either as a JSON objects or lists of hashes (i.e. a string array). JSON objects have
        // (among other things) a 'from' field
        if (!fieldValue.Contains("from")) {
            SFString str = fieldValue;
            while (!str.empty()) {
                CTransaction trans;
                trans.hash = toAddress(nextTokenClear(str, ','));
                transactions[transactions.getCount()] = trans;
            }
            return true;
        }
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockNumber" ) { blockNumber = toUnsigned(fieldValue); return true; }
            break;
        case 'd':
            if ( fieldName % "difficulty" ) { difficulty = toUnsigned(fieldValue); return true; }
            break;
        case 'f':
            if ( fieldName % "finalized" ) { finalized = toBool(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gasLimit" ) { gasLimit = toGas(fieldValue); return true; }
            if ( fieldName % "gasUsed" ) { gasUsed = toGas(fieldValue); return true; }
            break;
        case 'h':
            if ( fieldName % "hash" ) { hash = toHash(fieldValue); return true; }
            break;
        case 'm':
            if ( fieldName % "miner" ) { miner = toAddress(fieldValue); return true; }
            break;
        case 'p':
            if ( fieldName % "parentHash" ) { parentHash = toHash(fieldValue); return true; }
            if ( fieldName % "price" ) { price = toDouble(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "timestamp" ) { timestamp = toTimestamp(fieldValue); return true; }
            if ( fieldName % "transactions" ) {
                char *p = (char *)fieldValue.c_str();
                while (p && *p) {
                    CTransaction item;
                    uint32_t nFields = 0;
                    p = item.parseJson(p, nFields);
                    if (nFields)
                        transactions[transactions.getCount()] = item;
                }
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CBlock::finishParse() {
    // EXISTING_CODE
    for (uint32_t i=0;i<transactions.getCount();i++) {
        CTransaction *trans = &transactions[i];
        trans->pBlock = this;
        if (blockNumber >= byzantiumBlock && trans->receipt.status == NO_STATUS) {
            // If we have NO_STATUS in a receipt after the byzantium block, we have to pick it up.
            CReceipt rec;
            getReceipt(rec, trans->hash);
            trans->receipt.status = rec.status;
        }
    }
    //finalized = isFinal(timestamp);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CBlock::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CBlock*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> gasLimit;
    archive >> gasUsed;
    archive >> hash;
    archive >> blockNumber;
    archive >> parentHash;
    archive >> miner;
    archive >> difficulty;
    archive >> price;
    archive >> finalized;
    archive >> timestamp;
    archive >> transactions;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CBlock::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << gasLimit;
    archive << gasUsed;
    archive << hash;
    archive << blockNumber;
    archive << parentHash;
    archive << miner;
    archive << difficulty;
    archive << price;
    archive << finalized;
    archive << timestamp;
    archive << transactions;

    return true;
}

//---------------------------------------------------------------------------
void CBlock::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CBlock, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBlock, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBlock, "gasLimit", T_GAS, ++fieldNum);
    ADD_FIELD(CBlock, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CBlock, "hash", T_HASH, ++fieldNum);
    ADD_FIELD(CBlock, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "parentHash", T_HASH, ++fieldNum);
    ADD_FIELD(CBlock, "miner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CBlock, "difficulty", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "price", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CBlock, "finalized", T_BOOL, ++fieldNum);
    ADD_FIELD(CBlock, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CBlock, "transactions", T_OBJECT|TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CBlock, "schema");
    HIDE_FIELD(CBlock, "deleted");
    HIDE_FIELD(CBlock, "showing");

    // EXISTING_CODE
    ADD_FIELD(CBlock, "date", T_DATE, ++fieldNum);
    HIDE_FIELD(CBlock, "date");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextBlockChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CBlock *blo = (const CBlock *)dataPtr;
    if (blo) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'd':
                if (fieldIn % "date")
                {
                    timestamp_t ts = (timestamp_t)blo->timestamp;
                    return dateFromTimeStamp(ts).Format(FMT_JSON);
                }
                break;
            case 'n':
                if ( fieldIn % "number" ) return asStringU(blo->blockNumber);
                break;
            case 't':
                if ( expContext().hashesOnly && fieldIn % "transactions" ) {
                    uint32_t cnt = blo->transactions.getCount();
                    if (!cnt) return EMPTY;
                    SFString ret;
                    for (uint32_t i = 0 ; i < cnt ; i++) {
                        ret += blo->transactions[i].hash;
                        ret += ((i < cnt-1) ? ",\n" : "\n");
                    }
                    return ret;
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, blo);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CBlock::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CBlock::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    SFBloom removed;
    if (m_schema <= getVersionNum(0,3,0)) {
        archive >> gasLimit;
        archive >> gasUsed;
        archive >> hash;
        archive >> removed; // used to be logsBloom
        archive >> blockNumber;
        archive >> parentHash;
        archive >> timestamp;
        archive >> transactions;
        // TODO -- technically we should re-read these values from the node
        SFString save = getCurlContext()->source;
        getCurlContext()->source = "local";
        CBlock upgrade;uint32_t unused;
        queryBlock(upgrade, asStringU(blockNumber), false, false, unused);
        getCurlContext()->source = save;
        miner = upgrade.miner;
        difficulty = upgrade.difficulty;
        price = 0.0;
        finalized = false;
        finishParse();
        done = true;
    } else if (m_schema <= getVersionNum(0,4,0)) {
        archive >> gasLimit;
        archive >> gasUsed;
        archive >> hash;
        archive >> blockNumber;
        archive >> parentHash;
        archive >> miner;
        archive >> difficulty;
        archive >> price;
        archive >> timestamp;
        archive >> transactions;
        finalized = false;
        finishParse();
        done = true;
    }
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CBlock& blo) {
    blo.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CBlock& blo) {
    blo.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFString CBlock::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextBlockChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockNumber" ) return asStringU(blockNumber);
            break;
        case 'd':
            if ( fieldName % "difficulty" ) return asStringU(difficulty);
            break;
        case 'f':
            if ( fieldName % "finalized" ) return asString(finalized);
            break;
        case 'g':
            if ( fieldName % "gasLimit" ) return fromGas(gasLimit);
            if ( fieldName % "gasUsed" ) return fromGas(gasUsed);
            break;
        case 'h':
            if ( fieldName % "hash" ) return fromHash(hash);
            break;
        case 'm':
            if ( fieldName % "miner" ) return fromAddress(miner);
            break;
        case 'p':
            if ( fieldName % "parentHash" ) return fromHash(parentHash);
            if ( fieldName % "price" ) return fmtFloat(price);
            break;
        case 't':
            if ( fieldName % "timestamp" ) return fromTimestamp(timestamp);
            if ( fieldName % "transactions" || fieldName % "transactionsCnt" ) {
                uint32_t cnt = transactions.getCount();
                if (fieldName.endsWith("Cnt"))
                    return asStringU(cnt);
                if (!cnt) return "";
                SFString retS;
                for (uint32_t i = 0 ; i < cnt ; i++) {
                    retS += transactions[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
    }

    // EXISTING_CODE
    if ( isTestMode() && fieldName % "blockHash" )
        return fromHash(hash);
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CBlock& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CBlock::getObjectAt(const SFString& fieldName, uint32_t index) const {
    if ( fieldName % "transactions" && index < transactions.getCount() )
        return &transactions[index];
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
#define EQ_TEST(a) { if (test.a != a) { cout << " diff at " << #a << " " << test.a << ":" << a << " "; return false; } }
bool CBlock::operator==(const CBlock& test) const {

    EQ_TEST(gasLimit);
    EQ_TEST(gasUsed);
    EQ_TEST(hash);
    EQ_TEST(blockNumber);
    EQ_TEST(parentHash);
    EQ_TEST(miner);
    EQ_TEST(difficulty);
    EQ_TEST(price);
    EQ_TEST(finalized);
    EQ_TEST(timestamp);
    EQ_TEST(transactions.getCount());
    for (uint32_t i = 0 ; i < transactions.getCount() ; i++)
        if (test.transactions[i] != transactions[i])
            return false;

    return true;
}

//---------------------------------------------------------------------------
class CAddressItem {
public:
    blknum_t blockNum;
    blknum_t transIndex;
    blknum_t traceId;
    SFAddress addr;
    CAddressItem(void) : blockNum(0), transIndex(0), traceId(0) { }
};
typedef SFArrayBase<CAddressItem> CAddressItemArray;

//---------------------------------------------------------------------------
int compareAddr(const void *ob1, const void *ob2) {
    const CAddressItem *a1 = (const CAddressItem*)ob1;
    const CAddressItem *a2 = (const CAddressItem*)ob2;
//    cout << "comparing: " << *a1 << " to: " << *a2 << " r: " << (*a1 != *a2) << "\n";
    return a1->addr.compare(a2->addr);
}

//---------------------------------------------------------------------------
bool accumulateAddresses(blknum_t bn, blknum_t tx, blknum_t tc, const SFAddress& addr, void *data) {
    if (zeroAddr(addr))
        return true;

    //    cout << "\n--------------------------------------------\n";
    CAddressItem search;
    search.addr = addr;
    search.blockNum = bn;
    search.transIndex = tx;
    search.traceId = tc;
    CAddressItemArray *array = (CAddressItemArray *)data;
    if (!array->Find(&search, compareAddr)) {
        array->addValue(search);
        array->Sort(compareAddr);
//        cout << "adding:    " << addr << "\n";
//    } else {
//        cout << "not adding " << addr << "\n";
    }
    return true;
}

//---------------------------------------------------------------------------
bool CBlock::forEveryUniqueAddress(ADDRESSFUNC func, TRANSFUNC filterFunc, void *data) {
    if (!func)
        return false;

    CAddressItemArray array;
    forEveryAddress(accumulateAddresses, filterFunc, &array);
    for (uint32_t i = 0 ; i < array.getCount() ; i++) {
        (*func)(array[i].blockNum, array[i].transIndex, array[i].traceId, array[i].addr, data);
    }
    return true;
}

//---------------------------------------------------------------------------
bool isPotentialAddr(SFUintBN test, SFAddress& addrOut) {

    addrOut = "";

    static const SFUintBN small = hex2BN(  "0x00000000000000ffffffffffffffffffffffffff"); // the smallest address we search for
    static const SFUintBN large = hex2BN("0x010000000000000000000000000000000000000000"); // the largest address we search for
    if (test <= small || test >= large)
        return false;

    addrOut = to_hex(test).c_str();
    // Totally a heuristic that can't really be supported, but a good probability that this isn't an address
    if (addrOut.endsWith("00000000"))
        return false;

    if (addrOut.length()<40)
        addrOut = padLeft(addrOut, 40, '0');
    addrOut = addrOut.substr(addrOut.length()-40,40);
    addrOut = toLower("0x" + addrOut);

    return true;
}

//---------------------------------------------------------------------------
void processPotentialAddrs(blknum_t bn, blknum_t tx, blknum_t tc, const SFString& potList, ADDRESSFUNC func, void *data) {

    if (!func)
        return;

    // Pull out 32-byte chunks and check to see if they are addresses
    SFAddress addr;
    for (uint32_t s = 0 ; s < potList.length() / 64 ; s++) {
        SFUintBN test  = hex2BN("0x" + potList.substr(s*64,64));
        if (isPotentialAddr(test, addr))
            (*func)(bn, tx, tc, addr, data);
    }
}

//---------------------------------------------------------------------------
bool CBlock::forEveryAddress(ADDRESSFUNC func, TRANSFUNC filterFunc, void *data) {

    if (!func)
        return false;

    (*func)(blockNumber, NOPOS, 0, miner, data);

    for (uint32_t tr = 0 ; tr < transactions.getCount() ; tr++) {
        CTransaction *trans   = &transactions[tr];
        CReceipt     *receipt = &trans->receipt;
        (*func)(blockNumber, tr, 0, trans->from, data);
        (*func)(blockNumber, tr, 0, trans->to,   data);
        (*func)(blockNumber, tr, 0, receipt->contractAddress, data);
        processPotentialAddrs(blockNumber, tr, 0, trans->input.substr(10), func, data);
        for (uint32_t l = 0 ; l < receipt->logs.getCount() ; l++) {
            CLogEntry *log = &receipt->logs[l];
            (*func)(blockNumber, tr, 0, log->address, data);
            for (uint32_t t = 0 ; t < log->topics.getCount() ; t++) {
                SFAddress addr;
                if (isPotentialAddr(log->topics[t], addr)) {
                    (*func)(blockNumber, tr, 0, addr, data);
                }
            }
            processPotentialAddrs(blockNumber, tr, 0, log->data.substr(2), func, data);
        }

        // If we're not filtering, or the filter passes, proceed. Note the filter depends on the
        // transaction only, not on any address.
        if (!filterFunc || !filterFunc(trans, data)) { // may look at DDos range and nTraces for example
            CTraceArray traces;
            getTraces(traces, trans->hash);
            for (uint32_t t = 0 ; t < traces.getCount() ; t++) {
                CTrace *trace = &traces[t];
                (*func)(blockNumber, tr, t+10, trace->action.from, data);
                (*func)(blockNumber, tr, t+10, trace->action.to, data);
                (*func)(blockNumber, tr, t+10, trace->action.refundAddress, data);
                (*func)(blockNumber, tr, t+10, trace->action.address, data);
                (*func)(blockNumber, tr, t+10, trace->result.address, data);
                SFString input = trace->action.input.substr(10);
                if (!input.empty())
                    processPotentialAddrs(blockNumber, tr, t+10, input, func, data);
            }
        }
    }
    return true;
}
// EXISTING_CODE
}  // namespace qblocks

