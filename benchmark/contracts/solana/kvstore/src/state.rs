use std::collections::HashMap;
use borsh::{BorshSerialize, BorshDeserialize};
use solana_program::program_error::ProgramError;

#[derive(BorshSerialize, BorshDeserialize, Debug, Clone)]
pub struct Data {
    pub length: usize,
    pub data: HashMap<String, String>,
}

impl Data {
    pub fn unpack(dst: &[u8]) -> Result<Self, ProgramError>{
        let stored_map;
        let usize_len = (usize::BITS / 8) as usize;
        let data_length = usize::try_from_slice(&dst[..usize_len]).unwrap();
        match data_length {
            0 => {
                stored_map = HashMap::<String, String>::try_from_slice(&dst[usize_len..(usize_len + 4)]).unwrap();
            }
            _ => {
                stored_map = HashMap::<String, String>::try_from_slice(&dst[usize_len..data_length]).unwrap();
            }
        }

        Ok(Data {
            length: data_length,
            data: stored_map,
        })
    }
}