#include "MsgStruct.h"

MsgFromUser::MsgFromUser()
{
	header_.EffectName = "";
	header_.enable = false;
	for (int i = 0; i < MsgBodySize_; i++)
	{
		body_.params[i] = 0.0;
	}
}

MsgFromUser::~MsgFromUser()
{
}

void MsgFromUser::ResetMsg()
{
	header_.EffectName = "";
	header_.enable = false;
	for (int i = 0; i < MsgBodySize_; i++)
	{
		body_.params[i] = 0.0;
	}

}
