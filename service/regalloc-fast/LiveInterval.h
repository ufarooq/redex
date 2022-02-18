/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "IRInstruction.h"
#include "LinearScan.h"
#include "Liveness.h"
#include <unordered_map>
#include <utility>
#include <vector>

namespace fastregalloc {

using LiveIntervalPointIndices =
    std::unordered_map<LiveIntervalPoint, uint32_t, LiveIntervalPoint::Hasher>;

using RangeInBlock = std::pair<LiveIntervalPoint, LiveIntervalPoint>;
using VRegAliveRangeInBlock = std::unordered_map<vreg_t, RangeInBlock>;
using VRegAliveInsns = std::unordered_map<vreg_t, std::vector<RangeInBlock>>;

using IntervalEndPoints = std::pair<uint32_t, uint32_t>;
using VRegBlockRanges = std::vector<IntervalEndPoints>;

/*
 * All vregs in the live-in set for a basic block starting at instruction i have
 * a live interval that includes i. All vregs in the live-out set for a basic
 * block ending at instruction j have a live interval that includes j.
 * If a vreg occurs in a basic block:
 * If it's not in the live-in set, its live interval needs to be extended to its
 * first Def in this block. If it's not in the live-out set, its live interval
 * needs to be extended to its last Def/Use in the block. If it's neither in
 * live-in nor in live-out, then a new interval is added from first Def to last
 * Use of this vreg within the basic block.
 */
VRegAliveRangeInBlock get_live_range_in_block(
    const LivenessFixpointIterator& fixpoint_iter, cfg::Block* block);

/*
 * Order the instruction list. Then for each vreg, turn each instruction range
 * into idx range, and study the smallest connected range that covers all
 * ranges, which is the live interval of this vreg. Put live interval and vreg
 * info into the result vector and sort before return.
 */
LiveIntervals init_live_intervals(
    IRCode* code, std::vector<LiveIntervalPoint>* live_interval_points);

IntervalEndPoints calculate_live_interval(
    std::vector<RangeInBlock>& ranges, const LiveIntervalPointIndices& indices);

} // namespace fastregalloc
