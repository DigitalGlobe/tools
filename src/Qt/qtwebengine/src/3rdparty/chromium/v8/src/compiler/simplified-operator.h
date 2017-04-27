// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_COMPILER_SIMPLIFIED_OPERATOR_H_
#define V8_COMPILER_SIMPLIFIED_OPERATOR_H_

#include <iosfwd>

#include "src/compiler/type-hints.h"
#include "src/handles.h"
#include "src/machine-type.h"
#include "src/objects.h"

namespace v8 {
namespace internal {

// Forward declarations.
class Type;
class Zone;


namespace compiler {

// Forward declarations.
class Operator;
struct SimplifiedOperatorGlobalCache;

enum BaseTaggedness : uint8_t { kUntaggedBase, kTaggedBase };

size_t hash_value(BaseTaggedness);

std::ostream& operator<<(std::ostream&, BaseTaggedness);


// An access descriptor for loads/stores of array buffers.
class BufferAccess final {
 public:
  explicit BufferAccess(ExternalArrayType external_array_type)
      : external_array_type_(external_array_type) {}

  ExternalArrayType external_array_type() const { return external_array_type_; }
  MachineType machine_type() const;

 private:
  ExternalArrayType const external_array_type_;
};

bool operator==(BufferAccess, BufferAccess);
bool operator!=(BufferAccess, BufferAccess);

size_t hash_value(BufferAccess);

std::ostream& operator<<(std::ostream&, BufferAccess);

BufferAccess const BufferAccessOf(const Operator* op) WARN_UNUSED_RESULT;


// An access descriptor for loads/stores of fixed structures like field
// accesses of heap objects. Accesses from either tagged or untagged base
// pointers are supported; untagging is done automatically during lowering.
struct FieldAccess {
  BaseTaggedness base_is_tagged;  // specifies if the base pointer is tagged.
  int offset;                     // offset of the field, without tag.
  MaybeHandle<Name> name;         // debugging only.
  Type* type;                     // type of the field.
  MachineType machine_type;       // machine type of the field.
  WriteBarrierKind write_barrier_kind;  // write barrier hint.

  int tag() const { return base_is_tagged == kTaggedBase ? kHeapObjectTag : 0; }
};

bool operator==(FieldAccess const&, FieldAccess const&);
bool operator!=(FieldAccess const&, FieldAccess const&);

size_t hash_value(FieldAccess const&);

std::ostream& operator<<(std::ostream&, FieldAccess const&);

FieldAccess const& FieldAccessOf(const Operator* op) WARN_UNUSED_RESULT;


// An access descriptor for loads/stores of indexed structures like characters
// in strings or off-heap backing stores. Accesses from either tagged or
// untagged base pointers are supported; untagging is done automatically during
// lowering.
struct ElementAccess {
  BaseTaggedness base_is_tagged;  // specifies if the base pointer is tagged.
  int header_size;                // size of the header, without tag.
  Type* type;                     // type of the element.
  MachineType machine_type;       // machine type of the element.
  WriteBarrierKind write_barrier_kind;  // write barrier hint.

  int tag() const { return base_is_tagged == kTaggedBase ? kHeapObjectTag : 0; }
};

bool operator==(ElementAccess const&, ElementAccess const&);
bool operator!=(ElementAccess const&, ElementAccess const&);

size_t hash_value(ElementAccess const&);

std::ostream& operator<<(std::ostream&, ElementAccess const&);

ElementAccess const& ElementAccessOf(const Operator* op) WARN_UNUSED_RESULT;

enum class CheckFloat64HoleMode : uint8_t {
  kNeverReturnHole,  // Never return the hole (deoptimize instead).
  kAllowReturnHole   // Allow to return the hole (signaling NaN).
};

size_t hash_value(CheckFloat64HoleMode);

std::ostream& operator<<(std::ostream&, CheckFloat64HoleMode);

CheckFloat64HoleMode CheckFloat64HoleModeOf(const Operator*) WARN_UNUSED_RESULT;

enum class CheckTaggedHoleMode : uint8_t {
  kNeverReturnHole,        // Never return the hole (deoptimize instead).
  kConvertHoleToUndefined  // Convert the hole to undefined.
};

size_t hash_value(CheckTaggedHoleMode);

std::ostream& operator<<(std::ostream&, CheckTaggedHoleMode);

CheckTaggedHoleMode CheckTaggedHoleModeOf(const Operator*) WARN_UNUSED_RESULT;

Type* TypeOf(const Operator* op) WARN_UNUSED_RESULT;

BinaryOperationHints::Hint BinaryOperationHintOf(const Operator* op);

CompareOperationHints::Hint CompareOperationHintOf(const Operator* op);

// Interface for building simplified operators, which represent the
// medium-level operations of V8, including adding numbers, allocating objects,
// indexing into objects and arrays, etc.
// All operators are typed but many are representation independent.

// Number values from JS can be in one of these representations:
//   - Tagged: word-sized integer that is either
//     - a signed small integer (31 or 32 bits plus a tag)
//     - a tagged pointer to a HeapNumber object that has a float64 field
//   - Int32: an untagged signed 32-bit integer
//   - Uint32: an untagged unsigned 32-bit integer
//   - Float64: an untagged float64

// Additional representations for intermediate code or non-JS code:
//   - Int64: an untagged signed 64-bit integer
//   - Uint64: an untagged unsigned 64-bit integer
//   - Float32: an untagged float32

// Boolean values can be:
//   - Bool: a tagged pointer to either the canonical JS #false or
//           the canonical JS #true object
//   - Bit: an untagged integer 0 or 1, but word-sized
class SimplifiedOperatorBuilder final : public ZoneObject {
 public:
  explicit SimplifiedOperatorBuilder(Zone* zone);

  const Operator* BooleanNot();
  const Operator* BooleanToNumber();

  const Operator* NumberEqual();
  const Operator* NumberLessThan();
  const Operator* NumberLessThanOrEqual();
  const Operator* NumberAdd();
  const Operator* NumberSubtract();
  const Operator* NumberMultiply();
  const Operator* NumberDivide();
  const Operator* NumberModulus();
  const Operator* NumberBitwiseOr();
  const Operator* NumberBitwiseXor();
  const Operator* NumberBitwiseAnd();
  const Operator* NumberShiftLeft();
  const Operator* NumberShiftRight();
  const Operator* NumberShiftRightLogical();
  const Operator* NumberImul();
  const Operator* NumberAbs();
  const Operator* NumberClz32();
  const Operator* NumberCeil();
  const Operator* NumberFloor();
  const Operator* NumberFround();
  const Operator* NumberAtan();
  const Operator* NumberAtan2();
  const Operator* NumberAtanh();
  const Operator* NumberCbrt();
  const Operator* NumberCos();
  const Operator* NumberExp();
  const Operator* NumberExpm1();
  const Operator* NumberLog();
  const Operator* NumberLog1p();
  const Operator* NumberLog10();
  const Operator* NumberLog2();
  const Operator* NumberRound();
  const Operator* NumberSin();
  const Operator* NumberSqrt();
  const Operator* NumberTan();
  const Operator* NumberTrunc();
  const Operator* NumberToInt32();
  const Operator* NumberToUint32();

  const Operator* NumberSilenceNaN();

  const Operator* SpeculativeNumberAdd(BinaryOperationHints::Hint hint);
  const Operator* SpeculativeNumberSubtract(BinaryOperationHints::Hint hint);
  const Operator* SpeculativeNumberMultiply(BinaryOperationHints::Hint hint);
  const Operator* SpeculativeNumberDivide(BinaryOperationHints::Hint hint);
  const Operator* SpeculativeNumberModulus(BinaryOperationHints::Hint hint);

  const Operator* SpeculativeNumberLessThan(CompareOperationHints::Hint hint);
  const Operator* SpeculativeNumberLessThanOrEqual(
      CompareOperationHints::Hint hint);
  const Operator* SpeculativeNumberEqual(CompareOperationHints::Hint hint);

  const Operator* ReferenceEqual(Type* type);

  const Operator* StringEqual();
  const Operator* StringLessThan();
  const Operator* StringLessThanOrEqual();
  const Operator* StringFromCharCode();
  const Operator* StringToNumber();

  const Operator* PlainPrimitiveToNumber();
  const Operator* PlainPrimitiveToWord32();
  const Operator* PlainPrimitiveToFloat64();

  const Operator* ChangeTaggedSignedToInt32();
  const Operator* ChangeTaggedToInt32();
  const Operator* ChangeTaggedToUint32();
  const Operator* ChangeTaggedToFloat64();
  const Operator* ChangeInt31ToTaggedSigned();
  const Operator* ChangeInt32ToTagged();
  const Operator* ChangeUint32ToTagged();
  const Operator* ChangeFloat64ToTagged();
  const Operator* ChangeTaggedToBit();
  const Operator* ChangeBitToTagged();
  const Operator* TruncateTaggedToWord32();
  const Operator* TruncateTaggedToFloat64();

  const Operator* CheckBounds();
  const Operator* CheckTaggedPointer();
  const Operator* CheckTaggedSigned();

  const Operator* CheckedInt32Add();
  const Operator* CheckedInt32Sub();
  const Operator* CheckedUint32ToInt32();
  const Operator* CheckedFloat64ToInt32();
  const Operator* CheckedTaggedToInt32();
  const Operator* CheckedTaggedToFloat64();

  const Operator* CheckFloat64Hole(CheckFloat64HoleMode);
  const Operator* CheckTaggedHole(CheckTaggedHoleMode);

  const Operator* ObjectIsCallable();
  const Operator* ObjectIsNumber();
  const Operator* ObjectIsReceiver();
  const Operator* ObjectIsSmi();
  const Operator* ObjectIsString();
  const Operator* ObjectIsUndetectable();

  const Operator* TypeGuard(Type* type);

  const Operator* Allocate(PretenureFlag pretenure = NOT_TENURED);

  const Operator* LoadField(FieldAccess const&);
  const Operator* StoreField(FieldAccess const&);

  // load-buffer buffer, offset, length
  const Operator* LoadBuffer(BufferAccess);

  // store-buffer buffer, offset, length, value
  const Operator* StoreBuffer(BufferAccess);

  // load-element [base + index], length
  const Operator* LoadElement(ElementAccess const&);

  // store-element [base + index], length, value
  const Operator* StoreElement(ElementAccess const&);

 private:
  Zone* zone() const { return zone_; }

  const SimplifiedOperatorGlobalCache& cache_;
  Zone* const zone_;

  DISALLOW_COPY_AND_ASSIGN(SimplifiedOperatorBuilder);
};

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_COMPILER_SIMPLIFIED_OPERATOR_H_
