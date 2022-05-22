use std::collections::HashMap;
use borsh::{BorshSerialize};
use solana_program::{
    entrypoint_deprecated::ProgramResult,
    account_info::{AccountInfo, next_account_info},
    msg,
    log::{sol_log_slice, sol_log},
    program_error::ProgramError,
};
use crate::instruction::Instruction;
use crate::state::Data;

pub struct Processor {}

impl Processor {
    pub fn process(
        accounts: &[AccountInfo],
        instruction_data: &[u8],
    ) -> ProgramResult {
        let accounts_iter = &mut accounts.iter();
        let data_account = next_account_info(accounts_iter)?;

        let mut data = data_account.try_borrow_mut_data()?;

        let mut store = Data::new(&data).unwrap();

        let instruction = Instruction::unpack(&instruction_data);

        match instruction {
            Instruction::Get(key) => {
                let result = Self::process_get_value(&store, key);
                sol_log(&format!("Get: {:?}", result));
            },
            Instruction::Set(key, value) => {
                Self::process_set_value(&mut store, key, value);
                sol_log(&format!("Set: Data[{:?}] = {}", key, value));
            },
            _ => {
                let r = Self::usize_to_bytes(&0);
                sol_log(&format!("Usize to bytes: {:?}", r));
            }
        };

        Data::pack(&store, &mut data);

        Ok(())
    }

    fn process_set_value(store: &mut HashMap<[u8; 20], usize>, key: [u8; 20], value: usize) {
        store.insert(key, value);
    }

    fn process_get_value(store: &HashMap<[u8; 20], usize>, key: [u8; 20]) -> Option<&usize> {
        store.get(&key[..])
    }

    fn usize_to_bytes(v: usize) -> Result<[u8; 20], ProgramError> {
        let mut ret: [u8; 20] = [0; 20];
        match v {
            0 => {
                Ok(ret)
            },
            _ => {
                while v > 0 {
                    ret =
                }
            }
        }
    }
    // fn get_key(key: usize) -> [u8; 20] {
    //     ley key
    // }
}