
use borsh::{BorshSerialize, BorshDeserialize};
use solana_program::{
    entrypoint::ProgramResult,
    entrypoint,
    msg,
    account_info::{AccountInfo, next_account_info},
    pubkey::Pubkey,
    program_error::ProgramError,
};

entrypoint!(process);

#[derive(BorshSerialize, BorshDeserialize, Clone, Debug)]
pub struct Instruction {
    size: u128,
    signature: u128,
}

fn initialize_array(size: u128, array: &mut [u128]) {
    for idx in 0..size {
        array[idx as usize] = size - idx;
    }
}

fn quick_sort(arr: &mut [u128], left: u128, right: u128) -> ProgramResult {
    let mut i = left as usize;
    let mut j = right as usize;
    if i == j {
        return Ok(());
    }


    let pivot = arr[(left + (right - left) / 2) as usize];

    while i <= j {
        while arr[i] < pivot { i += 1; }
        while pivot < arr[j] { j -= 1; }
        if i <= j {
            let temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i += 1;
            if j != 0 { j -= 1; }
        }
    }
    if left < (j as u128) { return quick_sort(arr, left, j as u128); }
    if (i as u128) < right { return quick_sort(arr, i as u128, right); }
    Ok(())

}

#[allow(dead_code, unused)]
fn process(
    _program_id: &Pubkey,
    accounts: &[AccountInfo],
    instruction_data: &[u8],
) -> ProgramResult {

    let account_iter = &mut accounts.into_iter();
    let account_data = next_account_info(account_iter)?;

    let data = Instruction::try_from_slice(instruction_data)?;
    let stored_data_size = account_data.try_data_len()? / 16;

    if data.size > stored_data_size as u128 {
        return Err(ProgramError::InvalidInstructionData);
    }

    let stored_data = account_data.try_borrow_mut_data()?;

    unsafe {
        let (_useless_1, data, _useless_2) = stored_data.align_to::<u128>();
        let mut data_vec = data.to_vec();
        initialize_array(stored_data_size as u128,  &mut data_vec);

        quick_sort(&mut data_vec, 0, (stored_data_size - 1) as u128);
        msg!("{:?}", )
    }

    Ok(())
}