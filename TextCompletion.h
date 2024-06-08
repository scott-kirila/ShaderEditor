//
// Created by Scott Kirila on 5/28/2024.
//

#ifndef TEXTCOMPLETION_H
#define TEXTCOMPLETION_H

#include <string>
#include <vector>

#include "Callbacks.h"

class TextCompletion {
public:
    TextCompletion();
    ~TextCompletion() = default;

    std::string GetCurrentWord(const ImGuiInputTextCallbackData* CallbackData);
    void PopulateMatches(const ImGuiInputTextCallbackData* CallbackData);
    void DisplayMatches(const ImGuiInputTextCallbackData* CallbackData);
    void DoComplete(ImGuiInputTextCallbackData* data);
    void ClearResults();

    std::vector<std::string> m_Matches{};

    int m_CurrentIndex{};

    bool m_CanComplete{};
    bool m_DoComplete{};

private:
    char* m_CurrentWordStart{};
    char* m_CurrentWordEnd{};
    std::string m_SelectedMatch{};
    int m_WordStart{};
    int m_MaxDisplayMatches{2};

    std::vector<std::string> m_Dictionary = {
        // Internal
        "FragCoord", "Resolution", "Time",

        // Keywords
        "const", "uniform", "buffer", "shared", "attribute", "varying",
        "coherent", "volatile", "restrict", "readonly", "writeonly",
        "atomic_uint",
        "layout",
        "centroid", "flat", "smooth", "noperspective",
        "patch", "sample",
        "invariant", "precise",
        "break", "continue", "do", "for", "while", "switch", "case", "default",
        "if", "else",
        "subroutine",
        "in", "out", "inout",
        "int", "void", "bool", "true", "false", "float", "double",
        "discard", "return",
        "vec2", "vec3", "vec4", "ivec2", "ivec3", "ivec4", "bvec2", "bvec3", "bvec4",
        "uint", "uvec2", "uvec3", "uvec4",
        "dvec2", "dvec3", "dvec4",
        "mat2", "mat3", "mat4",
        "mat2x2", "mat2x3", "mat2x4",
        "mat3x2", "mat3x3", "mat3x4",
        "mat4x2", "mat4x3", "mat4x4",
        "dmat2", "dmat3", "dmat4",
        "dmat2x2", "dmat2x3", "dmat2x4",
        "dmat3x2", "dmat3x3", "dmat3x4",
        "dmat4x2", "dmat4x3", "dmat4x4",
        "lowp", "mediump", "highp", "precision",
        "sampler1D", "sampler1DShadow", "sampler1DArray", "sampler1DArrayShadow",
        "isampler1D", "isampler1DArray", "usampler1D", "usampler1DArray",
        "sampler2D", "sampler2DShadow", "sampler2DArray", "sampler2DArrayShadow",
        "isampler2D", "isampler2DArray", "usampler2D", "usampler2DArray",
        "sampler2DRect", "sampler2DRectShadow", "isampler2DRect", "usampler2DRect",
        "sampler2DMS", "isampler2DMS", "usampler2DMS",
        "sampler2DMSArray", "isampler2DMSArray", "usampler2DMSArray",
        "sampler3D", "isampler3D", "usampler3D",
        "samplerCube", "samplerCubeShadow", "isamplerCube", "usamplerCube",
        "samplerCubeArray", "samplerCubeArrayShadow",
        "isamplerCubeArray", "usamplerCubeArray",
        "samplerBuffer", "isamplerBuffer", "usamplerBuffer",
        "image1D", "iimage1D", "uimage1D",
        "image1DArray", "iimage1DArray", "uimage1DArray",
        "image2D", "iimage2D", "uimage2D",
        "image2DArray", "iimage2DArray", "uimage2DArray",
        "image2DRect", "iimage2DRect", "uimage2DRect",
        "image2DMS", "iimage2DMS", "uimage2DMS",
        "image2DMSArray", "iimage2DMSArray", "uimage2DMSArray",
        "image3D", "iimage3D", "uimage3D",
        "imageCube", "iimageCube", "uimageCube",
        "imageCubeArray", "iimageCubeArray", "uimageCubeArray",
        "imageBuffer", "iimageBuffer", "uimageBuffer",
        "struct",

        // Built-in Fragment Shader Variables
        "gl_FragCoord",
        "gl_FrontFacing",
        "gl_ClipDistance",
        "gl_CullDistance",
        "gl_PointCoord",
        "gl_PrimitiveID",
        "gl_SampleID",
        "gl_SamplePosition",
        "gl_SampleMaskIn",
        "gl_Layer",
        "gl_ViewportIndex",
        "gl_HelperInvocation",
        "gl_FragDepth",
        "gl_SampleMask",

        // Built-in Constants
        "gl_MaxVertexAttribs",
        "gl_MaxVertexUniformVectors",
        "gl_MaxVertexUniformComponents",
        "gl_MaxVertexOutputComponents",
        "gl_MaxVaryingComponents",
        "gl_MaxVaryingVectors",
        "gl_MaxVertexTextureImageUnits",
        "gl_MaxVertexImageUniforms",
        "gl_MaxVertexAtomicCounters",
        "gl_MaxVertexAtomicCounterBuffers",

        "gl_MaxTessPatchComponents",
        "gl_MaxPatchVertices",
        "gl_MaxTessGenLevel",

        "gl_MaxTessControlInputComponents",
        "gl_MaxTessControlOutputComponents",
        "gl_MaxTessControlTextureImageUnits",
        "gl_MaxTessControlUniformComponents",
        "gl_MaxTessControlTotalOutputComponents",
        "gl_MaxTessControlImageUniforms",
        "gl_MaxTessControlAtomicCounters",
        "gl_MaxTessControlAtomicCounterBuffers",

        "gl_MaxTessEvaluationInputComponents",
        "gl_MaxTessEvaluationOutputComponents",
        "gl_MaxTessEvaluationTextureImageUnits",
        "gl_MaxTessEvaluationUniformComponents",
        "gl_MaxTessEvaluationImageUniforms",
        "gl_MaxTessEvaluationAtomicCounters",
        "gl_MaxTessEvaluationAtomicCounterBuffers",

        "gl_MaxGeometryInputComponents",
        "gl_MaxGeometryOutputComponents",
        "gl_MaxGeometryImageUniforms",
        "gl_MaxGeometryTextureImageUnits",
        "gl_MaxGeometryOutputVertices",
        "gl_MaxGeometryTotalOutputComponents",
        "gl_MaxGeometryUniformComponents",
        "gl_MaxGeometryAtomicCounters",
        "gl_MaxGeometryAtomicCounterBuffers",

        "gl_MaxFragmentImageUniforms",
        "gl_MaxFragmentInputComponents",
        "gl_MaxFragmentUniformVectors",
        "gl_MaxFragmentUniformComponents",
        "gl_MaxFragmentAtomicCounters",
        "gl_MaxFragmentAtomicCounterBuffers",

        "gl_MaxDrawBuffers",
        "gl_MaxTextureImageUnits",
        "gl_MinProgramTexelOffset",
        "gl_MaxProgramTexelOffset",
        "gl_MaxImageUnits",
        "gl_MaxSamples",
        "gl_MaxImageSamples",
        "gl_MaxClipDistances",
        "gl_MaxCullD",
        "gl_MaxViewports",

        "gl_MaxComputeImageUniforms",
        "gl_MaxComputeWorkGroupCount",
        "gl_MaxComputeWorkGroupSize",
        "gl_MaxComputeUniformComponents",
        "gl_MaxComputeTextureImageUnits",
        "gl_MaxComputeAtomicCounters",
        "gl_MaxComputeAtomicCounterBuffers",

        "gl_MaxCombinedTextureImageUnits",
        "gl_MaxCombinedImageUniforms",
        "gl_MaxCombinedShaderOutputResources",
        "gl_MaxCombinedAtomicCounters",
        "gl_MaxCombinedAtomicCounterBuffers",
        "gl_MaxCombinedClipAndCullDistances",
        "gl_MaxAtomicCounterBindings",
        "gl_MaxAtomicCounterBufferSize",

        "gl_MaxTransformFeedbackBuffers",
        "gl_MaxTransformFeedbackInterleavedComponents",

        // Functions
        // Angles and Trigonometry
        "radians", "degrees", "sin", "cos", "tan", "asin", "acos", "atan",
        "sinh", "cosh", "tanh", "asinh", "acosh", "atanh",

        // Exponential
        "pow", "exp", "log", "exp2", "log2", "sqrt", "inversesqrt",

        // Common
        "abs", "sign", "floor", "trunc", "round", "roundEven", "ceil", "fract",
        "mod", "modf", "min", "max", "clamp", "mix", "step", "smoothstep", "isnan", "isinf",
        "floatBitsToInt", "floatBitsToUint", "intBitsToFloat", "uintBitsToFloat",
        "fma", "frexp", "ldexp",

        // Floating-point Pack and Unpack Functions
        "packUnorm2x16", "packSnorm2x16", "packUnorm4x8", "packSnorm4x8",
        "unpackUnorm2x16", "unpackSnorm2x16", "unpackUnorm4x8", "unpackSnorm4x8",
        "packHalf2x16", "unpackHalf2x16", "packDouble2x32", "unpackDouble2x32",

        // Geometric
        "length", "distance", "dot", "cross", "normalize", "faceforward", "reflect", "refract",

        // Matrix
        "matrixCompMult", "outerProduct", "transpose", "determinant", "inverse",

        // Vector Relational
        "bvec", "ivec", "uvec", "vec",
        "lessThan", "lessThanEqual", "greaterThan", "greaterThanEqual", "equal", "notEqual",
        "any", "all", "not",

        // Integer
        "uaddCarry", "usubBorrow", "umulExtended", "imulExtended", "bitfieldExtract", "bitfieldInsert",
        "bitfieldReverse", "bitCount", "findLSB", "findMSB",

        // Texture
        "textureSize", "textureQueryLod", "textureQueryLevels", "textureSamples", "texture",
        "textureProj", "textureLod", "textureOffset", "texelFetch", "texelFetchOffset",
        "textureProjOffset", "textureLodOffset", "textureProjLod", "textureProjLodOffset",
        "textureGrad", "textureGradOffset", "textureProjGrad", "textureProjGradOffset",

        // Texture Gather
        "textureGather", "textureGatherOffset", "textureGatherOffsets",

        // Atomic Counter
        "atomicCounterIncrement", "atomicCounterDecrement", "atomicCounter",
        "atomicCounterAdd", "atomicCounterSubtract", "atomicCounterMin", "atomicCounterMax",
        "atomicCounterAnd", "atomicCounterOr", "atomicCounterXor", "atomicCounterExchange",
        "atomicCounterCompSwap",

        // Atomic Memory
        "atomicAdd", "atomicMin", "atomicMax", "atomicAnd", "atomicOr", "atomicXor",
        "atomicExchange", "atomicCompSwap",

        // Image
        "imageSize", "imageSamples", "imageLoad", "imageStore", "imageAtomicAdd",
        "imageAtomicMin", "imageAtomicMax", "imageAtomicAnd", "imageAtomicOr", "imageAtomicXor",
        "imageAtomicExchange", "imageAtomicCompSwap",

        // Derivatives
        "dFdx", "dFdy", "dFdxFine", "dFdyFine", "dFdxCoarse", "dFdyCoarse",
        "fwidth", "fwidthFine", "fwidthCoarse",

        // Interpolation
        "interpolateAtCentroid", "interpolateAtSample", "interpolateAtOffset",
    };
};
#endif //TEXTCOMPLETION_H
