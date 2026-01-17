#pragma once

#include "SoftwareInterface.h"
#include "KissDriver.h"
#include "Transport.h"

class KissInterface : public SoftwareInterface<KissDriver> {
    public:
        KissInterface(Transport *transport) : SoftwareInterface(transport) {}
}; 