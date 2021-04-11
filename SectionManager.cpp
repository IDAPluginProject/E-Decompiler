#include "SectionManager.h"
#include <segment.hpp>
#include <bytes.hpp>

SectionManager::SectionManager()
{

}

SectionManager::~SectionManager()
{
	
}

bool SectionManager::InitSectionManager()
{
	mVec_segInfo.clear();
	int segCount = get_segm_qty();
	for (int idx = 0; idx < segCount; ++idx)
	{
		SegmentInfomation tmpInfo;
		segment_t* pSegment = getnseg(idx);

		tmpInfo.m_segStart = pSegment->start_ea;
		tmpInfo.m_segSize = pSegment->size();

		qstring SectionName;
		get_segm_name(&SectionName, pSegment);
		tmpInfo.m_segName = SectionName;

		tmpInfo.m_segData.resize(pSegment->size());
		get_bytes(&tmpInfo.m_segData[0], pSegment->size(), pSegment->start_ea, GMB_READALL);

		mVec_segInfo.push_back(tmpInfo);
	}
	return true;
}

uint8* SectionManager::LinearAddrToVirtualAddr(ea_t LinerAddr)
{
	segment_t* pSegment = getseg(LinerAddr);
	if (!pSegment) {
		return NULL;
	}
	uint32 offset = LinerAddr - pSegment->start_ea;
	for (unsigned int n = 0; n < mVec_segInfo.size(); ++n) {
		ea_t endAddr = mVec_segInfo[n].m_segStart + mVec_segInfo[n].m_segSize;
		if (LinerAddr >= mVec_segInfo[n].m_segStart && LinerAddr < endAddr) {
			return &mVec_segInfo[n].m_segData[offset];
		}
	}
	return NULL;
}

