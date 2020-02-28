Code Conventions
================

## Naming Convention
Private struct and class fields must have the `m_` prefix and be in `lowerCamelCase`.

Private struct and class methods must have the `_` prefix and be in `lowerCamelCase`.

Public struct and class fields and methods must be named in `UpperCamelCase`.

All struct, enum and class names must be be in `UpperCamelCase`.

All local variables must be in `lowerCamelCase`.

All parameters must have the `_` suffix and be in `lowerCamelCase`.

All namespaces are based on paths and are prefixed with `NerdThings::Ngine`.

# Commenting Styles
Single line comments must have a space following the `//`.
Multi-line comments must have an `*` followed by a space before each line of text. E.g
```
/*
 * This is a multi-line comment
 */
```
If you are commenting anything outside a function body, it must be doxygen compatible. E.g
```
/**
 * This function does stuff
 *
 * @param paramA_ The first parameter.
 * @return The answer.
 */
std::string getTheAnswer(int paramA_);
```
