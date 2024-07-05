#pragma once
enum class NodeState
{
	RUNNING,
	SUCCESS,
	FAILED
};

//�p�����ē��������B
class Work
{
public:

	virtual NodeState operator()() = 0;
	Work() {};
};