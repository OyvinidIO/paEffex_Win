#pragma once
#include<string>

using namespace std; 




class MsgFromUser 
{

private:
	struct MsgHeader_ {
		string EffectName;
		bool enable;
	};

	static const int MsgBodySize_ = 10; // static const? 
	struct MsgBody_ {
		double params[MsgBodySize_];
	};

public:
	MsgHeader_ header_;
	MsgBody_ body_;

	MsgFromUser();
	~MsgFromUser();
	void ResetMsg(); // Sets all member variables to zero, false or empty string

};