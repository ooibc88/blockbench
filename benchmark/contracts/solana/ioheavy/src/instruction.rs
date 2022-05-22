use borsh::BorshDeserialize;
use solana_program::{
    msg,
};

#[derive(Debug)]
pub enum Instruction {
    Get([u8; 20]),
    Set([u8; 20], usize),
    Other,
}

impl Instruction {
    pub fn unpack(instruction_data: &[u8]) -> Self {
        let (tag, rest) = instruction_data.split_first().unwrap();
        let mut value_20_bytes: [u8; 20] = [0; 20];


        match tag {
            0 => {
                &value_20_bytes.copy_from_slice(&rest);
                Instruction::Get(value_20_bytes)
            },
            1 => {
                &value_20_bytes.copy_from_slice(&rest[..20]);
                let value = usize::try_from_slice(&rest[20..]).unwrap();
                Instruction::Set(value_20_bytes, value)
            },
            _ => Instruction::Other,
        }
    }
}