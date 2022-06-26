#![allow(non_snake_case)] // Get rid of the errors!
#![allow(dead_code)]
/*#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        let result = 2 + 2;
        assert_eq!(result, 4);
    }
}*/

mod setup;

use setup::SetupInfo;
use setup::Info;

#[repr(C)]
pub struct OneTwo {
    
}

#[no_mangle]
pub extern "C" fn init_SetupInfo(support_type: u32) -> SetupInfo {
    match SetupInfo::new(support_type) {
        Ok(x) => return x,
        Err(x) => panic!("Error")
    }
}

#[no_mangle]
pub extern "C" fn show()
{
    println!("SHOW");
    //return OneTwo {one: 1, two: 2};
}