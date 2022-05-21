use solana_program::msg;
use std::mem;

pub struct Instruction {}

impl Instruction {
    const USIZE_BYTES: usize = (usize::BITS / 8) as usize;

    pub fn unpack(instruction_data: &[u8]) -> (usize, u128){
        let size = usize::from_le_bytes(instruction_data[..Self::USIZE_BYTES].try_into().unwrap());
        let signature = u128::from_le_bytes(instruction_data[Self::USIZE_BYTES..].try_into().unwrap());

        (size, signature)
    }
}