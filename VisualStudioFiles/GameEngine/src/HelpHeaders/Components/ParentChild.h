#pragma once

#include <vector>

#include "../Component.h"

class ParentChild : public Component<ParentChild> {


public:

	unsigned int parEntity = 0;
	unsigned int currentEntity = 0;
	std::vector<unsigned int> childrenEntity;
	int childrenEntitySize = 0;

	virtual void SerializeComponent() override 
	{
		SF(DelimiterType::NEW_LINE, parEntity);
		SF(DelimiterType::NEW_LINE, currentEntity);
		
		childrenEntitySize = childrenEntity.size();
		SF(DelimiterType::NEW_LINE, childrenEntitySize);
		
		SFVecIntsT(childrenEntity, "child");
	}
	virtual void DeSerializeComponent() override
	{
		DSF(&parEntity);
		DSF(&currentEntity);
		DSF(&childrenEntitySize);
		DSFVecIntsT("child", childrenEntity, childrenEntitySize);
	}

	virtual void UIfyComponent() override 
	{
		int parentEntity = (int)parEntity;
		UIfyInt(parentEntity);

		float size = childrenEntity.size();
		UIfyFloat(size);
		for (int i = 0; i < childrenEntity.size(); i++)
		{
			int childEntity = (int)childrenEntity[i];
			UIfyInt(childEntity);
		}
	}
};