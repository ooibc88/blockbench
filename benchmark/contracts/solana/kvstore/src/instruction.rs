use borsh::{BorshSerialize, BorshDeserialize};
use solana_program::{
    program_error::ProgramError,
};

#[derive(BorshSerialize, BorshDeserialize, Debug, Clone)]
pub enum Instruction {
    Set(String, String),
    Get(String),
}

impl Instruction {
    pub fn unpack(instruction_data: &[u8]) -> Result<Self, ProgramError> {
        let (tag, rest) = instruction_data.split_first().ok_or(ProgramError::InvalidInstructionData)?;
        let inputs = String::from_utf8(rest.to_vec()).unwrap();

        Ok(match tag {
            1 => {
                let mut split_input = inputs.split_whitespace();
                let key = match split_input.next() {
                    Some(key) => String::from(key),
                    _ => String::from(""),
                };
                let value = match split_input.next() {
                    Some(value) => String::from(value),
                    _ => String::from(""),
                };

                Instruction::Set(key, value)
            },
            2 => {
                Instruction::Get(inputs)
            },
            _ => return Err(ProgramError::InvalidInstructionData),
        })
    }
}