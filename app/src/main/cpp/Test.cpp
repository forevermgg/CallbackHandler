#include "Test.h"

Test::Test() = default;

Test::~Test() noexcept = default;

Test::PickingQuery &Test::Consumer(CallbackHandler *handler,
                                   Test::PickingQueryResultCallback callback) {
  FPickingQuery *pQuery = FPickingQuery::get(handler, callback);
  mActivePickingQuery = pQuery;
  return *pQuery;
}

void Test::Producer() {
  if (mActivePickingQuery) {
    mActivePickingQuery->callback(mActivePickingQuery->result,
                                  mActivePickingQuery);
  }
}
