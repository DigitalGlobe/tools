// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_COMPILER_MACHINE_OPERATOR_H_
#define V8_COMPILER_MACHINE_OPERATOR_H_

#include "src/base/flags.h"
#include "src/machine-type.h"

namespace v8 {
namespace internal {
namespace compiler {

// Forward declarations.
struct MachineOperatorGlobalCache;
class Operator;


// For operators that are not supported on all platforms.
class OptionalOperator final {
 public:
  explicit OptionalOperator(const Operator* op) : op_(op) {}

  bool IsSupported() const { return op_ != nullptr; }
  const Operator* op() const {
    DCHECK_NOT_NULL(op_);
    return op_;
  }

 private:
  const Operator* const op_;
};


// A Load needs a MachineType.
typedef MachineType LoadRepresentation;

LoadRepresentation LoadRepresentationOf(Operator const*);

// A Store needs a MachineType and a WriteBarrierKind in order to emit the
// correct write barrier.
class StoreRepresentation final {
 public:
  StoreRepresentation(MachineRepresentation representation,
                      WriteBarrierKind write_barrier_kind)
      : representation_(representation),
        write_barrier_kind_(write_barrier_kind) {}

  MachineRepresentation representation() const { return representation_; }
  WriteBarrierKind write_barrier_kind() const { return write_barrier_kind_; }

 private:
  MachineRepresentation representation_;
  WriteBarrierKind write_barrier_kind_;
};

bool operator==(StoreRepresentation, StoreRepresentation);
bool operator!=(StoreRepresentation, StoreRepresentation);

size_t hash_value(StoreRepresentation);

std::ostream& operator<<(std::ostream&, StoreRepresentation);

StoreRepresentation const& StoreRepresentationOf(Operator const*);


// A CheckedLoad needs a MachineType.
typedef MachineType CheckedLoadRepresentation;

CheckedLoadRepresentation CheckedLoadRepresentationOf(Operator const*);


// A CheckedStore needs a MachineType.
typedef MachineRepresentation CheckedStoreRepresentation;

CheckedStoreRepresentation CheckedStoreRepresentationOf(Operator const*);

MachineRepresentation StackSlotRepresentationOf(Operator const* op);

MachineRepresentation AtomicStoreRepresentationOf(Operator const* op);

// Interface for building machine-level operators. These operators are
// machine-level but machine-independent and thus define a language suitable
// for generating code to run on architectures such as ia32, x64, arm, etc.
class MachineOperatorBuilder final : public ZoneObject {
 public:
  // Flags that specify which operations are available. This is useful
  // for operations that are unsupported by some back-ends.
  enum Flag {
    kNoFlags = 0u,
    // Note that Float*Max behaves like `(b < a) ? a : b`, not like Math.max().
    // Note that Float*Min behaves like `(a < b) ? a : b`, not like Math.min().
    kFloat32Max = 1u << 0,
    kFloat32Min = 1u << 1,
    kFloat64Max = 1u << 2,
    kFloat64Min = 1u << 3,
    kFloat32RoundDown = 1u << 4,
    kFloat64RoundDown = 1u << 5,
    kFloat32RoundUp = 1u << 6,
    kFloat64RoundUp = 1u << 7,
    kFloat32RoundTruncate = 1u << 8,
    kFloat64RoundTruncate = 1u << 9,
    kFloat32RoundTiesEven = 1u << 10,
    kFloat64RoundTiesEven = 1u << 11,
    kFloat64RoundTiesAway = 1u << 12,
    kInt32DivIsSafe = 1u << 13,
    kUint32DivIsSafe = 1u << 14,
    kWord32ShiftIsSafe = 1u << 15,
    kWord32Ctz = 1u << 16,
    kWord64Ctz = 1u << 17,
    kWord32Popcnt = 1u << 18,
    kWord64Popcnt = 1u << 19,
    kWord32ReverseBits = 1u << 20,
    kWord64ReverseBits = 1u << 21,
    kFloat32Neg = 1u << 22,
    kFloat64Neg = 1u << 23,
    kAllOptionalOps =
        kFloat32Max | kFloat32Min | kFloat64Max | kFloat64Min |
        kFloat32RoundDown | kFloat64RoundDown | kFloat32RoundUp |
        kFloat64RoundUp | kFloat32RoundTruncate | kFloat64RoundTruncate |
        kFloat64RoundTiesAway | kFloat32RoundTiesEven | kFloat64RoundTiesEven |
        kWord32Ctz | kWord64Ctz | kWord32Popcnt | kWord64Popcnt |
        kWord32ReverseBits | kWord64ReverseBits | kFloat32Neg | kFloat64Neg
  };
  typedef base::Flags<Flag, unsigned> Flags;

  class AlignmentRequirements {
   public:
    enum UnalignedAccessSupport { kNoSupport, kSomeSupport, kFullSupport };

    bool IsUnalignedLoadSupported(const MachineType& machineType,
                                  uint8_t alignment) const {
      return IsUnalignedSupported(unalignedLoadSupportedTypes_, machineType,
                                  alignment);
    }

    bool IsUnalignedStoreSupported(const MachineType& machineType,
                                   uint8_t alignment) const {
      return IsUnalignedSupported(unalignedStoreSupportedTypes_, machineType,
                                  alignment);
    }

    static AlignmentRequirements FullUnalignedAccessSupport() {
      return AlignmentRequirements(kFullSupport);
    }
    static AlignmentRequirements NoUnalignedAccessSupport() {
      return AlignmentRequirements(kNoSupport);
    }
    static AlignmentRequirements SomeUnalignedAccessSupport(
        const Vector<MachineType>& unalignedLoadSupportedTypes,
        const Vector<MachineType>& unalignedStoreSupportedTypes) {
      return AlignmentRequirements(kSomeSupport, unalignedLoadSupportedTypes,
                                   unalignedStoreSupportedTypes);
    }

   private:
    explicit AlignmentRequirements(
        AlignmentRequirements::UnalignedAccessSupport unalignedAccessSupport,
        Vector<MachineType> unalignedLoadSupportedTypes =
            Vector<MachineType>(NULL, 0),
        Vector<MachineType> unalignedStoreSupportedTypes =
            Vector<MachineType>(NULL, 0))
        : unalignedSupport_(unalignedAccessSupport),
          unalignedLoadSupportedTypes_(unalignedLoadSupportedTypes),
          unalignedStoreSupportedTypes_(unalignedStoreSupportedTypes) {}

    bool IsUnalignedSupported(const Vector<MachineType>& supported,
                              const MachineType& machineType,
                              uint8_t alignment) const {
      if (unalignedSupport_ == kFullSupport) {
        return true;
      } else if (unalignedSupport_ == kNoSupport) {
        return false;
      } else {
        for (MachineType m : supported) {
          if (m == machineType) {
            return true;
          }
        }
        return false;
      }
    }

    const AlignmentRequirements::UnalignedAccessSupport unalignedSupport_;
    const Vector<MachineType> unalignedLoadSupportedTypes_;
    const Vector<MachineType> unalignedStoreSupportedTypes_;
  };

  explicit MachineOperatorBuilder(
      Zone* zone,
      MachineRepresentation word = MachineType::PointerRepresentation(),
      Flags supportedOperators = kNoFlags,
      AlignmentRequirements alignmentRequirements =
          AlignmentRequirements::NoUnalignedAccessSupport());

  const Operator* Comment(const char* msg);
  const Operator* DebugBreak();

  const Operator* Word32And();
  const Operator* Word32Or();
  const Operator* Word32Xor();
  const Operator* Word32Shl();
  const Operator* Word32Shr();
  const Operator* Word32Sar();
  const Operator* Word32Ror();
  const Operator* Word32Equal();
  const Operator* Word32Clz();
  const OptionalOperator Word32Ctz();
  const OptionalOperator Word32Popcnt();
  const OptionalOperator Word64Popcnt();
  const Operator* Word64PopcntPlaceholder();
  const OptionalOperator Word32ReverseBits();
  const OptionalOperator Word64ReverseBits();
  bool Word32ShiftIsSafe() const { return flags_ & kWord32ShiftIsSafe; }

  const Operator* Word64And();
  const Operator* Word64Or();
  const Operator* Word64Xor();
  const Operator* Word64Shl();
  const Operator* Word64Shr();
  const Operator* Word64Sar();
  const Operator* Word64Ror();
  const Operator* Word64Clz();
  const OptionalOperator Word64Ctz();
  const Operator* Word64CtzPlaceholder();
  const Operator* Word64Equal();

  const Operator* Int32PairAdd();
  const Operator* Int32PairSub();
  const Operator* Int32PairMul();
  const Operator* Word32PairShl();
  const Operator* Word32PairShr();
  const Operator* Word32PairSar();

  const Operator* Int32Add();
  const Operator* Int32AddWithOverflow();
  const Operator* Int32Sub();
  const Operator* Int32SubWithOverflow();
  const Operator* Int32Mul();
  const Operator* Int32MulHigh();
  const Operator* Int32Div();
  const Operator* Int32Mod();
  const Operator* Int32LessThan();
  const Operator* Int32LessThanOrEqual();
  const Operator* Uint32Div();
  const Operator* Uint32LessThan();
  const Operator* Uint32LessThanOrEqual();
  const Operator* Uint32Mod();
  const Operator* Uint32MulHigh();
  bool Int32DivIsSafe() const { return flags_ & kInt32DivIsSafe; }
  bool Uint32DivIsSafe() const { return flags_ & kUint32DivIsSafe; }

  const Operator* Int64Add();
  const Operator* Int64AddWithOverflow();
  const Operator* Int64Sub();
  const Operator* Int64SubWithOverflow();
  const Operator* Int64Mul();
  const Operator* Int64Div();
  const Operator* Int64Mod();
  const Operator* Int64LessThan();
  const Operator* Int64LessThanOrEqual();
  const Operator* Uint64Div();
  const Operator* Uint64LessThan();
  const Operator* Uint64LessThanOrEqual();
  const Operator* Uint64Mod();

  // This operator reinterprets the bits of a word as tagged pointer.
  const Operator* BitcastWordToTagged();

  // JavaScript float64 to int32/uint32 truncation.
  const Operator* TruncateFloat64ToWord32();

  // These operators change the representation of numbers while preserving the
  // value of the number. Narrowing operators assume the input is representable
  // in the target type and are *not* defined for other inputs.
  // Use narrowing change operators only when there is a static guarantee that
  // the input value is representable in the target value.
  const Operator* ChangeFloat32ToFloat64();
  const Operator* ChangeFloat64ToInt32();   // narrowing
  const Operator* ChangeFloat64ToUint32();  // narrowing
  const Operator* TruncateFloat64ToUint32();
  const Operator* TruncateFloat32ToInt32();
  const Operator* TruncateFloat32ToUint32();
  const Operator* TryTruncateFloat32ToInt64();
  const Operator* TryTruncateFloat64ToInt64();
  const Operator* TryTruncateFloat32ToUint64();
  const Operator* TryTruncateFloat64ToUint64();
  const Operator* ChangeInt32ToFloat64();
  const Operator* ChangeInt32ToInt64();
  const Operator* ChangeUint32ToFloat64();
  const Operator* ChangeUint32ToUint64();

  // These operators truncate or round numbers, both changing the representation
  // of the number and mapping multiple input values onto the same output value.
  const Operator* TruncateFloat64ToFloat32();
  const Operator* TruncateInt64ToInt32();
  const Operator* RoundFloat64ToInt32();
  const Operator* RoundInt32ToFloat32();
  const Operator* RoundInt64ToFloat32();
  const Operator* RoundInt64ToFloat64();
  const Operator* RoundUint32ToFloat32();
  const Operator* RoundUint64ToFloat32();
  const Operator* RoundUint64ToFloat64();

  // These operators reinterpret the bits of a floating point number as an
  // integer and vice versa.
  const Operator* BitcastFloat32ToInt32();
  const Operator* BitcastFloat64ToInt64();
  const Operator* BitcastInt32ToFloat32();
  const Operator* BitcastInt64ToFloat64();

  // Floating point operators always operate with IEEE 754 round-to-nearest
  // (single-precision).
  const Operator* Float32Add();
  const Operator* Float32Sub();
  const Operator* Float32SubPreserveNan();
  const Operator* Float32Mul();
  const Operator* Float32Div();
  const Operator* Float32Sqrt();

  // Floating point operators always operate with IEEE 754 round-to-nearest
  // (double-precision).
  const Operator* Float64Add();
  const Operator* Float64Sub();
  const Operator* Float64SubPreserveNan();
  const Operator* Float64Mul();
  const Operator* Float64Div();
  const Operator* Float64Mod();
  const Operator* Float64Sqrt();

  // Floating point comparisons complying to IEEE 754 (single-precision).
  const Operator* Float32Equal();
  const Operator* Float32LessThan();
  const Operator* Float32LessThanOrEqual();

  // Floating point comparisons complying to IEEE 754 (double-precision).
  const Operator* Float64Equal();
  const Operator* Float64LessThan();
  const Operator* Float64LessThanOrEqual();

  // Floating point min/max complying to IEEE 754 (single-precision).
  const OptionalOperator Float32Max();
  const OptionalOperator Float32Min();

  // Floating point min/max complying to IEEE 754 (double-precision).
  const OptionalOperator Float64Max();
  const OptionalOperator Float64Min();

  // Floating point abs complying to IEEE 754 (single-precision).
  const Operator* Float32Abs();

  // Floating point abs complying to IEEE 754 (double-precision).
  const Operator* Float64Abs();

  // Floating point rounding.
  const OptionalOperator Float32RoundDown();
  const OptionalOperator Float64RoundDown();
  const OptionalOperator Float32RoundUp();
  const OptionalOperator Float64RoundUp();
  const OptionalOperator Float32RoundTruncate();
  const OptionalOperator Float64RoundTruncate();
  const OptionalOperator Float64RoundTiesAway();
  const OptionalOperator Float32RoundTiesEven();
  const OptionalOperator Float64RoundTiesEven();

  // Floating point neg.
  const OptionalOperator Float32Neg();
  const OptionalOperator Float64Neg();

  // Floating point trigonometric functions (double-precision).
  const Operator* Float64Atan();
  const Operator* Float64Atan2();
  const Operator* Float64Atanh();

  // Floating point trigonometric functions (double-precision).
  const Operator* Float64Cos();
  const Operator* Float64Sin();
  const Operator* Float64Tan();

  // Floating point exponential functions (double-precision).
  const Operator* Float64Exp();

  // Floating point logarithm (double-precision).
  const Operator* Float64Log();
  const Operator* Float64Log1p();
  const Operator* Float64Log2();
  const Operator* Float64Log10();

  const Operator* Float64Cbrt();
  const Operator* Float64Expm1();

  // Floating point bit representation.
  const Operator* Float64ExtractLowWord32();
  const Operator* Float64ExtractHighWord32();
  const Operator* Float64InsertLowWord32();
  const Operator* Float64InsertHighWord32();

  // Change signalling NaN to quiet NaN.
  // Identity for any input that is not signalling NaN.
  const Operator* Float64SilenceNaN();

  // SIMD operators.
  const Operator* CreateFloat32x4();
  const Operator* Float32x4ExtractLane();
  const Operator* Float32x4ReplaceLane();
  const Operator* Float32x4Abs();
  const Operator* Float32x4Neg();
  const Operator* Float32x4Sqrt();
  const Operator* Float32x4RecipApprox();
  const Operator* Float32x4RecipSqrtApprox();
  const Operator* Float32x4Add();
  const Operator* Float32x4Sub();
  const Operator* Float32x4Mul();
  const Operator* Float32x4Div();
  const Operator* Float32x4Min();
  const Operator* Float32x4Max();
  const Operator* Float32x4MinNum();
  const Operator* Float32x4MaxNum();
  const Operator* Float32x4Equal();
  const Operator* Float32x4NotEqual();
  const Operator* Float32x4LessThan();
  const Operator* Float32x4LessThanOrEqual();
  const Operator* Float32x4GreaterThan();
  const Operator* Float32x4GreaterThanOrEqual();
  const Operator* Float32x4Select();
  const Operator* Float32x4Swizzle();
  const Operator* Float32x4Shuffle();
  const Operator* Float32x4FromInt32x4();
  const Operator* Float32x4FromUint32x4();

  const Operator* CreateInt32x4();
  const Operator* Int32x4ExtractLane();
  const Operator* Int32x4ReplaceLane();
  const Operator* Int32x4Neg();
  const Operator* Int32x4Add();
  const Operator* Int32x4Sub();
  const Operator* Int32x4Mul();
  const Operator* Int32x4Min();
  const Operator* Int32x4Max();
  const Operator* Int32x4ShiftLeftByScalar();
  const Operator* Int32x4ShiftRightByScalar();
  const Operator* Int32x4Equal();
  const Operator* Int32x4NotEqual();
  const Operator* Int32x4LessThan();
  const Operator* Int32x4LessThanOrEqual();
  const Operator* Int32x4GreaterThan();
  const Operator* Int32x4GreaterThanOrEqual();
  const Operator* Int32x4Select();
  const Operator* Int32x4Swizzle();
  const Operator* Int32x4Shuffle();
  const Operator* Int32x4FromFloat32x4();

  const Operator* Uint32x4Min();
  const Operator* Uint32x4Max();
  const Operator* Uint32x4ShiftLeftByScalar();
  const Operator* Uint32x4ShiftRightByScalar();
  const Operator* Uint32x4LessThan();
  const Operator* Uint32x4LessThanOrEqual();
  const Operator* Uint32x4GreaterThan();
  const Operator* Uint32x4GreaterThanOrEqual();
  const Operator* Uint32x4FromFloat32x4();

  const Operator* CreateBool32x4();
  const Operator* Bool32x4ExtractLane();
  const Operator* Bool32x4ReplaceLane();
  const Operator* Bool32x4And();
  const Operator* Bool32x4Or();
  const Operator* Bool32x4Xor();
  const Operator* Bool32x4Not();
  const Operator* Bool32x4AnyTrue();
  const Operator* Bool32x4AllTrue();
  const Operator* Bool32x4Swizzle();
  const Operator* Bool32x4Shuffle();
  const Operator* Bool32x4Equal();
  const Operator* Bool32x4NotEqual();

  const Operator* CreateInt16x8();
  const Operator* Int16x8ExtractLane();
  const Operator* Int16x8ReplaceLane();
  const Operator* Int16x8Neg();
  const Operator* Int16x8Add();
  const Operator* Int16x8AddSaturate();
  const Operator* Int16x8Sub();
  const Operator* Int16x8SubSaturate();
  const Operator* Int16x8Mul();
  const Operator* Int16x8Min();
  const Operator* Int16x8Max();
  const Operator* Int16x8ShiftLeftByScalar();
  const Operator* Int16x8ShiftRightByScalar();
  const Operator* Int16x8Equal();
  const Operator* Int16x8NotEqual();
  const Operator* Int16x8LessThan();
  const Operator* Int16x8LessThanOrEqual();
  const Operator* Int16x8GreaterThan();
  const Operator* Int16x8GreaterThanOrEqual();
  const Operator* Int16x8Select();
  const Operator* Int16x8Swizzle();
  const Operator* Int16x8Shuffle();

  const Operator* Uint16x8AddSaturate();
  const Operator* Uint16x8SubSaturate();
  const Operator* Uint16x8Min();
  const Operator* Uint16x8Max();
  const Operator* Uint16x8ShiftLeftByScalar();
  const Operator* Uint16x8ShiftRightByScalar();
  const Operator* Uint16x8LessThan();
  const Operator* Uint16x8LessThanOrEqual();
  const Operator* Uint16x8GreaterThan();
  const Operator* Uint16x8GreaterThanOrEqual();

  const Operator* CreateBool16x8();
  const Operator* Bool16x8ExtractLane();
  const Operator* Bool16x8ReplaceLane();
  const Operator* Bool16x8And();
  const Operator* Bool16x8Or();
  const Operator* Bool16x8Xor();
  const Operator* Bool16x8Not();
  const Operator* Bool16x8AnyTrue();
  const Operator* Bool16x8AllTrue();
  const Operator* Bool16x8Swizzle();
  const Operator* Bool16x8Shuffle();
  const Operator* Bool16x8Equal();
  const Operator* Bool16x8NotEqual();

  const Operator* CreateInt8x16();
  const Operator* Int8x16ExtractLane();
  const Operator* Int8x16ReplaceLane();
  const Operator* Int8x16Neg();
  const Operator* Int8x16Add();
  const Operator* Int8x16AddSaturate();
  const Operator* Int8x16Sub();
  const Operator* Int8x16SubSaturate();
  const Operator* Int8x16Mul();
  const Operator* Int8x16Min();
  const Operator* Int8x16Max();
  const Operator* Int8x16ShiftLeftByScalar();
  const Operator* Int8x16ShiftRightByScalar();
  const Operator* Int8x16Equal();
  const Operator* Int8x16NotEqual();
  const Operator* Int8x16LessThan();
  const Operator* Int8x16LessThanOrEqual();
  const Operator* Int8x16GreaterThan();
  const Operator* Int8x16GreaterThanOrEqual();
  const Operator* Int8x16Select();
  const Operator* Int8x16Swizzle();
  const Operator* Int8x16Shuffle();

  const Operator* Uint8x16AddSaturate();
  const Operator* Uint8x16SubSaturate();
  const Operator* Uint8x16Min();
  const Operator* Uint8x16Max();
  const Operator* Uint8x16ShiftLeftByScalar();
  const Operator* Uint8x16ShiftRightByScalar();
  const Operator* Uint8x16LessThan();
  const Operator* Uint8x16LessThanOrEqual();
  const Operator* Uint8x16GreaterThan();
  const Operator* Uint8x16GreaterThanOrEqual();

  const Operator* CreateBool8x16();
  const Operator* Bool8x16ExtractLane();
  const Operator* Bool8x16ReplaceLane();
  const Operator* Bool8x16And();
  const Operator* Bool8x16Or();
  const Operator* Bool8x16Xor();
  const Operator* Bool8x16Not();
  const Operator* Bool8x16AnyTrue();
  const Operator* Bool8x16AllTrue();
  const Operator* Bool8x16Swizzle();
  const Operator* Bool8x16Shuffle();
  const Operator* Bool8x16Equal();
  const Operator* Bool8x16NotEqual();

  const Operator* Simd128Load();
  const Operator* Simd128Load1();
  const Operator* Simd128Load2();
  const Operator* Simd128Load3();
  const Operator* Simd128Store();
  const Operator* Simd128Store1();
  const Operator* Simd128Store2();
  const Operator* Simd128Store3();
  const Operator* Simd128And();
  const Operator* Simd128Or();
  const Operator* Simd128Xor();
  const Operator* Simd128Not();

  // load [base + index]
  const Operator* Load(LoadRepresentation rep);

  // store [base + index], value
  const Operator* Store(StoreRepresentation rep);

  const Operator* StackSlot(MachineRepresentation rep);

  // Access to the machine stack.
  const Operator* LoadStackPointer();
  const Operator* LoadFramePointer();
  const Operator* LoadParentFramePointer();

  // checked-load heap, index, length
  const Operator* CheckedLoad(CheckedLoadRepresentation);
  // checked-store heap, index, length, value
  const Operator* CheckedStore(CheckedStoreRepresentation);

  // atomic-load [base + index]
  const Operator* AtomicLoad(LoadRepresentation rep);
  // atomic-store [base + index], value
  const Operator* AtomicStore(MachineRepresentation rep);

  // Target machine word-size assumed by this builder.
  bool Is32() const { return word() == MachineRepresentation::kWord32; }
  bool Is64() const { return word() == MachineRepresentation::kWord64; }
  MachineRepresentation word() const { return word_; }

  bool UnalignedLoadSupported(const MachineType& machineType,
                              uint8_t alignment) {
    return alignment_requirements_.IsUnalignedLoadSupported(machineType,
                                                            alignment);
  }

  bool UnalignedStoreSupported(const MachineType& machineType,
                               uint8_t alignment) {
    return alignment_requirements_.IsUnalignedStoreSupported(machineType,
                                                             alignment);
  }

// Pseudo operators that translate to 32/64-bit operators depending on the
// word-size of the target machine assumed by this builder.
#define PSEUDO_OP_LIST(V) \
  V(Word, And)            \
  V(Word, Or)             \
  V(Word, Xor)            \
  V(Word, Shl)            \
  V(Word, Shr)            \
  V(Word, Sar)            \
  V(Word, Ror)            \
  V(Word, Clz)            \
  V(Word, Equal)          \
  V(Int, Add)             \
  V(Int, Sub)             \
  V(Int, Mul)             \
  V(Int, Div)             \
  V(Int, Mod)             \
  V(Int, LessThan)        \
  V(Int, LessThanOrEqual) \
  V(Uint, Div)            \
  V(Uint, LessThan)       \
  V(Uint, Mod)
#define PSEUDO_OP(Prefix, Suffix)                                \
  const Operator* Prefix##Suffix() {                             \
    return Is32() ? Prefix##32##Suffix() : Prefix##64##Suffix(); \
  }
  PSEUDO_OP_LIST(PSEUDO_OP)
#undef PSEUDO_OP
#undef PSEUDO_OP_LIST

 private:
  Zone* zone_;
  MachineOperatorGlobalCache const& cache_;
  MachineRepresentation const word_;
  Flags const flags_;
  AlignmentRequirements const alignment_requirements_;

  DISALLOW_COPY_AND_ASSIGN(MachineOperatorBuilder);
};


DEFINE_OPERATORS_FOR_FLAGS(MachineOperatorBuilder::Flags)

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_COMPILER_MACHINE_OPERATOR_H_
