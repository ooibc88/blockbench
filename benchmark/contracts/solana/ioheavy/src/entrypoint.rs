use solana_program::{
    entrypoint::ProgramResult,
    entrypoint,
    account_info::AccountInfo,
    pubkey::Pubkey,
};
use crate::processor::Processor;

entrypoint!(process_instruction);

fn process_instruction(
    _program_id: &Pubkey,
    accounts: &[AccountInfo],
    instruction_data: &[u8],
) -> ProgramResult {
    Processor::process(accounts, instruction_data)
}