use borsh::{BorshSerialize, BorshDeserialize};
use solana_program::{
    entrypoint::ProgramResult,
    entrypoint,
    msg,
    account_info::{AccountInfo, next_account_info},
    pubkey::Pubkey,
    program_error::ProgramError,
};
use crate::instruction::Instruction;

entrypoint!(process);

#[allow(dead_code, unused)]
fn process(
    _program_id: &Pubkey,
    accounts: &[AccountInfo],
    instruction_data: &[u8],
) -> ProgramResult {

    let ALPHABET = b"abcdefghijklmnopqrstuvwxy#$%^&*()_+[]{}|;:,./<>?`~abcdefghijklmnopqrstuvwxy#$%^&*()_+[]{}|;:,./<>?`~abcdefghijklmnopqrstuvwxy#$%^&*()_+[]{}|;:,./<>?`~";
    // let account_iter = &mut accounts.into_iter();
    // let account_data = next_account_info(account_iter)?;
    let instruction = Instruction::unpack(instruction_data);
    msg!("{:?}", instruction);
    Ok(())
}