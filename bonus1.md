### Which problem can occur if a box has a very large size?

If a box has a very large size, various problems can occur, it mostly depends on whether the number is bigger than 2,147,483,647 and whether the number of the sum of all the files the program read is bigger than 9,223,372,036,854,775,807.

## What happens if it's just a big number?
The execution program will be slown down, processing a big box or file potentially leads to memory exhaustion or performance issues.
If the computer memory is able to read and process the file and the sum of the content is less than 9,223,372,036,854,775,807 (max long int), there should not be any further problems.

## What happens when the size of the box is bigger than 2,147,483,647?

If the box size is bigger than the value of max_int the program goes in overflow when the box size is read resulting in a wrong rapresentation and in a undefined behavior.

## What happens when the sum of the sizes if the boxes and files read is biger than 9,223,372,036,854,775,807?

The ftell() function does not return the number it should and the fseek() function does not process the length (second argument) number as it should.
Both of these cases result in an undefined behaviour.