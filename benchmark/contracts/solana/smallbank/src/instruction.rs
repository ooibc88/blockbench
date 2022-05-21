use solana_program::program_error::ProgramError;
use crate::error::EscrowError::InvalidInstruction;

#[derive(Debug)]
pub enum Instruction {
    Almagate(String, String),
    GetBalance(String),
    UpdateBalance(String, usize),
    UpdateSaving(String, usize),
    SendPayment(String, String, usize),
    WriteCheck(String, usize),
    Other,
}

impl Instruction {
    pub fn unpack(instruction_data: &[u8]) -> Result<Self, ProgramError> {
        let (tag, rest) = instruction_data.split_first().ok_or(InvalidInstruction)?;
        Ok(match tag {
            0 => {
                let (arg0, arg1) = Self::unpack_string_string(rest).unwrap();
                Self::Almagate(arg0, arg1)
            },
            1 => {
                let arg0 = String::from_utf8(rest.to_vec()).unwrap();
                Self::GetBalance(arg0)
            },
            2 => {
                let (arg0, arg1) = Self::unpack_string_usize(rest).unwrap();
                Self::UpdateBalance(arg0, arg1)
            },
            3 => {
                let (arg0, arg1) = Self::unpack_string_usize(rest).unwrap();
                Self::UpdateSaving(arg0, arg1)
            }
            4 => {
                let (arg0, arg1, arg2) = Self::unpack_string_string_usize(rest).unwrap();
                Self::SendPayment(arg0, arg1, arg2)
            },
            5 => {
                let (arg0, arg1) = Self::unpack_string_usize(rest).unwrap();
                Self::WriteCheck(arg0, arg1)
            },
            _ => Self::Other
        })
    }

    fn unpack_string_string(rest: &[u8]) -> Result<(String, String), ProgramError> {
        let decoded_input = String::from_utf8(rest.to_vec()).unwrap();
        let mut input_iterator = decoded_input.split_whitespace();

        Ok((String::from(input_iterator.next().unwrap()),
            String::from(input_iterator.next().unwrap())))
    }

    fn unpack_string_usize(rest: &[u8]) -> Result<(String, usize), ProgramError> {
        let decoded_input = String::from_utf8(rest.to_vec()).unwrap();
        let mut input_iterator = decoded_input.split_whitespace();

        Ok((String::from(input_iterator.next().unwrap()),
            String::from(input_iterator.next().unwrap()).parse().unwrap()))
    }

    fn unpack_string_string_usize(rest: &[u8]) -> Result<(String, String, usize), ProgramError> {
        let decoded_input = String::from_utf8(rest.to_vec()).unwrap();
        let mut input_iterator = decoded_input.split_whitespace();

        Ok((String::from(input_iterator.next().unwrap()),
            String::from(input_iterator.next().unwrap()),
            String::from(input_iterator.next().unwrap()).parse().unwrap()))
    }
}