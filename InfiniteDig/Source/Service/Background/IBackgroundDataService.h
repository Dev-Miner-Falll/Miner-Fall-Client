#pragma once

#include <vector>
#include "Service/ILoadable.h"
#include "Data/Background/BackgroundData.h"

namespace Service
{
    class IBackgroundDataService : public ILoadable
    {
    public:
        virtual ~IBackgroundDataService()                                     = default;

        virtual const std::vector<BackgroundData>& GetBackgroundDatas() const = 0;
    };
}
