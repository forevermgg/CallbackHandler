#include "ThreadUtils.h"

namespace utils {

std::thread::id ThreadUtils::getThreadId() noexcept {
    return std::this_thread::get_id();
}

bool ThreadUtils::isThisThread(std::thread::id id) noexcept {
    return getThreadId() == id;
}

} // namespace utils
