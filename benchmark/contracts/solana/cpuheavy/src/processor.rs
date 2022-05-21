use borsh::{BorshSerialize, BorshDeserialize};
use solana_program::{
    pubkey::Pubkey,
    account_info::{AccountInfo, next_account_info},
    msg,
    entrypoint_deprecated::ProgramResult,
};
use solana_program::stake::instruction::initialize;
use crate::state::U8ToUsizeArray;
use crate::instruction::Instruction;

pub struct Processor {}

impl Processor {

    pub fn process(
        accounts: &[AccountInfo],
        instruction_data: &[u8],
    ) -> ProgramResult {
        let mut account_iter = &mut accounts.into_iter();
        let data_account = next_account_info(account_iter)?;

        let (size, signature) = Instruction::unpack(instruction_data);
        let mut data = data_account.try_borrow_mut_data().unwrap();

        Self::initialize(&mut data, size);
        Self::quick_sort(&mut data, 0, size - 1);

        Ok(())
    }

    fn initialize(data: &mut [u8], size: usize) {
        for idx in 0..size {
            data.set_usize(idx, size - idx);
        }
    }

    fn quick_sort(arr: &mut [u8], left: usize, right: usize) -> ProgramResult {
        let mut i = left;
        let mut j = right;

        if i == j {
            return Ok(());
        }

        let pivot = arr.get_usize(left + (right - left) / 2);

        while i <= j {
            while arr.get_usize(i) < pivot { i += 1; }
            while pivot < arr.get_usize(j) { j -= 1; }
            if i <= j {
                let temp = arr.get_usize(i);
                arr.set_usize(i,arr.get_usize(j));
                arr.set_usize(j,temp);
                i += 1;
                if j != 0 { j -= 1; }
            }
        }
        if left < j { return Self::quick_sort(arr, left, j); }
        if i < right { return Self::quick_sort(arr, i, right); }
        Ok(())

    }

}