#pragma once

#include <string>
#include <nn/os.hpp>
#include "offsetManager.hpp"
#include "filelogger.hpp"
#include "tcplogger.hpp"

#ifdef _DEBUG_LOGGING
#define DEBUG_LOG(...) pkcl::FileLogger::getInstance()->Log(__VA_ARGS__)
#else
#define DEBUG_LOG(...) ((void)0)
#endif

namespace pkcl {
class Debug {
public:    
    
    static void Initialize()
    {
        nn::os::InitializeMutex(&mutex, false, 0);
    }

    static void Finalize()
    {
        nn::os::FinalizeMutex(&mutex);
    }

    static inline void Backtrace(int depth = 5)
    {
        stack_frame_t *fp;

        __asm volatile ("mov %0, x29" : "=r" (fp));
        nn::os::LockMutex(&mutex);
        DEBUG_LOG("Stack trace:\n");
        for(int i = 0; i < depth && fp; i++, fp = fp->fp)
            DEBUG_LOG("%d)  0x%016lX\n", i, fp->lr - pkcl::OffsetManager::getInstance()->GetBaseAddr());
        nn::os::UnlockMutex(&mutex);
    }

private:
    typedef struct stack_frame {
        struct stack_frame *fp;
        uint64_t lr;
    } stack_frame_t;

    static inline nn::os::MutexType mutex;
};
};