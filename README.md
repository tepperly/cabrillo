# cabrillo
The goal of this project is to make a Cabrillo file parser that can read Cabrillo files that meet the
[official specification](https://wwrof.org/cabrillo/cabrillo-v3-header/) and the common variants that
people submit to contests. The Cabrillo file format is somewhat challenging to automatically parse because
it is a column-oriented file format. Many lines can be treated like a comma separate value (CSV) file format
where the "comma" is whitespace. However, this approach does not work when fields are left empty.

This [web article](https://medium.com/dsmli/extracting-tables-from-plain-text-files-statistically-with-numpy-72c1363cf287)
on a technique to discover where the column breaks are in the a plain text file serves as the insipiration
for the approach here.

## Background on the Cabrillo file format

The Cabrillo file format is the de facto computer standard for logging ham radio contests.
