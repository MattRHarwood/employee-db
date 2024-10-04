### Requirements

We want to create a command line utility to interact with the employee db.

Some examples of the kind of operations we want:

 - './dbview -a <entry>' - add a new entry
 - './dbview -u <entry>' - update an entry
 - './dbview -d <entry>' - delete an entry

 We will parse the whole db in to memory for and then run the commands on
 the employee data structures.
