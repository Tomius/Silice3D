// Copyright (c) Tamas Csala

#ifndef OPTIMIZATIONS_HPP_
#define OPTIMIZATIONS_HPP_

// ========================= Optimizations =========================

namespace Optimizations {

// CPU overhead "bugs" in original code
constexpr bool kSleepRobots = true;

// Driver overhead reduction
constexpr bool kInstanceGrouping = true;
constexpr bool kAttribModelMat = true;
constexpr bool kInvalidateBuffer = true;
constexpr bool kInstancing = true;
constexpr bool kPingPongBuffer = false; /* useless */

// GPU bottleneck fixes
constexpr bool kFrustumCulling = true;

// Not so successful optimizations
constexpr bool kDelayedModelMatrixEvalutaion = false; /* performance drop */
constexpr bool kDepthOrdering = false;                /* performance drop */
constexpr bool kInverseDepthOrdering = false;         /* useless */

// Already removed optimizations
// constexpr bool kSharedPrograms = false; /* useless */

static_assert(!kAttribModelMat || kInstanceGrouping,
              "kAttribModelMat should only be set if kInstanceGrouping is true");
static_assert(!kInstancing || kAttribModelMat,
              "kInstancing should only be set if kAttribModelMat is true");
static_assert(!kDepthOrdering || kAttribModelMat,
              "kDepthOrdering should only be set if kAttribModelMat is true");
static_assert(!kDepthOrdering || kDelayedModelMatrixEvalutaion,
              "kDepthOrdering should only be set if kDelayedModelMatrixEvalutaion is true");
static_assert(!kInverseDepthOrdering || kDepthOrdering,
              "kInverseDepthOrdering should only be set if kDepthOrdering is true");
static_assert(!kInvalidateBuffer || kAttribModelMat,
              "kInvalidateBuffer should only be set if kAttribModelMat is true");
}


#endif
