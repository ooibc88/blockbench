use borsh::{BorshSerialize, BorshDeserialize};
use solana_program::{
    entrypoint::ProgramResult,
    entrypoint,
    msg,
    account_info::{AccountInfo, next_account_info},
    pubkey::Pubkey,
    program_error::ProgramError,
};

#[derive(BorshSerialize, BorshDeserialize, Clone, Debug)]
pub struct Instruction {
    index: u8,
    input: Vec<u8>,
}

impl Instruction {
    pub fn unpack(instruction_data: &[u8]) -> Self {
        let (instruction, rest) = instruction_data.split_first();
        Instruction {
            index: instruction,
            input: rest,
        }
    }
}