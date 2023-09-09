#ifndef CALLBACK_HANDLER_TEST_H
#define CALLBACK_HANDLER_TEST_H
#include <string>
#include <vector>

#include "CallbackHandler.h"
#include "Handle.h"
#include "Invocable.h"
#include "logging.h"

class Test {
 public:
  explicit Test();
  ~Test() noexcept;
  /** Result of a picking query */
  struct PickingQueryResult {
    int code;
    std::string msg = "";
  };
  /** User data for PickingQueryResultCallback */
  struct PickingQuery {
    // note: this is enough to store a std::function<> -- just saying...
    void* storage[4];
  };
  using PickingQueryResultCallback = void (*)(PickingQueryResult const& result,
                                              PickingQuery* pq);
  struct FPickingQuery : public PickingQuery {
   private:
    FPickingQuery(CallbackHandler* handler,
                  PickingQueryResultCallback callback) noexcept
        : PickingQuery{}, handler(handler), callback(callback) {
      FOREVER_LOG(ERROR) << "FPickingQuery()";
    }
    ~FPickingQuery() { FOREVER_LOG(ERROR) << "~FPickingQuery()"; }

   public:
    static FPickingQuery* get(CallbackHandler* handler,
                              PickingQueryResultCallback callback) noexcept {
      return new FPickingQuery(handler, callback);
    }
    static void put(FPickingQuery* pQuery) noexcept { delete pQuery; }
    mutable FPickingQuery* next = nullptr;
    CallbackHandler* const handler;
    PickingQueryResultCallback const callback;
    PickingQueryResult result;
  };

  struct {
    CallbackHandler* handler = nullptr;
    CallbackHandler::Callback callback = {};
    void* user = nullptr;
  } completed;

  template <typename T, void (T::*method)(PickingQueryResult const&)>
  void Consumer(T* instance, CallbackHandler* handler = nullptr) noexcept {
    PickingQuery& query = Consumer(
        [](PickingQueryResult const& result, PickingQuery* pq) {
          void* user = pq->storage;
          (*static_cast<T**>(user)->*method)(result);
        },
        handler);
    query.storage[0] = instance;
  }

  template <typename T, void (T::*method)(PickingQueryResult const&)>
  void Consumer(T instance, CallbackHandler* handler = nullptr) noexcept {
    static_assert(sizeof(instance) <= sizeof(PickingQuery::storage),
                  "user data too large");
    PickingQuery& query = Consumer(
        [](PickingQueryResult const& result, PickingQuery* pq) {
          void* user = pq->storage;
          T* that = static_cast<T*>(user);
          (that->*method)(result);
          that->~T();
        },
        handler);
    new (query.storage) T(std::move(instance));
  }

  template <typename T>
  void Consumer(T functor, CallbackHandler* handler = nullptr) noexcept {
    static_assert(sizeof(functor) <= sizeof(PickingQuery::storage),
                  "functor too large");
    PickingQuery& query = Consumer(
        handler, (PickingQueryResultCallback)[](
                     PickingQueryResult const& result, PickingQuery* pq) {
          void* user = pq->storage;
          T& that = *static_cast<T*>(user);
          that(result);
          that.~T();
        });
    new (query.storage) T(std::move(functor));
  }

  void Producer();

  PickingQuery& Consumer(CallbackHandler* handler,
                         PickingQueryResultCallback callback);

  bool hasPicking() const noexcept {
    return mActivePickingQueriesList != nullptr;
  }

  struct CallbackData {
    CallbackData(CallbackData const&) = delete;
    CallbackData(CallbackData&&) = delete;
    CallbackData& operator=(CallbackData const&) = delete;
    CallbackData& operator=(CallbackData&&) = delete;
    void* storage[8] = {};
    static CallbackData* obtain(Test* allocator);
    static void release(CallbackData* data);

   protected:
    CallbackData() = default;
  };

  template <typename T>
  void scheduleCallback(CallbackHandler* handler, T&& functor) {
    CallbackData* data = CallbackData::obtain(this);
    static_assert(sizeof(T) <= sizeof(data->storage), "functor too large");
    new (data->storage) T(std::forward<T>(functor));
    scheduleCallback(
        handler, data, (CallbackHandler::Callback)[](void* data) {
          CallbackData* details = static_cast<CallbackData*>(data);
          void* user = details->storage;
          T& functor = *static_cast<T*>(user);
          functor();
          functor.~T();
          CallbackData::release(details);
        });
  }

  void scheduleCallback(CallbackHandler* handler, void* user,
                        CallbackHandler::Callback callback);

  void setCompletedCallback(CallbackHandler* handler,
                            FOREVER::Invocable<void(Test*)>&& callback);

  FPickingQuery* mActivePickingQueriesList = nullptr;
  std::vector<std::pair<void*, CallbackHandler::Callback>> mCallbacks;
  std::vector<std::tuple<CallbackHandler*, CallbackHandler::Callback, void*>>
      mServiceThreadCallbackQueue;
};

#endif  // CALLBACK_HANDLER_TEST_H
