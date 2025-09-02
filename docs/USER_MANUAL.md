# TSLCompiler (Extended Test Specification Language)

This is a derivative of the [TSLgenerator-manual.txt](https://www.github.com/alexorso/tslgenerator/blob/master/Docs/TSLgenerator-manual.txt) file provided by the TSLgenerator, excluding
invalid syntax.

## Usage
The TSLCompiler is ran with the following syntax:
`./tslcompiler [ -c ] [ -s ] [ -o output_file ] input_file`

Where
- `-c` is an optional parameter that prints the number of test frames generated.
- `-s` is an optional parameter that prints all generated test cases to the Standard Output.
    - **NOTE: If `-s` is used, the `-o` flag will be ignored.**
- `-o output_file` is an optional parameter that creates a file at the given `output_file` name.
    - `output_file` can also be a path, such as `~/Downloads/catpart.txt.tsl`.
    - **NOTE: If `-o` is not specified, the name of the output file will be `input_file.tsl`**
- `input_file` is the location of the TSL file to be read. **This parameter must be at the end, no matter what.**

## Comments
The '#' character marks a comment until the next newline.

e.g.
```
# Comment: [everything] is. ignored, even # chars
# The property list below will be ignored
a choice.           # [property Skin]
```


## Categories
A string ending with the ':' (colon) character is
interpreted as a category.

Any category that isn't followed by any choices (a nested Category) is not supported.

e.g.
```
# NOTE: To fix, either remove the 'Parameters:' Category, or the 'Pattern size:' Category.
Parameters:      # This is fine
    Pattern size:      # This will throw an error.
        empty.         [property Empty]
```


## Choices
A string ending with the '.' (period) character is
interpreted as a choice.

e.g.
```
this is another choice.     [single]
                      ^
```


## Property Lists
`[property <name1>, <name2>, ... <namek>]`

Sets the properties <name1>, <name2>, ... <namek> to
true.  They can later be referenced in an selector
expression.  Property names are case sensitive.

Each choice can have several property lists.

e.g.
```
# this choice uses the max number of properties
a choice.     [if ABC] [property 1, 2, 3, 4]
              [else] [property 5, 6, 7, 8, 9, 10]
```


## Error & Single Markings
`[error]`
`[single]`

Both markings signal that only one test frame should be
generated; the meaning you give each of them is the only
difference.  The one test frame will be appropriately
marked as an error or single.


## Selector Expressions
`[if <expression>] <error/single> <property list>`
`[else] <error/single> <property list>`

The `[else]` is optional, and so are the <error/single> and
<property list> for both `[if]` and `[else]`.

If <expression> evaluates to true then the following
three steps are applied to the statements after the `[if]`.
Otherwise, the steps are applied to the statements after
the `[else]` (if there is one).

 1. If there's an error or single marking only one frame
    is generated for this choice, and its dependencies are shown (if any).

 2. Otherwise this choice is selected to be combined with
    other choices.

 3. If there's a property list its properties are set to
    true.

A choice can have only one selector expression (you can't
nest them like `[if] ... [else] [if] ...` ).

### Expressions
The logical operators '&&' (and), '||' (or), and '!' (not) can be
used in a selector expression.  The normal order of
evaluation is used (not > and > or), and the terms can be
grouped using parentheses to change the order of
evaluation.

Examples:
```
[if A]
[if !B]
[if A || B]
[if A && B]
[if !(A && B)]
[if A && B || C]
[if A && (B || C) && D]
[if A || !B && !C]
[if !(!A || B) && C || D]
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
