use solana_program::{
    entrypoint::ProgramResult,
    entrypoint,
    account_info::AccountInfo,
    pubkey::Pubkey,
};
use crate::processor::Processor;

entrypoint!(process_instruction);

fn process_instruction(
    _program_id: &Pubkey,
    accounts: &[AccountInfo],
    instruction_data: &[u8],
) -> ProgramResult {
    // let mut accounts_iter = accounts.into_iter();
    // let data_account = next_account_info(&mut accounts_iter)?;
    //
    Processor::process(accounts, instruction_data)?;
    // let a = Instruction::unpack(&instruction_data)?;
    // let mut stored_data = data_account.try_borrow_data()?;
    // let hashmap = HashMap::<String, String>::new();
    // hashmap.serialize(&mut stored_data.to_vec());
    // let other_hashmap = HashMap::<String, String>::try_from_slice(&stored_data[..]);
    // msg!("{:?}", hashmap);
    // msg!("{:?}", stored_data);
    // msg!("{:?}", other_hashmap);
    Ok(())
}