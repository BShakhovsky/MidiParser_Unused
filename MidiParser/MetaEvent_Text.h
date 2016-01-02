# pragma once
# include "MetaEvent.h"

class MetaEvent_Text : public MetaEvent
{
public:
	virtual ~MetaEvent_Text() = default;
private:
	explicit MetaEvent_Text(char statusByte, char metaType) :
		MetaEvent(statusByte, metaType)
	{}
	void ReadText(const char* eventMsg);	// may throw
	META_DECL(Text);
};