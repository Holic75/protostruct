# Protostruct

## Description

A header only library, allowing to use simple cross-platform message format (remotely to google protobuf) for low RAM systems (like arduino).
A message is defined within your c++ code, using a series of MACROS in the following way:

    __PROTOSTRUCT_BEGIN(MyMessage1)
    __PROTOSTRUCT_ENTRY(optional, uint32_t, my_int1, 1)
    __PROTOSTRUCT_ENTRY(required, float, my_float1, 2)
    __PROTOSTRUCT_ENTRY(required, int16_t, my_int2, 3)
    __PROTOSTRUCT_ENTRY(repeated_fixed<4>, float, my_float_array, 4)
    __PROTOSTRUCT_ENTRY(repeated, MyMessage2, my_msg_array, 5)
    __PROTOSTRUCT_END
    

Enum type message entries (with name lookup) can be defined as follows:

    __PROTOENUM_BEGIN(MyEnum, uint8_t)
    __PROTOENUM_ENTRY(MyEnumValue1, 0, 1)
    __PROTOENUM_ENTRY(MyEnumValue2, 1, 2)
    __PROTOENUM_ENTRY(MyEnumValue3, 2, 3)
    __PROTOENUM_END
    
Oneof type entires are defined as follows:
    
    __PROTOUNION_BEGIN(MyUnion)
    __PROTOUNION_ENTRY(optional, float, union_float, 1)
    __PROTOUNION_ENTRY(required, uint8_t, union_uint, 2)
    __PROTOUNION_END

These message entires can be further reused in protostruct in the same way as basic numerical data types:

    __PROTOSTRUCT_BEGIN(MyMessage3)
    __PROTOSTRUCT_ENTRY(required, uint32_t, my_int1, 1)
    __PROTOSTRUCT_ENTRY(required, float, my_float1, 2)
    __PROTOSTRUCT_ENTRY(required, MyEnum, my_enum1, 3)
    __PROTOSTRUCT_ENTRY(optional, MyUnion, my_union, 4)
    __PROTOSTRUCT_END
    
See protostruct_test.cpp for examples of usage.