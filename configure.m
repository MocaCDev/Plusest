-- Just Testing

#GROUNDED[TARGET_OS]
TARGET_OS: linux, windows;

CREATE(let);
CREATE(bro);

-- CONFIGURE keyword with built-in wrappers.
CONFIGURE(let) {
    set as VarWrapper;
}

CONFIGURE(bro) {
    set as VarWrapper;
}