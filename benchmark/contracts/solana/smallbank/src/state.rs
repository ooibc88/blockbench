use std::collections::HashMap;
use borsh::{BorshSerialize, BorshDeserialize};
use solana_program::{
    program_error::ProgramError,
    msg,
};

#[derive(BorshSerialize, BorshDeserialize, Clone, Debug)]
pub struct Data {
    pub saving_store: HashMap<String, usize>,
    pub checking_store: HashMap<String, usize>,
}

impl Data {
    const USIZE_BYTES: usize = (usize::BITS / 8) as usize;

    pub fn new(src: &[u8]) -> Result<Self, ProgramError> {
        let (length_saving_store, length_checking_store) = &Self::get_stored_data_len(src);
        let saving_store;
        let checking_store;

        let saving_store_start = &(2*Self::USIZE_BYTES);
        let saving_store_end = &(*saving_store_start + *length_saving_store);

        let checking_store_start = saving_store_end;
        let checking_store_end = &(*checking_store_start + *length_checking_store);

        if *length_saving_store == 0 {
            saving_store = HashMap::<String, usize>::new();
        } else {
            saving_store = HashMap::<String, usize>::try_from_slice(&src[*saving_store_start..*saving_store_end])?;
        }

        if *length_checking_store == 0 {
            checking_store = HashMap::<String, usize>::new();
        } else {
            checking_store = HashMap::<String, usize>::try_from_slice(&src[*checking_store_start..*checking_store_end])?;
        }


        Ok(Data {
            saving_store,
            checking_store
        })
    }
    
    pub fn store(data: &Self, dst: &mut [u8]) {
        let mut serialized_saving_store = &data.saving_store.try_to_vec().unwrap();
        let mut serialized_checking_store = &data.checking_store.try_to_vec().unwrap();

        let saving_store_len = &serialized_saving_store.len();
        let checking_store_len =  &serialized_checking_store.len();

        let saving_store_start = &(2 * Self::USIZE_BYTES);
        let saving_store_end = &(*saving_store_start + *saving_store_len);

        let checking_store_start = &(*saving_store_end);
        let checking_store_end = &(*checking_store_start + *checking_store_len);

        dst[..Self::USIZE_BYTES].copy_from_slice(&saving_store_len.to_le_bytes());
        dst[Self::USIZE_BYTES..2 * Self::USIZE_BYTES].copy_from_slice(&checking_store_len.to_le_bytes());
        dst[*saving_store_start..*saving_store_end].copy_from_slice(serialized_saving_store);
        dst[*checking_store_start..*checking_store_end].copy_from_slice(serialized_checking_store);

        msg!("Data stored: {} bytes", *checking_store_end);
    }

    fn get_stored_data_len(src: &[u8]) -> (usize, usize) {
        (usize::try_from_slice(&src[..Self::USIZE_BYTES]).unwrap(),
        usize::try_from_slice(&src[Self::USIZE_BYTES..2 * Self::USIZE_BYTES]).unwrap())

    }
}

pub trait SafeGet{
    fn get_key_safe(&self, key: &str) -> usize;
}

impl SafeGet for HashMap<String, usize> {
    fn get_key_safe(&self, key: &str) -> usize {
        match self.get(key) {
            Some(value) => *value,
            None => 0,
        }
    }
}