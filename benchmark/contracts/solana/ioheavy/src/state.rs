use std::collections::HashMap;
use borsh::{BorshSerialize, BorshDeserialize};

#[derive(BorshSerialize, BorshDeserialize, Clone, Debug)]
pub struct Data {
    data: HashMap<[u8; 20], [u8]>,
}