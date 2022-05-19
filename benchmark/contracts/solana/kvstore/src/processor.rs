use borsh::{BorshSerialize};
use solana_program::{
    entrypoint_deprecated::ProgramResult,
    account_info::{AccountInfo, next_account_info},
    msg
};
use solana_program::program_error::ProgramError;
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

        let instruction = Instruction::unpack(&instruction_data)?;
        Ok(match instruction {
            Instruction::Set(key, value) => {
                Self::process_set_value(data_account, key, value).ok();
            },
            Instruction::Get(key) => {
                Self::process_get_value(data_account, key).ok();
            }
        })
    }

    fn process_set_value(
        data_account: &AccountInfo,
        key: String,
        value: String,
    ) -> ProgramResult {
        let usize_len = (usize::BITS / 8) as usize;

        let mut stored_data_raw = data_account.try_borrow_mut_data()?;

        let mut stored_data = Data::unpack(&stored_data_raw).unwrap();

        stored_data.data.insert(key, value);

        let mut serialized_object = stored_data.try_to_vec().unwrap();
        let object_size = serialized_object.len().to_le_bytes();
        serialized_object[..usize_len].copy_from_slice(&object_size);
        stored_data_raw[..serialized_object.len()].copy_from_slice(&serialized_object);

        Ok(())
    }

    fn process_get_value(
        data_account: &AccountInfo,
        key: String,
    ) -> ProgramResult {
        let mut stored_data_raw = data_account.try_borrow_mut_data()?;

        let stored_data = Data::unpack(&stored_data_raw).unwrap();
        let value = stored_data.data.get(&key).ok_or(ProgramError::InvalidArgument).unwrap();
        msg!("{}", value);
        Ok(())
    }
}