/*
 * Copyright (c) Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "types.h"

namespace nn::mem {

    class StandardAllocator {
        public:
            StandardAllocator();

            void Initialize(void *address, u64 size);

            void Finalize();

            void *Reallocate(void *address, u64 newSize);

            void *Allocate(u64 size);

            void Free(void *address);

            u64 GetSizeOf(const void *) const;

            u64 GetTotalFreeSize(void) const;

            void Dump();

            bool mIsInitialized;         // _0
            bool mIsEnabledThreadCache;  // _1
            u16 _2;
            u64 *mAllocAddr;  // _4
    };
}

namespace nn::init {

    void InitializeAllocator(void *address, size_t size, bool cache_enabled);
    void InitializeAllocator(void *address, size_t size);

    mem::StandardAllocator *GetAllocator();

}