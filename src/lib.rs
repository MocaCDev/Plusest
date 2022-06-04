#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        let result = 2 + 2;
        assert_eq!(result, 4);
    }
}

// Some wrappers will have a specific OS
const LINUX_OS: i32 = 0xB01;
const WINDOWS_OS: i32 = 0xB02;

#[repr(C)]
pub struct OneTwo {
    
}

#[no_mangle]
pub extern "C" fn show()
{
    println!("SHOW");
    //return OneTwo {one: 1, two: 2};
}