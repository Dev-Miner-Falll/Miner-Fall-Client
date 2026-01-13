#pragma once

#include "IBackgroundDataService.h"

namespace Service
{
    class LocalBackgroundDataService : public IBackgroundDataService
    {
    public:
        const std::vector<BackgroundData>& GetBackgroundDatas() const override;

    public:
        void Load() override;

    public:
        LocalBackgroundDataService();

        LocalBackgroundDataService(const LocalBackgroundDataService&)            = delete;
        LocalBackgroundDataService& operator=(const LocalBackgroundDataService&) = delete;

    private:
        std::vector<BackgroundData> m_data_vector;
    };
}
