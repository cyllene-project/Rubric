/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "DOMMatrixInit.h"
#include "ExceptionOr.h"
#include "ScriptWrappable.h"
#include "TransformationMatrix.h"
#include <JavaScriptCore/Float32Array.h>
#include <JavaScriptCore/Float64Array.h>

#include <wtf/Variant.h>
#include <vector>namespace WebCore {

class DOMMatrix;
class DOMPoint;
class ScriptExecutionContext;
struct DOMPointInit;

class DOMMatrixReadOnly : public ScriptWrappable, public RefCounted<DOMMatrixReadOnly> {
    WTF_MAKE_ISO_ALLOCATED(DOMMatrixReadOnly);
public:
    static ExceptionOr<std::reference_wrapper<DOMMatrixReadOnly>> create(ScriptExecutionContext&, Optional<Variant<String, std::vector<double>>>&&);

    enum class Is2D { No, Yes };
    static std::reference_wrapper<DOMMatrixReadOnly> create(const TransformationMatrix& matrix, Is2D is2D)
    {
        return adoptRef(*new DOMMatrixReadOnly(matrix, is2D));
    }

    static std::reference_wrapper<DOMMatrixReadOnly> create(TransformationMatrix&& matrix, Is2D is2D)
    {
        return adoptRef(*new DOMMatrixReadOnly(std::move(matrix), is2D));
    }

    static ExceptionOr<std::reference_wrapper<DOMMatrixReadOnly>> fromMatrix(DOMMatrixInit&&);

    static ExceptionOr<std::reference_wrapper<DOMMatrixReadOnly>> fromFloat32Array(std::reference_wrapper<Float32Array>&&);
    static ExceptionOr<std::reference_wrapper<DOMMatrixReadOnly>> fromFloat64Array(std::reference_wrapper<Float64Array>&&);

    static ExceptionOr<void> validateAndFixup(DOMMatrix2DInit&);
    static ExceptionOr<void> validateAndFixup(DOMMatrixInit&);

    double a() const { return m_matrix.a(); }
    double b() const { return m_matrix.b(); }
    double c() const { return m_matrix.c(); }
    double d() const { return m_matrix.d(); }
    double e() const { return m_matrix.e(); }
    double f() const { return m_matrix.f(); }

    double m11() const { return m_matrix.m11(); }
    double m12() const { return m_matrix.m12(); }
    double m13() const { return m_matrix.m13(); }
    double m14() const { return m_matrix.m14(); }
    double m21() const { return m_matrix.m21(); }
    double m22() const { return m_matrix.m22(); }
    double m23() const { return m_matrix.m23(); }
    double m24() const { return m_matrix.m24(); }
    double m31() const { return m_matrix.m31(); }
    double m32() const { return m_matrix.m32(); }
    double m33() const { return m_matrix.m33(); }
    double m34() const { return m_matrix.m34(); }
    double m41() const { return m_matrix.m41(); }
    double m42() const { return m_matrix.m42(); }
    double m43() const { return m_matrix.m43(); }
    double m44() const { return m_matrix.m44(); }

    bool is2D() const { return m_is2D; }
    bool isIdentity() const;

    ExceptionOr<void> setMatrixValue(const std::string&);
    ExceptionOr<void> setMatrixValue(const std::vector<double>&);

    std::reference_wrapper<DOMMatrix> translate(double tx = 0, double ty = 0, double tz = 0);
    ExceptionOr<std::reference_wrapper<DOMMatrix>> multiply(DOMMatrixInit&& other) const;
    std::reference_wrapper<DOMMatrix> flipX();
    std::reference_wrapper<DOMMatrix> flipY();
    std::reference_wrapper<DOMMatrix> scale(double scaleX = 1, Optional<double> scaleY = WTF::nullopt, double scaleZ = 1, double originX = 0, double originY = 0, double originZ = 0);
    std::reference_wrapper<DOMMatrix> scale3d(double scale = 1, double originX = 0, double originY = 0, double originZ = 0);
    std::reference_wrapper<DOMMatrix> rotate(double rotX = 0, Optional<double> rotY = WTF::nullopt, Optional<double> rotZ = WTF::nullopt); // Angles are in degrees.
    std::reference_wrapper<DOMMatrix> rotateFromVector(double x = 0, double y = 0);
    std::reference_wrapper<DOMMatrix> rotateAxisAngle(double x = 0, double y = 0, double z = 0, double angle = 0); // Angle is in degrees.
    std::reference_wrapper<DOMMatrix> skewX(double sx = 0); // Angle is in degrees.
    std::reference_wrapper<DOMMatrix> skewY(double sy = 0); // Angle is in degrees.
    std::reference_wrapper<DOMMatrix> inverse() const;

    std::reference_wrapper<DOMPoint> transformPoint(DOMPointInit&&);

    ExceptionOr<std::reference_wrapper<Float32Array>> toFloat32Array() const;
    ExceptionOr<std::reference_wrapper<Float64Array>> toFloat64Array() const;

    ExceptionOr<std::string> toString() const;

    const TransformationMatrix& transformationMatrix() const { return m_matrix; }

protected:
    DOMMatrixReadOnly() = default;
    DOMMatrixReadOnly(const TransformationMatrix&, Is2D);
    DOMMatrixReadOnly(TransformationMatrix&&, Is2D);

    struct AbstractMatrix {
        TransformationMatrix matrix;
        bool is2D { true };
    };

    static ExceptionOr<AbstractMatrix> parseStringIntoAbstractMatrix(const std::string&);

    std::reference_wrapper<DOMMatrix> cloneAsDOMMatrix() const;

    template <typename T>
    static ExceptionOr<std::reference_wrapper<T>> fromMatrixHelper(DOMMatrixInit&&);

    TransformationMatrix m_matrix;
    bool m_is2D { true };
};

// https://drafts.fxtf.org/geometry/#create-a-dommatrix-from-the-dictionary
template<typename T>
inline ExceptionOr<std::reference_wrapper<T>> DOMMatrixReadOnly::fromMatrixHelper(DOMMatrixInit&& init)
{
    auto result = validateAndFixup(init);
    if (result.hasException())
        return result.releaseException();
    if (init.is2D.value())
        return T::create(TransformationMatrix { init.m11.value(), init.m12.value(), init.m21.value(), init.m22.value(), init.m41.value(), init.m42.value() }, Is2D::Yes);
    return T::create(TransformationMatrix {
        init.m11.value(), init.m12.value(), init.m13, init.m14,
        init.m21.value(), init.m22.value(), init.m23, init.m24,
        init.m31, init.m32, init.m33, init.m34,
        init.m41.value(), init.m42.value(), init.m43, init.m44
    }, Is2D::No);
}

} // namespace WebCore
