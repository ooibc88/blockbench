use solana_program::{
    entrypoint::ProgramResult,
    entrypoint,
    account_info::AccountInfo,
    pubkey::Pubkey,
};

entrypoint!(process);

fn process(
    _program_id: &Pubkey,
    _accounts: &[AccountInfo],
    _instruction_data: &[u8]
) -> ProgramResult {
    Ok(())
}

