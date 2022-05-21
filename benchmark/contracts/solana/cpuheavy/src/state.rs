use solana_program::msg;
use borsh::{BorshDeserialize};

pub trait U8ToUsizeArray {
    const USIZE_BYTES: usize = (usize::BITS / 8) as usize;

    fn get_usize(&self, usize_idx: usize) -> usize;

    fn set_usize(&mut self, usize_idx: usize, value: usize);

    fn size_usize(&self) -> usize;

    fn print_usize(&self);

    fn is_sorted(&self) -> bool;
}

impl U8ToUsizeArray for [u8] {
    fn get_usize(&self, usize_idx: usize) -> usize {
        usize::try_from_slice(&self[(8 * usize_idx)..(8 * (usize_idx + 1))]).unwrap()
    }

    fn set_usize(&mut self, usize_idx: usize, value: usize) {
        let idx_of_usize = 8 * usize_idx;
        self[(8 * usize_idx)..(8 * (usize_idx + 1))].copy_from_slice(
          &value.to_le_bytes()
        );
    }

    fn size_usize(&self) -> usize {
        self.len() / 8
    }

    fn is_sorted(&self) -> bool {
        let last_idx = self.size_usize();
        let mut current = self.get_usize(0);
        let mut next: usize;
        for idx in 1..(last_idx - 1) {
            next = self.get_usize(idx);

            if current > next {
                return false;
            }
            current = next;
        }
        true
    }

    fn print_usize(&self) {
        let mut output_str = String::new();
        let last_idx = self.size_usize();

        for idx in 0..last_idx {
            if idx == 0 {
                output_str.push_str("[");
                output_str.push_str(&self.get_usize(idx).to_string());
                output_str.push_str(", ");
            }else if idx == last_idx - 1 {
                output_str.push_str(&self.get_usize(idx).to_string());
                output_str.push_str("]");
            }else{
                output_str.push_str(&self.get_usize(idx).to_string());
                output_str.push_str(", ");
            }

        }
        msg!("{}", output_str);
    }
}