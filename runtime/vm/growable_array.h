// Copyright (c) 2012, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
// Defines growable array classes, that differ where they are allocated:
// - GrowableArray: allocated on stack.
// - ZoneGrowableArray: allocated in the zone.
// - MallocGrowableArray: allocates using malloc/realloc; free is only called
//   at destruction.

#ifndef RUNTIME_VM_GROWABLE_ARRAY_H_
#define RUNTIME_VM_GROWABLE_ARRAY_H_

#include "platform/growable_array.h"
#include "vm/thread_state.h"
#include "vm/zone.h"

namespace dart {

template <typename T>
class GrowableArray : public BaseGrowableArray<T, ValueObject, Zone> {
 public:
  GrowableArray(Zone* zone, intptr_t initial_capacity)
      : BaseGrowableArray<T, ValueObject, Zone>(initial_capacity,
                                                ASSERT_NOTNULL(zone)) {}
  explicit GrowableArray(intptr_t initial_capacity)
      : BaseGrowableArray<T, ValueObject, Zone>(
            initial_capacity,
            ASSERT_NOTNULL(ThreadState::Current()->zone())) {}
  GrowableArray()
      : BaseGrowableArray<T, ValueObject, Zone>(
            ASSERT_NOTNULL(ThreadState::Current()->zone())) {}

  GrowableArray(std::initializer_list<T> values)
      : BaseGrowableArray<T, ValueObject, Zone>(
            values.size(),
            ASSERT_NOTNULL(ThreadState::Current()->zone())) {
    for (auto& value : values) {
      this->Add(value);
    }
  }

  GrowableArray(GrowableArray&& other) = default;
  GrowableArray& operator=(GrowableArray&& other) = default;
  ~GrowableArray() = default;
  DISALLOW_COPY_AND_ASSIGN(GrowableArray);
};

template <typename T>
class ZoneGrowableArray : public BaseGrowableArray<T, ZoneAllocated, Zone> {
 public:
  ZoneGrowableArray(Zone* zone, intptr_t initial_capacity)
      : BaseGrowableArray<T, ZoneAllocated, Zone>(initial_capacity,
                                                  ASSERT_NOTNULL(zone)) {}
  explicit ZoneGrowableArray(intptr_t initial_capacity)
      : BaseGrowableArray<T, ZoneAllocated, Zone>(
            initial_capacity,
            ASSERT_NOTNULL(ThreadState::Current()->zone())) {}
  ZoneGrowableArray()
      : BaseGrowableArray<T, ZoneAllocated, Zone>(
            ASSERT_NOTNULL(ThreadState::Current()->zone())) {}

  ZoneGrowableArray(ZoneGrowableArray&& other) = default;
  ZoneGrowableArray& operator=(ZoneGrowableArray&& other) = default;
  ~ZoneGrowableArray() = default;
  DISALLOW_COPY_AND_ASSIGN(ZoneGrowableArray);
};

// T must be a Handle type.
template <typename T, typename B>
class BaseGrowableHandlePtrArray : public B {
 public:
  BaseGrowableHandlePtrArray(Zone* zone, intptr_t initial_capacity)
      : zone_(zone), array_(zone, initial_capacity) {}

  // Use unique zone handles to store objects.
  void Add(const T& t) { array_.Add(&T::ZoneHandle(zone_, t.ptr())); }

  T& operator[](intptr_t index) const { return *array_[index]; }

  const T& At(intptr_t index) const { return operator[](index); }

  void SetAt(intptr_t index, const T& t) {
    array_[index] = &T::ZoneHandle(zone_, t.ptr());
  }

  intptr_t length() const { return array_.length(); }

  void Clear() { array_.Clear(); }

  const GrowableArray<T*>& growable_array() const { return array_; }

 private:
  Zone* zone_;
  GrowableArray<T*> array_;

  DISALLOW_COPY_AND_ASSIGN(BaseGrowableHandlePtrArray);
};

template <typename T>
class GrowableHandlePtrArray
    : public BaseGrowableHandlePtrArray<T, ValueObject> {
 public:
  GrowableHandlePtrArray(Zone* zone, intptr_t initial_capacity)
      : BaseGrowableHandlePtrArray<T, ValueObject>(zone, initial_capacity) {}
};

template <typename T>
class ZoneGrowableHandlePtrArray
    : public BaseGrowableHandlePtrArray<T, ZoneAllocated> {
 public:
  ZoneGrowableHandlePtrArray(Zone* zone, intptr_t initial_capacity)
      : BaseGrowableHandlePtrArray<T, ZoneAllocated>(zone, initial_capacity) {}
};

}  // namespace dart

#endif  // RUNTIME_VM_GROWABLE_ARRAY_H_
