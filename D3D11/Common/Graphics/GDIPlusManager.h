#pragma once

#include "Utility/D3D11Win.h"

class GDIPlusManager
{
public:
    GDIPlusManager();
    ~GDIPlusManager();

private:
    static ULONG_PTR token;
    static int refCount;
};