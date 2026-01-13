#pragma once

class IOpenableUI
{
public:
    virtual ~IOpenableUI() = default;

    virtual void OpenUI()  = 0;
    virtual void CloseUI() = 0;
};
