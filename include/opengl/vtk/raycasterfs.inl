const char* raycasterfs =
    "//VTK::System::Dec\n"
    "\n"
    "/*=========================================================================\n"
    "\n"
    " Program: Visualization Toolkit\n"
    " Module: raycasterfs.glsl\n"
    "\n"
    " Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen\n"
    " All rights reserved.\n"
    " See Copyright.txt or http://www.kitware.com/Copyright.htm for details.\n"
    "\n"
    " This software is distributed WITHOUT ANY WARRANTY; without even\n"
    " the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR\n"
    " PURPOSE. See the above copyright notice for more information.\n"
    "\n"
    "=========================================================================*/\n"
    "\n"
    "//////////////////////////////////////////////////////////////////////////////\n"
    "///\n"
    "/// Inputs\n"
    "///\n"
    "//////////////////////////////////////////////////////////////////////////////\n"
    "\n"
    "/// 3D texture coordinates form vertex shader\n"
    "varying vec3 ip_textureCoords;\n"
    "varying vec3 ip_vertexPos;\n"
    "\n"
    "//////////////////////////////////////////////////////////////////////////////\n"
    "///\n"
    "/// Outputs\n"
    "///\n"
    "//////////////////////////////////////////////////////////////////////////////\n"
    "\n"
    "vec4 g_fragColor = vec4(0.0);\n"
    "\n"
    "//////////////////////////////////////////////////////////////////////////////\n"
    "///\n"
    "/// Uniforms, attributes, and globals\n"
    "///\n"
    "//////////////////////////////////////////////////////////////////////////////\n"
    "vec3 g_dataPos;\n"
    "vec3 g_dirStep;\n"
    "vec4 g_srcColor;\n"
    "vec4 g_eyePosObj;\n"
    "bool g_exit;\n"
    "\n"
    "uniform vec4 in_volume_scale;\n"
    "uniform vec4 in_volume_bias;\n"
    "\n"
    "//VTK::Output::Dec\n"
    "\n"
    "//VTK::Base::Dec\n"
    "\n"
    "//VTK::Termination::Dec\n"
    "\n"
    "//VTK::Cropping::Dec\n"
    "\n"
    "//VTK::Shading::Dec\n"
    "\n"
    "//VTK::BinaryMask::Dec\n"
    "\n"
    "//VTK::CompositeMask::Dec\n"
    "\n"
    "//VTK::ComputeOpacity::Dec\n"
    "\n"
    "//VTK::ComputeGradient::Dec\n"
    "\n"
    "//VTK::ComputeLighting::Dec\n"
    "\n"
    "//VTK::ComputeColor::Dec\n"
    "\n"
    "//VTK::ComputeRayDirection::Dec\n"
    "\n"
    "//VTK::Picking::Dec\n"
    "\n"
    "/// We support only 8 clipping planes for now\n"
    "/// The first value is the size of the data array for clipping\n"
    "/// planes (origin, normal)\n"
    "uniform float in_clippingPlanes[49];\n"
    "uniform float in_scale;\n"
    "uniform float in_bias;\n"
    "\n"
    "//////////////////////////////////////////////////////////////////////////////\n"
    "///\n"
    "/// Main\n"
    "///\n"
    "//////////////////////////////////////////////////////////////////////////////\n"
    "void main()\n"
    " {\n"
    " /// Initialize g_fragColor (output) to 0\n"
    " g_fragColor = vec4(0.0);\n"
    " g_dirStep = vec3(0.0);\n"
    " g_srcColor = vec4(0.0);\n"
    " g_exit = false;\n"
    "\n"
    " //VTK::Base::Init\n"
    "\n"
    " //VTK::Terminate::Init\n"
    "\n"
    " //VTK::Shading::Init\n"
    "\n"
    " //VTK::Cropping::Init\n"
    "\n"
    " //VTK::Clipping::Init\n"
    "\n"
    " //VTK::RenderToImage::Init\n"
    "\n"
    " //VTK::DepthPass::Init\n"
    "\n"
    " /// For all samples along the ray\n"
    " while (!g_exit)\n"
    " {\n"
    " //VTK::Base::Impl\n"
    "\n"
    " //VTK::Cropping::Impl\n"
    "\n"
    " //VTK::Clipping::Impl\n"
    "\n"
    " //VTK::BinaryMask::Impl\n"
    "\n"
    " //VTK::CompositeMask::Impl\n"
    "\n"
    " //VTK::Shading::Impl\n"
    "\n"
    " //VTK::RenderToImage::Impl\n"
    "\n"
    " //VTK::DepthPass::Impl\n"
    "\n"
    " /// Advance ray\n"
    " g_dataPos += g_dirStep;\n"
    "\n"
    " //VTK::Terminate::Impl\n"
    " }\n"
    "\n"
    " //VTK::Base::Exit\n"
    "\n"
    " //VTK::Terminate::Exit\n"
    "\n"
    " //VTK::Cropping::Exit\n"
    "\n"
    " //VTK::Clipping::Exit\n"
    "\n"
    " //VTK::Shading::Exit\n"
    "\n"
    " //VTK::Picking::Exit\n"
    "\n"
    " g_fragColor.r = g_fragColor.r * in_scale + in_bias * g_fragColor.a;\n"
    " g_fragColor.g = g_fragColor.g * in_scale + in_bias * g_fragColor.a;\n"
    " g_fragColor.b = g_fragColor.b * in_scale + in_bias * g_fragColor.a;\n"
    " gl_FragData[0] = g_fragColor;\n"
    "\n"
    " //VTK::RenderToImage::Exit\n"
    "\n"
    " //VTK::DepthPass::Exit\n"
    " }\n"
    "\n";

