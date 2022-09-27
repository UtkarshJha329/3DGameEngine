#pragma once

#include <vector>

#include "../Component.h"

class ParentChild : public Component<ParentChild> {


public:

	unsigned int parEntity = 0;
	unsigned int currentEntity = 0;
	std::vector<unsigned int> childrenEntity;

	int codedHierarchy = -1;

	virtual void SerializeComponent() override 
	{
		SF(DelimiterType::NEW_LINE, parEntity);
		SF(DelimiterType::NEW_LINE, currentEntity);
		//SF(DelimiterType::NEW_LINE, codedHierarchy);
		SFVecIntsT(childrenEntity);
	}
	virtual void DeSerializeComponent() override
	{
		DSF(&parEntity);
		DSF(&currentEntity);
		DSFVecIntsT(3, childrenEntity);
	}

	virtual void UIfyComponent() override 
	{
		int parentEntity = (int)parEntity;
		UIfyInt(parentEntity);

		int saveHierarchy = codedHierarchy;
		UIfyInt(saveHierarchy);

		float size = childrenEntity.size();
		UIfyFloat(size);
		for (int i = 0; i < childrenEntity.size(); i++)
		{
			int childEntity = (int)childrenEntity[i];
			UIfyInt(childEntity);
		}
	}
};