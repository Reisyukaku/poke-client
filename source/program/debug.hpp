#pragma once

#include <string>
#include <nn/os.hpp>
#include "offsetManager.hpp"

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
        printf("Stack trace:\n");
        for(int i = 0; i < depth && fp; i++, fp = fp->fp)
            printf("%d)  0x%016lX\n", i, fp->lr - pkcl::OffsetManager::getInstance()->GetBaseAddr());
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