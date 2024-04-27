# Bugs found while testing

## test driver
* When a mismatch for file size happens, if neither file is zero
length, then compare the file anyhow and post the line where it fails.
* Merge the different programs.
