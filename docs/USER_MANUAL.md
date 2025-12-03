# TSLCompiler (Extended Test Specification Language)

The TSLCompiler is a combinatory-based test case generator tool based on the [Category-Partition method](https://dl.acm.org/doi/pdf/10.1145/62959.62964).

This tool serves as an enhanced version of the [TSLgenerator](https://www.github.com/alexorso/tslgenerator), with syntax checking and error detection capabilities.

## Usage
The TSLCompiler is ran with the following syntax:
`./tslcompiler [ -c ] [ -s ] [ -o output_file ] input_file`

Where
- For output (Choose at most one):
  - `-s` is an optional parameter that prints all generated test cases to the Standard Output.
  - `-o output_file` is an optional parameter that creates a file at the given `output_file` path.
    - **NOTE: If `-o` is not specified, the name of the output file will be `input_file.tsl`**
- `-c` is an optional parameter that prints the number of test frames generated.
- `input_file` is the location of the TSL file to be read. **This parameter must be at the end, no matter what.**

Similar usage information will be printed to the screen when using the `-h` or `--help` flag.

# Syntax
## Comments
Comments are lines of text that will be ignored by the TSLCompiler. The '#' character marks a comment until the next newline.

e.g.
```
# Comment: [everything] is. ignored, even # chars
# The property list below will be ignored
a choice.           # [property Skin]
```

## Categories
### Definition
A Category represents a group of items to be considered. A string ending with the ':' (colon) character is interpreted as a category.

### Common Errors
1. Any category that isn't followed by any choices (a nested Category) is not supported.
e.g.
```
# NOTE: To fix, either remove the 'Parameters:' Category, or the 'Pattern size:' Category.
Parameters:      # This is fine
    Pattern size:      # This will throw an error.
        empty.         [property Empty]
```
2. Categories must have unique names. Duplicate names are not allowed.
e.g.
```
# This will yield an error, since there are two categories with the same name of "Value"
Value:
  Red.
  Orange.
  Yellow.

Value:
  $1.
  $5.
  $10.
```

## Choices
### Definition
A Choice represents some item in a Category under consideration. A string ending with the '.' (period) character is interpreted as a choice.
e.g.
```
Some Category:
    this is another choice.
                          ^
```

A test case is formed by the combination of one selected choice in each Category.
e.g.
```
Sample Category 1:
  Simple Choice 1.
  Simple Choice 2.
  
Sample Category 2:
  Simple Choice 3.
  Simple Choice 4.

# Yields the following output:
# Test Case 1
# 	Simple Category 1  : Simple Choice 1 
# 	Simple Category 2  : Simple Choice 3 
# 
# 
# Test Case 2
# 	Simple Category 1  : Simple Choice 1 
# 	Simple Category 2  : Simple Choice 4 
# 
# 
# Test Case 3
# 	Simple Category 1  : Simple Choice 2 
# 	Simple Category 2  : Simple Choice 3 
# 
# 
# Test Case 4
# 	Simple Category 1  : Simple Choice 2 
# 	Simple Category 2  : Simple Choice 4 
```

## Markers
### Definition
A marker is a tag that signifies that a Choice must only be selected once. There are two markers:
- `[error]` -> Represents an error case where the program is expected to malfunction in some way.
    - Example: Providing an invalid argument to a program.
- `[single]` -> Represents an edge case where the program should still work.
    - Example: Providing a file with nothing but emojis.
e.g.
```
Input:
  Text File.
  PDF.    [single]
  Binary. [error]
  
Output:
  Text.
  Image.
```

Both markings signal that only one test frame should be generated. The one test frame will be appropriately marked as an error or single.

### Common Errors
1. A Choice cannot have multiple markers, only one.
e.g.
```
Input:
  Text File.
  # This yields an error, since only one marker can be present.
  Binary.   [single][error]
```
  - NOTE: One exception where a Choice can have "multiple" markers is using a selector expression.
e.g.
```
Some Category 1:
  Some Valid Input. [property valid]
  Some Invalid Input. [property invalid]
  
Some Category 2:
  Some Outcome 1. [if valid][single][else][error]
```

## Properties
### Definition
A Property is a variable typically used to filter out certain choices from being selected. Property names are case sensitive.
e.g.
```
Simple Category 1:
    Simple Choice 1.    [property simple]

Simple Category 2:
    Simple Choice 2.    [if simple]
```

A choice can have multiple properties defined.
e.g.
```
Some Category:
  Some Choice.  [property ABC]

Some Other Category:
  a choice.     [if ABC] [property 1, 2, 3, 4]
                [else] [property 5, 6, 7, 8, 9, 10]
```

### Common Errors
1. Property names must be unique. Duplicate names are not allowed.
e.g.
```
Simple Category 1:
    Simple Choice 1.    [property test2]
    # Despite being a different choice, this will throw an error
    # since `test2` was already defined.
    Simple Choice 2.    [property test2]
```
2. A Property cannot be referenced in the same Category. This is because of how properties are only defined when a Choice is selected; Two or more Choices cannot be chosen in the same category.
e.g.
```
Simple Category 1:
    Simple Choice 1.    [property test1]
    # This yields an error because only one Choice is picked at a time. Property `test1` will never be defined for this if statement to come true.
    Simple Choice 2.    [if test1]
```

## Selector Expressions
### Expressions
An expression is a combination of properties separated by logical operators.

The logical operators '&&' (and), '||' (or), and '!' (not) can be used in a selector expression.  The normal order of evaluation is used (not > and > or), and the terms can be grouped using parentheses to change the order of evaluation.

Examples:
```
Properties:
  A.  [property A]
  B.  [property B]
  C.  [property C]
  
Choices:
  Choice 1. [if A]
  Choice 2. [if !B]
  Choice 3. [if A || B]
  Choice 4. [if A && B]
  Choice 5. [if !(A && B)]
  Choice 6. [if A && B || C]
  Choice 7. [if A && (B || C) && D]
  Choice 8. [if A || !B && !C]
  Choice 9. [if !(!A || B) && C || D]
```

### Selectors
A selector is an if/else expression that determines whether a Choice is selected.
e.g.
```
Simple Category 1:
    Simple Choice 1.    [property simple]

Simple Category 2:
    Simple Choice 2.    [if !simple][else]
```

Selectors can also conditionally define properties and markers.
e.g.
```
Simple Category 1:
  present.

Simple Category 2:
  present.                       [property abc]

Simple Category 3:
  present.                       [if abc] [property def]
  not present.                   [if abc][error][else][single]
```
The `[else]` is optional, and so are the `error/single` and property definitions for both `[if]` and `[else]`.

If <expression> evaluates to true then the following three steps are applied to the statements after the `[if]`. Otherwise, the steps are applied to the statements after the `[else]` (if there is one).

 1. If there's an error or single marking only one frame
    is generated for this choice, and its dependencies are shown (if any).

 2. Otherwise this choice is selected to be combined with
    other choices.

 3. If there's a property list its properties are set to
    true.
    
### Common Errors
1. A Choice can have only one selector expression (you can't nest them like `[if] ... [else] [if] ...` ).
2. A selector must come first in a Choice. The pattern `[property someProperty][if ...]` or `[single][if ...]` will yield an error.
e.g.
```
Simple Category 1:
    Simple Choice 1.    [property simple1]

Simple Category 2:
    # The original TSLgenerator would ignore the property definition if it detected
    # a selector, resulting in erroneous behavior.
    #
    # Because of that, this will yield an error. Common fixes are to just flip it to:
    # Simple Choice 2.  [if simple1][property simple2]
    Simple Choice 2.    [property simple2][if simple1]
```

## Examples of How Choices Are Selected

1. `a good choice is this.      [property Goodness]`
    - Reads: Set Goodness to true and combine this choice with others.

2.  `some choice. [if NonEmpty]`
    - Reads:  if NonEmpty is true combine this choice with others.

3.  `some choice. [if Hmmmmm] [single]`
    - Reads:  if Hmmmmm is true make a single frame with this choice.

4.  `some choice. [if Radical] [property Cool] [else] [error]`
    - Reads:
    ```
    if Radical is true set Cool to true and
        combine this choice with others,
    else make an error frame with this choice.
    ```

5.  `some choice. [if NoWay] [single] [else]`
    - Reads:
    ```
    if NoWay is true make a single frame with
        this choice,
    else combine this choice with others.
    ```

6.  `some choice. [error]`
    - Reads:  make an error frame with this choice.

7.  `some choice. [property Oh, Yeah]`
    - Reads:  set Oh and Yeah to true and combine this choice with others.
