#pragma once

#include <string>
#include "lib.hpp"
#include "../types.h"

namespace pkcl {
class FilesystemManager {
public:

    void TryMountSD()
    {
        if(!isMounted){
            isMounted = R_SUCCEEDED(nn::fs::MountSdCardForDebug("sd"));
            printf("Mounting SD card %s\n", isMounted ? "PASSED!" : "FAILED!");
        }
    }
	
    static FilesystemManager* getInstance() {
		if (instance == nullptr)
			instance = new FilesystemManager();
		return instance;
	};

	FilesystemManager(const FilesystemManager&) = delete;
	FilesystemManager& operator=(const FilesystemManager&) = delete;
	
protected:
    bool isMounted;
    
private:
	static FilesystemManager* instance;
	
	FilesystemManager() {
		//
	}
};
}