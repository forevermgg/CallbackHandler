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
