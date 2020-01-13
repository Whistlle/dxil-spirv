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

#include "dxil_pixel_ops.hpp"
#include <opcodes/converter_impl.hpp>

namespace DXIL2SPIRV
{
bool emit_discard_instruction(Converter::Impl &impl, const llvm::CallInst *instruction)
{
	Operation *op = impl.allocate(spv::OpDemoteToHelperInvocationEXT);
	impl.add(op);
	impl.spirv_module.enable_shader_discard();
	return true;
}

bool emit_derivative_instruction(spv::Op opcode, Converter::Impl &impl, const llvm::CallInst *instruction)
{
	Operation *op = impl.allocate(opcode, instruction);
	op->add_id(impl.get_id_for_value(instruction->getOperand(1)));

	impl.add(op);
	impl.builder().addCapability(spv::CapabilityDerivativeControl);
	return true;
}

bool emit_sample_index_instruction(Converter::Impl &impl, const llvm::CallInst *instruction)
{
	spv::Id var_id = impl.spirv_module.get_builtin_shader_input(spv::BuiltInSampleId);
	Operation *op = impl.allocate(spv::OpLoad, instruction);
	op->add_id(var_id);
	impl.add(op);
	impl.builder().addCapability(spv::CapabilitySampleRateShading);
	return true;
}

bool emit_coverage_instruction(Converter::Impl &impl, const llvm::CallInst *instruction)
{
	auto &builder = impl.builder();
	spv::Id var_id = impl.spirv_module.get_builtin_shader_input(spv::BuiltInSampleMask);

	Operation *ptr_op =
	    impl.allocate(spv::OpAccessChain, builder.makePointer(spv::StorageClassInput, builder.makeUintType(32)));
	ptr_op->add_id(var_id);
	ptr_op->add_id(builder.makeUintConstant(0));
	impl.add(ptr_op);

	Operation *load_op = impl.allocate(spv::OpLoad, instruction);
	load_op->add_id(ptr_op->id);
	impl.add(load_op);
	return true;
}
} // namespace DXIL2SPIRV
