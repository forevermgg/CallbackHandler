cmake_minimum_required(VERSION 3.4.1)
message("CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
set(LEVELDB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../third_party/leveldb)
message("LEVELDB_SRC ${LEVELDB_SRC}")

include_directories(${LEVELDB_SRC}/include)
include_directories(${LEVELDB_SRC}/)
# LOCAL_C_INCLUDES := $(LDB_PREFIX)/include $(LDB_PREFIX)/leveldb $(LDB_PREFIX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_REENTRANT -DOS_ANDROID -DLEVELDB_PLATFORM_POSIX -DNDEBUG -std=c++17 -fexceptions -Wall")

set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -fsigned-char")

set(LevelDb_Library_SRCS
        ${LEVELDB_SRC}/db/builder.cc
        ${LEVELDB_SRC}/db/c.cc
        ${LEVELDB_SRC}/db/db_impl.cc
        ${LEVELDB_SRC}/db/db_iter.cc
        ${LEVELDB_SRC}/db/dbformat.cc
        ${LEVELDB_SRC}/db/dumpfile.cc
        ${LEVELDB_SRC}/db/filename.cc
        ${LEVELDB_SRC}/db/leveldbutil.cc
        ${LEVELDB_SRC}/db/log_reader.cc
        ${LEVELDB_SRC}/db/log_writer.cc
        ${LEVELDB_SRC}/db/memtable.cc
        ${LEVELDB_SRC}/db/repair.cc
        ${LEVELDB_SRC}/db/table_cache.cc
        ${LEVELDB_SRC}/db/version_edit.cc
        ${LEVELDB_SRC}/db/version_set.cc
        ${LEVELDB_SRC}/db/write_batch.cc
        ${LEVELDB_SRC}/helpers/memenv/memenv.cc
        ${LEVELDB_SRC}/table/block.cc
        ${LEVELDB_SRC}/table/block_builder.cc
        ${LEVELDB_SRC}/table/filter_block.cc
        ${LEVELDB_SRC}/table/format.cc
        ${LEVELDB_SRC}/table/iterator.cc
        ${LEVELDB_SRC}/table/merger.cc
        ${LEVELDB_SRC}/table/table.cc
        ${LEVELDB_SRC}/table/table_builder.cc
        ${LEVELDB_SRC}/table/two_level_iterator.cc
        ${LEVELDB_SRC}/util/arena.cc
        ${LEVELDB_SRC}/util/bloom.cc
        ${LEVELDB_SRC}/util/cache.cc
        ${LEVELDB_SRC}/util/coding.cc
        ${LEVELDB_SRC}/util/comparator.cc
        ${LEVELDB_SRC}/util/crc32c.cc
        ${LEVELDB_SRC}/util/env.cc
        ${LEVELDB_SRC}/util/env_posix.cc
        ${LEVELDB_SRC}/util/filter_policy.cc
        ${LEVELDB_SRC}/util/hash.cc
        ${LEVELDB_SRC}/util/histogram.cc
        ${LEVELDB_SRC}/util/logging.cc
        ${LEVELDB_SRC}/util/options.cc
        ${LEVELDB_SRC}/util/status.cc
        )

add_library( # Sets the name of the library.
        leveldb

        ${LevelDb_Library_SRCS}
        )