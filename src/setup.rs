#[derive(Debug, Clone)]
#[repr(C)]
pub struct SetupInfo {
    OS_SUPPORT:    u32
}

pub enum Errors {
    NoSuchOSSupported(u32),
    NotSupportedType(u32)
}

pub trait ErrorFuncs {
    fn NotSupportedType(type_attempted: u32) -> Errors;
}

impl ErrorFuncs for Errors {
    fn NotSupportedType(type_attempted: u32) -> Errors {
        Errors::NotSupportedType(type_attempted)
    }
}

pub trait Info {
    fn new(support_type: u32) -> Result<Self, Errors> where Self: Sized;
}

// Some wrappers will have a specific OS. Some might support multiple OS's
const LINUX_OS: u32 = 0xB01;
const WINDOWS_OS: u32 = 0xB02;
const LINUX_ONLY: u32 = 0xA01;
const WINDOWS_ONLY: u32 = 0xA02;
const LINUX_AND_WINDOWS: u32 = 0xA03;

impl Info for SetupInfo {
    fn new(support_type: u32) -> Result<Self, Errors> where Self: Sized {
        if support_type != LINUX_ONLY &&
           support_type != WINDOWS_ONLY &&
           support_type != LINUX_AND_WINDOWS {
               return Err(Errors::NotSupportedType(support_type));
           }
        return Ok(
            Self {
                OS_SUPPORT: support_type
            }
        );
    }
}