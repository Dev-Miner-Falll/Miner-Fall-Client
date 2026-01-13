#pragma once

class ILoadable
{
public:
    virtual ~ILoadable() = default;

    virtual void Load()  = 0;
};
