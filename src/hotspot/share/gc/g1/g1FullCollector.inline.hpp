/*
 * Copyright (c) 2020, 2023, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef SHARE_GC_G1_G1FULLCOLLECTOR_INLINE_HPP
#define SHARE_GC_G1_G1FULLCOLLECTOR_INLINE_HPP

#include "gc/g1/g1FullCollector.hpp"

#include "gc/g1/g1FullGCHeapRegionAttr.hpp"
#include "gc/g1/heapRegion.inline.hpp"
#include "oops/oopsHierarchy.hpp"
#include "runtime/atomic.hpp"

bool G1FullCollector::is_compacting(oop obj) const {
  return _region_attr_table.is_compacting(cast_from_oop<HeapWord *>(obj));
}

bool G1FullCollector::is_skip_compacting(uint region_index) const {
  return _region_attr_table.is_skip_compacting(region_index);
}

bool G1FullCollector::is_skip_marking(oop obj) const {
  return _region_attr_table.is_skip_marking(cast_from_oop<HeapWord*>(obj));
}

bool G1FullCollector::is_compaction_target(uint region_index) const {
  return _region_attr_table.is_compacting(region_index) || is_free(region_index);
}

void G1FullCollector::set_free(uint region_idx) {
  _region_attr_table.set_free(region_idx);
}

bool G1FullCollector::is_free(uint region_idx) const {
  return _region_attr_table.is_free(region_idx);
}

void G1FullCollector::update_from_compacting_to_skip_compacting(uint region_idx) {
  _region_attr_table.verify_is_compacting(region_idx);
  _region_attr_table.set_skip_compacting(region_idx);
}

void G1FullCollector::update_from_skip_compacting_to_compacting(uint region_idx) {
  DEBUG_ONLY(_region_attr_table.verify_is_skip_compacting(region_idx);)
  _region_attr_table.set_compacting(region_idx);
}

void G1FullCollector::set_compaction_top(HeapRegion* r, HeapWord* value) {
  Atomic::store(&_compaction_tops[r->hrm_index()], value);
}

HeapWord* G1FullCollector::compaction_top(HeapRegion* r) const {
  return Atomic::load(&_compaction_tops[r->hrm_index()]);
}

void G1FullCollector::set_has_compaction_targets() {
  if (!_has_compaction_targets) {
    _has_compaction_targets = true;
  }
}

bool G1FullCollector::has_compaction_targets() const {
  return _has_compaction_targets;
}

void G1FullCollector::set_has_humongous() {
  if (!_has_humongous) {
    _has_humongous = true;
  }
}

bool G1FullCollector::has_humongous() {
  return _has_humongous;
}

void G1FullCollector::add_humongous_region(HeapRegion* hr) {
  _humongous_compaction_regions.append(hr);
}

GrowableArrayCHeap<HeapRegion*, mtGC>& G1FullCollector::humongous_compaction_regions() {
  return _humongous_compaction_regions;
}

#endif // SHARE_GC_G1_G1FULLCOLLECTOR_INLINE_HPP

