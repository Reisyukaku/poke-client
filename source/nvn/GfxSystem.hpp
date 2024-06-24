#pragma once

#include "nvn.h"
#include "nvn_Cpp.h"

namespace pkcl {
class NvnInfo {
public:
    int versionMin;
    int versionMaj;
    int supportDbg;
    int supportPassGeoShdr;
    int supportFragInter;
    int supportAdvBlend;
};

class GfxSystem {
public:

    nvn::Device *GetDevice()
    {
        return nvnDevice;
    }

    void SetDevice(nvn::Device *dev)
    {
        nvnDevice = dev;
    }
	
    static GfxSystem* getInstance() {
		if (instance == nullptr)
			instance = new GfxSystem();
		return instance;
	};

	GfxSystem(const GfxSystem&) = delete;
	GfxSystem& operator=(const GfxSystem&) = delete;
	
protected:
    nvn::Device *nvnDevice;
    
private:
	static GfxSystem* instance;
	
	GfxSystem() {
		//
	}
};
}