#include "Instruction.h"
#include "CPU.h"

namespace ControlDeck
{
	void Instruction::Execute(uint8 opCode)
	{
		const InstructionInfo* info = GetInstructionInfo(opCode);

		if (m_operation != nullptr)
		{
#ifdef DEBUG_PRINT
			uint8 opCode = m_cpu->ReadMemory8(m_cpu->PC);
			uint8 byte1 = m_cpu->ReadMemory8(m_cpu->PC + 1);
			uint8 byte2 = m_cpu->ReadMemory8(m_cpu->PC + 2);

			if (info->Bytes == 1)
			{
				printff("%04X %02X       %s\t\t", m_cpu->PC, opCode, m_name.c_str());
			}
			else if (info->Bytes == 2)
			{
				printff("%04X %02X %02X    %s\t\t", m_cpu->PC, opCode, byte1, m_name.c_str());
			}
			else
			{
				printff("%04X %02X %02X %02X %s\t\t", m_cpu->PC, opCode, byte1, byte2, m_name.c_str());
			}

			printff("A:%02X X:%02X Y:%02X P:%02X SP:%02X\n", m_cpu->Accumulator, m_cpu->XReg, m_cpu->YReg, m_cpu->ProcessorStatus, m_cpu->SP);
#endif
			m_operation->call(info->Mode);
			m_cpu->m_cycleCounter += info->Cycles;
		}
		else
		{
			throw("Not implemented");
		}
	}

	void Instruction::AddOperation(uint8 opCode, uint8 bytes, uint8 cycles, uint8 cylesPageBoundary, AdrMode mode)
	{
		m_instructions.push_back(std::move(make_unique<InstructionInfo>(opCode, bytes, cycles, cylesPageBoundary, mode)));
	}

	std::vector<uint8> Instruction::GetOpCodes() const
	{
		std::vector<uint8> opcodes;

		for (auto const& info : m_instructions)
		{
			opcodes.push_back(info->OpCode);
		}

		return opcodes;
	}

	const InstructionInfo* Instruction::GetInstructionInfo(uint8 OpCode) const
	{
		for (auto const& info : m_instructions)
		{
			if (info->OpCode == OpCode)
			{
				return info.get();
			}
		}

		throw("Invalid opcode for instruction.");
		return nullptr;
	}
}
