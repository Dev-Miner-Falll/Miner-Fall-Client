#pragma once

class ISaveable
{
public:
    virtual ~ISaveable() = default;

    virtual void Load() = 0;
    virtual void Save() = 0;
};
