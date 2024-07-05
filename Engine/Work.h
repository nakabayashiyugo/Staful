#pragma once
enum class NodeState
{
	RUNNING,
	SUCCESS,
	FAILED
};

//Œp³‚µ‚Ä“­‚«‚ğì‚éB
class Work
{
public:

	virtual NodeState operator()() = 0;
	Work() {};
};