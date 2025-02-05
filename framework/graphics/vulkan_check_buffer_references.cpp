/*
** Copyright (c) 2024 LunarG, Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the "Software"),
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the
** Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
*/

#include "vulkan_check_buffer_references.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(graphics)

template <>
void vulkan_check_buffer_references(const VkGraphicsPipelineCreateInfo* create_infos, uint32_t create_info_count)
{
    for (uint32_t i = 0; i < create_info_count; ++i)
    {
        for (uint32_t j = 0; j < create_infos[i].stageCount; ++j)
        {
            const void* pNext = create_infos[i].pStages[j].pNext;
            while (pNext != nullptr)
            {
                auto base = reinterpret_cast<const VkBaseInStructure*>(pNext);

                if (base->sType == VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO)
                {
                    auto module_create_info = reinterpret_cast<const VkShaderModuleCreateInfo*>(base);
                    graphics::vulkan_check_buffer_references(module_create_info->pCode, module_create_info->codeSize);
                }
                pNext = base->pNext;
            }
        }
    }
}

template <>
void vulkan_check_buffer_references(const VkComputePipelineCreateInfo* create_infos, uint32_t create_info_count)
{
    for (uint32_t i = 0; i < create_info_count; ++i)
    {
        const void* pNext = create_infos[i].stage.pNext;
        while (pNext != nullptr)
        {
            auto base = reinterpret_cast<const VkBaseInStructure*>(pNext);

            if (base->sType == VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO)
            {
                auto module_create_info = reinterpret_cast<const VkShaderModuleCreateInfo*>(base);
                graphics::vulkan_check_buffer_references(module_create_info->pCode, module_create_info->codeSize);
            }
            pNext = base->pNext;
        }
    }
}

GFXRECON_END_NAMESPACE(graphics)
GFXRECON_END_NAMESPACE(gfxrecon)