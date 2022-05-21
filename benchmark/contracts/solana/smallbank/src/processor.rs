use borsh::{BorshSerialize, BorshDeserialize};
use solana_program::{
    pubkey::Pubkey,
    account_info::{AccountInfo, next_account_info},
    msg,
    entrypoint_deprecated::ProgramResult,
};
use crate::state::{Data, SafeGet};
use crate::instruction::Instruction;

pub struct Processor {}

impl Processor {
    pub fn process(
        accounts: &[AccountInfo],
        instruction_data: &[u8],
    ) -> ProgramResult {
        let mut accounts_iter = &mut accounts.into_iter();
        let data_account = next_account_info(accounts_iter)?;

        let instruction = Instruction::unpack(instruction_data)?;
        let mut stored_data = data_account.try_borrow_mut_data()?;
        let mut data = Data::new(&stored_data)?;

        match instruction {
            Instruction::Almagate(arg0, arg1) => {
                Self::process_almagate(&mut data, arg0, arg1);
            },
            Instruction::GetBalance(arg0) => {
                Self::process_get_balance(&mut data, arg0);
            },
            Instruction::UpdateBalance(arg0, arg1) => {
                Self::process_update_balance(&mut data, arg0, arg1);
            },
            Instruction::UpdateSaving(arg0, arg1) => {
                Self::process_update_saving(&mut data, arg0, arg1);
            },
            Instruction::SendPayment(arg0, arg1, arg2) => {
                Self::process_send_payment(&mut data, arg0, arg1, arg2);
            },
            Instruction::WriteCheck(arg0, arg1) => {
                Self::process_write_check(&mut data, arg0, arg1);
            }
            _ => ()
        }

        Data::store(&data, &mut stored_data);
        msg!("{:?}", data.saving_store);
        msg!("{:?}", data.checking_store);
        Ok(())
    }

    fn process_almagate(data: &mut Data, arg0: String, arg1: String) {
        let bal1 = data.saving_store.get_key_safe(&arg0);
        let bal2 = data.checking_store.get_key_safe(&arg1);

        data.checking_store.insert(arg0, 0);
        data.saving_store.insert(arg1, bal1 + bal2);
    }

    fn process_get_balance(data: &mut Data, arg0: String) {
        let bal1 = data.saving_store.get_key_safe(&arg0);
        let bal2 = data.checking_store.get_key_safe(&arg0);

        let balance = bal1 + bal2;
        msg!("{}", balance);
    }

    fn process_update_balance(data: &mut Data, arg0: String, arg1: usize) {
        let bal1 = data.checking_store.get_key_safe(&arg0);
        let bal2 = arg1;

        data.checking_store.insert(arg0, bal1 + bal2);
    }

    fn process_update_saving(data: &mut Data, arg0: String, arg1: usize) {
        let bal1 = data.saving_store.get_key_safe(&arg0);
        let bal2 = arg1;

        data.saving_store.insert(arg0, bal1 + bal2);
    }

    fn process_send_payment(data: &mut Data, arg0: String, arg1: String, arg2: usize) {
        let mut bal1 = data.checking_store.get_key_safe(&arg0);
        let mut bal2 = data.checking_store.get_key_safe(&arg1);
        let amount = &arg2;

        bal1 -= *amount;
        bal2 += *amount;

        data.checking_store.insert(arg0, bal1);
        data.checking_store.insert(arg1, bal2);
    }

    fn process_write_check(data: &mut Data, arg0: String, arg1: usize) {
        let bal1 = data.checking_store.get_key_safe(&arg0);
        let bal2 = data.saving_store.get_key_safe(&arg0);
        let amount = arg1;

        if amount < bal1 + bal2 {
            data.checking_store.insert(arg0, bal1 - amount -1);
        }else {
            data.checking_store.insert(arg0, bal1 - amount);
        }

    }

}