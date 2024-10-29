## Employee Database

# Synopisis

    usage: dbview [-h help] [-n newfile] [-f filename] [-a add employee]

# DESCRIPTION
    Manage an employee database via the `dbview` command.

    The options are as follows:

    -h      help, show the usage of dbview.

    -f      filename, required option to indicate the relative bath of the .db file to edit.

    -n      newfile, option for creating a new database file if it doesnt exist, will be given the name filename.

    -a      add employee, adds a new employee entry to the database in the format "<name>,<address>,<hours>".

    -l      list employees, lists all employees in a file, will show the state after any other operations.

    -r      remove, remove an entry based on name in the format <name>.
