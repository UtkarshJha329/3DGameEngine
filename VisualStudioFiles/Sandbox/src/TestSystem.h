#pragma once


#include "SystemsManager.h"

class TestComp : public Component<TestComp> {

public:

	float tstFloat = 0;

	int x1 = -1;
	int x2 = -1;
	int x3 = -1;
	
	//int a[100];
	//
	//float ctstFloat = 0;
	
	//int xc1 = -1;
	//int xc2 = -1;
	//int xc3 = -1;


	//bool a = false;
	//comment
	void SerializeComponent() override;
	void DeSerializeComponent() override;
	void UIfyComponent() override;
};


class TestSystem : public System {

public:

	TestSystem();

	void UnorderedInit() override {}
	void PreAwake() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;

};

