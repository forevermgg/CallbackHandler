#include "Test.h"

#include "logging.h"

Test::Test() = default;

Test::~Test() noexcept = default;

Test::PickingQuery &Test::Consumer(CallbackHandler *handler,
                                   Test::PickingQueryResultCallback callback) {
  FPickingQuery *pQuery = FPickingQuery::get(handler, callback);
  pQuery->next = mActivePickingQueriesList;
  mActivePickingQueriesList = pQuery;
  return *pQuery;
}

void Test::Producer() {
  while (mActivePickingQueriesList) {
    FOREVER_LOG(ERROR) << "while (mActivePickingQueriesList)";
    FPickingQuery *const pQuery = mActivePickingQueriesList;
    mActivePickingQueriesList = pQuery->next;
    pQuery->callback(pQuery->result, pQuery);
    FPickingQuery::put(pQuery);
  }
}

void Test::scheduleCallback(CallbackHandler *handler, void *user,
                            CallbackHandler::Callback callback) {
  /*emplace_back()和push_back()的区别
  emplace_back() 和 push_back() 的区别，就在于底层实现的机制不同。push_back()
  向容器尾部添加元素时，
  首先会创建这个元素，然后再将这个元素拷贝或者移动到容器中（如果是拷贝的话，事后会自行销毁先前创建的这个元素）；
  而 emplace_back()
  在实现时，则是直接在容器尾部创建这个元素，省去了拷贝或移动元素的过程。*/
  if (handler) {
    mServiceThreadCallbackQueue.emplace_back(handler, callback, user);
    auto &serviceThreadCallbackQueue = mServiceThreadCallbackQueue;
    auto callbacks(std::move(serviceThreadCallbackQueue));
    // and make sure to call them without our lock held
    for (auto [handler, callback, user] : callbacks) {
      handler->post(user, callback);
    }
  } else {
    mCallbacks.emplace_back(user, callback);
    decltype(mCallbacks) callbacks;
    std::swap(callbacks, mCallbacks);
    for (auto &item : callbacks) {
      item.second(item.first);
    }
  }
}

void Test::setCompletedCallback(CallbackHandler *handler,
                                FOREVER::Invocable<void(Test *)> &&callback) {
  struct Callback {
    FOREVER::Invocable<void(Test *)> f;
    Test *s;
    static void func(void *user) {
      auto *const c = reinterpret_cast<Callback *>(user);
      c->f(c->s);
      delete c;
    }
  };
  auto *const user = new (std::nothrow) Callback{std::move(callback), this};
  completed.handler = handler;
  completed.callback = &Callback::func;
  completed.user = static_cast<void *>(user);
  if (completed.callback) {
    CallbackHandler *handler = completed.handler;
    void *user = completed.user;
    CallbackHandler::Callback callback = completed.callback;
    scheduleCallback(handler, user, callback);
  }
}

// This is called from an async driver method so it's in the GL thread, but purge is called
// on the user thread. This is typically called 0 or 1 times per frame.
void Test::scheduleRelease(AcquiredImage const& image) noexcept {
  /*scheduleCallback(image.handler, [image]() {
      image.callback(image.image, image.userData);
  });*/
}

void Test::setAcquiredImage(void* image, CallbackHandler* handler, Callback callback, void* userdata) {
  AcquiredImage acquiredImage = {image, callback, userdata, handler};
  scheduleRelease(acquiredImage);
}
