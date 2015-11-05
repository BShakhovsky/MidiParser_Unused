# pragma once
# include "MetaEvent.h"

class MetaEvent_Tempo : public MetaEvent
{
public:
	virtual ~MetaEvent_Tempo() override = default;
private:
	explicit MetaEvent_Tempo(char statusByte, char metaType) :
		MetaEvent(statusByte, metaType)
	{}
	META_DECL(Tempo);
};