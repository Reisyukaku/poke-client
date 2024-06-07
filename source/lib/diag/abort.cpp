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

#include "abort.hpp"
#include "tcplogger.hpp"

#include <algorithm>
#include <atomic>
#include <cstring>

namespace exl::diag {

    void NORETURN NOINLINE AbortImpl(const AbortCtx & ctx) {
        #ifdef EXL_SUPPORTS_REBOOTPAYLOAD
        /* Ensure abort handler doesn't recursively abort. */
        static std::atomic<bool> recurse_guard;
        auto recursing = recurse_guard.exchange(true);

        if(!recursing && util::IsSocErista()) {
            /* Reboot to abort payload.*/
            AbortToPayload(ctx);
        } else
        #endif
        // If this ever needs to be updated to a better color:
        // https://chrisyeh96.github.io/2020/03/28/terminal-colors.html
        exl::TcpLogger::PrintString("\x1b[31m");
        exl::TcpLogger::PrintString("\n\nAssertion failed: %s = %u\n", ctx.expr, ctx.value);
        exl::TcpLogger::PrintString("At %s:%u\n", ctx.file, ctx.line);
        exl::TcpLogger::PrintString("  %s\n", ctx.func);
        if (ctx.format != nullptr) {
            exl::TcpLogger::PrintString(ctx.format, ctx.args);
            exl::TcpLogger::PrintString("\n");
        }
        exl::TcpLogger::PrintString("\x1b[0m");

        svcBreak(0x6942021, ctx.value, 0);
        UNREACHABLE;
    }

    /* TODO: better assert/abort support. */
    void NORETURN NOINLINE AssertionFailureImpl(const char *file, int line, const char *func, const char *expr, u64 value, const char *format, ...) {
        va_list list;
        va_start(list, format);
        exl::diag::AbortCtx ctx {
            .value = (Result) value,
            .file = file,
            .line = line,
            .func = func,
            .expr = expr,
            .format = format,
            .args = list
        };
        AbortImpl(ctx);
        va_end(list);
    }
    void NORETURN NOINLINE AssertionFailureImpl(const char *file, int line, const char *func, const char *expr, u64 value) {
        exl::diag::AbortCtx ctx {
            .value = value,
            .file = file,
            .line = line,
            .func = func,
            .expr = expr,
            .format = nullptr,
        };
        AbortImpl(ctx);
    }
    void NORETURN NOINLINE AbortImpl(const char *file, int line, const char *func, const char *expr, u64 value, const char *format, ...) {
        va_list list;
        va_start(list, format);
        exl::diag::AbortCtx ctx {
            .value = value,
            .file = file,
            .line = line,
            .func = func,
            .expr = expr,
            .format = format,
            .args = list
        };
        AbortImpl(ctx);
        va_end(list);
    }
    void NORETURN NOINLINE AbortImpl(const char *file, int line, const char *func, const char *expr, u64 value) {
        exl::diag::AbortCtx ctx {
            .value = value,
            .file = file,
            .line = line,
            .func = func,
            .expr = expr,
            .format = nullptr,
        };
        AbortImpl(ctx);
    }

};

/* C shim for libnx */
extern "C" NORETURN void exl_abort(Result r) {
    exl::diag::AbortCtx ctx {
        .value = r,
        .file = "Unknown",
        .line = 0,
        .func = "exl_abort",
        .expr = "Unknown cause!",
        .format = nullptr,
    };
    AbortImpl(ctx);
}