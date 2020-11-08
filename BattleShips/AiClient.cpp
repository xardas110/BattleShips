#include "AiClient.h"
#include "NetworkEvents.h"

void AiClient::RequestShoot()
{
    if (GetNumShots() > 0)
    {
        iVec2D pos;
        pos.x = std::rand() % numCol;
        pos.y = std::rand() % numRow;
        Json response(Json::Object);
        response.Set("Events", Json::Array);
        response.Set("Status", S_OK);
        response["Events"].Add(EVENT_SHOOT);
        response.Set("Event_Shoot", Json::Object);
        response["Event_Shoot"].Set("Pos", Convert2Dto1D(pos));
        client.Send(response.Stringify().c_str());
    }
}
