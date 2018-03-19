# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

Supposedly, the longest word in the English language.

## According to its man page, what does `getrusage` do?

Gets the amount of resources used so far by a given function and any functions it's called

## Per that same man page, how many members are in a variable of type `struct rusage`?

16

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

Otherwise, you'd have to be concerned that before or after don't exist. This way, you're able to check whether their pointer is NULL

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

It starts at the beginning of the dictionary and goes until it's reached beyond the last character of the dictionary.
looks character by character. As long as the character could be part of a word (letter or apostrophe, as long as the apostrophe isn't the first letter),
it adds the character to an array that will contain the word.
If it has added more characters than the possible length of the word, it cannot possibly be a word, so it ignores the whole array and
skips until it's reached the end of that string of letters. It then starts again from the next word.
The next word begins from the first character that is a letter.
If the character is a number, it'll skip until it's reached the end of that string of letters to ignore the word, and then start from the next word.
If the character is neither, we say we've gone past the end of the word, provided the word has some length.
We then turn the word into a string by adding '\0', check the word, print if mispelled and start again from the next word.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

We don't know how long any series of letters will be. fscanf would have to store teh result in a string,
but no matter what size we make that string, for all we know the file could contain a larger string.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

speller.c uses the input later on and assumes it hasn't changed.
You're passing in a pointer to each function, so unless you call it a constant, the function could theoretically try to change it.