#pragma once
#include "PlayerClient.h"
class AiClient : public PlayerClient
{
public:
	virtual void RequestShoot() override;
};

