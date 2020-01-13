/*
 * Copyright 2019-2020 Hans-Kristian Arntzen for Valve Corporation
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#pragma once
#include "opcodes/opcodes.hpp"

namespace DXIL2SPIRV
{
bool get_image_dimensions_query_size(Converter::Impl &impl, spv::Id image_id, uint32_t *num_coords);
bool get_image_dimensions(Converter::Impl &impl, spv::Id image_id, uint32_t *num_coords, uint32_t *num_dimensions);

bool emit_sample_instruction(DXIL::Op opcode, Converter::Impl &impl, const llvm::CallInst *instruction);
bool emit_sample_grad_instruction(Converter::Impl &impl, const llvm::CallInst *instruction);
bool emit_texture_gather_instruction(bool compare, Converter::Impl &impl, const llvm::CallInst *instruction);
bool emit_texture_load_instruction(Converter::Impl &impl, const llvm::CallInst *instruction);
bool emit_texture_store_instruction(Converter::Impl &impl, const llvm::CallInst *instruction);
bool emit_get_dimensions_instruction(Converter::Impl &impl, const llvm::CallInst *instruction);

bool emit_calculate_lod_instruction(Converter::Impl &impl, const llvm::CallInst *instruction);

template <DXIL::Op opcode>
static inline bool emit_sample_instruction_dispatch(Converter::Impl &impl, const llvm::CallInst *instruction)
{
	return emit_sample_instruction(opcode, impl, instruction);
}

template <bool compare>
static inline bool emit_texture_gather_dispatch(Converter::Impl &impl, const llvm::CallInst *instruction)
{
	return emit_texture_gather_instruction(compare, impl, instruction);
}
} // namespace DXIL2SPIRV
