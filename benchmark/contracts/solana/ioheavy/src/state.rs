use borsh::{BorshSerialize, BorshDeserialize};
use std::collections::HashMap;
use solana_program::{
    msg,
};
use solana_program::log::sol_log;

pub struct Data {
    ret: [u8; 100],
    store: HashMap<[u8; 20], usize>
}

impl Data {
    const USIZE_BYTES: usize = (usize::BITS / 8) as usize;
    const ALPHABET: [u8; 150] = *b"abcdefghijklmnopqrstuvwxy#$%^&*()_+[]{}|;:,./<>?`~abcdefghijklmnopqrstuvwxy#$%^&*()_+[]{}|;:,./<>?`~abcdefghijklmnopqrstuvwxy#$%^&*()_+[]{}|;:,./<>?`~";

    pub fn new(src: &[u8]) -> Option<HashMap<[u8; 20], usize>> {
        let (length, data) = src.split_at(Self::USIZE_BYTES);
        match usize::try_from_slice(&length[..]).unwrap() {
            0 => {
                Some(HashMap::<[u8; 20], usize>::new())
            },
            _ => {
                let length_value = usize::try_from_slice(&length).unwrap();
                Some(HashMap::<[u8; 20], usize>::try_from_slice(&data[..length_value]).unwrap())
            },
        }
    }

    pub fn pack(src: &HashMap<[u8; 20], usize>, dst: &mut [u8]) {
        let serialized_object = &src.try_to_vec().unwrap();
        let object_length = (serialized_object.len() as usize).to_le_bytes();
        let object_end = serialized_object.len() + Self::USIZE_BYTES;

        dst[..Self::USIZE_BYTES].copy_from_slice(&object_length);
        dst[Self::USIZE_BYTES..object_end].copy_from_slice(&serialized_object);
    }
}

pub trait Bytes {
    fn bytes20(&self) -> [u8; 20];
    fn uint(&self) -> usize;
}

impl Bytes for [u8; 20] {
    fn bytes20(&self) -> [u8; 20] {

    }

    fn uint(&self) -> usize {

    }
}

